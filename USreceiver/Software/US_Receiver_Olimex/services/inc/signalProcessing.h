/**
	* @file signalProcessing.h
	* @brief 
	*
	* @author Romain TAPREST
	* @date 26 nov 2015
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
#define SIGNAL_THEORICAL_AVERAGE	0x400/2	// Half of the saturation value (half of 10 bits for this ADC)

typedef struct
{
	uint16_t signalsStrength[NB_OF_SIGNALS]; 	// Constantly updated values
	uint8_t currentSignal;
	uint32_t numberOfSamples;									// Number of samples used to compute strengths so far
}t_signalsData;
	
	
	
 /******************************************************************************
	*
	*   PUBLIC FUNCTIONS
	*
	*****************************************************************************/

void sProcInit(void);

void sProcResetSignalData(void);

void sProcUpdateSignalStrength(uint16_t sample);

void sProcGetSignalsStrengthValues(uint16_t array[], uint8_t* size);


#endif
