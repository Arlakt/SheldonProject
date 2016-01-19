#include "find_position.h"

t_position pos = {0, 100, 0};

extern int keepRunning;
extern pthread_mutex_t compute_pos_mux;
extern pthread_mutex_t track_pos_mux;

//position of each receiver embedded on the drone
//angle with the back-to-front axis
static int angle_step = 45 ;
//front : 0 ; back : 180 ; right : 90 ; left : 270
static int receiver_position[SIZE_ARRAY] = {-90, -45, 0, 45, 90, 135, 180, -135};

static int distanceHistory[DISTANCE_HISTORY_SIZE] = {0};
static int distHistPointer=0, distHistfull=0;

//handler for a signal
void intHandlerThread2(int sig){
	keepRunning=0;
	printf("CTRL+C signal in find_position\n");
}

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

/**
 * @brief	Compute the distance with a moving average
 *			keeping history of previous distances
 */
int computeMeanDistance(int currentDistance)
{
	int distance=0, i=0;

	distanceHistory[distHistPointer] = currentDistance;
	distHistPointer++;
	if(distHistPointer >= DISTANCE_HISTORY_SIZE)
	{
		distHistfull = 1;
		distHistPointer = 0;
	}

	// If the history has been fully filled, do average on the whole history
	if(distHistfull)
	{
		for(i=0;i<DISTANCE_HISTORY_SIZE;i++)
			distance += distanceHistory[i];
		distance /= DISTANCE_HISTORY_SIZE;
	}
	// If it is the first time buffer is fill, do not do average on the whole history
	else
	{
		for(i=0;i<distHistPointer;i++)
			distance += distanceHistory[i];
		if(i==0)
			distance = 0;
		else
			distance /= i;
	}
	return distance;
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
	int currentDistance = 0;

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

		// Compute distance
		//--------------------------------------------
		///@todo to improve
		currentDistance = (float)((float)(MAX_STRENGTH_DISTANCE - MIN_STRENGTH_DISTANCE)/(MAX_STRENGTH - MIN_STRENGTH)) * array[maxIndex] + MIN_STRENGTH_DISTANCE;
		*distance = computeMeanDistance(currentDistance);
		//*distance = currentDistance;

		// Only for distance calibration
		//printf("Puissance : %d - Distance : %d\n",array[maxIndex],*distance);
	}
	return result;
}

/**
 * @brief	Get emitter position in pos_aux thanks to data from signals_power
 *
 */
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
    	(*pos_aux).signalDetected = 1; // true	
	}
	else
	{
		(*pos_aux).signalDetected = 0; // false
	}
    return 0;
}


/**
 * @brief	Function designed to be the main of a thread
 * 			Compute emitter position and update it in the global variable
 */
void * compute_position(void * arg){
    unsigned int signals_power [8] = {0, 0, 0, 0, 0, 0, 0, 0};
   
    //init to read serial port
    int fd = serial_init("/dev/ttyACM0");
	if (fd == -1)
		exit(1);
	
	//handle the ctrl -c to make the drone land
	struct sigaction act;
	memset(&act,0,sizeof(act));
	act.sa_handler = intHandlerThread2;
	sigaction(SIGINT, &act, NULL);

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
                
        // unlock mutex for tracking thread
        pthread_mutex_unlock(&track_pos_mux);
    }
    pthread_exit(NULL);
}
