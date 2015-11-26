/**
	* @file sampleAcquisition.h
	* @brief ADC acquisition of an ultrasound sample
	*
	*     This file contains the service for acquire sample from the ADC.
	*			Initialization, configuration and start. The status LED toggles at each valid sample saved.
	*
	*
	* @author Romain TAPREST
	* @date 26 nov 2015
	*/


#ifndef S_SAMPLEACQUISITION_H
#define S_SAMPLEACQUISITION_H


/******************************************************************************
	* 
	*   INCLUDED FILES
	*
	*****************************************************************************/


#include <stdint.h>


/******************************************************************************
	* 
	*   CONSTANTS
	*
	*****************************************************************************/

#define ADC1_DR_Address    	((uint32_t)0x4001244C)
#define TIM1_CCR1_Address  	((uint32_t)0x40012C34)


#define SIGNAL_BUFFER_SIZE	2048
#define SIGNAL_HALF_BUFFER_SIZE 		(SIGNAL_BUFFER_SIZE/2)

extern uint16_t adcBuffer[SIGNAL_BUFFER_SIZE];		// Static buffer to save a signal
extern uint16_t idDataToProcess;


/******************************************************************************
	*
	*   PUBLIC FUNCTIONS
	*
	*****************************************************************************/


/**
	* @brief	Init the sampling routine.
	*					Blocking function.
	*/
void sampleAcquisitionInit( void );



#endif					/* S_SERIALCOMM_HS_SAMPLEACQUISITION_H */
