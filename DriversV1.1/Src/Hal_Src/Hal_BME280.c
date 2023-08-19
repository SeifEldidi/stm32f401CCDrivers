/*
 * Hal_BME280.c
 *
 *  Created on: Aug 18, 2023
 *      Author: Seif Eldidi
 */
#include "../../Inc/HAL/Hal_BME280.h"

void HAL_BME280Init(BME_Init_t * Config)
{
	uint8_t Data[5];
	Data[0] = Config->Humidity_Smp;
	Data[1] = CTRL_MEAS;
	Data[2] = (((Config->Temp_Smp & 0x07)<<5)|(Config->Pressure_Smp<<2)|0x03);
	Data[3] = CONFIG_REG;
	Data[4] = 0x00;
	xMCAL_I2C_MSTRSend(I2C1,BME_280_SA,CTRL_HUMIDITY,Data,5);
}

BME280_S32_t Tfine;

BME280_S32_t HAL_BME280_TEMP_Read()
{
	uint8_t  Temp[DEF_MES_LEN];
	uint8_t  Tepm_2[TEMP_COMPEN_LEN];
	BME280_S32_t adc_T =0;
	uint16_t dig_T1 =0;
	int16_t  dig_T2 =0;
	int16_t  dig_T3 =0;
	BME280_S32_t  var1=0 ,var2,T=0;

	xMCAL_I2C_MSTRRec(I2C1,BME_280_SA,TEMP_REG,Temp,DEF_MES_LEN);
	xMCAL_I2C_MSTRRec(I2C1,BME_280_SA,TEMP_COMPEN_REG,Tepm_2,TEMP_COMPEN_LEN);

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

BME280_S32_t HAL_BME280_Pressure_Read()
{
	uint8_t Temp[DEF_MES_LEN];
	uint8_t Temp_2[PRESS_COMPEN_LEN];
	BME280_S32_t adc_P = 0;
	uint16_t dig_P1 = 0;
	int16_t dig_P2 = 0;
	int16_t dig_P3 = 0;
	int16_t dig_P4 = 0;
	int16_t dig_P5 = 0;
	int16_t dig_P6 = 0;
	int16_t dig_P7 = 0;
	int16_t dig_P8 = 0;
	int16_t dig_P9 = 0;
	BME280_S32_t var1 = 0, var2, p = 0;
    /*-----------Read I2C in Buffer-------------*/
	xMCAL_I2C_MSTRRec(I2C1, BME_280_SA, PRESSURE, Temp, DEF_MES_LEN);
	xMCAL_I2C_MSTRRec(I2C1, BME_280_SA, PRESS_COMPEN_REG, Temp_2, PRESS_COMPEN_LEN);
	/*-----------Processs Buffer----------------*/
	adc_P  = (uint32_t)( (Temp[0]<<12) + (Temp[1]<<4) + ((Temp[0]&0xF0>>4)<<0));
	dig_P1 = (((uint16_t)Temp_2[0]) + ((uint16_t)Temp_2[1]<<8) );
	dig_P2 = (((uint16_t)Temp_2[2]) + ((uint16_t)Temp_2[3]<<8) );
	dig_P3 = (((uint16_t)Temp_2[4]) + ((uint16_t)Temp_2[5]<<8) );
	dig_P4 = (((uint16_t)Temp_2[6]) + ((uint16_t)Temp_2[7]<<8) );
	dig_P5 = (((uint16_t)Temp_2[8]) + ((uint16_t)Temp_2[9]<<8) );
	dig_P6 = (((uint16_t)Temp_2[10]) + ((uint16_t)Temp_2[11]<<8) );
	dig_P7 = (((uint16_t)Temp_2[12]) + ((uint16_t)Temp_2[13]<<8) );
	dig_P8 = (((uint16_t)Temp_2[14]) + ((uint16_t)Temp_2[15]<<8) );
	dig_P9 = (((uint16_t)Temp_2[16]) + ((uint16_t)Temp_2[17]<<8) );
	/*-------------Proccess Data---------*/
	var1 = ((BME280_S64_t)Tfine) - 128000;
	var2 = var1 * var1 * (BME280_S64_t)dig_P6;
	var2 = var2 + ((var1*(BME280_S64_t)dig_P5)<<17);
	var2 = var2 + (((BME280_S64_t)dig_P4)<<35);
	var1 = ((var1 * var1 * (BME280_S64_t)dig_P3)>>8) + ((var1 * (BME280_S64_t)dig_P2)<<12);
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

BME280_S32_t HAL_BME280_Humidity_Read()
{
	uint8_t Temp[HUM_MES_LEN];
	uint8_t Temp_2[HUM_COMPEN_LEN];
	BME280_S32_t adc_H = 0;
	uint8_t dig_H1 = 0;
	int16_t dig_H2 = 0;
	uint8_t dig_H3 = 0;
	int16_t dig_H4 = 0;
	int16_t dig_H5 = 0;
	int8_t  dig_H6 = 0;
	/*-----------Read I2C in Buffer-------------*/
	xMCAL_I2C_MSTRRec(I2C1, BME_280_SA, HUMIDITY, Temp, HUM_MES_LEN);
	xMCAL_I2C_MSTRRec(I2C1, BME_280_SA, HUM_COMPEN_REG, Temp_2,1);
	xMCAL_I2C_MSTRRec(I2C1, BME_280_SA, HUM_COMPEN_REG_1, Temp_2+1,HUM_COMPEN_LEN-1);
	/*-----------Processs Buffer----------------*/
	adc_H =  (int32_t) ((Temp[0] << 8) + (Temp[1] ));
	dig_H1 = ((uint8_t)  Temp_2[0]);
	dig_H2 = (((int16_t) Temp_2[1]) + ((int16_t) Temp_2[2] << 8));
	dig_H3 = ((uint8_t)  Temp_2[3]);
	dig_H4 = (((int16_t)Temp[4])<<4) + (((int16_t)Temp[5])&0x0F);
	dig_H5 = (((int16_t) Temp_2[6])&0x0F) + (((int16_t) Temp_2[7])<<4);
	dig_H6 = ((int8_t) Temp_2[8]);
	/*-------------Proccess Data---------*/
	BME280_S32_t v_x1_u32r;

	v_x1_u32r = (Tfine - ((BME280_S32_t)76800));
	v_x1_u32r = (((((adc_H << 14) - (((BME280_S32_t)dig_H4) << 20) - (((BME280_S32_t)dig_H5) * v_x1_u32r)) + ((BME280_S32_t)16384)) >> 15) * (((((((v_x1_u32r *
	((BME280_S32_t)dig_H6)) >> 10) * (((v_x1_u32r * ((BME280_S32_t)dig_H3)) >> 11) + ((BME280_S32_t)32768))) >> 10) + ((BME280_S32_t)2097152)) * ((BME280_S32_t)dig_H2) + 8192) >> 14));
	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((BME280_S32_t)dig_H1)) >> 4));
	v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
	v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
	return (BME280_U32_t)(v_x1_u32r>>12);
}

ERR HAL_BME_Read(BME_Mes * Mes)
{
	ERR Err_state = HAL_OK;
	if(Mes != NULL)
	{
		Mes->Temp = HAL_BME280_TEMP_Read();
		Mes->Press = HAL_BME280_Pressure_Read();
		Mes->Hum = HAL_BME280_Humidity_Read();
	}else{
		Err_state = HAL_ERR;
	}
	return Err_state;
}
