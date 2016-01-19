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

extern int num_seq;
int convert_float(float a);

char *at_ref(char *buf, int control);
char *at_pcmd(char *buf, pcmd_t pcmd);
char *at_ftrim(char *buf);
char *at_calib(char *buf, ardrone_calibration_device_t id);
char *at_config(char *buf, const char *name, const char *value);
char *at_config_ids(char *buf, const char *sessionId, const char *userId, const char *appId);
char *at_comwdg(char *buf);
char *at_ctrl(char *buf, int x);

#endif // AT_COMMANDS_H
