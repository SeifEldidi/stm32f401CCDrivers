/*
 * Hal_ADC.c
 *
 *  Created on: Jul 27, 2023
 *      Author: Seif pc
 */

#include "../../Inc/HAL/Hal_ADC.h"

#if ADC_INTERRUPT_EN == EN
static void (*ADC_Callback) (void) =NULL;
#endif

#if ADC_SINGLECONV_EN

ERR xHAL_ADC_Init(ADC_CConfig_t *Adc,uint8_t Int_flag)
{
	ERR Err_Status = HAL_OK;
	if( Adc != NULL)
	{
		HAL_RCC_ADC1_EN();
		uint8_t CHN_No      = ((uint8_t)Adc->Channel);
		uint8_t Shift       = (CHN_No>9)?CHN_No-10:CHN_No;
		uint32_t *SMPRX_REG = (CHN_No>9)?(uint32_t *)(&ADC->SMPR1):(uint32_t *)(&ADC->SMPR2);

		GPIO_Typedef *GPIO_PORT = (CHN_No<=7)?GPIOA:(CHN_No>7 && CHN_No<10)?GPIOB :GPIOC;
		uint8_t Pin_NO = (CHN_No<=7)?CHN_No:(CHN_No>7 && CHN_No<10)?CHN_No-8 :CHN_No-10;
		GPIO_InitStruct ADC_IN_AN ;

		if(  Pin_NO ==0 )
			ADC_IN_AN.Pin  = GPIO_PIN_0;
		else if( Pin_NO ==1 )
			ADC_IN_AN.Pin  = GPIO_PIN_1;
		else if( Pin_NO ==1 )
			ADC_IN_AN.Pin  = GPIO_PIN_2;
		else if( Pin_NO ==2 )
			ADC_IN_AN.Pin  = GPIO_PIN_2;
		else if( Pin_NO ==3 )
			ADC_IN_AN.Pin  = GPIO_PIN_3;
		else if( Pin_NO ==4 )
			ADC_IN_AN.Pin  = GPIO_PIN_4;
		else if( Pin_NO ==5 )
			ADC_IN_AN.Pin  = GPIO_PIN_5;
		else if( Pin_NO ==6 )
			ADC_IN_AN.Pin  = GPIO_PIN_6;
		else if( Pin_NO ==7 )
			ADC_IN_AN.Pin  = GPIO_PIN_7;
		else if( Pin_NO ==8 )
			ADC_IN_AN.Pin  = GPIO_PIN_0;
		else if( Pin_NO ==9 )
			ADC_IN_AN.Pin  = GPIO_PIN_1;
		else if (Pin_NO == 10)
			ADC_IN_AN.Pin = GPIO_PIN_0;
		else if (Pin_NO == 11)
			ADC_IN_AN.Pin = GPIO_PIN_1;
		else if (Pin_NO == 12)
			ADC_IN_AN.Pin = GPIO_PIN_2;
		else if (Pin_NO == 13)
			ADC_IN_AN.Pin = GPIO_PIN_3;
		else if (Pin_NO == 14)
			ADC_IN_AN.Pin = GPIO_PIN_4;
		else if (Pin_NO == 15)
			ADC_IN_AN.Pin = GPIO_PIN_5;

		ADC_IN_AN.mode = GPIO_MODE_AN_INPUT;
		HAL_GPIO_Init (GPIO_PORT ,&ADC_IN_AN);

		Shift *= 3;
		uint8_t ADC_CHANNEL = (CHN_No & 0x1F);
		*SMPRX_REG |= (Adc->SMPT<<Shift);

		ADC->SQR3   = ADC_CHANNEL;
		ADC->CR1 &= ADC_CLEAR_3;
		ADC1_COMMON->CCR &= ADC_APB_CLEAR;
		switch ( Adc->RES )
		{
		    case ADC_RESOLUTION_12BIT:
		    	ADC->CR1 |= ADC_RES_12;
		    	break;
		    case ADC_RESOLUTION_10BIT:
		    	ADC->CR1 |= ADC_RES_10;
		    	break;
		    case ADC_RESOLUTION_8BIT:
		    	ADC->CR1 |= ADC_RES_8;
		    	break;
		    case ADC_RESOLUTION_6BIT:
		    	ADC->CR1 |= ADC_RES_6;
		    	break;
		    default:
		    	break;
		}
		switch( Adc->APB_P )
		{
			case ADC_APBP_2:
				ADC1_COMMON->CCR |= ADC_APB_2;
				break;
			case ADC_APBP_4:
				ADC1_COMMON->CCR |= ADC_APB_4;
				break;
			case ADC_APBP_6:
				ADC1_COMMON->CCR |= ADC_APB_6;
				break;
			case ADC_APBP_8:
				ADC1_COMMON->CCR |= ADC_APB_8;
				break;
			default:
				break;
		}
		switch( Adc->Allign)
		{
			case LEFT_ALLIGNMENT:
				ADC->CR2 |= ADC_ALLIGN_L;
				break;
			case RIGHT_ALLIGNMENT:
				ADC->CR2 &= ADC_ALLIGN_R;
				break;
			default:
				break;
		}
		if (Adc->Cont_SNG == ADC_CONTINOUS_MODE) {
			ADC->CR2 |= ADC_CONT_EN;
		}
#if ADC_TEMPSENSOR_READ == EN

#endif
#if ANALOG_WDGEN == EN
		ADC->CR1 |=  CHN_No;
		ADC->CR1 |=  ADC_AWDEN_EN;
		ADC->HTR  =  Adc->WDEGN.WDG_HIGH;
		ADC->LTR  =  Adc->WDEGN.WDG_LOW;
#endif
		ADC->SQR3 = CHN_No;
#if ADC_INTERRUPT_EN == EN
		if( Int_flag == ADC_SINGLE_INT_EN )
		{
			/*---Disable Interrupts---*/
			__disable_irq();
			/*---Enable Interrupt On EOC bit ---*/
			ADC->CR1 |= ADC_EOCIE_EN;
			/*----Config Callback-----*/
			ADC_Callback = Adc->ADC_Callback;
			/*----Enable interrupts---*/
			__enable_irq();
			/*---Enable NVIC register-----*/
			NVIC_EnableIRQ(ADC_IRQ);
		}
#endif
		ADC->CR2 |= ADC_ADCON_EN;
		if (Adc->Cont_SNG == ADC_CONTINOUS_MODE) {
			ADC->CR2 |= ADC_SWSTART_EN;
		}
	}else{
		Err_Status = HAL_ERR;
	}
	return Err_Status;
}

uint16_t sHAL_ADC_ResultBlocking(uint8_t Allign ,uint8_t channel)
{
	uint16_t Result;
	uint8_t ADC_CHANNEL = (channel & 0x1F);
	/*-------Select Channel------*/
	ADC->SQR3   = ADC_CHANNEL;
    /*--------Start Conversion------*/
	ADC->CR2 |= ADC_SWSTART_EN;
	/*--------Wait Polling------*/
	while (!(ADC->SR & ADC_EOCFLG_READ ));
	/*--------Check Allignment-----*/
	if( Allign == RIGHT_ALLIGNMENT)
		Result = ADC->DR;
	else if(Allign == LEFT_ALLIGNMENT)
		Result = (ADC->DR>>4);
	return Result;
}

uint16_t sHAL_ADC_ResultBlockingCont(uint8_t Allign )
{
	static uint8_t NO_C = 0;
	uint16_t Result;
	/*-------Select Channel------*/
	if(NO_C == 0 )
	{
		NO_C = 1;
		ADC->CR2 |= ADC_SWSTART_EN;
	}
	/*--------Wait Polling------*/
	while (!(ADC->SR & ADC_EOCFLG_READ));
	/*--------Check Allignment-----*/
	if (Allign == RIGHT_ALLIGNMENT)
		Result = ADC->DR;
	else if (Allign == LEFT_ALLIGNMENT)
		Result = (ADC->DR >> 4);
	return Result;
}


#if ADC_INTERRUPT_EN == EN
uint16_t sHAL_ADC_Result(uint8_t Allign ,uint8_t channel)
{
	uint16_t Result;
	uint8_t ADC_CHANNEL = (channel & 0x1F);
	if (Allign == RIGHT_ALLIGNMENT)
		Result = ADC->DR;
	else if (Allign == LEFT_ALLIGNMENT)
		Result = (ADC->DR >> 4);
	/*-------Select Channel------*/
	ADC->SQR3 = ADC_CHANNEL;
	/*--------Start Conversion------*/
	ADC->CR2 |= ADC_SWSTART_EN;
	/*--------Check Allignment-----*/
	return Result;
}

uint16_t sHAL_ADC_ResultCont(uint8_t Allign)
{
	uint16_t Result;
	/*--------Check Allignment-----*/
	if (Allign == RIGHT_ALLIGNMENT)
		Result = ADC->DR;
	else if (Allign == LEFT_ALLIGNMENT)
		Result = (ADC->DR >> 4);
	return Result;
}

#endif

#endif

#if ADC_GROUPCONV_DMA_EN || ADC_GROUPCONV_EN
static ERR xHAL_ADC_GENSET(ADC_CConfig_t *Adc)
{
	ERR ERR_Status = HAL_OK;
	HAL_RCC_ADC1_EN();
	ADC->CR1 &= ADC_CLEAR_3;
	ADC1_COMMON->CCR &= ADC_APB_CLEAR;
	switch (Adc->RES) {
		case ADC_RESOLUTION_12BIT:
			ADC->CR1 |= ADC_RES_12;
			break;
		case ADC_RESOLUTION_10BIT:
			ADC->CR1 |= ADC_RES_10;
			break;
		case ADC_RESOLUTION_8BIT:
			ADC->CR1 |= ADC_RES_8;
			break;
		case ADC_RESOLUTION_6BIT:
			ADC->CR1 |= ADC_RES_6;
			break;
		default:
			ERR_Status = HAL_ERR;
			break;
	}
	switch (Adc->APB_P) {
		case ADC_APBP_2:
			ADC1_COMMON->CCR |= ADC_APB_2;
			break;
		case ADC_APBP_4:
			ADC1_COMMON->CCR |= ADC_APB_4;
			break;
		case ADC_APBP_6:
			ADC1_COMMON->CCR |= ADC_APB_6;
			break;
		case ADC_APBP_8:
			ADC1_COMMON->CCR |= ADC_APB_8;
			break;
		default:
			ERR_Status = HAL_ERR;
			break;
	}
	switch (Adc->Allign) {
		case LEFT_ALLIGNMENT:
			ADC->CR2 |= ADC_ALLIGN_L;
			break;
		case RIGHT_ALLIGNMENT:
			ADC->CR2 &= ADC_ALLIGN_R;
			break;
		default:
			ERR_Status = HAL_ERR;
			break;
	}
	return ERR_Status;
}
static ERR xHAL_ADC_GroupChannel_Init(uint8_t CHN_NO)
{
	ERR ERR_Status = HAL_OK;
	GPIO_Typedef *GPIO_PORT = (CHN_NO <= 7) ? GPIOA :(CHN_NO > 7 && CHN_NO < 10) ? GPIOB : GPIOC;
	uint8_t Pin_NO = (CHN_NO <= 7) ? CHN_NO :(CHN_NO > 7 && CHN_NO < 10) ? CHN_NO - 8 : CHN_NO - 10;
	GPIO_InitStruct ADC_IN_AN;
	ADC_IN_AN.mode = GPIO_MODE_AN_INPUT;

	if (Pin_NO == 0)
		ADC_IN_AN.Pin = GPIO_PIN_0;
	else if (Pin_NO == 1)
		ADC_IN_AN.Pin = GPIO_PIN_1;
	else if (Pin_NO == 1)
		ADC_IN_AN.Pin = GPIO_PIN_2;
	else if (Pin_NO == 2)
		ADC_IN_AN.Pin = GPIO_PIN_2;
	else if (Pin_NO == 3)
		ADC_IN_AN.Pin = GPIO_PIN_3;
	else if (Pin_NO == 4)
		ADC_IN_AN.Pin = GPIO_PIN_4;
	else if (Pin_NO == 5)
		ADC_IN_AN.Pin = GPIO_PIN_5;
	else if (Pin_NO == 6)
		ADC_IN_AN.Pin = GPIO_PIN_6;
	else if (Pin_NO == 7)
		ADC_IN_AN.Pin = GPIO_PIN_7;
	else if (Pin_NO == 8)
		ADC_IN_AN.Pin = GPIO_PIN_0;
	else if (Pin_NO == 9)
		ADC_IN_AN.Pin = GPIO_PIN_1;
	else if (Pin_NO == 10)
		ADC_IN_AN.Pin = GPIO_PIN_0;
	else if (Pin_NO == 11)
		ADC_IN_AN.Pin = GPIO_PIN_1;
	else if (Pin_NO == 12)
		ADC_IN_AN.Pin = GPIO_PIN_2;
	else if (Pin_NO == 13)
		ADC_IN_AN.Pin = GPIO_PIN_3;
	else if (Pin_NO == 14)
		ADC_IN_AN.Pin = GPIO_PIN_4;
	else if (Pin_NO == 15)
		ADC_IN_AN.Pin = GPIO_PIN_5;

	HAL_GPIO_Init (GPIO_PORT ,&ADC_IN_AN);
	return ERR_Status;
}
#endif
//
#if ADC_GROUPCONV_DMA_EN
static void vHAL_ADC_Dma_Init(uint16_t NDTR,uint16_t *Buffer)
{
	/*-----Configure DMA-----*/
	HAL_RCC_DMA2_EN();
	/*Clear DMA2 Stream*/
	DMA2_Stream0->CR = 0;
	/*--Wait Until Cleared--*/
	while ((DMA2_Stream0->CR & DMA_EN));
	/*-----Enable DMA Stream 0 or 4----*/
	/*--DMA CONFIG--*/
	DMA2_Stream0->CR |= DMA_PSIZE_16BITS | DMA_MSIZE_16BITS | DMA_MINC_EN| DMA_CIRC_EN;
	/*--DMA SRC and DEST*/
	DMA2_Stream0->PAR = (uint32_t) (&ADC->DR);
	DMA2_Stream0->M0AR = (uint32_t) (Buffer);
	DMA2_Stream0->NDTR = NDTR;
	/*--DMA EN ----*/
	DMA2_Stream0->CR |= DMA_EN;
}
#endif

#if ADC_GROUPCONV_DMA_EN || ADC_GROUPCONV_EN
static void vHAL_ADC_Helper(uint8_t No_channels_init,uint8_t Ch_IN,uint8_t ADC_SMP)
{
	uint8_t Channel_Shift = 0;
	if (No_channels_init > 0 && No_channels_init < 7) {
		Channel_Shift = ((No_channels_init - 1) * 5);
		ADC->SQR3 |= (((Ch_IN) & 0xFF) << Channel_Shift);
		ADC->SMPR2 |= ((ADC_SMP) << ((No_channels_init - 1) * 3));
	} else if (No_channels_init >= 7 && No_channels_init < 13) {
		Channel_Shift = ((No_channels_init - 7) * 5);
		ADC->SQR2 |= (((Ch_IN) & 0xFF) << Channel_Shift);
		if (No_channels_init >= 7 && No_channels_init <= 10)
			ADC->SMPR2 |= ((ADC_SMP) << ((No_channels_init - 1) * 3));
		else
			ADC->SMPR1 |= ((ADC_SMP) << ((No_channels_init - 11) * 3));
	} else if (No_channels_init >= 13 && No_channels_init < 15) {
		Channel_Shift = ((No_channels_init - 13) * 5);
		ADC->SQR1 |= (((Ch_IN) & 0xFF) << Channel_Shift);
		ADC->SMPR1 |= ((ADC_SMP) << ((No_channels_init - 11) * 3));
	}
}
#endif

#if ADC_GROUPCONV_DMA_EN
ERR xHAL_ADC_GroupInitDMA(ADC_CConfig_t *Adc,uint16_t *Buffer)
{
	ERR ERR_Status = HAL_OK;
	if( Adc != NULL)
	{
		const uint8_t NO_CHN=16;
		uint8_t No_channels_init = 0;
		uint8_t Channel_Shift = 0;
		uint8_t Ch_IN =0;
		xHAL_ADC_GENSET(Adc);
		for(Ch_IN =0;Ch_IN<=NO_CHN-1;Ch_IN++)
		{
			/*Init Channel*/
			if(READ_BIT(Adc->Channel,Ch_IN))
			{
				No_channels_init++;
				ERR_Status = xHAL_ADC_GroupChannel_Init(Ch_IN);
				vHAL_ADC_Helper(No_channels_init,Ch_IN,ADC_SMP_3CYC);
			}else{

			}
		}
#if ANALOG_WDGEN == EN
				if(Adc->AWDG == AWDG_ALL)
				{
					ADC->CR1 |= ADC_AWDEN_EN;
					ADC->CR1 &= ADC_AWDSGL_DIS;
					ADC->HTR  =  Adc->WDEGN.WDG_HIGH;
					ADC->LTR  =  Adc->WDEGN.WDG_LOW;
				}
				else{
					ADC->CR1 |= ADC_AWDEN_EN;
					ADC->CR1 |= ADC_AWDSGL_EN;
					ADC->CR1 |= Adc->AWDG;
					ADC->HTR  =  Adc->WDEGN.WDG_HIGH;
					ADC->LTR  =  Adc->WDEGN.WDG_LOW;
				}
#endif
#if ADC_TEMPSENSOR_READ == EN
		if(Adc->Temp_Sensor_Read == TEMPSENS_EN)
		{
			uint8_t TEMP_SEND_CHN = 16U;
			No_channels_init ++;
			vHAL_ADC_Helper(No_channels_init,TEMP_SEND_CHN,ADC_SMP_3CYC);
		}else{}
#endif
		ADC->CR1 |= ADC_SCAN_EN;
		ADC->SQR1 = ((No_channels_init-1)<<20);
		if (Adc->Cont_SNG == ADC_CONTINOUS_MODE) {
			ADC->CR2 |= ADC_DMA_EN|ADC_DDS_EN|ADC_EOCS_READ|ADC_CONT_EN;
		}else
			ADC->CR2 |= ADC_DMA_EN|ADC_DDS_EN|ADC_EOCS_READ;
		/*-----Configure DMA-----*/
		HAL_ADC_Dma_Init(No_channels_init,Buffer);
		/*------ADC EN----*/
		ADC->CR2 |= ADC_ADCON_EN;
#if ADC_TEMPSENSOR_READ == EN
		if(Adc->Temp_Sensor_Read == TEMPSENS_EN)
			ADC1_COMMON->CCR |= ADC_TSVREFE_EN;
#endif
		if (Adc->Cont_SNG == ADC_CONTINOUS_MODE) {
				ADC->CR2 |= ADC_SWSTART_EN;
		}
	}else{
		ERR_Status = HAL_ERR;
	}
	return ERR_Status;
}

#endif

#if ADC_GROUPCONV_EN
ERR xHAL_ADC_GroupInitNDMABased(ADC_CConfig_t *Adc,uint8_t Flag)
{
	ERR ERR_Status = HAL_OK;
	if (Adc != NULL) {
		const uint8_t NO_CHN = 16;
		uint8_t No_channels_init = 0;
		uint8_t Channel_Shift = 0;
		uint8_t Ch_IN = 0;
		xHAL_ADC_GENSET(Adc);
		for (Ch_IN = 0; Ch_IN <= NO_CHN - 1; Ch_IN++) {
			/*Init Channel*/
			if (READ_BIT(Adc->Channel, Ch_IN)) {
				No_channels_init++;
				ERR_Status = xHAL_ADC_GroupChannel_Init(Ch_IN);
				vHAL_ADC_Helper(No_channels_init, Ch_IN,ADC_SMP_480CYC);
			} else {

			}
		}
#if ANALOG_WDGEN == EN
			if(Adc->AWDG == AWDG_ALL)
			{
					ADC->CR1 |= ADC_AWDEN_EN;
					ADC->CR1 &= ADC_AWDSGL_DIS;
					ADC->HTR  =  Adc->WDEGN.WDG_HIGH;
					ADC->LTR  =  Adc->WDEGN.WDG_LOW;
			}
			else{
					ADC->CR1 |= ADC_AWDEN_EN;
					ADC->CR1 |= ADC_AWDSGL_EN;
					ADC->CR1 |= Adc->AWDG;
					ADC->HTR  =  Adc->WDEGN.WDG_HIGH;
					ADC->LTR  =  Adc->WDEGN.WDG_LOW;
			}
#endif
#if ADC_TEMPSENSOR_READ == EN
		if(Adc->Temp_Sensor_Read == TEMPSENS_EN)
		{
			uint8_t TEMP_SEND_CHN = 16U;
			No_channels_init ++;
			vHAL_ADC_Helper(No_channels_init,TEMP_SEND_CHN,ADC_SMP_480CYC);
		}else{}
#endif
		ADC->CR1 |= ADC_SCAN_EN ;
		if(Adc->Cont_SNG == ADC_CONTINOUS_MODE)
			ADC->CR2 |= ADC_EOCS_READ|ADC_CONT_EN;
		else
			ADC->CR2 |= ADC_EOCS_READ;
		ADC->SQR1 = ((No_channels_init - 1) << 20);
		/*---------Interrupt --------*/
		/*----Must slow access Time---*/
#if ADC_INTERRUPT_EN == EN
		if( Flag == ADC_GROUP_INTERRUPTS_EN )
		{
			/*---Disable Interrupts---*/
			__disable_irq();
			/*---Enable Interrupt On EOC bit ---*/
			ADC->CR1 |= ADC_EOCIE_EN;
			ADC->CR1 |= ADC_OVRIE_EN;
			/*----Config Callback-----*/
			ADC_Callback = Adc->ADC_Callback;
			/*----Enable interrupts---*/
			__enable_irq();
			/*---Enable NVIC register-----*/
			NVIC_EnableIRQ(ADC_IRQ);
		}
#endif
		ADC->CR2 |= ADC_ADCON_EN;
#if ADC_TEMPSENSOR_READ == EN
		if(Adc->Temp_Sensor_Read == TEMPSENS_EN)
			ADC1_COMMON->CCR |= ADC_TSVREFE_EN;
#endif
		if (Adc->Cont_SNG == ADC_CONTINOUS_MODE) {
			ADC->CR2 |= ADC_SWSTART_EN;
		}
	}
	return ERR_Status;
}

#if ADC_INTERRUPT_EN == DIS
ERR xHAL_ADC_GroupReadPBased(ADC_CConfig_t *Adc,uint16_t *Buffer)
{

}
#endif
#endif

#if ADC_INTERRUPT_EN == EN
void ADC_IRQHandler(void)
{
#if ADC_SINGLECONV_EN
	if(ADC->SR  & ADC_EOCFLG_READ)
	{
		if(ADC_Callback)
			ADC_Callback();
	}
#endif
#if ADC_GROUPCONV_EN
	/*-----May Enter Multiple Times with OVR Run Error abort and slow down conversion time-----*/
	if(ADC->SR  & ADC_OVR_READ)
	{
		/*overrun Error*/
		/*--Clear Overrun Bit---*/
		ADC->SR &= ~ADC_OVR_READ;
		/*---Restart ADC-----*/
		ADC->CR2 |= ADC_SWSTART_EN;
	}
	else if(ADC->SR  & ADC_EOCFLG_READ)
	{
		if(ADC_Callback)
			ADC_Callback();
	}
#endif
}
#endif
