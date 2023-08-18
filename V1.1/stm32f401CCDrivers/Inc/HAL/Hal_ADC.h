/*
 * Hal_ADC.h
 *
 *  Created on: Jul 27, 2023
 *      Author: Seif pc
 */

#ifndef HAL_HAL_ADC_H_
#define HAL_HAL_ADC_H_

/*-------------Includes Section Start---------------*/
#include <stdint.h>
#include "../std_macros.h"
#include "Hal_ADC_cfg.h"
#include "Hal_RCC.h"
#include "Hal_GPIO.h"
#include "HAL_Dma.h"
#include "../core/CortexM4_core_NVIC.h"
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/
#define APB2_PERIPHBASE			 0x40010000UL
#define ADC_OFFSET				 0x00002000UL
#define ADC_COMOFFSET			 0x00002300UL
#define ADC_BASE_PERIPH			 (ADC_OFFSET+APB2_PERIPHBASE)
#define ADC_CTRL_OFFSET			 (APB2_PERIPHBASE+ADC_COMOFFSET)
#define ADC				 		 ((__IO ADC_TypeDef*)(ADC_BASE_PERIPH))
#define ADC1_COMMON				 ((__IO ADC_Common_TypeDef*)(ADC_CTRL_OFFSET))
/*----------Single ModeConfig-------*/
#define LEFT_ALLIGNMENT 		 0x00
#define RIGHT_ALLIGNMENT		 0x01

#define ADC_RESOLUTION_12BIT	 0x00000000UL
#define ADC_RESOLUTION_10BIT	 0x00000001UL
#define ADC_RESOLUTION_8BIT		 0x00000002UL
#define ADC_RESOLUTION_6BIT		 0x00000003UL

#define ADC_CONTINOUS_MODE		 0x00000000UL
#define ADC_SINGLE_MODE			 0x00000001UL

#define ADC_CONT_NCHG_CHN		0x00000001UL
#define ADC_CONT_CHG_CHN		0x00000000UL

#define ADC_APBP_2				(0U)
#define ADC_APBP_4				(1U)
#define ADC_APBP_6				(2U)
#define ADC_APBP_8				(3U)

#if ADC_GROUPCONV_DMA == EN
#define ADC_IN0_CHN				0U
#define ADC_IN1_CHN				1U
#define ADC_IN2_CHN				2U
#define ADC_IN3_CHN				3U
#define ADC_IN4_CHN				4U
#define ADC_IN5_CHN				5U
#define ADC_IN6_CHN				6U
#define ADC_IN7_CHN				7U
#define ADC_IN8_CHN				8U
#define ADC_IN9_CHN				9U
#define ADC_IN10_CHN			10U
#define ADC_IN11_CHN			11U
#define ADC_IN12_CHN			12U
#define ADC_IN13_CHN			13U
#define ADC_IN14_CHN			14U
#define ADC_IN15_CHN			15U
#endif

#if ADC_TEMPSENSOR_READ == EN
#define TEMPSENS_EN			0x00U
#define TEMPSENS_DIS		0x01U
#endif

#define AWDG_ALL 		0xFFFF

#if ADC_SINGLECONV
#define ADC_SINGLE_INT_EN	EN
#define ADC_SINGLE_INT_DIS	DIS
#endif

#if ADC_GROUPCONV_EN
#define ADC_GROUP_INTERRUPTS_EN  EN
#define ADC_GROUP_INTERRUPTS_DIS DIS
#endif
/*----------ADC_CRQ-----------*/
#define ADC_OVR					 5U
#define ADC_OVR_READ			 (1<<ADC_OVR)

#define ADC_STRTFLG				 4U
#define ADC_STRF_READ			 (1<<ADC_STRTFLG)

#define ADC_JSTRTFLG			 3U
#define ADC_JSTRF_READ			 (1<<ADC_JSTRTFLG)

#define ADC_JEOCFLG				 2U
#define ADC_JEOCFLG_READ		 (1<<ADC_JEOCFLG)

#define ADC_EOCFLG				 1U
#define ADC_EOCFLG_READ		     (1<<ADC_EOCFLG)

#define ADC_AWDFLG				 0U
#define ADC_AWDFLG_READ		     (1<<ADC_AWDFLG)
/*---------ADC_CR1--------------*/
#define ADC_OVRIE				 26U
#define ADC_OVRIE_EN			 (1<<ADC_OVRIE)
#define ADC_OVRIE_DIS			 (~ADC_OVRIE_EN)

#define ADC_RES					 24U
#define ADC_CLEAR_3				 (~(0b11<<ADC_RES))
#define ADC_RES_12				 (0b00<<ADC_RES)
#define ADC_RES_10				 (0b01<<ADC_RES)
#define ADC_RES_8				 (0b10<<ADC_RES)
#define ADC_RES_6				 (0b11<<ADC_RES)

#define ADC_AWDEN				23U
#define ADC_AWDEN_EN			(1<<ADC_AWDEN)
#define ADC_AWDEN_DIS			(~ADC_AWDEN_EN)

#define ADC_JAWDEN				22U
#define ADC_JAWDEN_EN			(1<<ADC_JAWDEN)
#define ADC_JAWDEN_DIS			(~ADC_JAWDEN_EN)

#define ADC_JAUTO				10U
#define ADC_JAUTO_EN			(1<<ADC_JAUTO)

#define ADC_AWDSGL				9U
#define ADC_AWDSGL_EN			(1<<ADC_AWDSGL)
#define ADC_AWDSGL_DIS			(~ADC_AWDSGL_EN)

#define ADC_EOCIE				5
#define ADC_EOCIE_EN			(1<<ADC_EOCIE)

#define ADC_SCAN				8U
#define ADC_SCAN_EN			    (1<<ADC_SCAN)
/*---------ADC_CR2--------------*/
#define ADC_SWSTART				30U
#define ADC_SWSTART_EN			(1<<ADC_SWSTART)

#define ADC_JSWSTART			22U
#define ADC_JSWSTART_EN			(1<<ADC_JSWSTART)

#define ADC_ALIGN				11U
#define ADC_ALLIGN_R 			(~ADC_ALLIGN_L)
#define ADC_ALLIGN_L			(1<<ADC_ALIGN)

#define ADC_EOCS				10U
#define ADC_EOCS_READ 			(1<<ADC_EOCS)

#define ADC_DDS					9U
#define ADC_DDS_EN				(1<<ADC_DDS)

#define ADC_DMA					8
#define ADC_DMA_EN				(1<<8)

#define ADC_CONT				1U
#define ADC_CONT_EN				(1<<ADC_CONT)

#define ADC_ADON				0U
#define ADC_ADCON_EN			(1<<ADC_ADON)
/*--------ADC_SAMPLE------------*/
#define ADC_SMP_3CYC			0U
#define ADC_SMP_15CYC			1U
#define ADC_SMP_28CYC			2U
#define ADC_SMP_56CYC			3U
#define ADC_SMP_84CYC			4U
#define ADC_SMP_112CYC			5U
#define ADC_SMP_144CYC			6U
#define ADC_SMP_480CYC			7U
/*----------ADC_CLK----------*/
#define	ADC_TSVREFE				23U
#define ADC_TSVREFE_EN			(1<<ADC_TSVREFE)

#define ADC_VBATE				22U
#define ADC_VBATE_EN			(1<<ADC_VBATE)

#define ADC_PRESCALER			16U
#define ADC_APB_CLEAR			(~(0b11<<ADC_PRESCALER))
#define ADC_APB_2				(0<<ADC_PRESCALER)
#define ADC_APB_4				(1<<ADC_PRESCALER)
#define ADC_APB_6				(2<<ADC_PRESCALER)
#define ADC_APB_8				(3<<ADC_PRESCALER)
/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/

/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/
#if ADC_SINGLECONV
typedef enum
{
	ADC_IN0,
	ADC_IN1,
	ADC_IN2,
	ADC_IN3,
	ADC_IN4,
	ADC_IN5,
	ADC_IN6,
	ADC_IN7,
	ADC_IN8,
	ADC_IN9,
	ADC_IN10,
	ADC_IN11,
	ADC_IN12,
	ADC_IN13,
	ADC_IN14,
	ADC_IN15,
	ADC_IN16,
	ADC_IN17,
	ADC_IN18,
}ADC_CHANNEL;
#endif

#if ADC_GROUPCONV_DMA_EN || ADC_GROUPCONV
typedef enum
{
	ADC_IN0 = 1,
	ADC_IN1 = 2,
	ADC_IN2 = 4 ,
	ADC_IN3 = 8,
	ADC_IN4 = 16,
	ADC_IN5 = 32,
	ADC_IN6 = 64,
	ADC_IN7 = 128,
	ADC_IN8 = 256,
	ADC_IN9 = 512,
	ADC_IN10= 1024,
	ADC_IN11= 2048,
	ADC_IN12= 4096,
	ADC_IN13= 8192,
	ADC_IN14= 16384,
	ADC_IN15= 32768,
}ADC_CHANNEL;
#endif

typedef struct
{
  __IO uint32_t SR;     /*!< ADC status register,                         Address offset: 0x00 */
  __IO uint32_t CR1;    /*!< ADC control register 1,                      Address offset: 0x04 */
  __IO uint32_t CR2;    /*!< ADC control register 2,                      Address offset: 0x08 */
  __IO uint32_t SMPR1;  /*!< ADC sample time register 1,                  Address offset: 0x0C */
  __IO uint32_t SMPR2;  /*!< ADC sample time register 2,                  Address offset: 0x10 */
  __IO uint32_t JOFR1;  /*!< ADC injected channel data offset register 1, Address offset: 0x14 */
  __IO uint32_t JOFR2;  /*!< ADC injected channel data offset register 2, Address offset: 0x18 */
  __IO uint32_t JOFR3;  /*!< ADC injected channel data offset register 3, Address offset: 0x1C */
  __IO uint32_t JOFR4;  /*!< ADC injected channel data offset register 4, Address offset: 0x20 */
  __IO uint32_t HTR;    /*!< ADC watchdog higher threshold register,      Address offset: 0x24 */
  __IO uint32_t LTR;    /*!< ADC watchdog lower threshold register,       Address offset: 0x28 */
  __IO uint32_t SQR1;   /*!< ADC regular sequence register 1,             Address offset: 0x2C */
  __IO uint32_t SQR2;   /*!< ADC regular sequence register 2,             Address offset: 0x30 */
  __IO uint32_t SQR3;   /*!< ADC regular sequence register 3,             Address offset: 0x34 */
  __IO uint32_t JSQR;   /*!< ADC injected sequence register,              Address offset: 0x38*/
  __IO uint32_t JDR1;   /*!< ADC injected data register 1,                Address offset: 0x3C */
  __IO uint32_t JDR2;   /*!< ADC injected data register 2,                Address offset: 0x40 */
  __IO uint32_t JDR3;   /*!< ADC injected data register 3,                Address offset: 0x44 */
  __IO uint32_t JDR4;   /*!< ADC injected data register 4,                Address offset: 0x48 */
  __IO uint32_t DR;     /*!< ADC regular data register,                   Address offset: 0x4C */
} ADC_TypeDef;

typedef struct
{
  __IO uint32_t CSR;    /*!< ADC Common status register,                  Address offset: ADC1 base address + 0x300 */
  __IO uint32_t CCR;    /*!< ADC common control register,                 Address offset: ADC1 base address + 0x304 */
  __IO uint32_t CDR;    /*!< ADC common regular data register for dual
                             AND triple modes,                            Address offset: ADC1 base address + 0x308 */
} ADC_Common_TypeDef;


#if ANALOG_WDGEN == EN
typedef struct
{
	uint16_t WDG_HIGH;
	uint16_t WDG_LOW;
}
#endif

/* Configuration Parameter for Single Conversion*/
#if ADC_SINGLECONV_EN
typedef struct
{
	ADC_CHANNEL Channel; /*<Select Channel to be converted>*/
	uint8_t Allign;  /*<Select Either Left or Right Allignment>*/
	uint8_t Cont_SNG;
#if ANALOG_WDGEN == EN
	ANALOG_WDGEN WDEGN
#endif
#if ADC_INTERRUPT_EN == EN
	void (*ADC_Callback) (void) ;
#endif
	uint8_t SMPT;
	uint8_t RES;
	uint8_t APB_P;
}ADC_CConfig_t;
#endif

/*-----Multiple Channel At the Same Time using DMA----*/
#if ADC_GROUPCONV_DMA_EN ||ADC_GROUPCONV
typedef struct {
	uint16_t Channel; /*<Channels in group 16bits for 16 different channel>*/
	uint8_t Allign; /*<Select Either Left or Right Allignment>*/
	uint8_t Cont_SNG;
	uint16_t AWDG;
#if ANALOG_WDGEN == EN
	uint16_t AWDG;  /*<Watchdog Channel>*/
	ANALOG_WDGEN WDEGN
#endif
#if ADC_INTERRUPT_EN == EN
	void (*ADC_Callback) (void) ;
#endif
#if ADC_TEMPSENSOR_READ == EN
	uint8_t Temp_Sensor_Read;
#endif
	uint8_t RES;
	uint8_t APB_P;
} ADC_CConfig_t;
#endif

/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/
#if ADC_SINGLECONV_EN
ERR xHAL_ADC_Init(ADC_CConfig_t *Adc,uint8_t Int_flag);
uint16_t sHAL_ADC_ResultBlocking(uint8_t Allign ,uint8_t channel);
uint16_t sHAL_ADC_ResultBlockingCont(uint8_t Allign);
#if ADC_INTERRUPT_EN == EN
uint16_t sHAL_ADC_Result(uint8_t Allign ,uint8_t channel);
uint16_t sHAL_ADC_ResultCont(uint8_t Allign);
#endif
#endif

#if ADC_GROUPCONV_DMA_EN
ERR xHAL_ADC_GroupInitDMA(ADC_CConfig_t *Adc,uint16_t *Buffer);
#endif

/*----Most Likely Will Not Work---*/
#if ADC_GROUPCONV_EN
ERR xHAL_ADC_GroupInitNDMABased(ADC_CConfig_t *Adc,uint8_t Flag);
ERR xHAL_ADC_GroupReadPBased(ADC_CConfig_t *Adc,uint16_t *Buffer);
#endif
/*-----------Software Interfaces End--------------*/


#endif /* HAL_HAL_ADC_H_ */
