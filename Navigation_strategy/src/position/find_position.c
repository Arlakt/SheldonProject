#include "./../API/find_position.h"

#define SIZE_ARRAY 8

//position of each receiver embedded on the drone
//angle with the back-to-front axis
//front : 0 ; back : 180 ; right : 90 ; left : 270
int receiver_position[SIZE_ARRAY] = {0, 45, 90, 135, 180, 225, 270, 315};

int find_maximum(int * signals_power)
{
    int max = 0;
    for(int i=1; i<SIZE_ARRAY; i++)
    {
	if (signals_power[i] > signals_power[max])
	    max = i;
    }
    return max;
}

t_position basic_position(int * signals_power)
{
    t_position pos;

    int angle_max = find_maximum(signals_power);
    pos.angle = receiver_position[angle_max];
    pos.distance = signals_power[angle_max];

    return pos;
}
