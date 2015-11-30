#ifndef FIND_POSITION
#define FIND_POSITION_H

#include <stdlib.h>
#include <stdio.h>

typedef struct _position{
    int angle; //in degrees, modulo 360
    int distance; //in meter
} t_position;

//finds the receiver with the maximum value
//signals_power is an array containing the signal value on each receiver
t_position basic_position(int * signals_power);

#endif // FIND_POSITION_H
