/*
 * Hal_BME280.c
 *
 *  Created on: Aug 18, 2023
 *      Author: Seif Eldidi
 */
#include "../../Inc/HAL/Hal_BME280.h"

void HAL_BME280Init()
{
	uint8_t Data[]={0x01,CTRL_MEAS,0x27,CONFIG_REG,0x00};
	xMCAL_I2C_MSTRSend(I2C1,BME_280_SA,CTRL_HUMIDITY,Data,5);
}

BME280_S32_t Tfine;

int HAL_BME280_TEMP_Read()
{
	uint8_t  Temp[3];
	uint8_t  Tepm_2[6];
	BME280_S32_t adc_T =0;
	uint16_t dig_T1 =0;
	int16_t  dig_T2 =0;
	int16_t  dig_T3 =0;
	BME280_S32_t  var1=0 ,var2,T=0;

	xMCAL_I2C_MSTRRec(I2C1,BME_280_SA,TEMP_REG,Temp,3);
	xMCAL_I2C_MSTRRec(I2C1,BME_280_SA,TEMP_COMPENSATE,Tepm_2,6);

	dig_T1 = (((uint16_t)Tepm_2[0]) + ((uint16_t)Tepm_2[1]<<8) );
	dig_T2 = (((int16_t)Tepm_2[2]) + ((int16_t)Tepm_2[3]<<8));
	dig_T3 = (((int16_t)Tepm_2[4]) + ((int16_t)Tepm_2[5]<<8)) ;

	adc_T =(uint32_t)( (Temp[0]<<12) + (Temp[1]<<4) + ((Temp[0]&0xF0>>4)<<0));

	var1 = ((((adc_T>>3) - ((BME280_S32_t)dig_T1<<1))) * ((BME280_S32_t)dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((BME280_S32_t)dig_T1)) * ((adc_T>>4) - ((BME280_S32_t)dig_T1))) >> 12) * ((BME280_S32_t)dig_T3)) >> 14;
	Tfine = var1 + var2;
	T = (Tfine * 5 + 128) >> 8	;

	return T;
}

int HAL_BME280_Pressure_Read()
{
	uint8_t Temp[3];
	uint8_t Tepm_2[9];
	BME280_S32_t adc_P = 0;
	uint16_t dig_P1 = 0;
	int16_t dig_P2 = 0;
	int16_t dig_P3 = 0;
	BME280_S32_t var1 = 0, var2, p = 0;

	xMCAL_I2C_MSTRRec(I2C1, BME_280_SA, PRESSURE, Temp, 3);
	xMCAL_I2C_MSTRRec(I2C1, BME_280_SA, TEMP_COMPENSATE, Tepm_2, 6);

	BME280_S64_t var1, var2, p; var1 = ((BME280_S64_t)t_fine) - 128000;
	var2 = var1 * var1 * (BME280_S64_t)dig_P6; var2 = var2 + ((var1*(BME280_S64_t)dig_P5)<<17);
	var2 = var2 + (((BME280_S64_t)dig_P4)<<35); var1 = ((var1 * var1 * (BME280_S64_t)dig_P3)>>8) + ((var1 * (BME280_S64_t)dig_P2)<<12);
	var1 = (((((BME280_S64_t)1)<<47)+var1))*((BME280_S64_t)dig_P1)>>33;
	if (var1 == 0) {
		return 0;
	// avoid exception caused by division by zero
	}
	p = 1048576-adc_P; p = (((p<<31)-var2)*3125)/var1;
	var1 = (((BME280_S64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((BME280_S64_t)dig_P8) * p) >> 19; p = ((p + var1 + var2) >> 8) + (((BME280_S64_t)dig_P7)<<4);
	return (BME280_U32_t)p;
}
