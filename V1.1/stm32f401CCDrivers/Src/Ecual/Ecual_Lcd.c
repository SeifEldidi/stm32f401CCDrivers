/*
 * Ecual_Lcd.c
 *
 *  Created on: Aug 6, 2023
 *      Author: Seif pc
 */

#include "../../Inc/Ecual/Ecual_LCD.h"

#ifdef  Enable

static void EcualLcd_4BIT_EN(LCD_4Bit_t * LCD)
{
	HAL_GPIO_WritePin(LCD->Instance , LCD->E_Pin ,  GPIO_PIN_SET);
	vHAL_TIMERdelayUs(TIM2,5);
	HAL_GPIO_WritePin(LCD->Instance , LCD->E_Pin ,  GPIO_PIN_RESET);
}

static void EcualLcd_Send4bits(LCD_4Bit_t * LCD , uint8_t data)
{
	uint8_t bits_sent = 0;
	for(bits_sent = 0 ;bits_sent<=LCD_4BIT_PINS-1;bits_sent++)
		HAL_GPIO_WritePin(LCD->Instance,LCD->Data_Pins[bits_sent],((data>>bits_sent) & BIT_MASK));
}

static void EcualLCD_SEND4BitsCMD(LCD_4Bit_t * LCD , uint8_t data)
{
	HAL_GPIO_WritePin(LCD->Instance , LCD->R_S_Pin , GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->Instance , LCD->R_W_Pin , GPIO_PIN_RESET);
	EcualLcd_Send4bits(LCD,data>>LCD_4BIT_PINS);
	EcualLcd_4BIT_EN(LCD);
	EcualLcd_Send4bits(LCD,data & 0x0F );
	EcualLcd_4BIT_EN(LCD);
}

static void EcualLCD_SEND4BitsData(LCD_4Bit_t * LCD , uint8_t data)
{
	HAL_GPIO_WritePin(LCD->Instance , LCD->R_S_Pin , GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD->Instance , LCD->R_W_Pin , GPIO_PIN_RESET);
	EcualLcd_Send4bits(LCD,data>>LCD_4BIT_PINS);
	EcualLcd_4BIT_EN(LCD);
	EcualLcd_Send4bits(LCD,data & 0x0F );
	EcualLcd_4BIT_EN(LCD);
}

static void Timer_Init()
{
	Timer_Config_t Timer_t = {
				.Timer_Mode = TIMX_DELAY_MODE,
				.Instance = TIM2,
				.Timer.Int_EN_DIS = TIMER_INT_DIS,
				.Timer.TimerCallback = NULL,
				.Timer.Base_MS = BASE_1US_TICK,
				.Timer.Counter_Mode= TIMER_COUNTER_UP,
	};
	xHAL_TIMERInit(&Timer_t);
}

ERR xEcualLcd_Init(LCD_4Bit_t * LCD)
{
	ERR Err_State = HAL_OK;
	if( LCD != NULL)
	{
		/*------Timer----*/
		Timer_Init();
		/*------GPIO------*/
		GPIO_InitStruct Gpio ={
				.Pin = LCD->Data_Pins[0]|LCD->Data_Pins[1]|LCD->Data_Pins[2]|LCD->Data_Pins[3]|
				LCD->R_S_Pin|LCD->R_W_Pin|LCD->E_Pin,
				.Speed = GPIO_Speed_50MHz,
				.mode = GPIO_MODE_OUTPUT_PP,
				.pull = GPIO_NOPULL,
		};
		HAL_GPIO_Init(LCD->Instance,&Gpio);
		/*-----Init LCD------*/
		vHAL_TIMERdelayUs(TIM2,40000);
		EcualLCD_SEND4BitsCMD(LCD, LCD_8BIT_TWO_LINE);
		vHAL_TIMERdelayUs(TIM2,5000);
		EcualLCD_SEND4BitsCMD(LCD, LCD_8BIT_TWO_LINE);
		vHAL_TIMERdelayUs(TIM2,150);
		EcualLCD_SEND4BitsCMD(LCD, LCD_8BIT_TWO_LINE);
		vHAL_TIMERdelayUs(TIM2,150);
		EcualLCD_SEND4BitsCMD(LCD, LCD_4BIT_TWO_LINE);
		vHAL_TIMERdelayUs(TIM2,150);

		EcualLCD_SEND4BitsCMD(LCD, LCD_CLEAR_DISPLAY);
		vHAL_TIMERdelayUs(TIM2,1500);
		EcualLCD_SEND4BitsCMD(LCD, LCD_DISPLAY_ON_CURSOR_OFF);
		vHAL_TIMERdelayUs(TIM2,1500);
		EcualLCD_SEND4BitsCMD(LCD, LCD_SET_DDRAM_ADD);
		vHAL_TIMERdelayUs(TIM2,1500);
		EcualLCD_SEND4BitsCMD(LCD, LCD_CLEAR_DISPLAY);
		vHAL_TIMERdelayUs(TIM2,1500);
	}else{
		Err_State = HAL_ERR;
	}
	return Err_State;
}

void vEcualLCD_Send(LCD_4Bit_t * LCD ,uint8_t Data ,uint8_t flag)
{
	if(flag == ECUAL_LCD_DATA)
	{
		EcualLCD_SEND4BitsData(LCD,Data);
		vHAL_TIMERdelayUs(TIM2,35);
	}
	else
	{
		EcualLCD_SEND4BitsCMD(LCD,Data);
		vHAL_TIMERdelayUs(TIM2,35);
	}
}

void vEcualLCD_SendStr(LCD_4Bit_t * LCD ,uint8_t *Data )
{
	uint8_t Byte = 0;
	while(*(Data+Byte)!='\0')
	{
		EcualLCD_SEND4BitsData(LCD,*(Data+Byte));
		vHAL_TIMERdelayUs(TIM2,35);
		Byte++;
	}
}

void vEcualLCD_SendNum(LCD_4Bit_t * LCD ,uint32_t Data )
{

}

void vEcualLCD_SendSpecialChar(LCD_4Bit_t * LCD ,uint8_t *Data )
{

}

void vEcualLcd_SetCursor(LCD_4Bit_t * LCD , uint8_t x ,uint8_t y)
{
	switch(y)
	{
		case 0:
			EcualLCD_SEND4BitsCMD(LCD,(LCD_SET_DDRAM_ADD) + x);
			break;
		case 1:
			EcualLCD_SEND4BitsCMD(LCD,(LCD_SET_DDRAM_ADD|ROW2_ADDRESS) + x);
			break;
		case 2:
			EcualLCD_SEND4BitsCMD(LCD,(LCD_SET_DDRAM_ADD|ROW3_ADDRESS) + x);
			break;
		case 3:
			EcualLCD_SEND4BitsCMD(LCD,(LCD_SET_DDRAM_ADD|ROW4_ADDRESS) + x);
			break;
	}
	vHAL_TIMERdelayUs(TIM2,35);
}
#endif
