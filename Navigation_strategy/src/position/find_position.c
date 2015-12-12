#include "./../API/track_position.h"

//position of each receiver embedded on the drone
//angle with the back-to-front axis
//front : 0 ; back : 180 ; right : 90 ; left : 270
static int receiver_position[SIZE_ARRAY] = {0, 45, 90, 135, 180, 225, 270, 315};

//finds the receiver receiving the maximum signal
int find_maximum(unsigned int * signals_power)
{
    int max = 0;
    for(int i=1; i<SIZE_ARRAY; i++)
    {
	if (signals_power[i] > signals_power[max])
	    max = i;
    }
    return max;
}

//finds the receiver with the maximum value
//signals_power is an array containing the signal value on each receiver
int basic_position(unsigned int * signals_power, t_position * pos_aux)
{
    int rcv_max = find_maximum(signals_power);
    (*pos_aux).angle = receiver_position[rcv_max];
    (*pos_aux).distance = signals_power[rcv_max];

    return 0;
}
/*
//compute the exact angle
//signals_power is an array containing the signal value on each receiver
int exact_position(int * signals_power, t_position * pos_aux)
{
    int rcv_max = find_maximum(signals_power);
    int right_rcv = retrieve_right(rcv_max);
    int left_rcv = retrieve_left(rcv_max);
    //sum of values of : max receiver signal power + left receiver of the max + right of the max
    int total_power = signals_power[rcv_max]+signals_power[right_rcv]+signals_power[left_rcv];
    int angle_delta;
    (*pos_aux).angle = receiver_position[angle_max];
    (*pos_aux).distance = signals_power[angle_max];
    return 0;
}*/

//function designed to be the main of a thread
//put the position of the beacon in shared variable pos
void * compute_position(void * arg){
    int i = 0;
    //while(i < 10){
    while(1){
        //locks its own mutex
        pthread_mutex_lock(&compute_pos_mux);

	unsigned int * signals_power = (unsigned int *) arg;
        basic_position(signals_power, &pos);
	printf("**************************\nCompute\n**************************\n");

	//release the mutex for printing
        pthread_mutex_unlock(&track_pos_mux);
	i++;
    }
    pthread_exit(NULL);
}
