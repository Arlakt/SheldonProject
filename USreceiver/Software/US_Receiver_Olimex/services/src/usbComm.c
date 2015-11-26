/**
	* @file usbComm.c
	* @brief Serial Communication Protocol file core
	*
	*     This file contains the service serial communication USB.
	*			Initialization, configuration and send mode.
	*
	*
	* @author Romain TAPREST
	* @date 26 nov 2015
	*/


/******************************************************************************
	* 
	*   INCLUDED FILES
	*
	*****************************************************************************/

#include "usbComm.h"
#include "stm32f10x.h"
#include "usb_cdc.h"
#include "serialFrame.h"

	
/******************************************************************************
	*
	*   PRIVATE FUNCTIONS
	*
	*****************************************************************************/
	
	

/******************************************************************************
	*
	*   PUBLIC FUNCTIONS
	*
	*****************************************************************************/


/**
	* @brief	Configure the usb serial communication.
	*					Blocking function.
	* @param 	Void
	* @return Void
	*/
void usbCommInit( void )
{
	
	Set_System();
  Set_USBClock();
  USB_Interrupts_Config();
  USB_Init();
	while (USB_GetState() != CONFIGURED) {} // Wait USB is ready
	
}

/**
	* @brief Send back data received over USB
	*/
void usbCommLoopBack(void)
{ 		
		if(USB_GetTxSize() > 0)
		{
			USB_Send( USB_Receive() );
		}
}

	
/**
	*	@brief	Send character via USB
	* 			
	* @param 	c		Char to send  
	* @return Void
	*/
void usbCommSendChar( uint8_t c )
{ 		
	USB_Send( c );
}


/**
	*	@brief	Send an array of uint8_t over USB
	* 		
	* @param 	array		Array of uint8_t to send  
	* @param 	size		Size of the array
	* @return Void
	*/
void usbCommSendData( uint8_t * array, uint16_t size )
{ 		
	uint16_t idChar = 0;
		
	for (idChar = 0; idChar < size; idChar++ )
	{
			// Send one char
			usbCommSendChar( array[idChar] );
	}
}

/**
	* @brief	Reads a received byte over USB if there is actually one.
	*	@param	byte	Pointer to the byte to write in if there is a received byte
	* @return	true	if there was a byte to read
	*					false	if there was not
	*/
bool usbCommReadByte(uint8_t *byte)
{
	uint8_t TxSize = 	USB_GetTxSize();
	if( TxSize > 0){
			*byte = USB_Receive();
		return true;
	}
	else{
		return false;
	}
}

/**
	* @brief	Reads several received bytes into an array
	* @param	array			Pointer to the array to write received bytes
	*	@param	maxSize		The size of the array (= max number of bytes to read)
	*	@param	readSize		The number of bytes which where read and written into array
	*/
void usbCommReadArray(uint8_t *array, uint16_t maxSize, uint16_t* readSize)
{
	uint8_t TxSize = 	USB_GetTxSize();
	uint16_t i=0;

	if(maxSize < TxSize){
		*readSize = maxSize;
	}
	else{
		*readSize = TxSize;
	}
	for(i=0;i<*readSize;i++){
		array[i] = USB_Receive();
	}

	return;
}

/**
	* @brief 	Freezes the program until there is an input on the USB
	* @return	First byte received
	*/
uint8_t usbCommWaitInput(void)
{
	while(USB_GetTxSize() == 0);
	return USB_Receive();
}

