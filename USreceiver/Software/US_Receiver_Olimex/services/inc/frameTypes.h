/**
 * \file frameTypes.h
 *
 * \brief Frame types definition
 *
 * \author Yohan Marchiset
 *
 * \date 28 Oct 2014
 */

#ifndef __FRAME_TYPES__
#define __FRAME_TYPES__

/**
 * \brief Time frame type
 */
#define TIME_FRAME 't'
/**
 * \brief Distance frame type
 */
#define DISTANCE_FRAME 'd'
/**
 * \brief Position frame type
 */
#define POSITION_FRAME 'p'
/**
 * \brief Command frame type
 */
#define COMMAND_FRAME 'c'
/**
 * \brief Discovery frame type
 */
#define DISCOVERY_FRAME 'D'
/**
 * \brief Mission frame type
 */
#define MISSION_FRAME 'M'
/**
 * \brief PC in control mode
 */
#define PC_CTRL 'P'
/**
 * \brief Drone in control mode (selfcontrol)
 */
#define DRONE_CTRL 'S'


/**
 * \brief Mission state : Launched
 */
#define LAUNCH_MISSION  'L'

/**
 * \brief Mission state : Stopped
 */
#define STOP_MISSION  'S'

/**
 * \brief Mission state : Finished
 */
#define MISSION_FINISHED  'F'

/**
 * \brief Mission state : none
 */
#define MISSION_NONE  'N'


#endif
