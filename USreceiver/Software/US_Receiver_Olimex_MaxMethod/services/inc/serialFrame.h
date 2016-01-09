/**
	* @file serialFrame.h
	*
	* @brief Serial frame definition and utilities
	*
	* @author Romain TAPREST
	* @date 09 jan 2016
	*/

#ifndef __SERIAL_FRAME__
#define __SERIAL_FRAME__

#include "typesAndConstants.h"


/* 
 * Special commands from/to PC
 */
#define RESET_COMMAND	'S'
#define START_OF_FRAME	0xFF


/*
*	-----------------------------------------------------------
*				public functions
*	-----------------------------------------------------------
*/

void createSerialFrameForSignalsMaximum(uint8_t frame[], uint16_t signalsMaximum[], uint8_t nbOfSignals, uint16_t *frameSize);


/*
*	-----------------------------------------------------------
*				private functions
*	-----------------------------------------------------------
*/

uint16_t createCRC(char * tab, uint8_t tabLen);


#endif
