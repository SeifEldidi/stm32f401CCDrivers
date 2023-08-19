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

#define TEMP_COMPEN_REG		0x88
#define PRESS_COMPEN_REG	0x8E
#define HUM_COMPEN_REG		0xA1
#define HUM_COMPEN_REG_1	0xE1
#define DEF_MES_LEN			3U
#define HUM_MES_LEN			2U
#define PRESS_COMPEN_LEN	18U
#define TEMP_COMPEN_LEN		6U
#define HUM_COMPEN_LEN		9U

#define READ_SKIP			0U
#define OVERSAMPLING_1		1U
#define OVERSAMPLING_2		2U
#define OVERSAMPLING_4		3U
#define OVERSAMPLING_8		4U
#define OVERSAMPLING_16		5U
/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/
/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/
typedef  uint32_t BME280_U32_t;
typedef  int32_t  BME280_S32_t;
typedef  int64_t  BME280_S64_t;

typedef struct
{
	uint8_t Temp_Smp;
	uint8_t Pressure_Smp;
	uint8_t Humidity_Smp;
}BME_Init_t;

typedef struct
{
	BME280_S32_t Temp,Press,Hum;
}BME_Mes;

/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/
void HAL_BME280Init(BME_Init_t * Config);
BME280_S32_t HAL_BME280_Pressure_Read();
BME280_S32_t HAL_BME280_Humidity_Read();
BME280_S32_t HAL_BME280_TEMP_Read();
ERR HAL_BME_Read(BME_Mes * Mes);
/*-----------Software Interfaces End--------------*/


#endif /* HAL_HAL_BME280_H_ */
