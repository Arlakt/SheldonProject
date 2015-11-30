/**
  * @file 	PositionComputing.c
  * @brief 	This file implementes elements used for position computing function 
  *         thanks to ultrasound receiver data.
  */

//--------------------------------------------
//		INCLUDES
//--------------------------------------------
#include "PositionComputing.h"

//--------------------------------------------
//		VARIABLES
//--------------------------------------------
static float computeDistanceCoeff =  (DIST_FOR_MAX_VALUE - DIST_FOR_MIN_VALUE) / (MAX_STRENGTH_POSSIBLE - MIN_STRENGTH_TO_DETECT);

//--------------------------------------------
//		PRIVATE FUNCTIONS
//--------------------------------------------

unsigned int getMaxInArray(unsigned int array[], unsigned int size)
{
	unsigned int i;
	unsigned int maxValue = 0;
	unsigned int maxIndex = 0;

	for(i=0;i<size;i++)
	{	
		if(array[i] >= maxValue)
		{
			maxValue = array[i];
			maxIndex = i;
		}		
	}
	return maxIndex;
}


//--------------------------------------------
//		PUBLIC FUNCTIONS
//--------------------------------------------

unsigned int getEmitterPosition(unsigned int signalsStrength[], int *angle, unsigned int *distance)
{
	unsigned int maxIndex = 0;
	
	// Get max strength signal 
	maxIndex = getMaxInArray(signalsStrength, NB_OF_RECEIVERS);
	
	// If max strength not high enough, no signal detected
	if(signalsStrength[maxIndex] < MIN_STRENGTH_TO_DETECT)
		return 0;

	// Compute angle
	if(maxIndex <= 4)
		*angle = ANGLE_BETWEEN_RECEIVERS * maxIndex;
	else
		*angle = - ANGLE_BETWEEN_RECEIVERS * (NB_OF_RECEIVERS-maxIndex);

	// Compute distance
	*distance = computeDistanceCoeff * signalsStrength[maxIndex];

	return 1;
}


