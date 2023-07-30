/*
 * Hal_ADC.c
 *
 *  Created on: Jul 27, 2023
 *      Author: Seif pc
 */

#include "../../Inc/HAL/Hal_ADC.h"

#if ADC_SINGLECONV == EN

ERR HAL_ADC_Init(ADC_SConfig_t *Adc)
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
		*SMPRX_REG |= (ADC_SMP_15CYC<<Shift);

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
		ADC->SQR3 = CHN_No;
		ADC->CR2 |= ADC_ADCON_EN;
	}else{
		Err_Status = HAL_ERR;
	}
	return Err_Status;
}

uint16_t HAL_ADC_ResultBlocking(uint8_t Allign ,uint8_t channel)
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

#endif
