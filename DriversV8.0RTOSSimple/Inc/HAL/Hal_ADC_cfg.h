/*
 * Hal_ADC_cfg.h
 *
 *  Created on: Jul 27, 2023
 *      Author: Seif pc
 */

#ifndef HAL_HAL_ADC_CFG_H_
#define HAL_HAL_ADC_CFG_H_

/*-------------Includes Section Start---------------*/
#include <stdint.h>
#include "../std_macros.h"
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/
#define ADC_TEMPSENSOR_READ	DIS
#define ANALOG_WDGEN		DIS
#define ADC_SINGLECONV		EN
#define ADC_GROUPCONV		DIS
#define ADC_GROUPCONV_DMA	DIS
#define ADC_GROUPTINJ_DMA	DIS
#define ADC_INTERRUPT_EN	EN


#define ADC_GROUPCONV_DMA_EN 	ADC_GROUPCONV_DMA == EN && ADC_GROUPINJCONV == DIS && ADC_GROUPTINJ_DMA == DIS&&ADC_GROUPCONV==DIS \
	&& ADC_SINGLECONV == DIS && ADC_GROUPTINJ_DMA == DIS &&(ADC_TEMPSENSOR_READ == EN || ADC_TEMPSENSOR_READ== DIS)

#define ADC_GROUPCONV_EN  ADC_GROUPCONV == EN && ADC_GROUPINJCONV == DIS && ADC_GROUPTINJ_DMA == DIS \
	&& ADC_SINGLECONV == DIS && ADC_GROUPCONV_DMA == DIS &&(ADC_TEMPSENSOR_READ == EN || ADC_TEMPSENSOR_READ== DIS)

#define ADC_SINGLECONV_EN  ADC_SINGLECONV == EN && ADC_GROUPCONV == DIS && ADC_GROUPINJCONV ==DIS && ADC_GROUPTINJ_DMA==DIS\
	   &&(ADC_TEMPSENSOR_READ == EN || ADC_TEMPSENSOR_READ== DIS)
/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/
/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/


/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/

/*-----------Software Interfaces End--------------*/


#endif /* HAL_HAL_ADC_CFG_H_ */
