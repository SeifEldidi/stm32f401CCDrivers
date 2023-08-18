/*
 * Hal_Usart.c
 *
 *  Created on: Aug 1, 2023
 *      Author: Seif pc
 */
#include "../Inc/HAL/Hal_Usart.h"


static uint16_t uHAL_UsartBaudRate(uint32_t Periph_Clock,uint32_t BaudRate)
{
	return ((Periph_Clock +(BaudRate/2U))/BaudRate);
}

/* Name   : ERR xHal_UsartParity_Init(uint8_t Parity)
 * brief  : Software API to initiliaze Usart Parity
 * param  : takes uint8_t Parity Mode which sets Parity in UART->CR1 Register
 * return : whether the operation was successful or not
 * 			HAL_ERR Fail
 * 			HAL_OK  Success
 */
static ERR xHal_UsartParity_Init(USART_Typedef * USART,uint8_t Parity)
{
	ERR Err_Status = HAL_OK;
	switch(Parity)
	{
		case USART_PARITY_EVEN:
			USART->CR1 |= PCE_EN;
			break;
		case USART_PARITY_ODD:
			USART->CR1 |= PCE_EN|PS_ODD;
			break;
		case USART_NPARITY:
			USART->CR1 &= PCE_DIS;
			break;
		default:
			Err_Status = HAL_ERR;
	}
	return Err_Status;
}

/* Name   : static ERR xHal_UsartStopBits_Init(uint8_t No_StopBits)
 * brief  : static Software API to initiliaze Usart No of stop Bits
 * param  : takes uint8_t No_stop_bits Mode and sets USART->CR2 register Accordingly
 * return : whether the operation was successful or not
 * 			HAL_ERR Fail
 * 			HAL_OK  Success
 */
static ERR xHal_UsartStopBits_Init(USART_Typedef * USART,uint8_t No_StopBits)
{
	ERR Err_Status = HAL_OK;
	switch(No_StopBits)
	{
		case USART_STOPBITS_1:
			USART->CR2|= STOP_1BIT;
			break;
		case USART_STOPBITS_0_5:
			USART->CR2 |= STOP_0_5BIT;
			break;
		case USART_STOPBITS_2:
			USART->CR2 |= STOP_2BIT;
			break;
		default:
			Err_Status = HAL_ERR;
	}
	return Err_Status;
}

/* Name   : static ERR xHal_UsartDataSize_Init(uint8_t Data_Size)
 * brief  : static Software API to Set the data size in the Uart Frame
 * param  : takes uint8_t Data Size which determines whether data size is 8bits or 9bits
 * return : whether the operation was successful or not
 * 			HAL_ERR Fail
 * 			HAL_OK  Success
 */
static ERR xHal_UsartDataSize_Init(USART_Typedef * USART,uint8_t Data_Size)
{
	ERR Err_Status = HAL_OK;
	switch(Data_Size)
	{
		case USART_DATA_SIZE_8:
			USART->CR1 &= M_8BITS;
			break;
		case USART_DATA_SIZE_9:
			USART->CR1 |= M_9BITS;
			break;
		default:
			Err_Status = HAL_ERR;
	}
	return Err_Status;
}

/* Name   : static ERR xHal_UsartAFRHelper_Init(__IO uint32_t *GPIO_AFR,uint8_t SHIFT_TX,uint8_t SHIFT_RX,uint8_t USART_Dir,uint8_t Flag)
 * brief  : static Software GPIO AFR Helper to initiliaze AFR register
 * param  :  __IO uint32_t * which is a pointer to the which AFR register
 * param  : uint8_T Shift_TX Shifting Constant for TX PIN
 * param  : uint8_T Shift_RX Shifting Constant for RX PIN
 * param  : uint8_t USART_Dir AFR function
 * param  : uint8_t Flag   determines USART Direction
 * return : whether the operation was successful or not
 * 			HAL_ERR Fail
 * 			HAL_OK  Success
 */
static ERR xHal_UsartAFRHelper_Init(__IO uint32_t *GPIO_AFR,uint8_t SHIFT_TX,uint8_t SHIFT_RX,uint8_t USART_Dir,uint8_t Flag)
{
	ERR Err_state = HAL_OK;
	if (GPIO_AFR != NULL) {
		switch(Flag)
		{
			case USART_TX_MODE:
				*GPIO_AFR |= (USART_Dir << SHIFT_TX);
				break;
			case USART_RX_MODE:
				*GPIO_AFR |= (USART_Dir << SHIFT_RX);
				break;
			case USART_TX_RX_MODE:
				*GPIO_AFR |= (USART_Dir << SHIFT_TX);
				*GPIO_AFR |= (USART_Dir << SHIFT_RX);
				break;
			default:
				Err_state = HAL_ERR;
		}
	}
	else
		Err_state = HAL_ERR;
return Err_state;
}

/* Name   : static ERR xHal_UsartAFR_Init(__IO uint32_t *GPIO_AFR,uint8_T SHIFT_TX,uint8_t SHIFT_RX)
 * brief  : static Software GPIO AFR to select certain pins to do alternate Function
 * param  : takes __IO uint32_t * which is a pointer to the register and uint8_T Shift_TX and RX which are shifting constants
 * 			related to the position of the pin also function expects the developer to pass the usart instance
 * return : whether the operation was successful or not
 * 			HAL_ERR Fail
 * 			HAL_OK  Success
 */
static ERR xHal_UsartAFR_Init(__IO USART_Typedef * USART,__IO uint32_t *GPIO_AFR,uint8_t SHIFT_TX,uint8_t SHIFT_RX,uint8_t Flag)
{
	ERR Err_state = HAL_OK;
	if(GPIO_AFR != NULL  && USART != NULL)
	{
		if(USART == USART1)
		{
			Err_state = xHal_UsartAFRHelper_Init(GPIO_AFR,SHIFT_TX,SHIFT_RX,AF_USART_1,Flag);
		}else if(USART == USART2)
		{
			Err_state = xHal_UsartAFRHelper_Init(GPIO_AFR,SHIFT_TX,SHIFT_RX,AF_USART_2,Flag);
		}else if(USART == USART6)
		{
			Err_state = xHal_UsartAFRHelper_Init(GPIO_AFR,SHIFT_TX,SHIFT_RX,AF_USART_6,Flag);
		}else
			Err_state = HAL_ERR;
	}else{
		Err_state = HAL_ERR;
	}
	return Err_state;
}


#if ConfigUSART_DMAA_EN_

static ERR xHal_UsartDMATX_init(USART_Typedef * USART,DMA_Stream_TypeDef *DMA_Stream_TX,DMA_Config_t *D_Config,uint32_t Channel)
{
	ERR Err_Status = HAL_OK;
#if ConfigUSARTDMA_INT_EN == EN
	if(DMA_Stream_TX == DMA1_Stream6 )
		NVIC_EnableIRQ(DMA1_Stream6_IRQ);
	else if(DMA_Stream_TX == DMA2_Stream7 )
		NVIC_EnableIRQ(DMA2_Stream7_IRQ);
#endif
	/*Reset Stream */
	DMA_Stream_TX->CR &= (~DMA_EN);
	while (DMA_Stream_TX->CR & DMA_EN);
	/*-----Enable Stream------*/
	DMA_Stream_TX->CR |= Channel | DMA_MSIZE_8BITS | DMA_PSIZE_8BITS
			| DMA_MINC_EN | DMA_CIRC_EN | DMA_M2P;

	DMA_Stream_TX->M0AR = D_Config->DMA_MAR_TX;
	DMA_Stream_TX->PAR = (uint32_t) (&USART->DR);
	DMA_Stream_TX->NDTR = D_Config->NTDR_TX;

#if ConfigUSARTDMA_INT_EN == EN
	/*--Dma Interrupt when Data is Recieved--*/
	DMA_Stream_TX->CR |= DMA_TCIE_EN;
#endif
	DMA_Stream_TX->CR |= DMA_EN;
	return Err_Status;
}

static ERR xHal_UsartDMARX_init(USART_Typedef * USART,DMA_Stream_TypeDef *DMA_Stream_RX,DMA_Config_t *D_Config,uint32_t Channel)
{
	ERR Err_Status = HAL_OK;
#if ConfigUSARTDMA_INT_EN == EN
	if (DMA_Stream_RX == DMA1_Stream5)
		NVIC_EnableIRQ(DMA1_Stream5_IRQ);
	else if (DMA_Stream_RX == DMA2_Stream5)
		NVIC_EnableIRQ(DMA2_Stream5_IRQ);
	else if (DMA_Stream_RX == DMA2_Stream2 )
		NVIC_EnableIRQ(DMA2_Stream2_IRQ);
#endif
	/*Reset Stream */
	DMA_Stream_RX->CR &= (~DMA_EN);
	while (DMA_Stream_RX->CR & DMA_EN);
	/*-----Enable Stream------*/
	DMA_Stream_RX->CR |= Channel | DMA_MSIZE_8BITS | DMA_PSIZE_8BITS |DMA_P2M
			| DMA_MINC_EN | DMA_CIRC_EN;

	DMA_Stream_RX->M0AR = D_Config->DMA_MAR_TX;
	DMA_Stream_RX->PAR = (uint32_t) (&USART->DR);
	DMA_Stream_RX->NDTR = D_Config->NTDR_TX;

#if ConfigUSARTDMA_INT_EN == EN
	/*--Dma Interrupt when Transfer is Complete--*/
	DMA_Stream_TX->CR |= DMA_TCIE_EN;
#endif
	DMA_Stream_RX->CR |= DMA_EN;
	return Err_Status;
}

/* Name   : static ERR xHal_UsartDMA_Init(uint8_t DMA_Mode)
 * brief  : static Software API to initiliaze DMA to take control of USART
 * param  : takes uint8_t DMA_Mode to enable DMA for transmitter , reciever or both
 * return : whether the operation was successful or not
 * 			HAL_ERR Fail
 * 			HAL_OK  Success
 */
static ERR xHal_UsartDMA_Init(USART_Typedef *USART,DMA_Config_t *D_Config)
{
	ERR Err_Status = HAL_OK;
	if(D_Config->DMA_EN_Dis != DMA_DIS )
	{
		DMA_Stream_TypeDef *DMA_Stream_TX =(USART==USART2)?DMA1_Stream6:DMA2_Stream7;
		DMA_Stream_TypeDef *DMA_Stream_RX =(USART==USART2)?DMA1_Stream5:(USART==USART1)?DMA2_Stream5:DMA2_Stream2;
		DMA_TypeDef *DMA   =(USART==USART2)?DMA1:DMA2;
		uint32_t Channel    =(USART==USART6)?DMA_CHN_5:DMA_CHN_4;
		if(DMA == DMA1)
		{
			HAL_RCC_DMA1_EN();
		}else if(DMA == DMA2 )
		{
			HAL_RCC_DMA2_EN();
		}else
			Err_Status = HAL_ERR;
		if(Err_Status == HAL_OK)
		{
			switch(D_Config->DMA_EN_Dis)
			{
				case DMA_EN_TX:
					xHal_UsartDMATX_init(USART,DMA_Stream_TX,D_Config,Channel);
					USART->CR3 |= DMA_TX_EN;
					break;
				case DMA_EN_RX:
					xHal_UsartDMATX_init(USART,DMA_Stream_RX,D_Config,Channel);
					USART->CR3 |= DMA_RX_EN;
					break;
				case DMA_EN_TX_RX:
					xHal_UsartDMATX_init(USART,DMA_Stream_TX,D_Config,Channel);
					xHal_UsartDMATX_init(USART,DMA_Stream_RX,D_Config,Channel);
					USART->CR3 |= DMA_RX_EN | DMA_TX_EN;
					break;
				default:
					Err_Status = HAL_ERR;
			}
		}
	}
	return Err_Status;
}

#endif



#if ConfigUSART_INT_EN

static void  (*USART1TX_Callback)(void);
static void  (*USART1RX_Callback)(void);

static void  (*USART2TX_Callback)(void);
static void  (*USART2RX_Callback)(void);

static void  (*USART6TX_Callback)(void);
static void  (*USART6RX_Callback)(void);

/* Name   : static ERR xHAL_UsartInterruptsInit(USART_Config * Usart)
 * brief  : static Software API to initiliaze Interrupts for USART
 * param  : USART_Config * Usart to Init Uart Interrupts
 * return : whether the operation was successful or not
 * 			HAL_ERR Fail
 * 			HAL_OK  Success
 */
static ERR xHAL_UsartInterruptsInit(USART_Config * Usart)
{
	ERR Err_State = HAL_OK;
	NVIC_IRQN NVIC_ENUM ;
	void  (**USARTTX_Callback)(void)=NULL;
	void  (**USARTRX_Callback)(void)=NULL;
	if(Usart->Instance == USART1)
	{
		NVIC_ENUM = USART1_IRQ;
		USARTTX_Callback = &USART1TX_Callback;
		USARTRX_Callback = &USART1RX_Callback;
	}
	else if(Usart->Instance == USART2)
	{
		NVIC_ENUM = USART2_IRQ;
		USARTTX_Callback = &USART2TX_Callback;
		USARTRX_Callback = &USART2RX_Callback;
	}
	else if(Usart->Instance == USART6)
	{
		NVIC_ENUM = USART6_IRQ;
		USARTTX_Callback = &USART6TX_Callback;
		USARTRX_Callback = &USART6RX_Callback;
	}
	else
		Err_State = HAL_ERR;
	if( Err_State == HAL_OK)
	{
		__disable_irq();
		switch (Usart->TX_RX_Interrupts) {
			case INT_TX:
#if ConfigUSART_INT_EN_TX == EN
				(*USARTTX_Callback) = Usart->Tx_Callback;
				NVIC_EnableIRQ(NVIC_ENUM);
				NVIC_SetPriority(NVIC_ENUM, 0b1000);
				Usart->Instance->CR1 |=TCIE_EN;
#endif
				break;
			case INT_RX:
#if ConfigUSART_INT_EN_RX == EN
				(*USARTRX_Callback) = Usart->Rx_Callback;
				NVIC_EnableIRQ(NVIC_ENUM);
				NVIC_SetPriority(NVIC_ENUM, 0b1000);
				Usart->Instance->CR1 |=RXNEIE_EN;
#endif
				break;
			case INT_TX_RX:
#if ConfigUSART_INT_EN
				(*USARTTX_Callback) = Usart->Tx_Callback;
				(*USARTRX_Callback) = Usart->Rx_Callback;
				NVIC_EnableIRQ(NVIC_ENUM);
				NVIC_SetPriority(NVIC_ENUM, 0b1000);
				Usart->Instance->CR1 |=TCIE_EN|RXNEIE_EN;
#endif
			default:
				break;
		}
		__enable_irq();
	}
	return Err_State;
}

void USART1_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(USART1_IRQ);
#if ConfigUSART_INT_EN_TX == EN
	if(USART1->SR & TXE_READ)
	{
		if(USART1TX_Callback != NULL)
			USART1TX_Callback();
	}
#endif
#if ConfigUSART_INT_EN_RX == EN
	if(USART1->SR & RXNE_READ)
	{
		if(USART1RX_Callback != NULL)
				USART1RX_Callback();
	}
#endif
}

void USART2_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(USART2_IRQ);
#if ConfigUSART_INT_EN_TX == EN
	if(USART2->SR & TXE_READ)
	{
		if(USART2TX_Callback != NULL)
			USART2TX_Callback();
	}
#endif
#if ConfigUSART_INT_EN_RX == EN
	if(USART2->SR & RXNE_READ)
	{
		if(USART2RX_Callback != NULL)
				USART2RX_Callback();
	}
#endif
}

void USART6_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(USART6_IRQ);
#if ConfigUSART_INT_EN_TX == EN
	if(USART6->SR & TXE_READ)
	{
		if(USART6TX_Callback != NULL)
			USART6TX_Callback();
	}
#endif
#if ConfigUSART_INT_EN_RX == EN
	if(USART6->SR & RXNE_READ)
	{
		if(USART6RX_Callback != NULL)
				USART6RX_Callback();
	}
#endif
}



#endif


#if ConfigUSART_ASYNC_Mode == EN

/* Name   : ERR xHAL_UsartInit (USART_Config * Usart)
 * brief  : Software API to initiliaze Usart in Asynchronous Mode to Enable ASYNC Mode must be enabled in
 *          the configuration FILE  macro must be enabled ConfigUSART_ASYNC_Mode
 * param  : takes pointer to USART object
 * return : whether the operation was successful or not
 * 			HAL_ERR Fail
 * 			HAL_OK  Success
 */

ERR xHAL_UsartInit(USART_Config * Usart)
{
	ERR Err_State = HAL_OK;
	if(Usart != NULL)
	{
		/*-------Enable Clock To Usart Module------*/
		if( Usart ->Instance == USART1)
		{
			HAL_RCC_USART1_EN();
		}else if( Usart->Instance == USART2)
		{
			HAL_RCC_USART2_EN();
		}
		else if(Usart->Instance == USART6)
		{
			HAL_RCC_USART6_EN();
		}else
			Err_State = HAL_ERR;
		/*-----Proceed if the Status is successful---*/
		if( Err_State == HAL_OK)
		{
			__IO USART_Typedef * USART_Inst = Usart->Instance;
			/*---------GPIO Init-------*/
			__IO GPIO_Typedef * GPIO_PORT     =  GPIOA;
			/*---------GPIO_AFR SET-----*/
			__IO uint32_t * GPIO_AFR     = (Usart->Instance == USART2)? &(GPIO_PORT->AFRL) :&(GPIO_PORT->AFRH);
			uint8_t SHIFT_TX			 = (Usart->Instance == USART2)? 8  :(Usart->Instance == USART1)?4:12;
			uint8_t SHIFT_RX			 = (Usart->Instance == USART2)? 12 :(Usart->Instance == USART1)?8:16;
			/*---------GPIO SET-----*/
			uint16_t        GPIO_PIN_TX  = (Usart->Instance == USART6)? GPIO_PIN_11 :(Usart->Instance == USART2)? GPIO_PIN_2 : GPIO_PIN_9;
			uint16_t        GPIO_PIN_RX  = (Usart->Instance == USART6)? GPIO_PIN_12 :(Usart->Instance == USART2)? GPIO_PIN_3 : GPIO_PIN_10;
			GPIO_InitStruct GPIO_TX ,GPIO_RX ;
			GPIO_TX.Pin = GPIO_PIN_TX;
			GPIO_RX.Pin = GPIO_PIN_RX;

			GPIO_TX.mode  = GPIO_MODE_AF_PP;
			GPIO_TX.pull  = GPIO_NOPULL;
			GPIO_RX.mode  = GPIO_MODE_AF_PP;
			GPIO_RX.pull  = GPIO_NOPULL;
			GPIO_TX.Speed = GPIO_Speed_50MHz;
			GPIO_RX.Speed = GPIO_Speed_50MHz;
			switch( Usart ->Mode )
			{
				case USART_TX_MODE:
					USART_Inst->CR1 |= TE_EN;
					HAL_GPIO_Init(GPIO_PORT,&GPIO_TX);
					break;
				case USART_RX_MODE:
					USART_Inst->CR1 |= RE_EN;
					HAL_GPIO_Init(GPIO_PORT,&GPIO_RX);
					break;
				case USART_TX_RX_MODE:
					USART_Inst->CR1 |= RE_EN|TE_EN ;
					HAL_GPIO_Init(GPIO_PORT,&GPIO_RX);
					HAL_GPIO_Init(GPIO_PORT,&GPIO_TX);
					break;
			}
			/*---------------Configure AFR_AFL Regs------------*/
			Err_State   =  xHal_UsartAFR_Init(USART_Inst,GPIO_AFR,SHIFT_TX,SHIFT_RX,Usart->Mode);
			/*-----------Configure Parity and StopBits and Data Size------*/
			Err_State  |= xHal_UsartDataSize_Init(Usart->Instance,Usart->Data_Size);
			Err_State  |= xHal_UsartParity_Init(Usart->Instance,Usart->Parity);
			Err_State  |= xHal_UsartStopBits_Init(Usart->Instance,Usart->No_StopBits);
			/*---------------Set Buad Rate---------*/
			uint32_t Periph_Clock = 0;
			if(USART_Inst == USART2)
			   Periph_Clock = HAL_RCC_GET_APB1FREQ();
			else
			   Periph_Clock = HAL_RCC_GET_APB2FREQ();
			USART_Inst->BRR = uHAL_UsartBaudRate(Periph_Clock,Usart->BaudRate);
			/*-------------Configure Either DMA or Interrupts-------*/
			if( Err_State == HAL_OK)
			{
				#if ConfigUSART_DMAA_EN_
					Err_State = xHal_UsartDMA_Init(Usart->Instance,&Usart->D_config);
				#endif
				#if ConfigUSART_INT_EN
					Err_State = xHAL_UsartInterruptsInit(Usart);
				#endif
			}
			USART_Inst->CR1 |= UE_EN;
		}else{

		}
	}else{
		Err_State = HAL_ERR;
	}
	return Err_State;
}

/* Name   : ERR xHAL_UsartSend1P(USART_Typedef *Usart , uint8_t data)
 * brief  : Software API to Send Single Byte Over Uart
 *          the configuration FILE  macro must be enabled ConfigUSART_ASYNC_Mode
 *          Data is sent as Follows IDLE -> START ->DATA(8 OR 9BITS) ->PARITY(Optional)->STOPBits(1,2,1.5)->IDLE
 * param  : takes pointer to USART Instance ,Buffer, Lenght of Buffer
 * return : whether the operation was successful or not
 * 			HAL_ERR Fail
 * 			HAL_OK  Success
 */

ERR xHAL_UsartSendBYTEP(USART_Typedef *Usart , uint8_t data)
{
	ERR Err_State = HAL_OK;
	if(Usart != NULL)
	{
		while (!(Usart->SR & TXE_READ));
		Usart->DR = data;
	}else
		Err_State = HAL_ERR;
	return Err_State;
}


/* Name   : ERR xHAL_UsartSendNBYTESP (USART_Config * Usart , uint8_t *Buffer ,uint8_t Len)
 * brief  : Software API to Send Buffer Over Uart
 *          the configuration FILE  macro must be enabled ConfigUSART_ASYNC_Mode
 * param  : takes pointer to USART Instance ,Buffer, Length of Buffer
 * return : whether the operation was successful or not
 * 			HAL_ERR Fail
 * 			HAL_OK  Success
 */
ERR xHAL_UsartSendNBYTESP(USART_Typedef * Usart , uint8_t *Buffer ,uint8_t Len,uint8_t TermChar)
{
	ERR Err_State = HAL_OK;
	if(Usart != NULL  && Buffer !=NULL)
	{
		uint32_t ByteTX = 0;
		while(ByteTX < Len && (*(Buffer+ByteTX)!= TermChar))
		{
			Err_State = xHAL_UsartSendBYTEP(Usart,*(Buffer+ByteTX));
			ByteTX++;
		}
	}else
		Err_State = HAL_ERR;
	return Err_State;
}

ERR xHAL_UsartRX1BYTESP(USART_Typedef * Usart , uint8_t *Buffer)
{
	ERR Err_State = HAL_OK;
	if(Usart != NULL  && Buffer !=NULL)
	{
	    while(!(Usart->SR & RXNE_READ));
		*Buffer = Usart->DR;
	}else
		Err_State = HAL_ERR;
	return Err_State;
}

#endif
