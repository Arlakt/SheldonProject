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
static t_signalsData g_signalData;

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
	sProcResetSignalData();
}

void sProcResetSignalData(void)
{
	uint8_t i=0;
	
	g_signalData.currentSignal = 0;
	g_signalData.numberOfSamples = 0;
	for(i=0;i<NB_OF_SIGNALS;i++)
		g_signalData.signalsStrength[i] = 0;
}

void sProcUpdateSignalStrength(uint16_t sample)
{
	uint16_t tempValue=0;
	
	// Updating number of samples
	if(g_signalData.currentSignal == 0)
		(g_signalData.numberOfSamples)++;
	
	// This condition should never happen, but this is a protection against division by zero
	// For instance, it can happen if there is an overflow (in normal use, numberOfSamples can't get so high)
	if(g_signalData.numberOfSamples == 0)
		g_signalData.numberOfSamples = 1;
	
	// Remove average to center value on zero
	sample -= SIGNAL_THEORICAL_AVERAGE;
	
	// Compute square and reduce it on 16 bits by truncature of 4 LSB
	tempValue = (uint16_t)( (uint32_t)( (uint32_t)sample*(uint32_t)sample ) >> 4 );
	
	// Updating signal strength (moving average)
	g_signalData.signalsStrength[g_signalData.currentSignal] = ( (g_signalData.numberOfSamples - 1)* g_signalData.signalsStrength[g_signalData.currentSignal] + tempValue ) / g_signalData.numberOfSamples ;
	
	// Updating signal number
	(g_signalData.currentSignal)++;
	if(g_signalData.currentSignal >= NB_OF_SIGNALS)
		g_signalData.currentSignal = 0;
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
		array[i] = g_signalData.signalsStrength[i];
	
	*size = NB_OF_SIGNALS;
}

