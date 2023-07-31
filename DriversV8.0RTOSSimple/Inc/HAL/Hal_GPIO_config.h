/*
 * Hal_GPIO_config.h
 *
 *  Created on: Jul 27, 2023
 *      Author: Seif pc
 */

#ifndef HAL_HAL_GPIO_CONFIG_H_
#define HAL_HAL_GPIO_CONFIG_H_


/*-------------Includes Section Start---------------*/
#include <stdint.h>
#include "../std_macros.h"
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/
#define GPIO_EXTI_LINE0_EN	DIS
#define GPIO_EXTI_LINE1_EN	DIS
#define GPIO_EXTI_LINE2_EN	DIS
#define GPIO_EXTI_LINE3_EN	DIS
#define GPIO_EXTI_LINE4_EN	DIS
#define GPIO_EXTI_LINE5_EN	DIS
#define GPIO_EXTI_LINE6_EN	DIS
#define GPIO_EXTI_LINE7_EN	DIS
#define GPIO_EXTI_LINE8_EN	DIS
#define GPIO_EXTI_LINE9_EN	DIS
#define GPIO_EXTI_LINE10_EN	DIS
#define GPIO_EXTI_LINE11_EN	DIS
#define GPIO_EXTI_LINE12_EN	DIS
#define GPIO_EXTI_LINE13_EN	DIS
#define GPIO_EXTI_LINE14_EN	DIS
#define GPIO_EXTI_LINE15_EN	DIS

#define GPIO_ENT_EN (GPIO0_INT_EN || GPIO1_INT_EN || GPIO2_INT_EN ||\
	GPIO3_INT_EN || GPIO4_INT_EN || GPIO5_INT_EN||\
	GPIO6_INT_EN || GPIO7_INT_EN || GPIO8_INT_EN||\
	GPIO9_INT_EN || GPIO10_INT_EN || GPIO11_INT_EN||\
	GPIO12_INT_EN || GPIO13_INT_EN || GPIO14_INT_EN ||GPIO15_INT_EN )

#define GPIO0_INT_EN 	(GPIO_EXTI_LINE0_EN == EN)
#define GPIO1_INT_EN 	(GPIO_EXTI_LINE1_EN == EN)
#define GPIO2_INT_EN	(GPIO_EXTI_LINE2_EN == EN)
#define GPIO3_INT_EN 	(GPIO_EXTI_LINE3_EN == EN)
#define GPIO4_INT_EN	(GPIO_EXTI_LINE4_EN == EN)
#define GPIO5_INT_EN 	(GPIO_EXTI_LINE5_EN == EN)
#define GPIO6_INT_EN	(GPIO_EXTI_LINE6_EN == EN)
#define GPIO7_INT_EN 	(GPIO_EXTI_LINE7_EN == EN)
#define GPIO8_INT_EN	(GPIO_EXTI_LINE8_EN == EN)
#define GPIO9_INT_EN 	(GPIO_EXTI_LINE9_EN == EN)
#define GPIO10_INT_EN	(GPIO_EXTI_LINE10_EN == EN)
#define GPIO11_INT_EN 	(GPIO_EXTI_LINE11_EN == EN)
#define GPIO12_INT_EN	(GPIO_EXTI_LINE12_EN == EN)
#define GPIO13_INT_EN 	(GPIO_EXTI_LINE13_EN == EN)
#define GPIO14_INT_EN	(GPIO_EXTI_LINE14_EN == EN)
#define GPIO15_INT_EN 	(GPIO_EXTI_LINE15_EN == EN)
/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/
/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/


/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/

/*-----------Software Interfaces End--------------*/

#endif /* HAL_HAL_GPIO_CONFIG_H_ */
