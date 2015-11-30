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
int basic_position(int * signals_power, t_position * pos);

#endif // FIND_POSITION_H
