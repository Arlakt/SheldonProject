#include <stdio.h>
#include <unistd.h>
#include "./../API_movement/flight_functions.h"

extern int sockfd;

int main ()
{
	char message [512];
	int n = 1;
	int tps = 1;

    if (init_socket() != 0)
    {
        printf("[FAILED] Socket initialization failed\n");
    }
    else
    {
		sleep(1);
		set_trim(message, n++);
		oriented_roundel_detection(message, n++);
		front_cam_detecting(message, n++);
		hover_mode(message, n++);

		usleep(50000);
		while(tps < 167)
		{
			take_off(message, n++);
			tps++;
		}
		tps = 0;

		while(tps < 1000)
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
	
