#include "./../API/track_position.h"

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

    struct timeval old_tv = {0};
    struct timeval tv = {0};
    long int elapsed_time = 0; // in microsecondss

    gettimeofday(&old_tv, NULL); 

    (void) arg;
    int i = 0;
    while(i < 10){
        while (elapsed_time < 40000)
	{
	    gettimeofday(&tv, NULL);
	    elapsed_time = (tv.tv_sec-old_tv.tv_sec)*1000000 + (tv.tv_usec-old_tv.tv_usec);
	}

        //locks its own mutex
        pthread_mutex_lock(&track_pos_mux);
	//send the move command
	printf("Move toward : Angle : %d \nDistance : %d \nElapsed time : %ld\n", pos.angle, pos.distance, elapsed_time);
       // printf("sec: %ld usec : %ld\n", tv.tv_sec, tv.tv_usec);
        //release the calculating mutex
        pthread_mutex_unlock(&compute_pos_mux);

        gettimeofday(&old_tv, NULL);
	elapsed_time=0;

	i++;
    }
    pthread_exit(NULL);
}
