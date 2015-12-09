#ifndef TRACK_POSITION
#define TRACK_POSITION_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

#define SIZE_ARRAY 8

typedef struct _position{
    int angle; //in degrees, modulo 360
    int distance; //in meter
} t_position;

//shared variable of position of the beacon
t_position pos;

//mutex arbitrating computing and tracking
extern pthread_mutex_t compute_pos_mux;
extern pthread_mutex_t track_pos_mux;


//finds the receiver with the maximum value
//signals_power is an array containing the signal value on each receiver
int basic_position(int * signals_power, t_position * pos);

//function designed to be the main of a thread
//put the position of the beacon in shared variable pos
void * compute_position(void * arg);

#endif // TRACK_POSITION_H
