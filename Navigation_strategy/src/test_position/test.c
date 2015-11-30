#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "./../API/find_position.h"

//pthread_t thread position;

int main ()
{
	int signal [8] = {128, 255, 98, 3, 5, 0, -5, -2};
	t_position pos = basic_position(signal);
	printf("Angle : %d \n Distance : %d \n", pos.angle, pos.distance);
	return 0;
}
	
