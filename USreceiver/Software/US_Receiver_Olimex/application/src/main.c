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
#include "signalProcessing.h"
	
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
	
	/*******************
	 * Initializations *
	 *******************/
	
	
	// Clock
	CLOCK_Configure();		// Initialization of the whole clock tree
												// See clock_conf.h for more info on current config

	// USB communication
	usbCommInit();	

	// Signals processing
	sProcInit();
	
	// Signals acquisition
	sampleAcquisitionInit();
	
	/***********
	 * Process *
	 ***********/

	
	while(1)
	{
		
	}

	return 0;
}


