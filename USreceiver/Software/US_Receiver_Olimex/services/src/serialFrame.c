#include "serialFrame.h"
#include "lib_crc.h"
#include <stdio.h>

/*
*	-----------------------------------------------------------
*				public functions
*	-----------------------------------------------------------
*/

uint16_t currentSerialSeqNum = 0;
/**
 * \brief Table used as return value for conversion function
 */
uint8_t converted[CONVERTED_SERIAL_FRAME_SIZE];


serialFrame createSerialFrame(char type, int32_t data1, int32_t data2, int32_t data3, int32_t data4, int32_t rss1, int32_t rss2, int32_t rss3, int32_t rss4){
	// creation of the frame
	serialFrame f;
	
	char tab[CONVERTED_SERIAL_FRAME_SIZE-2];
	
	// initializing the fields
	f.seqNum = currentSerialSeqNum;
	f.type = type;
	f.data[0]= data1;
	f.data[1]= data2;
	f.data[2]= data3;
	f.data[3]= data4;
	f.rss[0]= rss1;
	f.rss[1]= rss2;
	f.rss[2]= rss3;
	f.rss[3]= rss4;
	
	// converting the frame into a char * for CRC generation purpose
	serialFrameToChar(f,tab);
	// CRC generation
	f.crc = createCRC(tab, CONVERTED_SERIAL_FRAME_SIZE-2);
	// sequence number management
	if(currentSerialSeqNum<MAX_SERIAL_SEQ_NUM)
		currentSerialSeqNum++;
	else
		currentSerialSeqNum = 0;
		
	return f;
}


uint8_t * createSendableFrame(char type, int32_t data1, int32_t data2, int32_t data3, int32_t data4, int32_t rss1, int32_t rss2, int32_t rss3, int32_t rss4){
	// creating a frame with the given parameters
	serialFrame uf = createSerialFrame(type, data1, data2, data3, data4, rss1, rss2, rss3, rss4);
	// returning its converted uint8_t form
	return serialFrameToUint(uf);
}


uint8_t retrieveReadFrame(uint8_t * read, serialFrame * uf){
	char tab[CONVERTED_SERIAL_FRAME_SIZE-2];
	uint16_t crcRead = 0;
	// converting back the uint8_t form of the frame in its classic form
	*uf = serialFrameFromUint(read);
	// generating the CRC for this converted form
	
	serialFrameToChar(*uf,tab);
	crcRead = createCRC(tab, CONVERTED_SERIAL_FRAME_SIZE-2);
	// comparing this generated CRC to the one contained in the frame to check integrity of the whole frame
	if(crcRead==uf->crc)
		return 0;
	else
		return 1;
}


uint8_t * serialFrameToUint(serialFrame uf){
	// generation of the uint8_t * thanks to masks and divisions
	// the uint8_t * used as return is a global variable so that no malloc should be used
	converted[0] = (uint8_t)(uf.seqNum>>8);
	converted[1] = (uint8_t)(uf.seqNum & 0x00FF);
	converted[2] = (uint8_t)uf.type;
	converted[3] = (uint8_t)(uf.data[0]>>24);
	converted[4] = (uint8_t)((uf.data[0]>>16)&0x00FF);
	converted[5] = (uint8_t)((uf.data[0]>>8)&0x0000FF);
	converted[6] = (uint8_t)(uf.data[0]&0x000000FF);
	converted[7] = (uint8_t)(uf.data[1]>>24);
	converted[8] = (uint8_t)((uf.data[1]>>16)&0x00FF);
	converted[9] = (uint8_t)((uf.data[1]>>8)&0x0000FF);
	converted[10] = (uint8_t)(uf.data[1]&0x000000FF);
	converted[11] = (uint8_t)(uf.data[2]>>24);
	converted[12] = (uint8_t)((uf.data[2]>>16)&0x00FF);
	converted[13] = (uint8_t)((uf.data[2]>>8)&0x0000FF);
	converted[14] = (uint8_t)(uf.data[2]&0x000000FF);
	converted[15] = (uint8_t)(uf.data[3]>>24);
	converted[16] = (uint8_t)((uf.data[3]>>16)&0x00FF);
	converted[17] = (uint8_t)((uf.data[3]>>8)&0x0000FF);
	converted[18] = (uint8_t)(uf.data[3]&0x000000FF);
	converted[19] = (uint8_t)(uf.rss[0]>>24);
	converted[20] = (uint8_t)((uf.rss[0]>>16)&0x00FF);
	converted[21] = (uint8_t)((uf.rss[0]>>8)&0x0000FF);
	converted[22] = (uint8_t)(uf.rss[0]&0x000000FF);
	converted[23] = (uint8_t)(uf.rss[1]>>24);
	converted[24] = (uint8_t)((uf.rss[1]>>16)&0x00FF);
	converted[25] = (uint8_t)((uf.rss[1]>>8)&0x0000FF);
	converted[26] = (uint8_t)(uf.rss[1]&0x000000FF);
	converted[27] = (uint8_t)(uf.rss[2]>>24);
	converted[28] = (uint8_t)((uf.rss[2]>>16)&0x00FF);
	converted[29] = (uint8_t)((uf.rss[2]>>8)&0x0000FF);
	converted[30] = (uint8_t)(uf.rss[2]&0x000000FF);
	converted[31] = (uint8_t)(uf.rss[3]>>24);
	converted[32] = (uint8_t)((uf.rss[3]>>16)&0x00FF);
	converted[33] = (uint8_t)((uf.rss[3]>>8)&0x0000FF);
	converted[34] = (uint8_t)(uf.rss[3]&0x000000FF);
	converted[35] = (uint8_t)(uf.crc>>8);
	converted[36] = (uint8_t)(uf.crc & 0x00FF);
	
	return converted;
}


serialFrame serialFrameFromUint(uint8_t * toConvert){
	// calculating frame fields from uint8_t * assuming it is a valid converted form of an serial frame
	// using multiplication and casts to recreate frame fields
	serialFrame uf;
	uf.seqNum = (((uint16_t)toConvert[0])<<8)+(uint16_t)(toConvert[1]);
	uf.type = (char)toConvert[2];
	uf.data[0] = ((int32_t)(toConvert[3])<<24)+((int32_t)(toConvert[4])<<16)+((int32_t)(toConvert[5])<<8)+((int32_t)(toConvert[6]));
	uf.data[1] = ((int32_t)(toConvert[7])<<24)+((int32_t)(toConvert[8])<<16)+((int32_t)(toConvert[9])<<8)+((int32_t)(toConvert[10]));
	uf.data[2] = ((int32_t)(toConvert[11])<<24)+((int32_t)(toConvert[12])<<16)+((int32_t)(toConvert[13])<<8)+((int32_t)(toConvert[14]));
	uf.data[3] = ((int32_t)(toConvert[15])<<24)+((int32_t)(toConvert[16])<<16)+((int32_t)(toConvert[17])<<8)+((int32_t)(toConvert[18]));
	uf.rss[0] = ((int32_t)(toConvert[19])<<24)+((int32_t)(toConvert[20])<<16)+((int32_t)(toConvert[21])<<8)+((int32_t)(toConvert[22]));
	uf.rss[1] = ((int32_t)(toConvert[23])<<24)+((int32_t)(toConvert[24])<<16)+((int32_t)(toConvert[25])<<8)+((int32_t)(toConvert[26]));
	uf.rss[2] = ((int32_t)(toConvert[27])<<24)+((int32_t)(toConvert[28])<<16)+((int32_t)(toConvert[29])<<8)+((int32_t)(toConvert[30]));
	uf.rss[3] = ((int32_t)(toConvert[31])<<24)+((int32_t)(toConvert[32])<<16)+((int32_t)(toConvert[33])<<8)+((int32_t)(toConvert[34]));
	uf.crc = ((uint16_t)(toConvert[35]<<8))+((uint16_t)(toConvert[36]));
	
	return uf;
}

/*
*	-----------------------------------------------------------
*				private functions
*	-----------------------------------------------------------
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


void serialFrameToChar(serialFrame f, char * tab){
	// generation of the char * thanks to masks and divisions
	tab[0] = (char)(f.seqNum>>8);
	tab[1] = (char)(f.seqNum & 0x00FF);
	tab[2] = f.type;
	tab[3] = (char)(f.data[0]>>24);
	tab[4] = (char)((f.data[0]>>16)&0x00FF);
	tab[5] = (char)((f.data[0]>>8)&0x0000FF);
	tab[6] = (char)(f.data[0]&0x000000FF);
	tab[7] = (char)(f.data[1]>>24);
	tab[8] = (char)((f.data[1]>>16)&0x00FF);
	tab[9] = (char)((f.data[1]>>8)&0x0000FF);
	tab[10] = (char)(f.data[1]&0x000000FF);
	tab[11] = (char)(f.data[2]>>24);
	tab[12] = (char)((f.data[2]>>16)&0x00FF);
	tab[13] = (char)((f.data[2]>>8)&0x0000FF);
	tab[14] = (char)(f.data[2]&0x000000FF);
	tab[15] = (char)(f.data[3]>>24);
	tab[16] = (char)((f.data[3]>>16)&0x00FF);
	tab[17] = (char)((f.data[3]>>8)&0x0000FF);
	tab[18] = (char)(f.data[3]&0x000000FF);
	tab[19] = (char)(f.rss[0]>>24);
	tab[20] = (char)((f.rss[0]>>16)&0x00FF);
	tab[21] = (char)((f.rss[0]>>8)&0x0000FF);
	tab[22] = (char)(f.rss[0]&0x000000FF);
	tab[23] = (char)(f.rss[1]>>24);
	tab[24] = (char)((f.rss[1]>>16)&0x00FF);
	tab[25] = (char)((f.rss[1]>>8)&0x0000FF);
	tab[26] = (char)(f.rss[1]&0x000000FF);
	tab[27] = (char)(f.rss[2]>>24);
	tab[28] = (char)((f.rss[2]>>16)&0x00FF);
	tab[29] = (char)((f.rss[2]>>8)&0x0000FF);
	tab[30] = (char)(f.rss[2]&0x000000FF);
	tab[31] = (char)(f.rss[3]>>24);
	tab[32] = (char)((f.rss[3]>>16)&0x00FF);
	tab[33] = (char)((f.rss[3]>>8)&0x0000FF);
	tab[34] = (char)(f.rss[3]&0x000000FF);
}
