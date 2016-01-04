#include "./../API/track_position.h"

#include "./../API/common.h"

extern int keepRunning;
extern int testVariable;

//handler for a signal
void intHandlerThread2(int sig){
	keepRunning=0;
	printf("thread 2 : track position\n");
}

//used to prototype track_position
//prints the position of the beacon anytime the thread is active
void * print_position(void * arg){

    (void) arg;
    int i = 0;
    while(i < 10){

        //locks its own mutex
        pthread_mutex_lock(&track_pos_mux);

	printf("Print Angle : %d \nDistance : %d \n", pos.angle, pos.distance);
        
        //release the calculating mutex
        pthread_mutex_unlock(&compute_pos_mux);

	i++;
    }
    pthread_exit(NULL);
}

//function designed to be the main of a thread
//send the movement orders of the drone in order to follow the beacon
void * track_position(void * arg){

	// check time interval
    struct timeval old_tv = {0};
    struct timeval tv = {0};
    long int elapsed_time = 0; // in microsecondss
    
	//handle the ctrl -c to make the drone land
	struct sigaction act;
	memset(&act,0,sizeof(act));
	act.sa_handler = intHandlerThread2;
	sigaction(SIGINT, &act, NULL);
    
    // moves
    char message [512];
	int n = 1;
	int tps = 1;
	int wait =1;

    gettimeofday(&old_tv, NULL); 

    (void) arg;
    int i = 0;

	if (init_socket() != 0)
    {
        printf("[FAILED] Socket initialization failed\n");
    }
    else //complex_move(...;float roll_power, float pitch_power, float vertical_power, float yaw_power)
    {
		sleep(1);
        printf("demarrage\n");
		set_trim(message, n++, wait);
		
		while(tps < 167)
		{
			take_off(message, n++, wait);
			tps++;
		}
		
		//stop waiting 40 us after a command send
		wait = 0;
		
		while(keepRunning){
		    while (elapsed_time < 35000)
			{
				gettimeofday(&tv, NULL);
				elapsed_time = (tv.tv_sec-old_tv.tv_sec)*1000000 + (tv.tv_usec-old_tv.tv_usec);
			}

			//locks its own mutex
			pthread_mutex_lock(&track_pos_mux);
			
			//send the move command
			//printf("Move toward : Angle : %d Distance : %d \n", pos.angle, pos.distance);
			//reset_com(message, wait);
			
			// TEST OF THE ROTATION
			if(testVariable == 0)
			{
				printf("Waiting...\n");
				set_simple_move(message, n++, CLKWISE, 0, wait);
			}
			else if(testVariable == 1)
			{
				printf("Rotation clockwise\n");
				set_simple_move(message, n++, CLKWISE, 0.4, wait);
			}
			else if(testVariable == 2)
			{	
				printf("Rotation counter-clockwise\n");
				set_simple_move(message, n++, ANTI_CLKWISE, 0.4, wait);
			}
			else if(testVariable == 9)
			{
				printf("Landing...\n");
				set_simple_move(message, n++, CLKWISE, 0, wait);
				keepRunning = 0;		
			}

			/*if(pos.angle == 0)
				set_simple_move(message, n++, CLKWISE, 0, wait);
      	 	 else if (pos.angle > 0)
				set_simple_move(message, n++, CLKWISE, 0.4, wait);
			else
				set_simple_move(message, n++, ANTI_CLKWISE, 0.4,wait);
			*/
			//release the calculating mutex
			pthread_mutex_unlock(&compute_pos_mux);

			gettimeofday(&old_tv, NULL);
			elapsed_time=0;

			i++;
		}
		landing(message, n++,wait);
		sleep(1);
	}
	pthread_exit(NULL);
}
