/*
 * Mcal_I2C.c
 *
 *  Created on: Aug 14, 2023
 *      Author: Seif pc
 */
#include "../../Inc/Mcal/Mcal_I2C.h"
/*-------------Static Functions Declaration----------------*/
static ERR xMcal_I2C_Slave_Init(__IO  I2C_TypeDef *Inst,uint8_t Flag ,uint16_t Add);
static ERR xMcal_I2C_Clock_Init(__IO  I2C_TypeDef *Inst);
static uint32_t xMcal_I2C_CR2_Init(__IO  I2C_TypeDef *Inst);
static ERR xMcal_I2C_Freq_Init(__IO  I2C_TypeDef *Inst,uint8_t F_S_M,uint8_t Duty);
static ERR xMcal_I2C_Trise_Init(__IO  I2C_TypeDef *Inst,uint8_t F_S_M);
static ERR xMcal_I2C_MSTRDuty_Init(__IO  I2C_TypeDef *Inst,uint8_t Duty,uint32_t Freq);
static void vMcal_I2C_PinInit(__IO  I2C_TypeDef *Inst);
#if I2C_INT_EN
static ERR xMcal_I2C_IntInit();
#if I2C1_INT_EN == EN
static void vMcal_I2C1SEND_INT();
static void vMcal_I2C1Read_INT();
#endif
#endif
/*-------------Static Variables DeFintions----------------*/
#if I2C_INT_EN
static uint8_t  I2C_DIR[I2C_UPPERMULTIPLICITY] = {I2C_WRITE,I2C_WRITE,I2C_WRITE};
static uint8_t  I2C_BUSY_FLAG[I2C_UPPERMULTIPLICITY]={ I2C_BUSY_IDLE ,I2C_BUSY_IDLE,I2C_BUSY_IDLE};
static uint8_t  I2C_REG_ADD[I2C_UPPERMULTIPLICITY];
static uint8_t  I2C_ADD_BUFFER[I2C_UPPERMULTIPLICITY] ; // I2C1,I2C2,I2C3
static uint8_t *I2C_DATA_BUFFER[I2C_UPPERMULTIPLICITY] ;
static uint8_t  I2C_DATA_LEN[I2C_UPPERMULTIPLICITY] ;
#endif
/*-------------Software Interfaces Defintions----------------*/
ERR xMCAL_I2C_Init(I2C_cfg_t * I2C)
{
	ERR I2C_State = HAL_OK;
	if(I2C != NULL)
	{
		/*-------GPIO Init------------*/
		vMcal_I2C_PinInit(I2C->Instance);
		/*-------Enable Clock Access to I2C------*/
		xMcal_I2C_Clock_Init(I2C->Instance);
		/*-------Reset I2C--------------------*/
		I2C->Instance->CR1 |= SWRST_RST;
		I2C->Instance->CR1 &= ~SWRST_RST;

		while(I2C->Instance->CR1 & SWRST_RST);
		/*--------Check whether I2C is slave or master-----*/
		if( I2C->SL_MSTR_MODE == I2C_SLAVE_MODE)
		{
			I2C_State = xMcal_I2C_Slave_Init(I2C->Instance,I2C->SL_SIZE,I2C->I2C_SLAVE_ADD);
			I2C_State = xMcal_I2C_CR2_Init(I2C->Instance);
			I2C_State = xMcal_I2C_Trise_Init(I2C->Instance,I2C_SM_MODE);
			/*-----Enable Perripheral---*/
			I2C->Instance->CR1 |= I2C_EN;
			I2C->Instance->CR1 |= ACK_EN;
		}else if(I2C->SL_MSTR_MODE == I2C_MSTR_MODE){
			I2C_State = xMcal_I2C_Freq_Init(I2C->Instance,I2C->SM_FM_SEL,I2C->FM_DUTY_SEL);
			I2C_State = xMcal_I2C_Trise_Init(I2C->Instance,I2C->SM_FM_SEL);
			/*Int Enable*/
#if I2C_INT_EN == EN
			I2C_State = xMcal_I2C_IntInit(I2C->Instance);
#endif
			/*-----Enable Perripheral---*/
			I2C->Instance->CR1 |= I2C_EN;
			I2C->Instance->CR1 |= ACK_EN;
		}else{
			I2C_State = HAL_ERR;
		}
	}else{
		I2C_State = HAL_ERR;
	}
	return I2C_State;
}

ERR  xMCAL_I2C_Finder(__IO I2C_TypeDef * I2C ,uint8_t *Data)
{
	uint16_t Dummy = 0x0000;
	uint8_t Pos =0;
	uint32_t I2CADD = 1;
	uint32_t TimeOut = 0;
	ERR I2C_State = HAL_OK;
	if (I2C != NULL && Data != NULL) {
		for(I2CADD = 1 ;I2CADD<= 127;I2CADD++)
		{
			CLEAR_BIT(I2C->SR1,AF);
			I2C_State = HAL_OK;
			TimeOut = 0 ;
			/*------Wait for Bus to be Empty-----*/
			while (I2C->SR2 & I2C_BUS_BUSY);
			/*--------Generate Start Bit------*/
			I2C->CR1 |= START_EN;
			/*--------Wait Untill Start Is Generated----*/
			while (!(I2C->SR1 & I2C_SB));
			/*-------Send Address-----------*/
			I2C->DR = (((I2CADD << 1)) | I2C_WRITE);
			while (!(I2C->SR1 & I2C_ADDR))
			{
				TimeOut++;
				if(TimeOut > 50000UL)
				{
					I2C_State = HAL_ERR;
					break;
				}
			}
			if(I2C_State == HAL_OK)
			{
				Dummy = I2C->SR2;
				I2C->CR1 |= STOP_EN;
				Data[Pos++] = I2CADD;
			}else{
				I2C->CR1 |= STOP_EN;
			}
		}
	}
	return I2C_State;
}

ERR  xMCAL_I2C_MSTRSend(__IO I2C_TypeDef * I2C , uint8_t SA,uint8_t Reg_Addr,uint8_t *Data,uint8_t Data_Len)
{
	uint16_t Dummy = 0x0000;
	uint32_t TimeOut = 0;
	ERR I2C_State = HAL_OK;
	CLEAR_BIT(I2C->SR1,AF);
	if(I2C != NULL && Data !=NULL)
	{
		uint32_t Byte_TX = 0;
		/*------Wait for Bus to be Empty-----*/
		while (I2C->SR2 & I2C_BUS_BUSY);
		/*--------Generate Start Bit------*/
		I2C->CR1 |= START_EN;
		/*--------Wait Untill Start Is Generated----*/
		while (!(I2C->SR1 & I2C_SB));
		/*-------Send Address-----------*/
		I2C->DR = (((SA << 1)) | I2C_WRITE);
		while (!(I2C->SR1 & I2C_ADDR))
		{
			TimeOut++;
			if (TimeOut > 50000UL)
			{
				I2C_State = HAL_ERR;
				break;
			}
		}
		if(I2C_State == HAL_OK)
		{
			Dummy = I2C->SR2;
			while (!(I2C->SR1 & TxE_SET));
			/*--------Send Data------*/
			I2C->DR = Reg_Addr;
			/*--------Wait for ack and TXE----*/
			for (Byte_TX = 0; Byte_TX <= Data_Len - 1; Byte_TX++) {
				while (!(I2C->SR1 & TxE_SET));
				I2C->DR = *(Data + Byte_TX);
			}
			while (!(I2C->SR1 & TxE_SET));
			while (!(I2C->SR1 & BTF_SET));
			I2C->CR1 |= STOP_EN;
		}else{
			I2C->CR1 |= STOP_EN;
		}
	}
	return I2C_State;
}

#if I2C_INT_EN
ERR  xMCAL_I2C_MSTRIntSend(I2C_TypeDef * I2C , uint8_t SA,uint8_t *Data,uint8_t Data_Len)
{
	ERR I2C_State = HAL_OK;
	if(I2C != NULL && Data != NULL)
	{
		uint8_t SEL;
		if(I2C == I2C1)
		{
			SEL = I2C1_SEL;
		}else if(I2C == I2C2)
		{
			SEL = I2C2_SEL;
		}else if(I2C == I2C3)
		{
			SEL = I2C3_SEL;
		}else{
			I2C_State = HAL_ERR;
		}
		if (I2C_BUSY_FLAG[SEL] == I2C_BUSY_IDLE)
		{
			I2C_DATA_BUFFER[SEL] = Data;
			I2C_ADD_BUFFER[SEL] = SA;
			I2C_DATA_LEN[SEL] = Data_Len;
			I2C_BUSY_FLAG[SEL] = I2C_BUSY_SET;
			I2C_DIR[SEL] = I2C_WRITE;
			I2C->CR1 |= ACK_EN;
			/*----Continue In interrupt----*/
			I2C->CR1 |= START_EN;
		}else{
			/*----Wait Do Nothing Until Busy is Cleared -----*/
		}
	}else{
		I2C_State = HAL_ERR;
	}
	return I2C_State;
}

ERR  xMCAL_I2C_MSTRIntRec(I2C_TypeDef * I2C , uint8_t SA,uint8_t Reg_ADD,uint8_t *Data,uint8_t Data_Len)
{
	ERR I2C_State = HAL_OK;
	if (I2C != NULL && Data != NULL) {
		uint8_t SEL;
		if (I2C == I2C1) {
			SEL = I2C1_SEL;
		} else if (I2C == I2C2) {
			SEL = I2C2_SEL;
		} else if (I2C == I2C3) {
			SEL = I2C3_SEL;
		} else {
			I2C_State = HAL_ERR;
		}
		if (I2C_BUSY_FLAG[SEL] == I2C_BUSY_IDLE) {
			I2C_DATA_BUFFER[SEL] = Data;
			I2C_ADD_BUFFER[SEL] = SA;
			I2C_DATA_LEN[SEL] = Data_Len;
			I2C_BUSY_FLAG[SEL] = I2C_BUSY_SET;
			I2C_REG_ADD[SEL] = Reg_ADD;
			I2C_DIR[SEL] = I2C_READ;
			I2C->CR1 |= ACK_EN;
			/*----Continue In interrupt----*/
			I2C->CR1 |= START_EN;
		} else {
			/*----Wait Do Nothing Until Busy is Cleared -----*/
		}
	} else {
		I2C_State = HAL_ERR;
	}
	return I2C_State;
}

#endif

#if I2C1_INT_EN == EN
static void vMcal_I2C1SEND_INT()
{
	static uint32_t Byte_TX = 0;
	static uint16_t Dummy = 0x00;
	/*--------Wait Untill Start Is Generated----*/
	if(I2C1->SR1 & I2C_SB)
		I2C1->DR = (((I2C_ADD_BUFFER[I2C1_SEL] << 1)) | I2C_WRITE);
	/*-------After Start Update Event of Addr should Arrive---*/
	if (I2C1->SR1 & I2C_ADDR)
		Dummy = I2C1->SR2;
	if(I2C1->SR1 & TxE_SET)
	{
		if( Byte_TX <= I2C_DATA_LEN[I2C1_SEL] -1)
		{
			I2C1->DR = *(I2C_DATA_BUFFER[I2C1_SEL] + Byte_TX);
			Byte_TX ++;
		}
	}
	if(I2C1->SR1 & BTF_SET)
	{
		I2C1->CR1 |= STOP_EN;
		I2C_BUSY_FLAG[I2C1_SEL] = I2C_BUSY_IDLE;
		Byte_TX = 0;
	}
}

static void vMcal_I2C1Read_INT()
{
	uint16_t Dummy = 0x0000;
	static int32_t Byte_RX = 0;
	static int8_t TX_RX = 0;
	static uint8_t EV_Flag = 0;
	if(TX_RX == 0)
	{
		if(I2C1->SR1 & I2C_SB)
			I2C1->DR = (((I2C_ADD_BUFFER[I2C1_SEL] << 1)) | I2C_WRITE);
		/*----Wait untill Address is sent---*/
		if (I2C1->SR1 & I2C_ADDR)
			Dummy = I2C1->SR2;
		if(I2C1->SR1 & TxE_SET)
		{
			if(EV_Flag == 0)
			{
				I2C1->DR = I2C_REG_ADD[I2C1_SEL];
				EV_Flag |=(1<<0);
			}else if(EV_Flag == 1)
			{
				I2C1->CR1 |= START_EN;
				EV_Flag  = 0x00;
				TX_RX =1;
			}
		}
	}else if(TX_RX == 1)
	{
		if(I2C1->SR1 & I2C_SB)
			I2C1->DR = (((I2C_ADD_BUFFER[I2C1_SEL] << 1)) | I2C_READ);
		if (I2C1->SR1 & I2C_ADDR )
		{
			Dummy = I2C1->SR2;
			if(I2C_DATA_LEN[I2C1_SEL] == 1)
				I2C1->CR1 &= ACK_DIS;
		}
		if(I2C1->SR1 & RxNE_SET )
		{
			if(I2C_DATA_LEN[I2C1_SEL] == 1)
			{
				I2C_BUSY_FLAG[I2C1_SEL] = I2C_BUSY_IDLE;
				*(I2C_DATA_BUFFER[I2C1_SEL] + Byte_RX) = I2C1->DR ;
				I2C1->CR1 |= STOP_EN;
				TX_RX = 0;
				EV_Flag = 0;
				Byte_RX = 0;
			}else{
				*(I2C_DATA_BUFFER[I2C1_SEL] + Byte_RX++) = I2C1->DR;
				if (Byte_RX == I2C_DATA_LEN[I2C1_SEL] - 2)
					/*---No Acknowledgment---*/
					I2C1->CR1 &= ACK_DIS;
				else if (Byte_RX == I2C_DATA_LEN[I2C1_SEL] - 1)
				{
					I2C_BUSY_FLAG[I2C1_SEL] = I2C_BUSY_IDLE;
					I2C1->CR1 |= STOP_EN;
					TX_RX = 0;
					EV_Flag = 0;
					Byte_RX =0;
				}
			}
		}
	}
}
#endif

ERR  xMCAL_I2C_MSTRRec (__IO I2C_TypeDef * I2C , uint8_t SA,uint8_t Reg_Addr,uint8_t *Data,uint8_t Data_Len)
{
	uint16_t Dummy = 0x0000;
	ERR I2C_State = HAL_OK;
	int32_t Byte_RX = 0;
	if (I2C != NULL && Data != NULL) {
		I2C->CR1 |= ACK_EN;
		/*------Wait for Bus to be Empty-----*/
		while (I2C->SR2 & I2C_BUS_BUSY);
		/*--------Generate Start Bit------*/
		I2C->CR1 |= START_EN;
		/*--------Wait Untill Start Is Generated----*/
		while (!(I2C->SR1 & I2C_SB));
		/*-------Send Address-----------*/
		I2C->DR = (((SA << 1)) | I2C_WRITE);
		while (!(I2C->SR1 & I2C_ADDR));
		Dummy = I2C->SR2;
		while (!(I2C->SR1 & TxE_SET));
		/*--------Send Register Address------*/
		I2C->DR = Reg_Addr;
		while (!(I2C->SR1 & TxE_SET));
		/*---After Acknowledgment Send Sr----*/
		/*--------Generate Repeated Start Bit------*/
		I2C->CR1 |= START_EN;
		while (!(I2C->SR1 & I2C_SB));
		/*-------Send Address-----------*/
		I2C->DR = (((SA << 1)) | I2C_READ);
		while (!(I2C->SR1 & I2C_ADDR));
		Dummy = I2C->SR2;
		/*------Recieve Data -------*/
		if(Data_Len == 1)
		{
			I2C->CR1 &= ACK_DIS;
			while (!(I2C->SR1 & RxNE_SET));
			*(Data + Byte_RX) = I2C->DR ;
			I2C1->CR1 |= STOP_EN;
		}else{
			for (Byte_RX = 0; Byte_RX <=Data_Len-1 ; Byte_RX++) {
				while (!(I2C->SR1 & RxNE_SET));
				if(Byte_RX == Data_Len-2)
					/*---No Acknowledgment---*/
					I2C->CR1 &= ACK_DIS;
				else if(Byte_RX == Data_Len-1)
					I2C1->CR1 |= STOP_EN;
				*(Data + Byte_RX) = I2C->DR ;
			}
		}
	} else {
		I2C_State = HAL_ERR;
	}
	return I2C_State;
}


/*-------------Static Functions Defintions----------------*/
static void vMcal_I2C_PinInit(__IO I2C_TypeDef *Inst)
{
	__IO GPIO_Typedef *GPIO_Port = NULL;
	GPIO_InitStruct GPIO;
	__IO uint32_t *AFR_Reg = NULL;

	uint8_t Pin_Number_SDA = (Inst ==I2C1)?GPIO_PIN_7:(Inst ==I2C2)?GPIO_PIN_11:GPIO_PIN_4;
	uint8_t Pin_Number_SCL = (Inst ==I2C1)?GPIO_PIN_6:(Inst ==I2C2)?GPIO_PIN_10:GPIO_PIN_8;
	AFR_Reg = (Inst == I2C1)?&(GPIOB->AFRL):(Inst ==I2C2)?&(GPIOB->AFRH):&(GPIOB->AFRL);

	if(Inst == I2C3)
	{
		GPIO_Port = GPIOB;
		GPIO.Pin = Pin_Number_SDA ;
		GPIO.Speed = GPIO_Speed_50MHz;
		GPIO.mode = GPIO_MODE_AF_OD;
		GPIO.pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIO_Port,&GPIO);
		GPIO_Port = GPIOA;
		GPIO.Pin = Pin_Number_SCL ;
		HAL_GPIO_Init(GPIO_Port,&GPIO);
	}else{
		GPIO_Port = GPIOB;
		GPIO.Pin = Pin_Number_SDA | Pin_Number_SCL;
		GPIO.Speed = GPIO_Speed_50MHz;
		GPIO.mode = GPIO_MODE_AF_OD;
		GPIO.pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIO_Port,&GPIO);
	}

	if(Inst == I2C3)
	{
		 AFR_Reg  = &(GPIOA->AFRH);
		*AFR_Reg &= ~(0xF<<0);
		*AFR_Reg |= (9<<0);

		 AFR_Reg  = &(GPIOB->AFRL);
		*AFR_Reg &= ~(0xF<<16);
		*AFR_Reg |= (4<<16);
	}else if(Inst == I2C1)
	{
		*AFR_Reg &=~(0xF<<28);
		*AFR_Reg &=~(0xF<<24);
		*AFR_Reg |=(4<<28);
		*AFR_Reg |=(4<<24);
	}else if(Inst == I2C2)
	{
		*AFR_Reg &=~(0xF<<8);
		*AFR_Reg &=~(0xF<<12);
		*AFR_Reg |=(4<<8);
		*AFR_Reg |=(4<<12);
	}

}

static ERR xMcal_I2C_Slave_Init(__IO I2C_TypeDef *Inst,uint8_t Flag ,uint16_t Add)
{
	ERR I2C_State = HAL_OK;
	switch (Flag) {
		case I2C_SLAVE_ADDR_7BIT:
			Inst->OAR1 &= ADD_7BIT;
			Inst->OAR1 |= (((uint8_t)(Add & 0x7F))<<1);
			break;
		case I2C_SLAVE_ADDR_10BIT:
			Inst->OAR1 |= ADD_10BIT;
			Inst->OAR1 |= Add & 0x03FF;
			break;
		default:
			I2C_State = HAL_ERR;
			break;
	}
	return I2C_State;
}

#if I2C_INT_EN
static ERR xMcal_I2C_IntInit(I2C_TypeDef *Inst)
{
#if I2C1_INT_EN == EN
	if( Inst == I2C1)
	{
		Inst->CR2 |= ITEVTEN_EN;
		NVIC_EnableIRQ(I2C1_EV_IRQ);
	}
#endif
#if I2C2_INT_EN == EN
	if(Inst == I2C2)
	{
		Inst->CR2 |= ITEVTEN_EN;
		NVIC_EnableIRQ(I2C2_EV_IRQ);
	}
#endif
#if I2C3_INT_EN == EN
	if(Inst == I2C3)
	{
		Inst->CR2 |= ITEVTEN_EN;
		NVIC_EnableIRQ(I2C3_EV_IRQ);
	}
#endif
}
#endif

static uint32_t xMcal_I2C_CR2_Init(__IO  I2C_TypeDef *Inst)
{
	uint32_t I2C_FREQ = HAL_RCC_GET_APB1FREQ();
	uint8_t I2C_CR2_FREQ = I2C_FREQ / I2C_PRESCALER;
	/*------SetUP I2C_CR2 Register-----*/
	Inst->CR2 |= I2C_CR2_FREQ;
	/*----Return----*/
	return I2C_FREQ;
}

static ERR xMcal_I2C_Freq_Init(__IO  I2C_TypeDef *Inst,uint8_t F_S_M,uint8_t Duty)
{
	ERR I2C_State = HAL_OK;
	uint32_t I2C_FREQ = 0;
	uint32_t CCR_Val;
	/*------SetUP I2C_CR2 Register-----*/
	I2C_FREQ = xMcal_I2C_CR2_Init(Inst);
	/*------Configure Clock Control Register-----*/
	switch (F_S_M) {
		case I2C_SM_MODE:
			Inst->CCR &= SM_MODE;
			CCR_Val = (I2C_FREQ / (2 * I2C_STANDARD_MODE));
			Inst->CCR |= (uint16_t)(CCR_Val & 0x7FF);
			break;
		case I2C_FM_MODE:
			Inst->CCR |= FM_MODE;
			xMcal_I2C_MSTRDuty_Init(Inst, Duty, I2C_FREQ);
			break;
		default:
			I2C_State = HAL_ERR;
			break;
	}
	return I2C_State;
}

static ERR xMcal_I2C_Trise_Init(__IO I2C_TypeDef *Inst,uint8_t F_S_M)
{
	ERR I2C_State = HAL_OK;
	uint32_t I2C_FREQ = HAL_RCC_GET_APB1FREQ();
	uint32_t Trise = 0;
	switch (F_S_M) {
		case I2C_SM_MODE:
			Trise = ((I2C_FREQ /I2C_PRESCALER) + 1);
			break;
		case I2C_FM_MODE:
			Trise = (((I2C_FREQ *3) /(I2C_PRESCALER*10)) + 1);
			break;
		default:
			I2C_State = HAL_ERR;
			break;
	}
	Inst->TRISE |= (Trise & 0x3F);
	return I2C_State;
}

static ERR xMcal_I2C_MSTRDuty_Init(__IO I2C_TypeDef *Inst,uint8_t Duty,uint32_t Freq)
{
	ERR I2C_State = HAL_OK;
	uint32_t CCR_Val = 0;
	switch(Duty)
	{
		case I2C_FM_DUTY_2:
			Inst->CCR &= DUTY_2;
			CCR_Val = (Freq/(3*I2C_FAST_MODE));
			break;
		case I2C_FM_DUTY_16_9:
			Inst->CCR |= DUTY_16_9;
			CCR_Val = (Freq/(25*I2C_FAST_MODE));
			break;
		default:
			I2C_State = HAL_ERR;
			break;
	}
	Inst->CCR |= (uint16_t)(CCR_Val & 0x7FF);
	return I2C_State;
}

static ERR xMcal_I2C_Clock_Init(__IO I2C_TypeDef *Inst)
{
	if(Inst == I2C1)
	{
		HAL_RCC_I2C1_EN();
	}else if(Inst == I2C2)
	{
		HAL_RCC_I2C2_EN();
	}else if(Inst == I2C3)
	{
		HAL_RCC_I2C3_EN();
	}else{
		return HAL_ERR;
	}
	return HAL_OK;
}

#if I2C1_INT_EN == EN
void I2C1_EV_IRQHandler(void)
{
	if(I2C_DIR[I2C1_SEL] == I2C_WRITE)
		vMcal_I2C1SEND_INT();
	else if(I2C_DIR[I2C1_SEL] == I2C_READ){
		vMcal_I2C1Read_INT();
	}
}
#endif

#if I2C2_INT_EN == EN
void I2C2_EV_IRQHandler(void)
{

}
#endif

#if I2C1_INT_EN == EN
void I2C3_EV_IRQHandler(void)
{

}
#endif
