/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Seif Eldidi
 * @brief          : Main program body
 ******************************************************************************
*/

#include <stdint.h>
#include "HAL/Hal_RCC.h"
#include "core/CortexM4_core_Systick.h"
#include "HAL/Hal_GPIO.h"
#include "core/CortexM4_core_NVIC.h"
#include "HAL/Hal_ADC.h"
#include "HAL/Hal_Usart.h"
#include "HAL/hal_Timer.h"
#include "../Inc/Mcal/Mcal_I2C.h"
#include "HAL/Hal_BME280.h"
#include "../Inc/Mcal/Mcal_Flash_driver.h"
#include "HAL/Hal_INA219.h"
#include "../PID/PID.h"

void SysClockInit()
{
	/* RCC configuration Structure */
	RCC_Config_t RCC_Config = {
			.MSTR_CLK_SRC = SW_CLK_PLLO,
			.PLL_SRC = PLL_HSE,
			.PLL_M = 25,
			.PLL_N = 336,
			.PLL_P = PLLP_4,
			.PLL_Q = PLLQ_7_S,
			.AHB_PRE = AHBDIV_1,
			.APB1_PRE = APB_AHB_2,
			.APB2_PRE = APB_AHB_1,
	};
	/* SystemClock Init */
	HAL_RCC_Init(&RCC_Config);
}

uint8_t Buffer[10] ;
uint8_t Buffer_1[] ="Sasa Mix\r\n";


void PeriphInit()
{
	TIMX_t Timer3 = {
			.Inst = TIM3,
			.Mode = TIMX_TIMER,
			.TimeBaseSet.Base_Mode =BASE_1MS_TICK,
			.TimeBaseSet.Mode = TIMX_UPCOUNTER,
	};
	TIMX_t Timer2 = {
		   .Inst = TIM2,
		   .Mode = TIMX_PWM_OUTPUT,
		   .TimxPwmSet.Freq = 25,
		   .TimxPwmSet.Channel_Active = TIMX_CHN_1|TIMX_CHN_2,
		   .TimxPwmSet.PWM_MODE = 0,
		   .TimxPwmSet.Duty_C = {0,50},
	};
	I2C_cfg_t I2C ={
			.SM_FM_SEL = I2C_SM_MODE,
			.Instance  = I2C1,
			.SL_MSTR_MODE = I2C_MSTR_MODE,
	};
	USART_Config Usart = {
			.Instance = USART1,
			.BaudRate = 115200,
			.Data_Size = USART_DATA_SIZE_8,
			.Mode = USART_TX_RX_MODE,
			.No_StopBits = USART_STOPBITS_1,
			.Parity = USART_NPARITY,
			.D_config.DMA_EN_Dis = DMA_EN_TX_RX,
			.D_config.DMA_TX_Mode = DMA_SINGLE_MODE,
			.D_config.TX_CB = NULL,
	};
	xHAL_UsartInit(&Usart);
	xMCAL_I2C_Init(&I2C);
	xHAL_TIMX_Init(&Timer2);
	xHAL_TIMX_Init(&Timer3);
}

void Main_Init()
{
	/*---------Init Clock----------*/
	SysClockInit();
	/*---------Init Peripherals-----*/
	PeriphInit();
}

int main(void)
{
	Main_Init();
	vHAL_UsartDMARecieveBuffer(USART1,(uint32_t)Buffer,sizeof(Buffer));
	while(1)
	{
		vHAL_UsartDMASendBuffer(USART1,(uint32_t)Buffer_1,sizeof(Buffer_1));
		vHAL_TIMXDelay(TIM3,10);
	}
	return 0;
}



