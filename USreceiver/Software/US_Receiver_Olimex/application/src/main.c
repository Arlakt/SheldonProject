/**
	* @file main.c
	* @brief Main function core
	*
	*      This file contains the main function that initializes, configurates
	* 			and start all applications with their services. 
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

#include "typesAndconstants.h"
#include "stm32f10x.h"
#include "Clock.h"
#include "serialFrame.h"
#include "usbComm.h"
#include "sampleAcquisition.h"
	
/******************************************************************************
	*
	*   PRIVATE FUNCTIONS
	*
	*****************************************************************************/
	
	

/******************************************************************************
	*
	*   MAIN FUNCTION
	*
	*****************************************************************************/

int main (void)
{

	/******************
	 * Main variables *
	 ******************/
	uint8_t frame[50] = {0};
	uint16_t size = 0;
	
	uint16_t signalStrengths[8] = {1,2,3,4,5,6,7,8};
	
	/*******************
	 * Initializations *
	 *******************/
	
	
	// Clock
	CLOCK_Configure();		// Initialization of the whole clock tree
												// See clock_conf.h for more info on current config

	// USB communication
	usbCommInit();	

	// Signals filtering
	//signalTraitementInit();
	
	// Signals acquisition
	//sampleAcquisitionInit();
	//sampleAcquisitionInit();
	
	/***********
	 * Process *
	 ***********/

	
	while(1)
	{
		usbCommWaitInput();
		createSerialFrameForSignalStrengths(frame, signalStrengths, 8, &size);
		usbCommSendData(frame, size);
	}

	return 0;
}


