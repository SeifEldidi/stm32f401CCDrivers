/*
 * Hal_Usart.c
 *
 *  Created on: Aug 1, 2023
 *      Author: Seif pc
 */
#include "../Inc/HAL/Hal_Usart.h"

/*-------------Static Variables--------*/
#if ConfigUSART_INT_EN
static USART_IRQ_Struct Usart1_Handler;
static USART_IRQ_Struct Usart2_Handler;
static USART_IRQ_Struct Usart6_Handler;
#endif

#if ConfigUSART_DMAA_EN_
USART_DMA_HANDLER Usart1 ;
USART_DMA_HANDLER Usart2 ;
USART_DMA_HANDLER Usart6 ;
#endif

/*--------------Static Functions Definition---------*/
static uint16_t uHAL_UsartBaudRate(uint32_t Periph_Clock,uint32_t BaudRate);
static ERR xHal_UsartParity_Init(__IO USART_Typedef * USART,uint8_t Parity);
static ERR xHal_UsartStopBits_Init(__IO USART_Typedef * USART,uint8_t No_StopBits);
static ERR xHal_UsartDataSize_Init(__IO USART_Typedef * USART,uint8_t Data_Size);
static ERR xHal_UsartAFRHelper_Init(__IO uint32_t *GPIO_AFR,uint8_t SHIFT_TX,uint8_t SHIFT_RX,uint8_t USART_Dir,uint8_t Flag);
static ERR xHal_UsartAFR_Init(__IO USART_Typedef * USART,__IO uint32_t *GPIO_AFR,uint8_t SHIFT_TX,uint8_t SHIFT_RX,uint8_t Flag);
#if ConfigUSART_DMAA_EN_
static ERR xHal_UsartDMA_Init(__IO USART_Typedef *USART,DMA_Config_t *D_Config);
#endif
#if ConfigUSART_INT_EN
static ERR xHAL_UsartInterruptsInit(USART_Config * Usart);
static void Usart1SendInterrupt(void);
static void Usart2SendInterrupt(void);
static void Usart6SendInterrupt(void);
static void Usart1ReceiveInterrupt(void);
static void Usart2ReceiveInterrupt(void);
static void Usart6ReceiveInterrupt(void);
static ERR Int_Request_GenerationTX(__IO USART_Typedef *Usart,USART_IRQ_Struct * Handler,int8_t * data,uint32_t Len);
static ERR Int_Request_GenerationRX(__IO USART_Typedef *Usart,USART_IRQ_Struct * Handler,int8_t * data,uint32_t Len);
#endif

#if ConfigUSART_DMAA_EN_
static void Usart1SendDMA_Interrupt(void);
static ERR xHal_UsartDMA_Init(__IO USART_Typedef *USART,DMA_Config_t *D_Config);
#endif

/*--------------Static Variables Definition---------*/
#if ConfigUSART_INT_EN

CallBack USART1TX_Callback;
CallBack USART1RX_Callback;

CallBack USART2TX_Callback;
CallBack USART2RX_Callback;

CallBack USART6TX_Callback;
CallBack USART6RX_Callback;
#endif

/*------------Static Functions Defintions-------*/
static int8_t ConvIntegerToString(int32_t Val,int8_t *String)
{
	uint8_t Cnt =0;
	uint8_t Len = 0 ;
	uint8_t Stack[10];
	uint8_t Stack_Top = -1;
	memset(String , '\0',11);
	while(Val != 0)
	{
		Stack[++Stack_Top] = Val%10;
		Val=Val/10;
		Len++;
	}
	while(Len!=0)
	{
		*(String +Cnt) = Stack[Stack_Top--] + '0';
		Cnt++;
		Len--;
	}
	return Cnt+1;
}

static int8_t ConvFloatToString(float Val,int8_t *String)
{
	uint8_t Cnt =0;
	uint8_t Len = 0 ;
	uint8_t Stack[10];
	uint8_t Stack_Top = -1;
	int32_t IntVal = (int32_t)Val;
	int32_t FloatVal = (((float)Val-(int32_t)Val)*1000);
	memset(String , '\0',22);
	if(IntVal != 0 )
	{
		while(IntVal != 0)
		{
			Stack[++Stack_Top] = IntVal%10;
			IntVal=IntVal/10;
			Len++;
		}
		while(Len!=0)
		{
			*(String +Cnt) = Stack[Stack_Top--] + '0';
			Cnt++;
			Len--;
		}
	}else{
		*(String +Cnt++) = '0';
	}
	*(String +Cnt++) = '.';
	Stack_Top = -1;
	Len = 0;
	if(FloatVal != 0)
	{
		while (FloatVal != 0) {
			Stack[++Stack_Top] = FloatVal % 10;
			FloatVal = FloatVal / 10;
			Len++;
		}
		while (Len != 0) {
			*(String + Cnt) = Stack[Stack_Top--] + '0';
			Cnt++;
			Len--;
		}
	}else{
		*(String +Cnt++) = '0';
	}
	return Cnt+1;
}

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
static ERR xHal_UsartParity_Init(__IO USART_Typedef * USART,uint8_t Parity)
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
static ERR xHal_UsartStopBits_Init(__IO USART_Typedef * USART,uint8_t No_StopBits)
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
static ERR xHal_UsartDataSize_Init(__IO USART_Typedef * USART,uint8_t Data_Size)
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
/* Name   : static ERR xHal_UsartDMA_Init(uint8_t DMA_Mode)
 * brief  : static Software API to initiliaze DMA to take control of USART
 * param  : takes uint8_t DMA_Mode to enable DMA for transmitter , reciever or both
 * return : whether the operation was successful or not
 * 			HAL_ERR Fail
 * 			HAL_OK  Success
 */
static ERR xHal_UsartDMA_Init(__IO USART_Typedef *USART,DMA_Config_t *D_Config)
{
	ERR Err_Status = HAL_OK;
	if(D_Config->DMA_EN_Dis != DMA_DIS )
	{
		/*-------------Find Stream For Tx and RX------*/
		DMA_Stream_TypeDef *DMA_Stream_TX =(USART==USART2)?DMA1_Stream6:DMA2_Stream7;
		DMA_Stream_TypeDef *DMA_Stream_RX =(USART==USART2)?DMA1_Stream5:(USART==USART1)?DMA2_Stream5:DMA2_Stream2;
		/*-------------Find DMA used --------*/
		DMA_TypeDef *DMA   =(USART==USART2)?DMA1:DMA2;
		uint32_t Channel    =(USART==USART6)?DMA_STREAMX_CHN5:DMA_STREAMX_CHN4;
		/*-------------Set Up Dir Used--------*/
		uint8_t Data_Dir_TX    = (D_Config->DMA_TX_Mode==DMA_SINGLE_MODE)?DMA_DIRECT_NCIRC_MODE:DMA_DIRECT_CIRC_MODE;
		uint8_t Data_Dir_RX    = (D_Config->DMA_RX_Mode==DMA_SINGLE_MODE)?DMA_DIRECT_NCIRC_MODE:DMA_DIRECT_CIRC_MODE;
		/*------------DMA ConfigStructure-----*/
		DMA_StreamX_t DMA_config = {
				.DMAinst = DMA,
				.Instance = DMA_Stream_TX,
				.DMA_StreamXMode = Data_Dir_TX,
				.Channel  = Channel,
				.DataDir  = DMA_STREAMX_M2P,
				.MemoryIncrementSize = DMA_PSIZE_INC_1,
				.PeripheralIncrementSize = DMA_PSIZE_INC_1,
				.MemoryIncEnable = MEMORY_INC_ENABLED,
				.StreamXPriority = DMA_STREAMX_PRIORITY_LOW,
				.Func = D_Config->TX_CB,
				.StreamXIntEnabled = DMA_STREAMX_INT_TCIE_ENABLED,
		};
		/*-----------Config DMA-----*/
		switch (D_Config->DMA_EN_Dis) {
				/*-----------Config DMA TX-----*/
				case DMA_EN_TX:
					MCAL_DmaStreamX_Init(&DMA_config);
					USART->CR3 |= DMA_TX_EN;
					break;
				/*-----------Config DMA RX-----*/
				case DMA_EN_RX:
					DMA_config.Instance = DMA_Stream_RX;
					DMA_config.DataDir = DMA_STREAMX_P2M;
					DMA_config.DMA_StreamXMode = Data_Dir_RX;
					MCAL_DmaStreamX_Init(&DMA_config);
					USART->CR3 |= DMA_RX_EN;
					break;
				/*-----------Config DMA RX-----*/
				case DMA_EN_TX_RX:
					MCAL_DmaStreamX_Init(&DMA_config);
					DMA_config.Instance = DMA_Stream_RX;
					DMA_config.DataDir = DMA_STREAMX_P2M;
					DMA_config.DMA_StreamXMode = Data_Dir_RX;
					MCAL_DmaStreamX_Init(&DMA_config);
					USART->CR3 |= DMA_RX_EN | DMA_TX_EN;
					break;
				default:
					Err_Status = HAL_ERR;
		}
	}
	return Err_Status;
}

static void Usart1SendDMA_Interrupt(void)
{
	Usart1.State[0] = USART_EMPTY;
	Usart1.Stream[0]->CR &= ~DMA_EN;
	USART1->CR3 &= ~DMA_TX_EN;
}

static void Usart1RecDMA_Interrupt(void)
{
	Usart1.State[1] = USART_EMPTY;
	Usart1.Stream[1]->CR &= ~DMA_EN;
	USART1->CR3 &= ~DMA_RX_EN;
}

#endif



#if ConfigUSART_INT_EN

static void Usart1SendInterrupt(void)
{
	if(Usart1_Handler.Pos[0] <Usart1_Handler.Len[0] && *(Usart1_Handler.Buffer[0] + Usart1_Handler.Pos[0])!='\0')
	{
		USART1->DR = *(Usart1_Handler.Buffer[0] + Usart1_Handler.Pos[0]);
		Usart1_Handler.Pos[0]++;
	}else if(Usart1_Handler.Pos[0] == Usart1_Handler.Len[0]||*(Usart1_Handler.Buffer[0] + Usart1_Handler.Pos[0])=='\0')
	{
		Usart1_Handler.State[0] = USART_EMPTY;
		Usart1_Handler.Len[0] = 0;
		Usart1_Handler.Pos[0] = 0;
		Usart1_Handler.Buffer[0] = NULL,
		/*----Clear Interrupt Enable on Usart---*/
		USART1->CR1 &= (~TCIE_EN);
	}
}

static void Usart2SendInterrupt(void)
{
	if (Usart2_Handler.Pos[0] < Usart2_Handler.Len[0])
	{
		USART2->DR = *(Usart2_Handler.Buffer[0] + Usart2_Handler.Pos[0]);
		Usart2_Handler.Pos[0]++;
	} else if (Usart2_Handler.Pos[0] == Usart2_Handler.Len[0]) {
		Usart2_Handler.State[0] = USART_EMPTY;
		Usart2_Handler.Len[0] = 0;
		Usart2_Handler.Pos[0] = 0;
		Usart2_Handler.Buffer[0] = NULL,
		/*----Clear Interrupt Enable on Usart---*/
		USART2->CR1 &= (~TCIE_EN);
	}
}

static void Usart6SendInterrupt(void)
{
	if (Usart6_Handler.Pos[0] < Usart6_Handler.Len[0])
	{
		USART6->DR = *(Usart6_Handler.Buffer[0] + Usart6_Handler.Pos[0]);
		Usart6_Handler.Pos[0]++;
	} else if (Usart6_Handler.Pos[0] == Usart6_Handler.Len[0])
	{
		Usart6_Handler.State[0] = USART_EMPTY;
		Usart6_Handler.Len[0] = 0;
		Usart6_Handler.Pos[0] = 0;
		Usart6_Handler.Buffer[0] = NULL,
		/*----Clear Interrupt Enable on Usart---*/
		USART6->CR1 &= (~TCIE_EN);
	}
}

static USART_INT_STATUS UsartXRXFlag(__IO USART_Typedef *Usart)
{
	USART_IRQ_Struct *IRQ_Handler = (Usart ==USART1)?&Usart1_Handler:(Usart ==USART2)?&Usart2_Handler:&Usart6_Handler;
	return IRQ_Handler->Status[1];
}

static void Usart1ReceiveInterrupt(void)
{
	if(Usart1_Handler.Pos[1] <Usart1_Handler.Len[1] && *(Usart1_Handler.Buffer[1] + Usart1_Handler.Pos[1])!='#')
	{
		/*----------Recieve Data----------*/
		*(Usart1_Handler.Buffer[1] + Usart1_Handler.Pos[1]) = USART1->DR;
		 Usart1_Handler.Pos[1]++;
	}else if(Usart1_Handler.Pos[1] == Usart1_Handler.Len[1]||*(Usart1_Handler.Buffer[1] + Usart1_Handler.Pos[1])=='#')
	{
		/*------------Terminate---------*/
		uint8_t Dum = USART1->DR;
		Usart1_Handler.State[1] = USART_EMPTY;
		/*----Clear Interrupt Enable on Usart---*/
		Usart1_Handler.Status[1] = USART_RX_DONE;
		USART1->CR1 &= (~RXNEIE_EN);
	}
}

static void Usart2ReceiveInterrupt(void)
{
	if (Usart2_Handler.Pos[1] < Usart2_Handler.Len[1]&& *(Usart2_Handler.Buffer[1] + Usart1_Handler.Pos[1]) != '#')
	{
		/*----------Recieve Data----------*/
	  *(Usart2_Handler.Buffer[1] + Usart1_Handler.Pos[1]) = USART1->DR;
		Usart1_Handler.Pos[1]++;
	} else if (Usart2_Handler.Pos[1] == Usart2_Handler.Len[1]|| *(Usart2_Handler.Buffer[1] + Usart2_Handler.Pos[1]) == '#') {
		/*------------Terminate---------*/
		uint8_t Dum = USART2->DR;
		Usart2_Handler.State[1] = USART_EMPTY;
		/*----Clear Interrupt Enable on Usart---*/
		Usart2_Handler.Status[1] = USART_RX_DONE;
		USART2->CR1 &= (~RXNEIE_EN);
	}
}

static void Usart6ReceiveInterrupt(void)
{
	if (Usart6_Handler.Pos[1] < Usart6_Handler.Len[1]&& *(Usart6_Handler.Buffer[1] + Usart6_Handler.Pos[1]) != '#')
	{
		/*----------Recieve Data----------*/
		  *(Usart6_Handler.Buffer[1] + Usart6_Handler.Pos[1]) = USART6->DR;
			Usart1_Handler.Pos[1]++;
	} else if (Usart6_Handler.Pos[1] == Usart6_Handler.Len[1]|| *(Usart6_Handler.Buffer[1] + Usart6_Handler.Pos[1]) == '#')
	{
		/*------------Terminate---------*/
		uint8_t Dum = USART6->DR;
		Usart6_Handler.State[1] = USART_EMPTY;
		/*----Clear Interrupt Enable on Usart---*/
		Usart6_Handler.Status[1] = USART_RX_DONE;
		USART6->CR1 &= (~RXNEIE_EN);
	}
}

static ERR Int_Request_GenerationTX(__IO USART_Typedef *Usart,USART_IRQ_Struct * Handler,int8_t * data,uint32_t Len)
{
	CallBack *USARTXTX_Callback = (Usart == USART1)?&USART1TX_Callback:(Usart == USART2)?&USART2TX_Callback:&USART6TX_Callback;
	CallBack USARTXSend_Callback = (Usart == USART1)?&Usart1SendInterrupt:(Usart == USART2)?&Usart2SendInterrupt:&Usart6SendInterrupt;
	if (Handler->State[0] != USART_BUSY) {
		Handler->Buffer[0] = data;
		Handler->Len[0] = Len;
		Handler->State[0] = USART_BUSY;
		Handler->Pos[0] = 0;
		*USARTXTX_Callback = USARTXSend_Callback;
		/*-----Initiate Data Transfer----*/
		Usart->DR = *(Handler->Buffer[0] + ((Handler->Pos[0])++));
		Usart->CR1 |= TCIE_EN;
	}else{
		return HAL_ERR;
	}
	return HAL_OK;
}

static ERR Int_Request_GenerationRX(__IO USART_Typedef *Usart,USART_IRQ_Struct * Handler,int8_t * data,uint32_t Len)
{
	CallBack *USARRX_Callback = (Usart == USART1)?&USART1RX_Callback:(Usart == USART2)?&USART2RX_Callback:&USART6RX_Callback;
	CallBack USARRXReceive_Callback = (Usart == USART1)?&Usart1ReceiveInterrupt:(Usart == USART2)?&Usart2ReceiveInterrupt:&Usart6ReceiveInterrupt;
	if (Handler->State[1] != USART_BUSY) {
		Handler->Status[1] = USART_RX_NDONE;
		Handler->Buffer[1] = data;
		Handler->Len[1] = Len;
		Handler->State[1] = USART_BUSY;
		Handler->Pos[1] = 0;
		*USARRX_Callback = USARRXReceive_Callback;
		/*-----Initiate Data Transfer----*/
		Usart->CR1 |= RXNEIE_EN;
	}else{
		return HAL_ERR;
	}
	return HAL_OK;
}
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
	CallBack * USARTTX_Callback = NULL;
	CallBack * USARTRX_Callback = NULL;
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
//				Usart->Instance->CR1 |=TCIE_EN;
#endif
				break;
			case INT_RX:
#if ConfigUSART_INT_EN_RX == EN
				(*USARTRX_Callback) = Usart->Rx_Callback;
				NVIC_EnableIRQ(NVIC_ENUM);
				NVIC_SetPriority(NVIC_ENUM, 0b1000);
//				Usart->Instance->CR1 |=RXNEIE_EN;
#endif
				break;
			case INT_TX_RX:
#if ConfigUSART_INT_EN
				(*USARTTX_Callback) = Usart->Tx_Callback;
				(*USARTRX_Callback) = Usart->Rx_Callback;
				NVIC_EnableIRQ(NVIC_ENUM);
				NVIC_SetPriority(NVIC_ENUM, 0b1000);
//				Usart->Instance->CR1 |=TCIE_EN|RXNEIE_EN;
#endif
			default:
				break;
		}
		__enable_irq();
	}
	return Err_State;
}

#if configUSART1_INT_EN
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

#endif

#if configUSART2_INT_EN
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
#endif

#if configUSART6_INT_EN
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



#endif

/*--------------Global Functions Definition---------*/
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

ERR xHAL_UsartSendBYTEP(__IO USART_Typedef *Usart , uint8_t data)
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
ERR xHAL_UsartSendNBYTESP(__IO USART_Typedef * Usart , uint8_t *Buffer ,uint8_t Len)
{
	ERR Err_State = HAL_OK;
	if(Usart != NULL  && Buffer !=NULL)
	{
		uint32_t ByteTX = 0;
		while(ByteTX < Len )
		{
			Err_State = xHAL_UsartSendBYTEP(Usart,*(Buffer+ByteTX));
			ByteTX++;
		}
	}else
		Err_State = HAL_ERR;
	return Err_State;
}

/* Name   : ERR xHAL_UsartSendNBYTESP (USART_Config * Usart , uint8_t *Buffer ,uint8_t Len)
 * brief  : Software API to Send String Over Uart
 *          the configuration FILE  macro must be enabled ConfigUSART_ASYNC_Mode
 * param  : takes pointer to USART Instance ,Buffer
 * return : whether the operation was successful or not
 * 			HAL_ERR Fail
 * 			HAL_OK  Success
 */
ERR xHAL_UsartSendString (__IO USART_Typedef * Usart , int8_t *Buffer )
{
	ERR Err_State = HAL_OK;
	if (Usart != NULL && Buffer != NULL) {
		uint32_t ByteTX = 0;
		while (*(Buffer +ByteTX)!='\0') {
			Err_State = xHAL_UsartSendBYTEP(Usart, *(Buffer + ByteTX));
			ByteTX++;
		}
	} else
		Err_State = HAL_ERR;
	return Err_State;
}

/* Name   : ERR xHAL_UsartLogInfo(USART_Typedef *Usart , uint8_t *Data ,...)
 * brief  : Software API to Send Data Over Uart in a Printf Fashion
 *          the configuration FILE  macro must be enabled ConfigUSART_ASYNC_Mode
 * param  : takes pointer to USART Instance ,Data
 * return : whether the operation was successful or not
 * 			HAL_ERR Fail
 * 			HAL_OK  Success
 */
ERR xHAL_UsartLogInfo(__IO USART_Typedef *Usart , int8_t *Data ,...)
{
	ERR Err_State = HAL_OK;
	if(Usart != NULL  && Data !=NULL)
	{
		va_list Ptr;
		va_start(Ptr, Data);
		uint8_t Str_Ptr = 0;
		int8_t String[22];
		int8_t Len = 0;

		while(*(Data + Str_Ptr) != '\0')
		{
			/*------Look for %-----*/
			if(*(Data + Str_Ptr) == '%')
			{
				Str_Ptr++;
				if(*(Data + Str_Ptr) == 'd')
				{
					int32_t Temp =  (int32_t)va_arg(Ptr, int);
					Len = ConvIntegerToString(Temp,String);
					xHAL_UsartSendString(Usart,String);

				}else if(*(Data + Str_Ptr) == 'h')
				{
					int16_t Temp =  (int16_t)va_arg(Ptr, int);
					Len = ConvIntegerToString(Temp,String);
					xHAL_UsartSendString(Usart,String);

				}else if(*(Data + Str_Ptr) == 'u')
				{
					uint32_t Temp = (uint32_t)va_arg(Ptr, int);
					Len = ConvIntegerToString(Temp,String);
					xHAL_UsartSendString(Usart,String);

				}else if(*(Data + Str_Ptr) == 's')
				{
					int8_t * Temp = (int8_t *)va_arg(Ptr, void *);
					xHAL_UsartSendString(Usart,Temp);
				}else if(*(Data + Str_Ptr) == 'f')
				{
					float Val = (float )va_arg(Ptr, double);
					Len = ConvFloatToString(Val, String);
					xHAL_UsartSendString(Usart, String);
				}
				Str_Ptr++;
			}else{
				xHAL_UsartSendBYTEP(Usart,*(Data + Str_Ptr));
				Str_Ptr++;
			}
		}
		va_end(Ptr);
	}else{
		Err_State = HAL_ERR;
	}
	return Err_State;
}

/* Name   : ERR xHAL_UsartRX1BYTESP(USART_Typedef * Usart , uint8_t *Buffer)
 * brief  : Software API to Recieve Data Over Uart Polling
 *          the configuration FILE  macro must be enabled ConfigUSART_ASYNC_Mode
 * param  : takes pointer to USART Instance ,Data
 * return : whether the operation was successful or not
 * 			HAL_ERR Fail
 * 			HAL_OK  Success
 */
ERR xHAL_UsartRX1BYTESP(__IO USART_Typedef * Usart , uint8_t *Buffer)
{
	ERR Err_State = HAL_OK;
	if(Usart != NULL  && Buffer !=NULL)
	{
		while (!(Usart->SR & RXNE_READ));
		*Buffer = Usart->DR;
	}else
		Err_State = HAL_ERR;
	return Err_State;
}


#if ConfigUSART_INT_EN
/* Name   : ERR xHAL_UsartSendBYTEAsync(USART_Typedef *Usart , uint8_t data)
 * brief  : Software API to Send Data Over Uart
 *          the configuration FILE  macro must be enabled ConfigUSART_ASYNC_Mode
 * param  : takes pointer to USART Instance
 * param  : takes a uint8_t data to send using interrupts
 * return : whether the operation was successful or not
 * 			HAL_ERR Fail
 * 			HAL_OK  Success
 */
ERR xHAL_UsartSendBYTEAsync(__IO USART_Typedef *Usart , int8_t data)
{
	ERR Usart_State = HAL_OK;
	if (Usart != NULL ) {
		if (Usart == USART1) {
			Usart_State = Int_Request_GenerationTX(USART1,&Usart1_Handler,&data,1);
		}else if(Usart == USART2)
		{
			Usart_State = Int_Request_GenerationTX(USART2,&Usart2_Handler,&data,1);
		}else{
			Usart_State = Int_Request_GenerationTX(USART6,&Usart6_Handler,&data,1);
		}
	} else {
		Usart_State = HAL_ERR;
	}
	return Usart_State;
}

/* Name   : ERR xHAL_UsartSendNBYTESAsync(USART_Typedef * Usart , uint8_t *Buffer ,uint8_t Len)
 * brief  : Software API to Send Data Over Uart
 *          the configuration FILE  macro must be enabled ConfigUSART_ASYNC_Mode
 * param  : takes pointer to USART Instance
 * param  : takes a uint8_t Buffer to send using interrupts
 * param  : uint8_t Len to specify the length of data
 * return : whether the operation was successful or not
 * 			HAL_ERR Fail
 * 			HAL_OK  Success
 */

ERR xHAL_UsartSendNBYTESAsync(__IO USART_Typedef * Usart , int8_t *Buffer ,uint8_t Len)
{
	ERR Usart_State = HAL_OK;
	if(Usart != NULL  && Buffer != NULL)
	{
		if(Usart == USART1)
		{
			Int_Request_GenerationTX(USART1,&Usart1_Handler,Buffer,Len);
		}else if(Usart == USART2)
		{
			Int_Request_GenerationTX(USART2,&Usart2_Handler,Buffer,Len);
		}else if(Usart == USART6)
		{
			Int_Request_GenerationTX(USART6,&Usart6_Handler,Buffer,Len);
		}
	}else{
		Usart_State = HAL_ERR;
	}
	return Usart_State;
}

/* Name   : ERR xHAL_UsartLogInfoAsync(USART_Typedef *Usart , uint8_t *Data ,...)
 * brief  : Software API to Send Data Over Uart in a Printf Fashion
 *          the configuration FILE  macro must be enabled ConfigUSART_ASYNC_Mode
 *          interrupts in transmission polling over string
 * param  : takes pointer to USART Instance
 * param  : takes a uint8_t Buffer to send using interrupts but polling is used to maintain determinsinm of cpu
 * param  : uint8_t Len to specify the length of data
 * return : whether the operation was successful or not
 * 			HAL_ERR Fail
 * 			HAL_OK  Success
 */
ERR xHAL_UsartLogInfoAsync(__IO USART_Typedef *Usart , uint8_t *Data ,...)
{
	ERR Err_State = HAL_OK;
	if(Usart != NULL  && Data !=NULL)
	{
		USART_IRQ_Struct *IRQ_Handler = (Usart ==USART1)?&Usart1_Handler:(Usart ==USART2)?&Usart2_Handler:&Usart6_Handler;
		va_list Ptr;
		va_start(Ptr, Data);
		uint8_t Str_Ptr = 0;
		uint8_t Temp_ptr;
		int8_t String[22];
		uint8_t Len = 0;

		while(*(Data + Str_Ptr) != '\0')
		{
			/*------Look for %-----*/
			if(*(Data + Str_Ptr) == '%')
			{
				Temp_ptr = Str_Ptr+1;
				if(*(Data + Temp_ptr) == 'd')
				{
					if (IRQ_Handler->State[0] == USART_EMPTY) {
						Str_Ptr++;
						Str_Ptr++;
						int32_t Temp = (int32_t) va_arg(Ptr, int);
						Len = ConvIntegerToString(Temp, String);
						xHAL_UsartSendNBYTESAsync(Usart, String, Len);
					}

				}else if(*(Data + Temp_ptr) == 'h')
				{
					if (IRQ_Handler->State[0] == USART_EMPTY) {
						Str_Ptr++;
						Str_Ptr++;
						int16_t Temp =  (int16_t)va_arg(Ptr, int);
						Len = ConvIntegerToString(Temp,String);
						xHAL_UsartSendNBYTESAsync(Usart, String, Len);
					}
				}else if(*(Data + Temp_ptr) == 'u')
				{
					if (IRQ_Handler->State[0] == USART_EMPTY) {
						Str_Ptr++;
						Str_Ptr++;
						uint32_t Temp = (uint32_t)va_arg(Ptr, int);
						Len = ConvIntegerToString(Temp,String);
						xHAL_UsartSendNBYTESAsync(Usart, String, Len);
					}
				}else if(*(Data + Temp_ptr) == 's')
				{
					if (IRQ_Handler->State[0] == USART_EMPTY) {
						Str_Ptr++;
						Str_Ptr++;
						int8_t * Temp = (int8_t *)va_arg(Ptr, void *);
						xHAL_UsartSendNBYTESAsync(Usart, Temp,50);
					}
				}
				else if (*(Data + Temp_ptr) == 'f') {
					if (IRQ_Handler->State[0] == USART_EMPTY) {
						float Val = (float )(va_arg(Ptr, double));
						Len = ConvFloatToString(Val, String);
						xHAL_UsartSendNBYTESAsync(Usart, String,Len);
						Str_Ptr++;
						Str_Ptr++;
					}
				}
			}else{
				if(IRQ_Handler->State[0] == USART_EMPTY )
				{
					xHAL_UsartSendBYTEAsync(USART1,*(Data + Str_Ptr));
					Str_Ptr++;
				}
			}
		}
		va_end(Ptr);
	}else{
		Err_State = HAL_ERR;
	}
	return Err_State;
}

void vHAL_UsartClearRXFlag(__IO USART_Typedef *Usart)
{
	USART_IRQ_Struct *IRQ_Handler = (Usart ==USART1)?&Usart1_Handler:(Usart ==USART2)?&Usart2_Handler:&Usart6_Handler;
	IRQ_Handler->Status[1] = USART_RX_NDONE;
}

ERR xHAL_UsartRXNBYTEAsync(__IO USART_Typedef * Usart,int8_t *Buffer,uint32_t Len,USART_INT_STATUS *RetStatus )
{
	ERR Usart_State = HAL_OK;
	if (Usart != NULL) {
		USART_INT_STATUS State = UsartXRXFlag(Usart);
		if(State != USART_RX_DONE)
		{
			*RetStatus = State;
			if (Usart == USART1) {
				Usart_State = Int_Request_GenerationRX(USART1, &Usart1_Handler, Buffer,Len);
			} else if (Usart == USART2) {
				Usart_State = Int_Request_GenerationRX(USART2, &Usart2_Handler, Buffer,Len);
			} else {
				Usart_State = Int_Request_GenerationRX(USART6, &Usart6_Handler, Buffer,Len);
			}
		}else if(State == USART_RX_DONE)
		{
			/*----User Must Clear State To Recieve Data Again ----*/
			*RetStatus = State;
		}
	} else {
		Usart_State = HAL_ERR;
	}
	return Usart_State;
}
#endif

#if ConfigUSART_DMAA_EN_

void vHAL_UsartDMASendBuffer(__IO USART_Typedef *Usart , uint32_t BufferAddress,uint32_t NTDR)
{
	if(Usart1.State[0] == USART_EMPTY)
	{
		Usart1.State[0] = USART_BUSY;
		/*-------------Find Stream For Tx------*/
		__IO DMA_Stream_TypeDef *DMA_Stream_TX =(Usart == USART2) ? DMA1_Stream6 : DMA2_Stream7;
		/*-------------Find DMA used --------*/
		Usart1.Stream[0] = DMA_Stream_TX;
		__IO DMA_TypeDef *DMA = (Usart == USART2) ? DMA1 : DMA2;
		/*---------Modify Registers-----*/
		DMA_Stream_TX->M0AR = BufferAddress;
		DMA_Stream_TX->NDTR = NTDR;
		DMA_Stream_TX->PAR  = (uint32_t)(&Usart->DR);
		/*------------Set Callback--------*/
		MCAL_DmaCallBackSet(DMA, DMA_Stream_TX, Usart1SendDMA_Interrupt);

		Usart->CR3 |= DMA_TX_EN;
		DMA_Stream_TX->CR |= DMA_EN;
	}
}

void vHAL_UsartDMARecieveBuffer(__IO USART_Typedef *Usart , uint32_t BufferAddress,uint32_t NTDR)
{
	if (Usart1.State[1] == USART_EMPTY)
	{
		Usart1.State[1] = USART_BUSY;
		/*-------------Find Stream For Tx------*/
		__IO DMA_Stream_TypeDef *DMA_Stream_RX =(Usart==USART2)?DMA1_Stream5:(Usart==USART1)?DMA2_Stream5:DMA2_Stream2;
		/*-------------Find DMA used --------*/
		Usart1.Stream[1] = DMA_Stream_RX;
		__IO DMA_TypeDef *DMA = (Usart == USART2) ? DMA1 : DMA2;
		/*---------Modify Registers-----*/
		DMA_Stream_RX->M0AR = BufferAddress;
		DMA_Stream_RX->NDTR = NTDR;
		DMA_Stream_RX->PAR = (uint32_t) (&Usart->DR);
		/*------------Set Callback--------*/
		MCAL_DmaCallBackSet(DMA, DMA_Stream_RX, Usart1RecDMA_Interrupt);

		Usart->CR3 |= DMA_RX_EN;
		DMA_Stream_RX->CR |= DMA_EN;
	}
}

#endif


#endif
