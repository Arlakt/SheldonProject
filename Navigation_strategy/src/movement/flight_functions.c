#include "./../API/flight_functions.h"
// CONFIGURATION DU DRONE
//#define TYPE_DE_CIBLE "\"detect:detect_type\",\"12\""

//******************************
//CONFIGS
//******************************

//if wait != 0, wait after sending the message
//if wait = 0, don't wait
char *normal_flight(char *message, int sequence, int wait)
{
    at_config(message, sequence, "control:flying_mode", "0");
    if (send_message(message,wait) != 0)
        printf("[FAILED] Message sending failed\n");
    return message;
}

char *hover_mode(char *message, int sequence, int wait)
{
    at_config(message, sequence, "control:flying_mode", "1");
    if (send_message(message,wait) != 0)
        printf("[FAILED] Message sending failed\n");
    return message;
}

char *oriented_roundel_detection(char *message, int sequence, int wait)
{
    at_config(message, sequence, "detect:detect_type", "12");
    if (send_message(message,wait) != 0)
        printf("[FAILED] Message sending failed\n");
    return message;
}

char *front_cam_detecting(char *message, int sequence, int wait)
{
    at_config(message, sequence, "detect:detections_select_h", "3"); //3 roundel oriented; 8 black roundel 
    if (send_message(message,wait) != 0)
        printf("[FAILED] Message sending failed\n");
    return message;
}

 char *bottom_cam_detecting_full_speed(char *message, int sequence, int wait) //60 fps
{
    at_config(message, sequence, "detect:detections_select_v", "3"); //3 roundel oriented; 8 black roundel
    if (send_message(message,wait) != 0)
        printf("[FAILED] Message sending failed\n");
    return message;
}

char *bottom_cam_detecting_half_speed(char *message, int sequence, int wait) //30 fps
{
    at_config(message, sequence, "detect:detections_select_v_hsync", "3"); //3 roundel oriented; 8 black roundel
    if (send_message(message,wait) != 0)
        printf("[FAILED] Message sending failed\n");
    return message;
}


//******************************
//CONTROLS
//******************************

char *emergency(char *message, int sequence, int wait)
{
	at_ref(message, sequence, 290717952);
    if (send_message(message,wait) != 0)
    	printf("[FAILED] Message sending failed\n");
	return message;
}

char *anti_emergency(char *message, int sequence, int wait)
{
	at_ref(message, sequence, 290717696);
    if (send_message(message,wait) != 0)
    	printf("[FAILED] Message sending failed\n");
	return message;
}

char *set_trim(char *message, int sequence, int wait)
{
	//char message [512];
	
	at_ftrim(message, sequence);
    if (send_message(message,wait) != 0)
    	printf("[FAILED] Message sending failed\n");
	return message;
}

char *take_off(char *message, int sequence, int wait)
{
	at_ref(message, sequence, 290718208);
    if (send_message(message,wait) != 0)
    	printf("[FAILED] Message sending failed\n");
	return message;
}

char *landing(char *message, int sequence, int wait)
{
	//char message [512];
	
	at_ref(message, sequence, 290717696);
    if (send_message(message,wait) != 0)
    	printf("[FAILED] Message sending failed\n");
	return message;
}

char *turnaround(char *message, int sequence, int wait)
{
    at_config(message, sequence, "control:flight_anim", "6,5000");
    if (send_message(message,wait) != 0)
        printf("[FAILED] Message sending failed\n");
    return message;
}

char *flip_ahead(char *message, int sequence, int wait)
{
    at_config(message, sequence, "control:flight_anim", "16,15");
    if (send_message(message,wait) != 0)
        printf("[FAILED] Message sending failed\n");
    return message;
}

char *set_simple_move(char *message, int sequence, direction dir, float power, int wait)
{
	
	switch(dir)
	{
		case LEFT:
			message = set_complex_move(message,sequence,-(power),0,0,0,wait);
			break;

		case RIGHT:
			message = set_complex_move(message,sequence,power,0,0,0,wait);
			break;
            
		case FRONT:
			message = set_complex_move(message,sequence,0,-(power),0,0,wait);
			break;
		    
		case BACK:
			message = set_complex_move(message,sequence,0,power,0,0,wait);
			break;

		case DOWN:
			message = set_complex_move(message,sequence,0,0,-(power),0,wait);
			break;

		case UP:
			message = set_complex_move(message,sequence,0,0,power,0,wait);
			break;

		case ANTI_CLKWISE:
			message = set_complex_move(message,sequence,0,0,0,-(power),wait);
			break;

		case CLKWISE:
			message = set_complex_move(message,sequence,0,0,0,power,wait);
			break;

		default:
			printf("Error enum direction\n");
			break;
	}
	return message;
}

//each power MUST be within [-1;1]
char *set_complex_move(char *message, int sequence, float roll_power, float pitch_power, float vertical_power, float yaw_power, int wait)
{
	pcmd_t command;

	command.progressive=1;
	command.rollTilt=roll_power;
	command.pitchTilt=pitch_power;
	command.verticalSpeed=vertical_power;
	command.angularSpeed=yaw_power;

	at_pcmd(message, sequence, command);

	if (send_message(message,wait) != 0)
		printf("[FAILED] Message sending failed\n");

	return message;
}

char *reset_com(char *message, int wait)
{
	at_comwdg(message);
    if (send_message(message,wait) != 0)
    	printf("[FAILED] Message sending failed\n");

	return message;
}
	
			

	
