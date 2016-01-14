#ifndef NAVDATA_H
#define NAVDATA_H

#include <stdint.h>

typedef float float32_t;

typedef struct {
    union {
        float v[3];
        struct {
            float x;
            float y;
            float z;
        };
    };
} vector31_t;

struct navdata_header {
    uint32_t		header;                     // Header: 0x55667788
    uint32_t		state;                      // State of the drone
    uint32_t		seq;                        // Sequence number
    uint32_t		vision;                     // Vision flag
};

enum {
    ARDRONE_FLY_MASK            = 1U << 0,  /*!< FLY MASK : (0) ardrone is landed, (1) ardrone is flying */
    ARDRONE_VIDEO_MASK          = 1U << 1,  /*!< VIDEO MASK : (0) video disable, (1) video enable */
    ARDRONE_VISION_MASK         = 1U << 2,  /*!< VISION MASK : (0) vision disable, (1) vision enable */
    ARDRONE_CONTROL_MASK        = 1U << 3,  /*!< CONTROL ALGO : (0) euler angles control, (1) angular speed control */
    ARDRONE_ALTITUDE_MASK       = 1U << 4,  /*!< ALTITUDE CONTROL ALGO : (0) altitude control inactive (1) altitude control active */
    ARDRONE_USER_FEEDBACK_START = 1U << 5,  /*!< USER feedback : Start button state */
    ARDRONE_COMMAND_MASK        = 1U << 6,  /*!< Control command ACK : (0) None, (1) one received */
    ARDRONE_CAMERA_MASK         = 1U << 7,  /*!< CAMERA MASK : (0) camera not ready, (1) Camera ready */
    ARDRONE_TRAVELLING_MASK     = 1U << 8,  /*!< Travelling mask : (0) disable, (1) enable */
    ARDRONE_USB_MASK            = 1U << 9,  /*!< USB key : (0) usb key not ready, (1) usb key ready */
    ARDRONE_NAVDATA_DEMO_MASK   = 1U << 10, /*!< Navdata demo : (0) All navdata, (1) only navdata demo */
    ARDRONE_NAVDATA_BOOTSTRAP   = 1U << 11, /*!< Navdata bootstrap : (0) options sent in all or demo mode, (1) no navdata options sent */
    ARDRONE_MOTORS_MASK         = 1U << 12, /*!< Motors status : (0) Ok, (1) Motors problem */
    ARDRONE_COM_LOST_MASK       = 1U << 13, /*!< Communication Lost : (1) com problem, (0) Com is ok */
    ARDRONE_SOFTWARE_FAULT      = 1U << 14, /*!< Software fault detected - user should land as quick as possible (1) */
    ARDRONE_VBAT_LOW            = 1U << 15, /*!< VBat low : (1) too low, (0) Ok */
    ARDRONE_USER_EL             = 1U << 16, /*!< User Emergency Landing : (1) User EL is ON, (0) User EL is OFF*/
    ARDRONE_TIMER_ELAPSED       = 1U << 17, /*!< Timer elapsed : (1) elapsed, (0) not elapsed */
    ARDRONE_MAGNETO_NEEDS_CALIB = 1U << 18, /*!< Magnetometer calibration state : (0) Ok, no calibration needed, (1) not ok, calibration needed */
    ARDRONE_ANGLES_OUT_OF_RANGE = 1U << 19, /*!< Angles : (0) Ok, (1) out of range */
    ARDRONE_WIND_MASK           = 1U << 20, /*!< WIND MASK: (0) ok, (1) Too much wind */
    ARDRONE_ULTRASOUND_MASK     = 1U << 21, /*!< Ultrasonic sensor : (0) Ok, (1) deaf */
    ARDRONE_CUTOUT_MASK         = 1U << 22, /*!< Cutout system detection : (0) Not detected, (1) detected */
    ARDRONE_PIC_VERSION_MASK    = 1U << 23, /*!< PIC Version number OK : (0) a bad version number, (1) version number is OK */
    ARDRONE_ATCODEC_THREAD_ON   = 1U << 24, /*!< ATCodec thread ON : (0) thread OFF (1) thread ON */
    ARDRONE_NAVDATA_THREAD_ON   = 1U << 25, /*!< Navdata thread ON : (0) thread OFF (1) thread ON */
    ARDRONE_VIDEO_THREAD_ON     = 1U << 26, /*!< Video thread ON : (0) thread OFF (1) thread ON */
    ARDRONE_ACQ_THREAD_ON       = 1U << 27, /*!< Acquisition thread ON : (0) thread OFF (1) thread ON */
    ARDRONE_CTRL_WATCHDOG_MASK  = 1U << 28, /*!< CTRL watchdog : (1) delay in control execution (> 5ms), (0) control is well scheduled */
    ARDRONE_ADC_WATCHDOG_MASK   = 1U << 29, /*!< ADC Watchdog : (1) delay in uart2 dsr (> 5ms), (0) uart2 is good */
    ARDRONE_COM_WATCHDOG_MASK   = 1U << 30, /*!< Communication Watchdog : (1) com problem, (0) Com is ok */
#if defined(__ARMCC_VERSION)
    /* ARMCC stores enums as signed 32-bit integers. 1<<31 is thus out of range, and must be defined as (-1) or its hexa representation
       Reference : http://infocenter.arm.com/help/topic/com.arm.doc.dui0436a/RVCT20_Errors_and_Warnings.pdf  (page 7) */
    ARDRONE_EMERGENCY_MASK      = (int)0x80000000  /*!< Emergency landing : (0) no emergency, (1) emergency */
#else
    ARDRONE_EMERGENCY_MASK      = 1U << 31  /*!< Emergency landing : (0) no emergency, (1) emergency */
#endif
};

struct navdata_option {
	uint16_t		tag;                        // Option identifier
	uint16_t		size;                       // Option size
};

struct navdata_demo {
    struct navdata_option header;
    uint32_t		ctrl_state;                 // Flying state (landed, flying, hovering, etc.) defined in CTRL_STATES enum. 
    uint32_t		vbat_flying_percentage;     // battery voltage filtered (mV) 
    float32_t		theta;                      // pitch angle in milli-degrees 
    float32_t		phi;                        // roll  angle
    float32_t		psi;                        // yaw   angle
    int32_t			altitude;                   // altitude in centimeters[??] 
    float32_t		vx;                         // estimated linear velocity
    float32_t		vy;                         // estimated linear velocity
    float32_t		vz;                         // estimated linear velocity
    uint32_t		num_frames;                 //!< streamed frame index  // Not used -> To integrate in video stage.
};

struct navdata_magneto {
    struct navdata_option header;
    int16_t			mx;
    int16_t			my;
    int16_t			mz;
    vector31_t		magneto_raw;
    vector31_t		magneto_rectified;
    vector31_t		magneto_offset;
    float			heading_unwrapped;
    float			heading_gyro_unwrapped;
    float			heading_fusion_unwrapped;
    char			magneto_calibration_ok;
    uint32_t		magneto_state;
    float			magneto_radius;
    float			error_mean;
    float			error_var;
};

typedef struct _navdata {
    struct navdata_header		header;
    struct navdata_demo			demo;
    struct navdata_magneto		magneto;
} navdata_t;

int navdata_init();
int navdata_start();
int navdata_get(navdata_t * data);

#endif
