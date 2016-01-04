#include "./../API/track_position.h"
#include "./../../../embedded-sw/serial.h"

#include "./../API/common.h"

extern int keepRunning;

//handler for a signal
void intHandlerThread1(int sig){
	keepRunning=0;
	printf("thread 1 : find position\n");
}

//position of each receiver embedded on the drone
//angle with the back-to-front axis
static int angle_step = 45 ;
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

/*
 * @brief	Compute source position from the array of signals strengths
 * @param	array 	pointer to the array of strengths
 * @param	angle 	the computed angle between -180° and +180°
 * @param	distance The computed distance in cm
 * @preturn	1 if an emitter has actually been detected, 
  *			0 else
 */
int find_pos(unsigned int* array, int* angle, int* distance)
{
	int result = 0;
	int maxIndex, indexLeft, indexRight;
	unsigned long strengthSum = 0;

	result = find_maximum(array, &maxIndex);

	if(result)
	{
		// Compute mean angle with three receivers (the max and both aside)
		//---------------------------------------------------------------------

		// Compute sum of all strengths
		indexLeft = (maxIndex-1 >= 0) ? maxIndex-1 : SIZE_ARRAY-1;
		indexRight = (maxIndex+1 < SIZE_ARRAY) ? maxIndex+1 : 0;
		strengthSum = array[maxIndex];
		strengthSum +=  array[indexRight];
		strengthSum += array[indexLeft];

		// Compute angle thanks to a weighted average (sum of weights is equal to 1)
		*angle = (((float)array[maxIndex])/strengthSum) * receiver_position[maxIndex];
		*angle += (((float)array[indexRight])/strengthSum) * (receiver_position[maxIndex]+angle_step);
		*angle += (((float)array[indexLeft])/strengthSum) * (receiver_position[maxIndex]-angle_step);
		
		if (*angle>180) *angle -= 360 ;
		//if (*angle<-180) *angle += 360 ;

		// Compute distance
		//--------------------------------------------
		///@todo to improve
		*distance = (float)((float)(MAX_STRENGTH_DISTANCE - MIN_STRENGTH_DISTANCE)/(MAX_STRENGTH - MIN_STRENGTH)) * array[maxIndex];

	}
	return result;
}

//finds the receiver with the maximum value
//signals_power is an array containing the signal value on each receiver
int basic_position(unsigned int * signals_power, t_position * pos_aux)
{
	int angle = 0;
	int distance = 0;
    int result = 0;
    
    result = find_pos(signals_power, &angle, &distance);
    
    if(result)
    {
    	(*pos_aux).angle = angle;
    	(*pos_aux).distance = distance;
	}
	else
	{
		(*pos_aux).angle = 0;
    	(*pos_aux).distance = 0; // WARNING TO BE CHANGED
	}
	/*
    int rcv_max = 0;
    int result = 0;
    
    result = find_pos(signals_power, &rcv_max);
    
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
	*/
    return 0;
}

//function designed to be the main of a thread
//put the position of the beacon in shared variable pos
void * compute_position(void * arg){
    int i = 0;
    unsigned int signals_power [8] = {0, 0, 0, 0, 0, 0, 0, 0};
    
    //handle the ctrl -c to make the drone land
	struct sigaction act;
	memset(&act,0,sizeof(act));
	act.sa_handler = intHandlerThread1;
	sigaction(SIGINT, &act, NULL);
   
    //init to read serial port
    int fd = serial_init("/dev/ttyACM0");
	if (fd == -1)
		exit(1);
	
    //while(i < 10){
    while(keepRunning){
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
