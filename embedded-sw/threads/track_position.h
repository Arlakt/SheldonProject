#ifndef TRACK_POSITION_H
#define TRACK_POSITION_H

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

#include "find_position.h"
#include <movement/flight_functions.h>
#include <movement/UDP_sender.h>
#include <signal.h> // for signals handling
#include <string.h> // for memset function
#include <time.h>

#define ANGLE_PRECISION 10 // in degrees

void print_position(void);
void * track_position(void * arg);

#endif // TRACK_POSITION_H
