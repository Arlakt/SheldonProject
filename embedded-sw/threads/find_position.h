#ifndef FIND_POSITION
#define FIND_POSITION_H

#ifndef BASIC
#define BASIC
#include <stdlib.h>
#include <stdio.h>
#endif

#ifndef PTHREAD_N_TIME
#define PTHREAD_N_TIME
#include <pthread.h>
#include <sys/time.h>
#endif

#define SIZE_ARRAY 8
#define MIN_STRENGTH_TO_DETECT	50	// Minimum strength to affirm that a signal is received

#define MAX_STRENGTH_DISTANCE	50 //in cm
#define MIN_STRENGTH_DISTANCE	400 //in cm

#define MAX_STRENGTH 	0xFFFF
#define MIN_STRENGTH   	0

typedef struct _position{
    int angle; //in degrees, modulo 360
    int distance; //in meter
} t_position;

//shared variable of position of the beacon
extern t_position pos;

//mutex arbitrating computing and tracking
//extern pthread_mutex_t compute_pos_mux;
//extern pthread_mutex_t track_pos_mux;


//finds the receiver with the maximum value
//signals_power is an array containing the signal value on each receiver
int basic_position(unsigned int * signals_power, t_position * pos);

//function designed to be the main of a thread
//put the position of the beacon in shared variable pos
void * compute_position(void * arg);

#endif // FIND_POSITION_H
