/*
 * CortexM4_core.h
 *
 *  Created on: Jul 25, 2023
 *      Author: Seif pc
 */

#ifndef CORE_CORTEXM4_CORE_SYSTICK_H_
#define CORE_CORTEXM4_CORE_SYSTICK_H_

/*-------------Includes Section Start---------------*/
#include "../std_macros.h"
#include "../HAL/Hal_RCC.h"
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/
#define SYSTICK_BASE_ADDRESS		0xE000E010UL
#define SYSTICK						((volatile SYSTICK_Typedef*)(SYSTICK_BASE_ADDRESS))

#define SYSTICK_INT_EN			DIS

#define SYS_CLK_8				0U
#define SYS_CLK_1				1U
/*-----------CTRL-------------*/
#define COUNTFLAG 		16U
#define COUNTFLAG_MSK	(1<<COUNTFLAG)

#define ENABLE			0U
#define COUNTER_ENABLE	(1<<ENABLE)
#define COUNTER_DISABLE (~COUNTER_ENABLE)

#define TICKINT			1U
#define TICKINT_EN		(1<<TICKINT)

#define	CLKSRC			2U
#define CLKSRC_8		(~(1<<CLKSRC))
#define CLKSRC_1		(1<<CLKSRC)

#define SYSTICK_MAX		0x00FFFFFFU
#define SYSTICK_CLEAR	0x00000000U
/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/


/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/
typedef struct
{
	__IO uint32_t CTRL;
	__IO uint32_t LOAD;
	__IO uint32_t VAL;
	__IO uint32_t CALIB;
}SYSTICK_Typedef;

typedef struct
{
	uint32_t LOAD_VAL;
	uint8_t CLCK_DIV;
#if SYSTICK_INT_EN == EN
	void (*SYS_Callback)(void);
#endif
}SYSTICK_CFG;

/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/
ERR HAL_SYSTICK_Init(SYSTICK_CFG *SYS);
#if SYSTICK_INT_EN == DIS
ERR Hal_SYSTICK_DelayMs(uint32_t delay);
ERR Hal_SYSTICK_DelayUs(uint32_t delay);
#endif
/*-----------Software Interfaces End--------------*/

#endif /* CORE_CORTEXM4_CORE_SYSTICK_H_ */
