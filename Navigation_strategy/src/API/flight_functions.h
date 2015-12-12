#ifndef __MESAGE_DRONE_H
#define __MESAGE_DRONE_H

#ifndef BASIC
#define BASIC
#include <stdlib.h>
#include <stdio.h>
#endif

#include "at_commands_builder.h"
#include "UDP_sender.h"

typedef enum
{
	LEFT,
	RIGHT,
	FRONT,
	BACK,
	UP,
	DOWN,
	ANTI_CLKWISE,
	CLKWISE
}direction;

//******************************
//CONFIGS
//******************************
char *normal_flight(char *message, int sequence, int wait);
char *hover_mode(char *message, int sequence, int wait);
char *oriented_roundel_detection(char *message, int sequence, int wait);
char *front_cam_detecting(char *message, int sequence, int wait);
char *bottom_cam_detecting_full_speed(char *message, int sequence, int wait); //60 fps
char *bottom_cam_detecting_half_speed(char *message, int sequence, int wait); // 30 fps

//******************************
//CONTROLS
//******************************

char *emergency(char *message, int sequence, int wait);
char *anti_emergency(char *message, int sequence, int wait);
char *set_trim(char *message, int sequence, int wait);
char *take_off(char *message, int sequence, int wait);
char *landing(char *message, int sequence, int wait);
char *turnaround(char *message, int sequence, int wait);
char *flip_ahead(char *message, int sequence, int wait);
char *set_simple_move(char *message, int sequence, direction dir, float power, int wait);
char *set_complex_move(char *message, int sequence, float roll_power, float pitch_power, float vertical_power, float yaw_power, int wait);
char *reset_com(char *message, int wait);
	


#endif

