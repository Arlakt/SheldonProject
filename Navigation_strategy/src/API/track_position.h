#ifndef TRACK_POSITION
#define TRACK_POSITION_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include "find_position.h"

//used to prototype track_position
//prints the position of the beacon anytime the thread is active
void * print_position(void * arg);

//function designed to be the main of a thread
//send the movement orders of the drone in order to follow the beacon
void * track_position(void * arg);

#endif // TRACK_POSITION_H
