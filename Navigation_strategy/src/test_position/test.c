#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "./../API/find_position.h"

//shared variable
static t_position pos = {10,10};

//declaration and initialization of the different mutex
static pthread_mutex_t compute_pos_mux = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t print_pos_mux   = PTHREAD_MUTEX_INITIALIZER;

void * compute_position(void * arg){
    int i = 0;
    while(i < 10){
        //locks its own mutex
        pthread_mutex_lock(&compute_pos_mux);

	int * signals_power = (int *) arg;
        basic_position(signals_power, &pos);
	printf("**************************\nCompute\n**************************\n");

	//release the mutex for printing
        pthread_mutex_unlock(&print_pos_mux);
	i++;
    }
    pthread_exit(NULL);
}

void * print_position(void * arg){

    (void) arg;
    int i = 0;
    while(i < 10){

        //locks its own mutex
        pthread_mutex_lock(&print_pos_mux);

	printf("Print Angle : %d \n Distance : %d \n", pos.angle, pos.distance);
        
        //release the calculating mutex
        pthread_mutex_unlock(&compute_pos_mux);

	i++;
    }
    pthread_exit(NULL);
}

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
        pthread_mutex_lock(&print_pos_mux);
	//send the move command
	printf("Move toward : Angle : %d \n Distance : %d \nElapsed time : %ld\n", pos.angle, pos.distance, elapsed_time);
        printf("sec: %ld usec : %ld\n", tv.tv_sec, tv.tv_usec);
        //release the calculating mutex
        pthread_mutex_unlock(&compute_pos_mux);

        gettimeofday(&old_tv, NULL);
	elapsed_time=0;

	i++;
    }
    pthread_exit(NULL);
}

int main ()
{
    int signal [8] = {128, 255, 98, 3, 5, 0, -5, -2};

    //declaration of the different threads
    pthread_t thread_position;
    pthread_t thread_print_position;

    //immediate lock of the mutex printing the position so first we calculate it at start
    pthread_mutex_lock(&print_pos_mux);

    //creation of the thread calculating the position of the beacon
    if(pthread_create(&thread_position, NULL, compute_position, signal) == -1) {
	printf("pthread_create position fail");
    }

    //creation of the thread tracking the position of the beacon
    if(pthread_create(&thread_print_position, NULL, track_position, NULL) == -1) {
	printf("pthread_create position fail");
    }
    
    //********************************************************************
    //********************************************************************

    //simulation of the drone getting in line with a beacon
    //********************************************************************
    //********************************************************************

    //waiting for the threads to finish before closing the main
    pthread_join(thread_position, NULL);
    pthread_join(thread_print_position, NULL);
    
    //destroy the mutex before closing the main
    pthread_mutex_destroy(&compute_pos_mux);
    pthread_mutex_destroy(&print_pos_mux);

    return 0;
}
	
