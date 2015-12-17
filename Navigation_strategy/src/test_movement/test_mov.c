#ifndef BASIC
#define BASIC
#include <stdlib.h>
#include <stdio.h>
#endif

#ifndef PTHREAD_N_TIME
#define PTHREAD
#include <pthread.h>
#endif

#include <signal.h>
#include <string.h>
#include "./../API/flight_functions.h"

extern int sockfd;
static int keepRunning = 1;

//declaration and initialization of the different mutex
pthread_mutex_t compute_pos_mux = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t track_pos_mux   = PTHREAD_MUTEX_INITIALIZER;


//handler for a signal
void intHandler(int sig){
	keepRunning=0;
}


int main ()
{
	char message [512];
	int n = 1;
	int tps = 1;
	int wait =1;
	/*
	//handle the ctrl -c to make the drone land
	struct sigaction act;
	memset(&act,0,sizeof(act));
	act.sa_handler = intHandler;
	sigaction(SIGINT, &act, NULL);
	*/

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
		tps = 0;
		
		//test de fonction
		at_config(message, n++, "pic:ultrasound_freq","2");
		
		//while(tps < 133)
		//while(1)
		while(keepRunning)
		{
			reset_com(message, wait);
	 		tps++;
		}
		tps = 0;
/*
        printf("debut commande\n");

		while(tps < 500)
		{
			//go front and up and turning clockwise			
			set_complex_move(message,n++,0,-0.05,0.1,0.05,wait);
			tps++;
		}
		tps = 0;

		set_complex_move(message, n++,0,0,0,0,wait);
		while(tps < 133)
		{
			reset_com(message,wait);
	 		tps++;
		}
		tps = 0;
*/
		landing(message, n++,wait);
		sleep(1);

	}
    close(sockfd);
	return 0;
}
	
