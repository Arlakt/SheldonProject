#ifndef FIND_POSITION_H
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

#include <serial/serial.h>
#include <signal.h> // for signals handling
#include <string.h> // for memset function

#define SIZE_ARRAY 8
#define MIN_STRENGTH_TO_DETECT	200	// Minimum strength to affirm that a signal is received

#define MAX_STRENGTH_DISTANCE	15 //in cm
#define MIN_STRENGTH_DISTANCE	300 //in cm

#define MAX_STRENGTH 	0xFFFF
#define MIN_STRENGTH   	1000

#define DISTANCE_HISTORY_SIZE 8

typedef struct _position{
    int angle; //in degrees, modulo 360
    int distance; //in meter
    int signalDetected; // boolean to know if a signal has been detected
} t_position;

//shared variable of position of the beacon
extern t_position pos;

//finds the receiver with the maximum value
//signals_power is an array containing the signal value on each receiver
int basic_position(unsigned int * signals_power, t_position * pos);

//function designed to be the main of a thread
//put the position of the beacon in shared variable pos
void * compute_position(void * arg);

#endif // FIND_POSITION_H
