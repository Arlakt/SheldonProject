/**
	* @file usbComm.c
	* @brief Serial Communication Protocol file core
	*
	*     This file contains the service serial communication USB.
	*			Initialization, configuration and send mode.
	*
	*
	* @author Romain TAPREST
	* @date 09 jan 2016
	*/


/******************************************************************************
	* 
	*   INCLUDED FILES
	*
	*****************************************************************************/

#include "usbComm.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "usb_cdc.h"
#include "serialFrame.h"
#include "signalProcessing.h"


	
/******************************************************************************
	*
	*   GLOBAL VARIABLES
	*
	*****************************************************************************/
	
extern t_signalsData g_signalData;

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
	
	usbCommInitPeriodicSending();
}


/**
	* @brief	Initializes a periodic data sending thanks to a Timer interruption
	*
	*/
void usbCommInitPeriodicSending(void)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure; // Timer base configuration
	NVIC_InitTypeDef NVIC_InitStructure; // IT
	
	// Timer 2 config
	/// 72MHz clock is entering in TIM2 because according to @see Reference manual
	/// if APB1 prescaler is different from 1, there is a x2 on TIM2 clock (36MHz x 2 = 72MHz)
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2 , ENABLE );
  TIM_TimeBaseStructInit( &TIM_TimeBaseStructure ); 
  TIM_TimeBaseStructure.TIM_Period = 				2000;  // 10kHz / 2000 = 5Hz  
  TIM_TimeBaseStructure.TIM_Prescaler = 		7200;   // 72MHz / 7200 = 10kHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = 	TIM_CounterMode_Down;  
  TIM_TimeBaseInit( TIM2, &TIM_TimeBaseStructure );
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	
	// Configure NVIC for Timer 2 interrupt
	NVIC_InitStructure.NVIC_IRQChannel = 										TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 	2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 				2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = 								ENABLE;
  NVIC_Init( &NVIC_InitStructure );
	
	TIM_Cmd( TIM2, ENABLE );
}

/**
	* @brief		This callback is periodicly called to send data over USB
	* @details	Period is set in @ref usbCommInitPeriodicSending
	*						This callback resets signal processing parameters
	*/
void TIM2_IRQHandler (void)
{
	uint16_t signalsMaximum[NB_OF_SIGNALS];
	uint8_t frame[NB_OF_SIGNALS*2 + 2];
	uint8_t size = 0;
	uint16_t frameSize = 0;
	
	// Get current signals strength
	sProcGetsignalsMaximumValues(signalsMaximum, &size);
	
	// Create the frame
	createSerialFrameForSignalsMaximum(frame, signalsMaximum, NB_OF_SIGNALS, &frameSize);
	
	// Send the frame
	usbCommSendData(frame, frameSize);
	
	// Reset signals max processing
	sProcInit();
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
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

