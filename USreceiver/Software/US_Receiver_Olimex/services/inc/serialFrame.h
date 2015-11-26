/**
 * \file serialFrame.h
 *
 * \brief Serial frame definition and utilities
 *
 * \author Yohan Marchiset
 *
 * \date 28 Oct 2014
 */

#ifndef __SERIAL_FRAME__
#define __SERIAL_FRAME__

#include <stdint.h>
#include "frameTypes.h"

/**
 * \brief Converted size of an serial frame in bytes
 */
#define CONVERTED_SERIAL_FRAME_SIZE 37
/**
 * \brief Maximum sequence number for the serial frames
 */
#define MAX_SERIAL_SEQ_NUM 50
/**
 * \brief Synchronization characters
 */
#define SYNC_CHAR_1 'S'
#define SYNC_CHAR_2 'D'
/**
 * \brief Current serial frame sequence number
 */
extern uint16_t currentSerialSeqNum;

/**
 * \brief Structure used a frame for serial communication
 */
typedef struct serialFrame{
	uint16_t seqNum;	/**< Sequence number of the frame */
	char type;			/**< Type of the frame, can be either d or t */
	int32_t data[4];	/**< Data contained in the frame : table of 4 times or distances */
	int32_t rss[4];	/**< RSS contained in the frame : table of 4 rss */
	uint16_t crc;		/**< generated CRC from the three previous fields, used for integrity tests */
} serialFrame;


/*
*	-----------------------------------------------------------
*				public functions
*	-----------------------------------------------------------
*/


/**
 * \brief Creation of a frame
 * \param[in]	type	type of the frame to create, either TIME_FRAME or DISTANCE_FRAME
 * \param[in]	data1	first data to send in the frame
 * \param[in]	data2	second data to send in the frame
 * \param[in]	data3	third data to send in the frame
 * \param[in]	data4	fourth data to send in the frame
 * \return 		a frame initialized with the values given as parameters and generated sequence number and CRC
 */
serialFrame createSerialFrame(char type, int32_t data1, int32_t data2, int32_t data3, int32_t data4, int32_t rss1, int32_t rss2, int32_t rss3, int32_t rss4);

/*
	API functions
*/

/**
 * \brief Creation of a converted uint8_t version of a frame
 * \param[in]	type	type of the frame to create, either TIME_FRAME or DISTANCE_FRAME
 * \param[in]	data1	first data to send in the frame
 * \param[in]	data2	second data to send in the frame
 * \param[in]	data3	third data to send in the frame
 * \param[in]	data4	fourth data to send in the frame
 * \return 		a converted uint8_t version of a fully initialized frame with the given parameters and generated sequence number and CRC
 */
uint8_t * createSendableFrame(char type, int32_t data1, int32_t data2, int32_t data3, int32_t data4, int32_t rss1, int32_t rss2, int32_t rss3, int32_t rss4);


/**
 * \brief Conversion function for read data after transfer, able to check frame integrity
 * \param[in]	read	the data read on the USART, must be a converted form of a frame
 * \param[in]	uf		a pointer to retrieve the frame in its classic form
 * \return		0 for success, 1 if integrity check fails
 */
uint8_t retrieveReadFrame(uint8_t * read, serialFrame * uf);


/*
	conversion function for serial frame
*/

/**
 * \brief Conversion function : from frame to uint8_t *
 * \param[in]	uf	serial frame that will be converted
 * \return		the converted uint8_t * version of the frame
 */
uint8_t * serialFrameToUint(serialFrame uf);

/**
 * \brief Conversion function : from uint8_t * to frame
 * \param[in]	tab	a uint8_t * to convert into a frame (must be a converted version of a frame or might fail)
 * \return		a serialFrame generated from the uint8_t * given as parameter
 */
serialFrame serialFrameFromUint(uint8_t * toConvert);


/*
*	-----------------------------------------------------------
*				private functions
*	-----------------------------------------------------------
*/


/**
 * \brief Creates a 16 bits CRC
 * \param[in]	tab		char * representing the frame (seqNum, type and the 4 data)
 * \param[in]	tabLen	len of the char * in number of elements
 * \return		the calculated 16 bits CRC
 */
uint16_t createCRC(char * tab, uint8_t tabLen);

/**
 * \brief Transforms a frame into a char *
 * \param[in]	f	serial frame to convert
 * \param[in]	tab	char * used to retrieve the converted form of the frame, this form is used for CRC calculation purpose
 */
void serialFrameToChar(serialFrame f, char * tab);

#endif
