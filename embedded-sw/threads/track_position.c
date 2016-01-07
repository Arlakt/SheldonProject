#include "track_position.h"
#include "find_position.h"
#include <movement/flight_functions.h>
#include <movement/UDP_sender.h>
#include <signal.h> // for signals handling
#include <string.h> // for memset function
#include <time.h>

extern int keepRunning;
extern pthread_mutex_t compute_pos_mux;
extern pthread_mutex_t track_pos_mux;

//handler for a signal
void intHandlerThread3(int sig){
	keepRunning=0;
	printf("CTRL+C signal in track_position\n");
}


/**
 *	@brief	Print current position of the emitter on standard output
 */
void print_position(void)
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];

	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (buffer, 80, "%X", timeinfo);

	// @todo make a mutex especially for the global variable pos
	if(pos.signalDetected)
		printf("%s > Angle : %d - Distance : %d \n", buffer, pos.angle, pos.distance);
	else
		printf("%s > No signal\n", buffer);
}

/**
 * @brief	function designed to be the main of a thread
 *			Sends movement commands to the drone in order to follow the beacon
 */
void * track_position(void * arg){

	// check time interval
    struct timeval old_tv = {0};
    struct timeval tv = {0};
    long int elapsed_time = 0; // in microsecondss
    
    // moves
    char message [512];
	int n = 1;
	int tps = 1;
	int wait =1;

	//handle the ctrl -c to make the drone land
	struct sigaction act;
	memset(&act,0,sizeof(act));
	act.sa_handler = intHandlerThread3;
	sigaction(SIGINT, &act, NULL);

    gettimeofday(&old_tv, NULL); 

	if (init_socket() != 0)
    {
        printf("[FAILED] Socket initialization failed\n");
    }
    else
    {

    	//////////////////////////////////////////////////////////
    	//	TAKING OFF 
    	//////////////////////////////////////////////////////////
		sleep(1);
        printf("Drone starts flying...\n");
		set_trim(message, n++, wait);
		
		// while(tps < 167)
		// {
		// 	take_off(message, n++, wait);
		// 	tps++;
		// }
		
		//stop waiting 40 us after a command send
		wait = 0;
		
		while(keepRunning){
		    while (elapsed_time < 35000)
			{
				gettimeofday(&tv, NULL);
				elapsed_time = (tv.tv_sec-old_tv.tv_sec)*1000000 + (tv.tv_usec-old_tv.tv_usec);
			}

			pthread_mutex_lock(&track_pos_mux);

			print_position();
			
			///////////////////////////////////////////////////////////////////////
			// MOVES TO HAVE THE RIGHT ANGLE AND RIGHT DISTANCE FROM THE EMIITER
			///////////////////////////////////////////////////////////////////////
			reset_com(message, wait);
			
			// If no signal has been detected
			// if(!pos.signalDetected)
			// {
			// 	// stop moving
			// 	set_simple_move(message, n++, FRONT, 0, wait);
			// }
			// // If a signal has been detected, move !
			// else
			// {
			// 	// @todo managing the distance
			// 	if(pos.angle >= -ANGLE_PRECISION/2 && pos.angle <= ANGLE_PRECISION/2)
			// 	{
			// 		// And now manage distance
			// 		if(pos.distance > 200) // in cm
			// 			set_simple_move(message, n++, FRONT, 0.05, wait);
			// 		else if(pos.distance < 180) // in cm
			// 			set_simple_move(message, n++, BACK, 0.05, wait);
			// 		else
			// 			set_simple_move(message, n++, FRONT, 0, wait);	
			// 	}				
	  //     	 	else if (pos.angle > ANGLE_PRECISION/2)
			// 		set_simple_move(message, n++, CLKWISE, 0.5, wait);
			// 	else
			// 		set_simple_move(message, n++, ANTI_CLKWISE, 0.5,wait);
				
			// }
			pthread_mutex_unlock(&compute_pos_mux);

			gettimeofday(&old_tv, NULL);
			elapsed_time=0;
		}

		///////////////////////////////////////////
		// LANDING
		///////////////////////////////////////////
		//landing(message, n++, wait);
		sleep(1);

	}
	pthread_exit(NULL);
}
