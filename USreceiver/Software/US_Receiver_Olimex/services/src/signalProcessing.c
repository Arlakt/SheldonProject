/**
	* @file signalProcessing.c
	* @brief 
	*
	*
	* @author Romain TAPREST
	* @date 26 nov 2015
	*/
	
	/******************************************************************************
	* 
	*   INCLUDED FILES
	*
	*****************************************************************************/
	
	#include "typesAndConstants.h"
	#include "signalProcessing.h"
	
	
	/******************************************************************************
	* 
	*   VARIABLES
	*
	*****************************************************************************/

// Global variable used to compute signals strength
t_signalsData g_signalData = {{0}, 0};

/******************************************************************************
	*
	*   PRIVATE FUNCTIONS
	*
	*****************************************************************************/
	
		
	/******************************************************************************
	*
	*   PUBLIC FUNCTIONS
	*
	*****************************************************************************/

void sProcInit(void)
{
	g_signalData.numberOfSamples = 0;
}


void sProcUpdateSignalStrength(uint16_t *adcSamplesBuffer)
{
	uint8_t i=0;
	uint16_t tempValue=0;
	int16_t ajustedSample = 0;
	// Make a copy to avoid an update of this global variable during this process
	uint32_t currentNumberOfSamples = g_signalData.numberOfSamples;
	uint16_t adcSamples[NB_OF_SIGNALS];
	
	for(i=0;i<NB_OF_SIGNALS;i++)
		adcSamples[i] = adcSamplesBuffer[i];
	
	// This condition should never happen, but this is a protection against division by zero
	// For instance, it can happen if there is an overflow (in normal use, numberOfSamples can't get so high)
	if(currentNumberOfSamples == 0)
		currentNumberOfSamples = 1;
	
	for(i=0;i<NB_OF_SIGNALS;i++)
	{
		// Remove average to center value on zero
		ajustedSample = (int16_t)((int32_t)adcSamples[i] - (int32_t)SIGNAL_THEORICAL_AVERAGE);
		
		// Compute square and reduce value to 8 bits (max = 2048*2048 = 22 bits ==> SHR 6 to have 16 bits)
		tempValue = (uint16_t)( (uint32_t)( (int32_t)ajustedSample*(int32_t)ajustedSample ) >> 6 );
		
		// Updating signal strength (moving average)
		g_signalData.signalsStrength[i] = \
			(uint16_t)((uint64_t)( (uint64_t)(currentNumberOfSamples - 1)* (uint64_t)(g_signalData.signalsStrength[i]) + tempValue) / (uint64_t)currentNumberOfSamples );
	}
}

/**
	* @brief	Get a copy of the current values of signals strength
	* @param	array	Pointer to the array in which values will be copied
	*					SIZE MUST BE AT LEAST = NB_OF_SIGNALS
	* @param	size	copied size
	*/
void sProcGetSignalsStrengthValues(uint16_t array[], uint8_t* size)
{
	uint8_t i=0;
	
	for(i=0;i<NB_OF_SIGNALS;i++)
	{
		// Adjust value between 0 and 0xFFFF in function of the duty cycle of the emitter
		if( g_signalData.signalsStrength[i] > 0xFFFF/EMITTER_SIGNAL_DIVISION )
			g_signalData.signalsStrength[i] = 0xFFFF/EMITTER_SIGNAL_DIVISION;
		array[i] = g_signalData.signalsStrength[i] * EMITTER_SIGNAL_DIVISION;
	}
	
	*size = NB_OF_SIGNALS;
}

