#include "track_position.h"
#include "find_position.h"
#include <movement/UDP_sender.h>

extern pthread_mutex_t compute_pos_mux;
extern pthread_mutex_t track_pos_mux;

/**
 *	@brief	Print current position of the emitter on standard output
 */
void print_position(void)
{
	// @todo make a mutex especially for the global variable pos
	printf("Angle : %d - Distance : %d \n", pos.angle, pos.distance);
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

    gettimeofday(&old_tv, NULL); 

	if (init_socket() != 0)
    {
        printf("[FAILED] Socket initialization failed\n");
    }
    else
    {
/*
    	//////////////////////////////////////////////////////////
    	//	TAKING OFF 
    	//////////////////////////////////////////////////////////
		sleep(1);
        printf("Drone starts flying...\n");
		set_trim(message, n++, wait);
		
		while(tps < 167)
		{
			take_off(message, n++, wait);
			tps++;
		}
*/		
		//stop waiting 40 us after a command send
		wait = 0;
		
		while(1){
		    while (elapsed_time < 35000)
			{
				gettimeofday(&tv, NULL);
				elapsed_time = (tv.tv_sec-old_tv.tv_sec)*1000000 + (tv.tv_usec-old_tv.tv_usec);
			}

			pthread_mutex_lock(&track_pos_mux);
/*			
			///////////////////////////////////////////////////////////////////////
			// MOVES TO HAVE THE RIGHT ANGLE AND RIGHT DISTANCE FROM THE EMIITER
			///////////////////////////////////////////////////////////////////////
			
			// @todo managing the distance
			if(pos.angle => -ANGLE_PRECISION/2 && pos.angle <= ANGLE_PRECISION/2)
				set_simple_move(message, n++, CLKWISE, 0, wait);
      	 	else if (pos.angle > ANGLE_PRECISION/2)
				set_simple_move(message, n++, CLKWISE, 0.4, wait);
			else
				set_simple_move(message, n++, ANTI_CLKWISE, 0.4,wait);
			
*/			
			pthread_mutex_unlock(&compute_pos_mux);

			gettimeofday(&old_tv, NULL);
			elapsed_time=0;
		}
	}
	pthread_exit(NULL);
}
