/*
 * Hal_BME280.h
 *
 *  Created on: Aug 18, 2023
 *      Author: Seif Eldidi
 */

#ifndef HAL_HAL_BME280_H_
#define HAL_HAL_BME280_H_

/*-------------Includes Section Start---------------*/
#include "../std_macros.h"
#include "../Mcal/Mcal_I2C.h"
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/
#define BME_280_SA	  0x76
#define CTRL_HUMIDITY 0xF2
#define CTRL_MEAS	  0xF4
#define CONFIG_REG	  0xF5
#define PRESSURE	  0xF7
#define TEMP_REG	  0xFA
#define HUMIDITY	  0xFD
#define TEMP_COMPENSATE	0x88
/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/
/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/
typedef  int32_t BME280_S32_t;

/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/
void HAL_BME280Init();
int HAL_BME280_TEMP_Read();
/*-----------Software Interfaces End--------------*/


#endif /* HAL_HAL_BME280_H_ */
