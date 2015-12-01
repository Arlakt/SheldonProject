#include <stdio.h>
#include <unistd.h>
#include "./../API/flight_functions.h"
#include <pthread.h>

extern int sockfd;

//declaration and initialization of the different mutex
    pthread_mutex_t compute_pos_mux = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t track_pos_mux   = PTHREAD_MUTEX_INITIALIZER;

int main ()
{
	char message [512];
	int n = 1;
	int tps = 1;

    if (init_socket() != 0)
    {
        printf("[FAILED] Socket initialization failed\n");
    }
    else //complex_move(...;float roll_power, float pitch_power, float vertical_power, float yaw_power)
    {
		sleep(1);
        	printf("demarrage\n");
		set_trim(message, n++);
		
		while(tps < 167)
		{
			take_off(message, n++);
			tps++;
		}
		tps = 0;
		while(tps < 133)
		{
			reset_com(message);
	 		tps++;
		}
		tps = 0;

        	printf("debut commande\n");

		while(tps < 500)
		{
			//go front and up and turning clockwise			
			set_complex_move(message,n++,0,-0.05,0.05,0.05);
			tps++;
		}
		tps = 0;

		set_complex_move(message, n++,0,0,0,0);
		while(tps < 133)
		{
			reset_com(message);
	 		tps++;
		}
		tps = 0;
		landing(message, n++);
		sleep(1);
	}
    close(sockfd);
	return 0;
}
	
