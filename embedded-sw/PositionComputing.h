/**
  * @file 	PositionComputing.h
  * @brief 	This file defines elements used for position computing function 
  *         thanks to ultrasound receiver data.
  */

#ifndef POSITION_COMPUTING_H
#define POSITION_COMPUTING_H

//--------------------------------------------
//		DEFINES
//--------------------------------------------

#define NB_OF_RECEIVERS		8

#define ANGLE_BETWEEN_RECEIVERS		360/NB_OF_RECEIVERS		// In degrees

#define MIN_STRENGTH_TO_DETECT	50	// Minimum strength to affirm that a signal is received
#define MAX_STRENGTH_POSSIBLE	0xFFFF	// Maximum strength that can be received when emitter is very close

#define DIST_FOR_MIN_VALUE	200	// distance in centimeters if signal strength = MIN_STRENGTH_TO_DETECT
#define DIST_FOR_MAX_VALUE	40	// distance in centimeters if signal strength = MAX_STRENGTH_POSSIBLE

//--------------------------------------------
//		PUBLIC FUNCTIONS
//--------------------------------------------

/**
  * @brief	Get emitter distance from the drone, and angle relative to the front of the drone
  *			computed thanks to signals strength sent by the receiver board.
  * @param[in]	signalsStrength	Array containing every signals strength
  * @param[out]	angle			The computed angle (between -180° and +180°, counter-clock wise (trigo))
  * @param[out] distance		The computed distance (en cm)
  * return	1 if an emitter has actually been detected
  *			0 if no signal strength is strong enough to affirm that there is an emitter around the drone
  */
unsigned int getEmitterPosition(unsigned int signalsStrength[], int *angle, unsigned int *distance);



#endif
