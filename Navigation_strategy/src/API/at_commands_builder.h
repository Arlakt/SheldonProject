#ifndef AT_COMMANDS_H
#define AT_COMMANDS_H

#ifndef BASIC
#define BASIC
#include <stdlib.h>
#include <stdio.h>
#endif

typedef enum
{
    ARDRONE_CALIBRATION_DEVICE_MAGNETOMETER = 0,
    ARDRONE_CALIBRATION_DEVICE_NUMBER,
} ardrone_calibration_device_t;

typedef struct pcmd
{
    int progressive;		//[flag]
    float rollTilt;         //[roll]
    float pitchTilt;		//[pitch]
    float verticalSpeed;	//[gaz]
    float angularSpeed;     //[yaw]
} pcmd_t;

int convert_float(float a);

char *at_ref(char *buf, int seq, int control);
char *at_pcmd(char *buf, int seq, pcmd_t pcmd);
char *at_ftrim(char *buf, int seq);
char *at_calib(char *buf, int seq, ardrone_calibration_device_t id);
char *at_config(char *buf, int seq, const char *name, const char *value);
char *at_config_ids(char *buf, int seq, const char *sessionId, const char *userId, const char *appId);
char *at_comwdg(char *buf);

#endif // AT_COMMANDS_H
