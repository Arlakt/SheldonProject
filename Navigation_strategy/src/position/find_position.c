#include "./../API/track_position.h"
#include "./../../../embedded-sw/serial.h"

//position of each receiver embedded on the drone
//angle with the back-to-front axis
//front : 0 ; back : 180 ; right : 90 ; left : 270
static int receiver_position[SIZE_ARRAY] = {-90, -45, 0, 45, 90, 135, 180, -135};

//finds the receiver receiving the maximum signal
//returns 1 if there is actually a result greater than the minimum threshold
int find_maximum(unsigned int * signals_power, int* max)
{
    int result = 0; //error = 0
    unsigned int maxValue = MIN_STRENGTH_TO_DETECT;
    int i=0;
    
    for(i=0; i<SIZE_ARRAY; i++)
    {
		if (signals_power[i] > maxValue)
		{
	    	*max = i;
	    	maxValue = signals_power[i];
	    	result = 1;
    	}
    }
    return result;
}

//finds the receiver with the maximum value
//signals_power is an array containing the signal value on each receiver
int basic_position(unsigned int * signals_power, t_position * pos_aux)
{
    int rcv_max = 0;
    int result = 0;
    
    result = find_maximum(signals_power, &rcv_max);
    
    if(result)
    {
    	(*pos_aux).angle = receiver_position[rcv_max];
    	(*pos_aux).distance = signals_power[rcv_max];
	}
	else
	{
		(*pos_aux).angle = 0;
    	(*pos_aux).distance = 0; // WARNING TO BE CHANGED
	}
    return 0;
}

//compute the exact angle
//signals_power is an array containing the signal value on each receiver
/*
int exact_position(int * signals_power, t_position * pos_aux)
{
    return 0;
}*/

//function designed to be the main of a thread
//put the position of the beacon in shared variable pos
void * compute_position(void * arg){
    int i = 0;
    unsigned int signals_power [8] = {0, 0, 0, 0, 0, 0, 0, 0};
   
    //init to read serial port
    int fd = serial_init("/dev/ttyACM0");
	if (fd == -1)
		exit(1);
	
    //while(i < 10){
    while(1){
        //locks its own mutex
        pthread_mutex_lock(&compute_pos_mux);
        
        //***************************************
        // either A or B
        //***************************************
        
        //A - signal provenant de la board
        
        serial_get_data(fd, signals_power);
        
        //***************************************
        //B - mock signal generated in test_pos.c
		//signals_power = (unsigned int *) arg;

        basic_position(signals_power, &pos);
                
        //
		//release the mutex for printing
        pthread_mutex_unlock(&track_pos_mux);
		i++;
    }
    pthread_exit(NULL);
}
