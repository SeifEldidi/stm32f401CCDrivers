/*
 * Hal_INA219.h
 *
 *  Created on: Aug 22, 2023
 *      Author: Seif pc
 */

#ifndef HAL_HAL_INA219_H_
#define HAL_HAL_INA219_H_

/*-------------Includes Section Start---------------*/
#include "../std_macros.h"
#include "../Mcal/Mcal_I2C.h"
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/
#define INA219_DEF_ADDRESS 			0x40
#define GND							0x00
#define VSS							0x01

#define INA219_CONFIG_REG			0x00
#define INA219_SHUNTVOLTAGE_REG		0x01
#define INA219_BUSVOLTAGE_REG		0x02
#define INA219_POWER_REG			0x03
#define INA219_CURRENT_REG			0x04
#define INA219_CALIBRATION_REG		0x05
#define RSHUNT						0.1

#define BUS_Voltage_16V				0
#define BUS_Voltage_32V				1

#define PGA_DIV1					0U
#define PGA_DIV2					1U
#define PGA_DIV4					2U
#define PGA_DIV8					3U

#define BUS_32V_CURR_2A				0x01
#define BUS_32V_CURR_1A				0x02
#define BUS_16V_CURR_400mA			0x03
/*-----------INA219_CONFIG_REG---------*/
#define RESET						15U
#define INA219_RESET				(1<<RESET)

#define BUS_VFRG					13U
#define BUS_VFRG_32V				(1<<BUS_VFRG)
#define BUS_VFRG_16V				(~(BUS_VFRG_32V))

#define PG							11U
#define PG_MSK						(3<<PG)
#define PG_1						(0<<PG)
#define PG_2						(1<<PG)
#define PG_4						(2<<PG)
#define PG_8						(3<<PG)

#define BADC									7U
#define BADC_MSK								(7<<BADC)
#define INA219_CONFIG_BADCRES_9BIT   			(0<<BADC)
#define INA219_CONFIG_BADCRES_10BIT  			(1<<BADC)
#define INA219_CONFIG_BADCRES_11BIT  			(2<<BADC)
#define INA219_CONFIG_BADCRES_12BIT  			(3<<BADC)
#define INA219_CONFIG_BADCRES_12BIT_2S_1060US 	(9<<BADC)
#define INA219_CONFIG_BADCRES_12BIT_4S_2130US 	(10<<BADC)
#define INA219_CONFIG_BADCRES_12BIT_8S_4260US 	(11<<BADC)
#define INA219_CONFIG_BADCRES_12BIT_16S_8510US 	(12<<BADC)
#define INA219_CONFIG_BADCRES_12BIT_32S_17MS 	(13<<BADC)
#define INA219_CONFIG_BADCRES_12BIT_64S_34MS 	(14<<BADC)
#define INA219_CONFIG_BADCRES_12BIT_128S_69MS 	(15<<BADC)

#define	SADC									3U
#define SADC_MSK								(7<<SADC)
#define INA219_CONFIG_SADCRES_9BIT   			(0<<SADC)
#define INA219_CONFIG_SADCRES_10BIT  			(1<<SADC)
#define INA219_CONFIG_SADCRES_11BIT  			(2<<SADC)
#define INA219_CONFIG_SADCRES_12BIT  			(3<<SADC)
#define INA219_CONFIG_SADCRES_12BIT_2S_1060US 	(9<<SADC)
#define INA219_CONFIG_SADCRES_12BIT_4S_2130US 	(10<<SADC)
#define INA219_CONFIG_SADCRES_12BIT_8S_4260US 	(11<<SADC)
#define INA219_CONFIG_SADCRES_12BIT_16S_8510US 	(12<<SADC)
#define INA219_CONFIG_SADCRES_12BIT_32S_17MS 	(13<<SADC)
#define INA219_CONFIG_SADCRES_12BIT_64S_34MS 	(14<<SADC)
#define INA219_CONFIG_SADCRES_12BIT_128S_69MS 	(15<<SADC)

#define OPMODE									0U
#define OPMODE_MSK								(7<<OPMODE)
#define POWERDOWN								(0<<OPMODE)
#define SHUNTVOLTAGE_TRIGGERED					(1<<OPMODE)
#define BUSVOLTAGE_TRIGGERED					(2<<OPMODE)
#define SHUNTBUS_TRIGGERED						(3<<OPMODE)
#define ADCOFFDISABLED							(4<<OPMODE)
#define SHUNTVOLTAGE_CONTINUOUS					(5<<OPMODE)
#define BUSVOLTAGE_CONTINUOUS					(6<<OPMODE)
#define SHUNTANDBUS_CONTINUOUS					(7<<OPMODE)
/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/
#define INA219_CALC_ADD(A1,A0) ( INA219_DEF_ADDRESS|= (A1==GND)?(0b00<<2):(0b01<<2) \
		INA219_DEF_ADDRESS|= (A0==GND)?(0b00<<0):(0b01<<0) )
/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/
/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/
void INA219_Init(uint8_t CalibVal);
float INA219_GetCurrentmA();
float INA219_GetShuntVoltagemV();
float INA219_GetBusVoltageV();
float INA219_GetPowermW();
/*-----------Software Interfaces End--------------*/

#endif /* HAL_HAL_INA219_H_ */
