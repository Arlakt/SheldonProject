#include <stdio.h>
#include <unistd.h>
#include "./../API/flight_functions.h"

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
			set_complex_move(message,n++,0,-0.05,0.05,0);
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
	
