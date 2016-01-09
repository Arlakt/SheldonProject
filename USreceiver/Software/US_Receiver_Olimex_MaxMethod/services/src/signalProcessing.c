/**
	* @file signalProcessing.c
	* @brief 
	*
	*
	* @author Romain TAPREST
	* @date 09 jan 2016
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

// Global variable used to compute signals max
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
	uint8_t i=0;
	
	g_signalData.numberOfSamples = 0;
	
	for(i=0;i<NB_OF_SIGNALS;i++)
		g_signalData.signalsMaximum[i] = 0;
}


void sProcUpdateSignalMaximum(uint16_t *adcSamplesBuffer)
{
	uint8_t i=0;
	uint16_t ajustedSampleAbsolute=0;
	int16_t ajustedSample = 0;
	// Make a copy to avoid an update of this global variable during this process
	uint16_t adcSamples[NB_OF_SIGNALS];
	
	for(i=0;i<NB_OF_SIGNALS;i++)
		adcSamples[i] = adcSamplesBuffer[i];
	
	for(i=0;i<NB_OF_SIGNALS;i++)
	{
		// Remove average to center value on zero
		ajustedSample = (int16_t)adcSamples[i] - SIGNAL_EXPERIMENTAL_AVERAGE; // SIGNAL_EXPERIMENTAL_AVERAGE or SIGNAL_THEORICAL_AVERAGE
		
		// Take absolute value
		if(ajustedSample < 0)
			ajustedSampleAbsolute = -ajustedSample;
		else
			ajustedSampleAbsolute = ajustedSample;
		
		// Updating maximum for this signal
		if(ajustedSampleAbsolute > g_signalData.signalsMaximum[i])
			g_signalData.signalsMaximum[i] = ajustedSampleAbsolute;
	}
}

/**
	* @brief	Get a copy of the current values of signals maximum
	* @param	array	Pointer to the array in which values will be copied
	*					SIZE MUST BE AT LEAST = NB_OF_SIGNALS
	* @param	size	copied size
	*/
void sProcGetsignalsMaximumValues(uint16_t array[], uint8_t* size)
{
	uint8_t i=0;
	
	for(i=0;i<NB_OF_SIGNALS;i++)
	{
		array[i] = g_signalData.signalsMaximum[i];
	}
	
	*size = NB_OF_SIGNALS;
}

