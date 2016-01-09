/**
	* @file signalProcessing.h
	* @brief 
	*
	* @author Romain TAPREST
	* @date 09 jan 2016
	*/


#ifndef SIGNAL_PROCESSING_H
#define SIGNAL_PROCESSING_H


 /******************************************************************************
	* 
	*   INCLUDED FILES
	*
	*****************************************************************************/
#include "typesAndConstants.h"

 /******************************************************************************
	* 
	*   TYPES AND CONSTANTS
	*
	*****************************************************************************/

#define NB_OF_SIGNALS	8
#define SIGNAL_THEORICAL_AVERAGE	2048	// Half of the saturation value (half of 12 bits for this ADC)
#define SIGNAL_EXPERIMENTAL_AVERAGE 2250
#define EMITTER_SIGNAL_DIVISION	4	// Division to make emitter pulse width. 
																	// Example : 	duty cycle of 25% ==> EMITTER_SIGNAL_DIVISION = 4
																	//						duty cycle of 50% ==> EMITTER_SIGNAL_DIVISION = 2


typedef struct
{
	uint16_t signalsMaximum[NB_OF_SIGNALS]; 	// Constantly updated values
	uint32_t numberOfSamples;									// Number of samples used to compute strengths so far
}t_signalsData;
	
	
	
 /******************************************************************************
	*
	*   PUBLIC FUNCTIONS
	*
	*****************************************************************************/

void sProcInit(void);

void sProcUpdateSignalMaximum(uint16_t *adcSamplesBuffer);

void sProcGetsignalsMaximumValues(uint16_t array[], uint8_t* size);


#endif
