/*
 * CortexM4_core_Systick.c
 *
 *  Created on: Jul 25, 2023
 *      Author: Seif pc
 */

#include "../../Inc/core/CortexM4_core_Systick.h"

#if SYSTICK_INT_EN == EN
   void (*SYS_CALLBACK) (void);
#endif

uint32_t SYSCTICK_CLK;

ERR HAL_SYSTICK_Init(SYSTICK_CFG *SYS)
{
	ERR Err_Status = HAL_OK;
	if( SYS != NULL)
	{
		uint32_t LOAD_VAL = 0x00000000;
		/*Find CLK frequency of SYSTICK*/
		SYSCTICK_CLK = HAL_RCC_GET_AHBFREQ();
		switch( SYS ->CLCK_DIV )
		{
			case SYS_CLK_8:
				SYSCTICK_CLK /= 8;
				break;
			case SYS_CLK_1:
				SYSTICK->CTRL |= CLKSRC_1;
				break;
			default:
				Err_Status = HAL_ERR;
		}
		#if SYSTICK_INT_EN == EN
		    /* Initilize Callback Function*/
			SYS_CALLBACK = SYS->SYS_Callback;
			/*Enable Interrupt through Ctrl Register */
			LOAD_VAL = (SYSCTICK_CLK/1000); //1ms
			SYSTICK->LOAD = LOAD_VAL;
			SYSTICK->VAL = SYSTICK_CLEAR;
			SYSTICK->CTRL |= TICKINT_EN;
			SYSTICK->CTRL |= COUNTER_ENABLE;
		#endif
	}
	else{
		Err_Status = HAL_ERR;
	}
	return Err_Status;
}

#if SYSTICK_INT_EN == DIS

ERR Hal_SYSTICK_DelayMs(uint32_t delay)
{
	ERR Err_Status = HAL_OK;
	uint32_t LOAD_VAL = 0;

	LOAD_VAL = (SYSCTICK_CLK/1000); //1ms
	if( LOAD_VAL > SYSTICK_MAX)
	{
		Err_Status = HAL_ERR;
	}else{
		uint32_t delay_cnt=0;
		SYSTICK->LOAD  = LOAD_VAL;
		SYSTICK->VAL   = SYSTICK_CLEAR;
		SYSTICK->CTRL |= COUNTER_ENABLE;
		/*Delay */
		for (delay_cnt = 0; delay_cnt <= delay - 1; delay_cnt++)
		{
			/*Wait Untill Flag is Set */
			while (!(SYSTICK->CTRL & COUNTFLAG_MSK));
		}
		/*Disable Counter*/
		SYSTICK->CTRL &= COUNTER_DISABLE;
	}
	return Err_Status;
}

ERR Hal_SYSTICK_DelayUs(uint32_t delay)
{
	ERR Err_Status = HAL_OK;
	uint32_t LOAD_VAL = 0;

	LOAD_VAL = (SYSCTICK_CLK / 1000000); //1us
	if (LOAD_VAL > SYSTICK_MAX) {
		Err_Status = HAL_ERR;
	} else {
		uint32_t delay_cnt=0;
		SYSTICK->LOAD = LOAD_VAL;
		SYSTICK->VAL = SYSTICK_CLEAR;
		SYSTICK->CTRL |= COUNTER_ENABLE;

		/*Delay */
		for (delay_cnt = 0; delay_cnt <= delay - 1; delay_cnt++)
		{
			/*Wait Untill Flag is Set */
			while (!(SYSTICK->CTRL & COUNTFLAG_MSK));
		}
		/*Disable Counter*/
		SYSTICK->CTRL &= COUNTER_DISABLE;
	}
	return Err_Status;
}
#endif

#if RTOS == DIS
#if SYSTICK_INT_EN == EN
  void SysTick_Handler(void)
  {
	  /* Call the callback Function*/
	  if( SYS_CALLBACK != NULL)
		  SYS_CALLBACK();
  }
#endif
#endif


