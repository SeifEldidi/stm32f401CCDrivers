/*
 * Hal_INA219.c
 *
 *  Created on: Aug 22, 2023
 *      Author: Seif pc
 */
#include "../../Inc/HAL/Hal_INA219.h"

uint8_t CalibValue = 0x00;

static void INA219_SetVoltage32V_I2A()
{
	/*--------PGA/8 -------*/
	int16_t INA_CalibVal = 4096; // LSB_CURRENT = 100uA
	int16_t INA_ConfigVal = 0;
	/*--------Write Values To Calibartion Register-------*/
	uint8_t I2CData[2];
	I2CData[0] = ((uint8_t)(INA_CalibVal>>8));
	I2CData[1] = ((uint8_t)(INA_CalibVal));
	xMCAL_I2C_MSTRSend(I2C1,INA219_DEF_ADDRESS,INA219_CALIBRATION_REG,I2CData,2);
	/*--------Configure INA219---------------------------*/
	INA_ConfigVal = BUS_VFRG_32V|PG_8|INA219_CONFIG_BADCRES_12BIT|INA219_CONFIG_SADCRES_12BIT|
			SHUNTANDBUS_CONTINUOUS;
	I2CData[0] = ((uint8_t)(INA_ConfigVal>>8));
	I2CData[1] = ((uint8_t)(INA_ConfigVal));
	xMCAL_I2C_MSTRSend(I2C1,INA219_DEF_ADDRESS,INA219_CONFIG_REG,I2CData,2);
}

static void INA219_SetVoltage32V_I1A()
{
	/*--------PGA/8 -------*/
	int16_t INA_CalibVal = 10240;/*--------*/
	int16_t INA_ConfigVal = 0;
	/*--------Write Values To Calibartion Register-------*/
	uint8_t I2CData[2];
	I2CData[0] = (INA_CalibVal>>8);
	I2CData[1] = ((uint8_t)INA_CalibVal);
	xMCAL_I2C_MSTRSend(I2C1,INA219_DEF_ADDRESS,INA219_CALIBRATION_REG,I2CData,2);
	/*--------Configure INA219---------------------------*/
	INA_ConfigVal = BUS_VFRG_32V|PG_8|INA219_CONFIG_BADCRES_12BIT|INA219_CONFIG_SADCRES_12BIT|
			SHUNTANDBUS_CONTINUOUS;
	I2CData[0] = ((uint8_t)(INA_ConfigVal>>8));
	I2CData[1] = ((uint8_t)(INA_ConfigVal));
	xMCAL_I2C_MSTRSend(I2C1,INA219_DEF_ADDRESS,INA219_CONFIG_REG,I2CData,2);
}

static void INA219_SetVoltage16V_I400mA()
{
	/*--------PGA/8 -------*/
	int16_t INA_CalibVal = 20480; // based on 20uA resolution max Curr = 0.65536A
	int16_t INA_ConfigVal = 0;
	/*--------Write Values To Calibartion Register-------*/
	uint8_t I2CData[2];
	I2CData[0] = (INA_CalibVal>>8);
	I2CData[1] = ((uint8_t)INA_CalibVal);
	xMCAL_I2C_MSTRSend(I2C1,INA219_DEF_ADDRESS,INA219_CALIBRATION_REG,I2CData,2);
	/*--------Configure INA219---------------------------*/
	INA_ConfigVal &= BUS_VFRG_16V;
	INA_ConfigVal |= PG_8|INA219_CONFIG_BADCRES_12BIT|INA219_CONFIG_SADCRES_12BIT|
			SHUNTANDBUS_CONTINUOUS;
	I2CData[0] = ((uint8_t)(INA_ConfigVal>>8));
	I2CData[1] = ((uint8_t)(INA_ConfigVal));
	xMCAL_I2C_MSTRSend(I2C1,INA219_DEF_ADDRESS,INA219_CONFIG_REG,I2CData,2);
}

void INA219_Init(uint8_t CalibVal)
{
	CalibValue = CalibVal;
	switch(CalibVal)
	{
		case BUS_32V_CURR_2A:
			INA219_SetVoltage32V_I2A();
			break;
		case BUS_32V_CURR_1A:
			INA219_SetVoltage32V_I1A();
			break;
		case BUS_16V_CURR_400mA:
			INA219_SetVoltage16V_I400mA();
			break;
		default:
			break;
	}
}

float INA219_GetCurrentmA()
{
	float Curr_Ret = 0;
	uint8_t Curr_Val[2] = {0};
	xMCAL_I2C_MSTRRec(I2C1,INA219_DEF_ADDRESS,INA219_CURRENT_REG,Curr_Val,2);
	Curr_Ret = (((int16_t)(Curr_Val[0]<<8)) + Curr_Val[1]);
	if(CalibValue == BUS_32V_CURR_2A )
	{
		Curr_Ret /= 10;
	}else if(CalibValue == BUS_32V_CURR_2A){
		Curr_Ret /= 25;
	}else {
		Curr_Ret /= 20;
	}
	return Curr_Ret;
}

float INA219_GetShuntVoltagemV()
{
	float Curr_Ret = 0;
	uint8_t ShuntVoltage[2] = { 0 };
	xMCAL_I2C_MSTRRec(I2C1, INA219_DEF_ADDRESS, INA219_SHUNTVOLTAGE_REG, ShuntVoltage, 2);
	Curr_Ret = (( ((int16_t)ShuntVoltage[0] << 8)) + ShuntVoltage[1]);
	Curr_Ret /= 100;
	return Curr_Ret;
}

float INA219_GetBusVoltageV()
{
	float Curr_Ret = 0;
	uint8_t BusVoltage[2] = { 0 };
	xMCAL_I2C_MSTRRec(I2C1, INA219_DEF_ADDRESS, INA219_BUSVOLTAGE_REG, BusVoltage, 2);
	Curr_Ret = ((int16_t)BusVoltage[0]<<5) +((BusVoltage[1]&0xF8)>>3);
	Curr_Ret = ((Curr_Ret *4)/1000);
	return Curr_Ret;
}

float INA219_GetPowermW()
{
	float Curr_Ret = 0;
	uint8_t Power_Val[2] = { 0 };
	xMCAL_I2C_MSTRRec(I2C1, INA219_DEF_ADDRESS, INA219_POWER_REG,
			Power_Val, 2);
	Curr_Ret = (((int16_t) (Power_Val[0] << 8)) + Power_Val[1]);
	if (CalibValue == BUS_32V_CURR_2A) {
		Curr_Ret*= (2.0f);
	} else if (CalibValue == BUS_32V_CURR_2A) {
		Curr_Ret*= (0.8f);
	} else {
		Curr_Ret*= (1.0f);
	}
	return Curr_Ret;
}
