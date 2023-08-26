/*
 * Hal_Timer_cfg.h
 *
 *  Created on: Aug 4, 2023
 *      Author: Seif pc
 */

#ifndef HAL_HAL_TIMER_CFG_H_
#define HAL_HAL_TIMER_CFG_H_

/*-------------Includes Section Start---------------*/
#include <stdint.h>
#include "../std_macros.h"
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/
#define configHALTIMER_DELAY_EN		EN // Counter and Timer Mode
#define configHALTIMX_ICP_EN		EN // ICP
#define configHALTIMX_CCP_EN		EN // OCP
#define configHALTIMX_PWM_EN 		EN // PWM Input and Output

#define configHALTIM1_INT_EN		EN
#define configHALTIM2_INT_EN		EN
#define configHALTIM3_INT_EN		DIS
#define configHALTIM4_INT_EN		EN
#define configHALTIM5_INT_EN		EN

#define configHALTIMX_INT_EN		configHALTIM1_INT_EN || configHALTIM2_INT_EN || configHALTIM3_INT_EN||\
									configHALTIM4_INT_EN || configHALTIM5_INT_EN
/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/
/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/


/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/

/*-----------Software Interfaces End--------------*/

#endif /* HAL_HAL_TIMER_CFG_H_ */
