/**
	* @file serialFrame.c
	* @brief Serial frame management
	*
	*
	*
	* @author Romain TAPREST
	* @date 26 nov 2015
	*/
	
/*
*	-----------------------------------------------------------
*				Includes
*	-----------------------------------------------------------
*/

#include "serialFrame.h"
#include "lib_crc.h"
#include "typesAndConstants.h"

/*
*	-----------------------------------------------------------
*				public functions
*	-----------------------------------------------------------
*/

/**
	* @brief	Create a serial frame in an array of bytes from raw signal strengths data
	*	@warning	frame[] size must be at least = nbOfSignals*2 + 2
	*
	* @param	frame[out]					Array of bytes in which the frame will be written (size must be large enough !)
	* @param	signalsStrenght[in]	Array of raw signal strengths on 16 bits
	* @param	nbOfSignals					Size of the signalStrengths array
	* @param	frameSize						Final size of the frame stored in the frame[] array (nb of bytes to send)
	*/
void createSerialFrameForSignalsStrength(uint8_t frame[], uint16_t signalsStrength[], uint8_t nbOfSignals, uint16_t *frameSize)
{
	uint8_t i=0;
	
	*frameSize = 0;
	
	frame[*frameSize] = START_OF_FRAME;
	(*frameSize)++;
	
	frame[*frameSize] = START_OF_FRAME;
	(*frameSize)++;
	
	for(i=0;i<nbOfSignals;i++)
	{
		frame[*frameSize] = (uint8_t) (signalsStrength[i] & 0xFF);
		(*frameSize)++;
		frame[*frameSize] = (uint8_t) ((signalsStrength[i] >> 8) & 0xFF);
		(*frameSize)++;
	}
	
}


/*
*	-----------------------------------------------------------
*				private functions
*	-----------------------------------------------------------
*/


/**
 * @brief Creates a 16 bits CRC
 * @param[in]	tab		char * representing the frame (seqNum, type and the 4 data)
 * @param[in]	tabLen	len of the char * in number of elements
 * @return		the calculated 16 bits CRC
 */
uint16_t createCRC(char * tab, uint8_t tabLen){
	// initial CRC value (0 for a 16 bits one)
	uint16_t crc = 0;
	int i = 0;
	// updating CRC value for each char in the converted form of the frame
	for(i=0;i<tabLen;i++){
		crc = update_crc_16(crc,*tab);
		tab++;
	}
	
	return crc;
}
