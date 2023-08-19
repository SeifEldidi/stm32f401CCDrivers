/*
 * Hal_GPIO.h
 *
 *  Created on: Jul 25, 2023
 *      Author: Seif pc
 */

#ifndef HAL_HAL_GPIO_H_
#define HAL_HAL_GPIO_H_

/*-------------Includes Section Start---------------*/
#include "../std_macros.h"
#include "Hal_RCC.h"
#include "Hal_GPIO_config.h"
#include "../core/CortexM4_core_NVIC.h"
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/

/*--------------GPIO Addresses-----------*/
#define AHB1_PERIPHBASE		0x40020000UL
#define GPIOA_OFFSET		0x00000000UL
#define GPIOB_OFFSET		0x00000400UL
#define GPIOC_OFFSET		0x00000800UL
#define GPIOD_OFFSET		0x00000C00UL
#define GPIOE_OFFSET		0x00001000UL
#define GPIOH_OFFSET		0x00001C00UL

#define APB2_PERIPHBASE		0x40010000UL
#define EXTI_OFFSET			0x00003C00UL
#define EXTI 				((volatile EXTI_Typedef *)(APB2_PERIPHBASE+EXTI_OFFSET))

#define SYSCFG_OFFSER		0x00003800UL
#define SYSCFG 				((volatile SYSCFG_Typedef *)(APB2_PERIPHBASE+SYSCFG_OFFSER))
#define EXTI_SYSCFG_DIV		2
/*-------------GPIO Declarations-------*/
#define GPIOA				((__IO GPIO_Typedef*)(AHB1_PERIPHBASE+GPIOA_OFFSET))
#define GPIOB				((__IO GPIO_Typedef*)(AHB1_PERIPHBASE+GPIOB_OFFSET))
#define GPIOC				((__IO GPIO_Typedef*)(AHB1_PERIPHBASE+GPIOC_OFFSET))
#define GPIOD				((__IO GPIO_Typedef*)(AHB1_PERIPHBASE+GPIOD_OFFSET))
#define GPIOE				((__IO GPIO_Typedef*)(AHB1_PERIPHBASE+GPIOE_OFFSET))
#define GPIOF				((__IO GPIO_Typedef*)(AHB1_PERIPHBASE+GPIOF_OFFSET))
#define GPIOH				((__IO GPIO_Typedef*)(AHB1_PERIPHBASE+GPIOH_OFFSET))

#define GPIO_NOPULL		0x00U
#define GPIO_PULLDOWN	0x01U
#define GPIO_PULLUP		0x02U

#define GPIO_MODE_INPUT				 0x0000U
#define GPIO_MODE_OUTPUT_PP			 0x0001U
#define GPIO_MODE_OUTPUT_OD			 0x0002U
#define GPIO_MODE_AF_PP				 0x0003U
#define GPIO_MODE_AF_OD				 0x0004U
#define GPIO_MODE_AN_INPUT 			 0x0005U
#define GPIO_MODE_IT_RISING          0x0006U
#define GPIO_MODE_IT_FALLING         0x0007U
#define GPIO_MODE_IT_RISING_FALLING  0x0008U
#define GPIO_MODE_EVT_RISING         0x0009U
#define GPIO_MODE_EVT_FALLING        0x00010U
#define GPIO_MODE_EVT_RISING_FALLING 0x00011U


#define MODERX_MInput	0b00U
#define MODERX_MGPOUT	0b01U
#define MODERX_MALT     0b10U
#define MODERX_MANA		0b11U

#define OSPEEDR_LOW		0b00U
#define OSPEEDR_MED		0b01U
#define OSPEEDR_HIGH	0b10U
#define OSPEEDR_VHIGH	0b11U

#define PUPDR_CLEAR		0b11U
#define PUPDR_NOPULL	0b00U
#define PUPDR_PULLUP	0b01U
#define PUPDR_PULLDOWN	0b10U
#define PUPDR_XX		0b11U

#define MODERX_PIN_0	0U
#define MODERX_PIN_1	2U
#define MODERX_PIN_2	4U
#define MODERX_PIN_3	6U
#define MODERX_PIN_4	8U
#define MODERX_PIN_5	10U
#define MODERX_PIN_6	12U
#define MODERX_PIN_7	14U
#define MODERX_PIN_8	16U
#define MODERX_PIN_9	18U
#define MODERX_PIN_10	20U
#define MODERX_PIN_11	22U
#define MODERX_PIN_12	24U
#define MODERX_PIN_13	26U
#define MODERX_PIN_14	28U
#define MODERX_PIN_15	30U

#define PIN_0	0U
#define PIN_1	1U
#define PIN_2	2U
#define PIN_3	3U
#define PIN_4	4U
#define PIN_5	5U
#define PIN_6	6U
#define PIN_7	7U
#define PIN_8	8U
#define PIN_9	9U
#define PIN_10	10U
#define PIN_11	11U
#define PIN_12	12U
#define PIN_13	13U
#define PIN_14	14U
#define PIN_15	15U

#define GPIO_PIN_0		0x0001U
#define GPIO_PIN_1		0x0002U
#define GPIO_PIN_2		0x0004U
#define GPIO_PIN_3		0X0008U
#define GPIO_PIN_4		0x0010U
#define GPIO_PIN_5		0x0020U
#define GPIO_PIN_6		0x0040U
#define GPIO_PIN_7		0x0080U
#define GPIO_PIN_8		0x0100U
#define GPIO_PIN_9		0x0200U
#define GPIO_PIN_10		0x0400U
#define GPIO_PIN_11		0x0800U
#define GPIO_PIN_12		0x1000U
#define GPIO_PIN_13		0x2000U
#define GPIO_PIN_14		0x4000U
#define GPIO_PIN_15		0x8000U

#define GPIO_Speed_2MHz   0U	/*!< Low speed */
#define GPIO_Speed_25MHz  1U   /*!< Medium speed */
#define GPIO_Speed_50MHz  2U	/*!< Fast speed */
#define GPIO_Speed_100MHz 3U  /*!< Very Fast speed */
/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/

/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/
typedef enum
{
	GPIO_PIN_RESET,
	GPIO_PIN_SET,
}GPIO_State;

typedef struct
{
	__IO uint32_t MODER;
	__IO uint32_t OTYPER;
	__IO uint32_t OSPEEDR;
	__IO uint32_t PUPDR;
	__IO uint32_t IDR;
	__IO uint32_t ODR;
	__IO uint32_t BSRR;
	__IO uint32_t LCKR;
	__IO uint32_t AFRL;
	__IO uint32_t AFRH;
}GPIO_Typedef;

typedef struct
{
	__IO uint32_t IMR;
	__IO uint32_t EMR;
	__IO uint32_t RTSR;
	__IO uint32_t FTSR;
	__IO uint32_t SWIER;
	__IO uint32_t PR;
}EXTI_Typedef;

typedef struct
{
	__IO uint32_t MEMRMP;
	__IO uint32_t PMC;
	__IO uint32_t EXTICR1;
	__IO uint32_t EXTICR2;
	__IO uint32_t EXTICR3;
	__IO uint32_t EXTICR4;
	__IO uint32_t CMPCR;
}SYSCFG_Typedef;

typedef struct
{
	uint16_t Pin;
	uint8_t  mode;
	uint8_t  pull;
	uint8_t  Speed;
#if GPIO_EXTI_LINE0_EN == EN
  void (*GPIO0_Callbck)(void);
#endif
#if GPIO_EXTI_LINE1_EN == EN
  void (*GPIO1_Callbck)(void);
#endif

#if GPIO_EXTI_LINE2_EN == EN
 void (*GPIO2_Callbck)(void);
#endif

#if GPIO_EXTI_LINE3_EN == EN
 void (*GPIO3_Callbck)(void);
#endif

#if GPIO_EXTI_LINE4_EN == EN
 void (*GPIO4_Callbck)(void);
#endif

#if GPIO_EXTI_LINE5_EN == EN
 void (*GPIO5_Callbck)(void);
#endif

#if GPIO_EXTI_LINE6_EN == EN
 void (*GPIO6_Callbck)(void);
#endif

#if GPIO_EXTI_LINE7_EN == EN
 void (*GPIO7_Callbck)(void);
#endif

#if GPIO_EXTI_LINE8_EN == EN
 void (*GPIO8_Callbck)(void);
#endif

#if GPIO_EXTI_LINE9_EN == EN
 void (*GPIO9_Callbck)(void);
#endif

#if GPIO_EXTI_LINE10_EN == EN
 void (*GPIO10_Callbck)(void);
#endif

#if GPIO_EXTI_LINE11_EN == EN
 void (*GPIO11_Callbck)(void);
#endif

#if GPIO_EXTI_LINE12_EN == EN
 void (*GPIO12_Callbck)(void);
#endif

#if GPIO_EXTI_LINE13_EN == EN
 void (*GPIO13_Callbck)(void);
#endif

#if GPIO_EXTI_LINE14_EN == EN
 void (*GPIO14_Callbck)(void);
#endif

#if GPIO_EXTI_LINE15_EN == EN
 void (*GPIO15_Callbck)(void);
#endif
}GPIO_InitStruct;

/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/
ERR  HAL_GPIO_Init( GPIO_Typedef * GPIO_PORT, GPIO_InitStruct * GPIO_S);
ERR  HAL_GPIO_WritePin( GPIO_Typedef * GPIO_PORT,uint16_t GPIO_PIN,GPIO_State State);
GPIO_State HAL_GPIO_ReadPin( GPIO_Typedef * GPIO_PORT, uint16_t GPIO_PIN);
ERR  HAL_GPIO_TogglePin( GPIO_Typedef * GPIO_PORT, uint16_t GPIO_PIN);
/*-----------Software Interfaces End--------------*/

#endif /* HAL_HAL_GPIO_H_ */
