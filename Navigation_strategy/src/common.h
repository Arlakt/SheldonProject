#ifndef COMMON_H
#define COMMON_H

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

#endif
