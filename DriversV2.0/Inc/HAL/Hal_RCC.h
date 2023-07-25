/*
 * Hal_RCC.h
 *
 *  Created on: Jul 24, 2023
 *      Author: Seif pc
 */

#ifndef HAL_RCC_H_
#define HAL_RCC_H_

/*-------------Includes Section Start---------------*/
#include "../std_macros.h"
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/
#define AHB_BASEPERIPH_ADD			0x40020000UL
#define RCC_BASE_ADDRESS_OFFSET		0x00003800UL
#define FLASH_UNIT_OFFSET			0x00003C00UL
#define RCC 						((volatile RCC_Typedef*)(AHB_BASEPERIPH_ADD+RCC_BASE_ADDRESS_OFFSET))
#define FLASH_ACR					*((volatile uint32_t *) (AHB_BASEPERIPH_ADD+FLASH_UNIT_OFFSET))
#define VOLTAGE_RANGE_2_7_3_3       EN
/*------------User Setting-----------*/
#define HAL_MCO1_PIN_EN			DIS
#define HAL_MCO2_PIN_EN			DIS

#define WATCHDOG_EN				DIS
#define RTC_EN					DIS

/*-------Flags to select Clock------*/
#define SW_CLK_HSI				0U
#define SW_CLK_HSE				1U
#define SW_CLK_PLLO				2U
/*------Flags to Select PLL CLOCK---*/
#define PLL_HSI					0U
#define PLL_HSE					1U
/*-------Flags for PLLP------------*/
#define PLLP_2					(0)
#define PLLP_4					(1)
#define PLLP_6					(2)
#define PLLP_8					(3)
/*-------Flags for PLLQ------------*/
#define PLLQ_2_S			    (0x02)
#define PLLQ_3_S				(0x03)
#define PLLQ_4_S				(0x04)
#define PLLQ_5_S				(0x05)
#define PLLQ_6_S				(0x06)
#define PLLQ_7_S				(0x07)
#define PLLQ_8_S				(0x08)
#define PLLQ_9_S				(0x09)
#define PLLQ_10_S				(0x0A)
#define PLLQ_11_S				(0x0B)
#define PLLQ_12_S				(0x0C)
#define PLLQ_13_S				(0x0D)
#define PLLQ_14_S			    (0x0E)
#define PLLQ_15_S				(0x0F)
/*-----Flags for AHB prescalers----*/
#define AHBDIV_1				0
#define AHBDIV_2				1U
#define AHBDIV_4				2U
#define AHBDIV_8				3U
#define AHBDIV_16				4U
#define AHBDIV_64				5U
#define AHBDIV_128				6U
#define AHBDIV_256				7U
#define AHBDIV_512				8U
/*-----Flags for APB prescalers----*/
#define APB_AHB_1				(0U)
#define APB_AHB_2				(1U)
#define APB_AHB_4				(2U)
#define APB_AHB_8				(3U)
#define APB_AHB_16				(4U)

#define HSI_ON					0U
#define HSE_ON					1U
/*------------RCC_CR-----------*/
#define PLLI2SRDY  			27U
#define PLLI2SRDY_READ		(1<<PLLI2SRDY)

#define PLLI2SON 			26U
#define PLLI2SON_EN			(1<<PLLI2SON)
#define PLLI2SON_DIS		(~PLLI2SON_EN)

#define PLLRDY				25U
#define PLLRDY_READ			(1<<PLLRDY)

#define PLLON				24U
#define PLLON_EN			(1<<PLLON)
#define PLLON_DIS			(~PLLON_EN)

#define CSSON				19U
#define CSSON_EN			(1<<CSSON)
#define CSSON_DIS			(~CSSON_EN)

#define HSEBYP				18U
#define HSE_EXTERNAL_OSC	(~(1<<HSEBYP))
#define HSE_EXTERNAL_CLK	(1<<HSEBYP)

#define HSERDY				17U
#define HSERDY_READ			(1<<HSERDY)

#define HSEON 				16U
#define HSEON_EN			(1<<HSEON)
#define HSEON_DIS			(~(HSEON_EN))

#define HSIRDY				1U
#define HSIRDY_READ			(1<<HSIRDY)

#define HSION				0U
#define HSION_EN			(1<<HSION)
#define HSION_DIS			(~HSION_EN)
/*--------------RCC_PLLCFGR------------*/
#define PLLQ				24U
#define PLLQ_2				(0x02<<PLLQ)
#define PLLQ_3				(0x03<<PLLQ)
#define PLLQ_4				(0x04<<PLLQ)
#define PLLQ_5				(0x05<<PLLQ)
#define PLLQ_6				(0x06<<PLLQ)
#define PLLQ_7				(0x07<<PLLQ)
#define PLLQ_8				(0x08<<PLLQ)
#define PLLQ_9				(0x09<<PLLQ)
#define PLLQ_10				(0x0A<<PLLQ)
#define PLLQ_11				(0x0B<<PLLQ)
#define PLLQ_12				(0x0C<<PLLQ)
#define PLLQ_13				(0x0D<<PLLQ)
#define PLLQ_14				(0x0E<<PLLQ)
#define PLLQ_15				(0x0F<<PLLQ)

#define PLLSRC				22
#define PLLSRC_MSK			(1<<PLLSRC)
#define PLLSRC_HSE			(1<<PLLSRC)
#define PLLSRC_HSI_MSK		(0U)
#define PLLSRC_HSI			(~PLLSRC_HSE)

#define PLLP				16U
#define PLLP_MSK			(0x00000003U<<PLLP)
#define PPLP_2				(0<<PLLP)
#define PPLP_4				(1<<PLLP)
#define PPLP_6				(2<<PLLP)
#define PPLP_8				(3<<PLLP)

#define PLLN 				6U
#define PLLN_MSK			(0x000001FFU<<PLLN)
#define PLLN_MAX			432
#define PLLN_MIN			50

#define PLLM				0U
#define PLLM_MSK			(0x0000003FU<<PLLM)
#define PLLM_MAX			63
#define PLLM_MIN			2
/*-------------RCC_CFGR------------*/
#define MCO2				30U
#define MCO2_SYSCLK			(0x00<<MCO2)
#define MCO2_PLLI2S			(0x01<<MCO2)
#define MCO2_HSE			(0x02<<MCO2)
#define MCO2_PLL			(0x03<<MCO2)

#define MCO2PRE				27U
#define MCO2PRE_1 			(0b000<<MCO2PRE)
#define MCO2PRE_2 			(0b100<<MCO2PRE)
#define MCO2PRE_3 			(0b101<<MCO2PRE)
#define MCO2PRE_4 			(0b110<<MCO2PRE)
#define MCO2PRE_5			(0b111<<MCO2PRE)

#define MCO1PRE				24U
#define MCO1PRE_1 			(0b000<<MCO1PRE)
#define MCO1PRE_2 			(0b100<<MCO1PRE)
#define MCO1PRE_3 			(0b101<<MCO1PRE)
#define MCO1PRE_4 			(0b110<<MCO1PRE)
#define MCO21PRE_5			(0b111<<MCO1PRE)

#define I2SSRC				23U
#define I2SSRC_PLL2I2S 		(~(I2SSRC_EXT))
#define I2SSRC_EXT			(1<<I2SSRC)

#define MCO1				21U
#define MCO1_HSI			(0x00<<MCO2)
#define MCO1_LSE			(0x01<<MCO2)
#define MCO1_HSE			(0x02<<MCO2)
#define MCO1_PLL			(0x03<<MCO2)

#define RTCPRE				16U
#define RTCPRE_MIN			2U
#define RTCPRE_MAX			31U

#define APB2_PRESCALER		13U
#define APB2_PRE_MSK		(0x00000007U<<APB2_PRESCALER)
#define APB2_AHB_1			(0b000<<APB2_PRESCALER)
#define APB2_AHB_2			(0b100<<APB2_PRESCALER)
#define APB2_AHB_4			(0b101<<APB2_PRESCALER)
#define APB2_AHB_8			(0b110<<APB2_PRESCALER)
#define APB2_AHB_16			(0b111<<APB2_PRESCALER)

#define APB1_PRESCALER		10U
#define APB1_PRE_MSK		(0x00000007U<<APB1_PRESCALER)
#define APB1_AHB_1			(0b000<<APB1_PRESCALER)
#define APB1_AHB_2			(0b100<<APB1_PRESCALER)
#define APB1_AHB_4			(0b101<<APB1_PRESCALER)
#define APB1_AHB_8			(0b110<<APB1_PRESCALER)
#define APB1_AHB_16			(0b111<<APB1_PRESCALER)

#define HPRE				4U
#define HPRE_MSK			(0x0000000FU<<HPRE)
#define AHB_1				(0b0000<<HPRE)
#define AHB_2				(0b1000<<HPRE)
#define AHB_4				(0b1001<<HPRE)
#define AHB_8				(0b1010<<HPRE)
#define AHB_16				(0b1011<<HPRE)
#define AHB_64				(0b1100<<HPRE)
#define AHB_128				(0b1101<<HPRE)
#define AHB_256				(0b1110<<HPRE)
#define AHB_512				(0b1111<<HPRE)

#define SWS					2U
#define SWS_MSK				(0x00000003U<<SWS)
#define SWS_HSI				(0b00<<SWS)
#define SWS_HSE				(0b01<<SWS)
#define SWS_PLL				(0b10<<SWS)

#define SW					0U
#define SW_HSI				(0b00<<SW)
#define SW_HSE				(0b01<<SW)
#define SW_PLL				(0b10<<SW)
/*------RCC_CIR REG--------*/
#define CSSC			23U
#define PLLI2SRDYC		21U
#define PLLRDYC			20U
#define	HSERDYC			19U
#define	HSIRDYC			18U
#define	LSERDYC			17U
#define LSIRDYC			16U

#define PLLI2SRDYIE		13U
#define PLLRDYIE		12U
#define HSERDYIE		11U
#define HSIRDYIE		10U
#define LSERDYIE		9U
#define LSIRDYIE		8U

#define CSSF			7U
#define PLLI2SRDYF		5U
#define PLLRDYF			4U
#define HSERDYF			3U
#define HSIRDYF			2U
#define LSERDYF			1U
#define LSIRDYF			0U
/*-------RCC_AHB1RSTR-----*/
#define GPIOA_RST		0U
#define GPIOB_RST		1U
#define GPIOC_RST		2U
#define GPIOD_RST		3U
#define GPIOE_RST		4U
#define GPIOH_RST		7U
#define CRCR_RST		12U
#define DMA1_RST		21U
#define DMA2_RST		22U
/*-------RCC_AHB2RSTR-----*/
#define USB_OTGFSRST	7U
/*-------RCC_APB1RSTR-----*/
#define TIM2_RST		0U
#define TIM3_RST		1U
#define TIM4_RST		2U
#define TIM5_RST		3U
#define WWDG_RST		11U
#define SPI2_RST		14U
#define SPI3_RST		15U
#define USART2_RST		17U
#define I2C1_RST		21U
#define I2C2_RST		22U
#define I2C3_RST		23U
#define PWR_RST			28U
/*-------RCC_APB2RSTR-----*/
#define TIM1_RST		0U
#define	USART1_RST		4U
#define USART6_RST		5U
#define ADC1_RST		8U
#define SDIO_RST		11U
#define SPI1_RST		12U
#define SPI4_RST		13U
#define SYSCFG_RST		14U
#define TIM9_RST		16U
#define TIM10_RST		17U
#define TIM11_RST		18U
/*-------RCC_BDCR--------*/
#define BDRST			16U
#define BDRST_EN		(1<<BDRST)

#define RTCEN			15U
#define RTCEN_EN		(1<<RTCEN)

#define RTCSEL			8U
#define RTCSEL_NCK		(0<<RTCSEL)
#define RTCSEL_LSE		(1<<RTCSEL)
#define RTCSEL_LSI		(2<<RTCSEL)
#define RTCSEL_HSE		(3<<RTCSEL)

#define LSEBYP			2U
#define LSEBYP_BYPASS	(1<<LSEBYP)

#define LSERDY			1U
#define LSERDY_Read		(1<<LSERDY)

#define LSEON			0U
#define LSEON_EN		(1<<LSEON)
/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/

/*-------RCC_AHB1RSTR-----*/
#define HAL_RCC_GPIOA_RST() (SET_BIT(RCC->AHB1RSTR,GPIOA_RST))
#define HAL_RCC_GPIOB_RST() (SET_BIT(RCC->AHB1RSTR,GPIOB_RST))
#define HAL_RCC_GPIOC_RST() (SET_BIT(RCC->AHB1RSTR,GPIOC_RST))
#define HAL_RCC_GPIOD_RST() (SET_BIT(RCC->AHB1RSTR,GPIOD_RST))
#define HAL_RCC_GPIOE_RST() (SET_BIT(RCC->AHB1RSTR,GPIOE_RST))
#define HAL_RCC_GPIOH_RST() (SET_BIT(RCC->AHB1RSTR,GPIOH_RST))
#define HAL_RCC_DMA1_RST()  (SET_BIT(RCC->AHB1RSTR,DMA1_RST))
#define HAL_RCC_DMA2_RST()  (SET_BIT(RCC->AHB1RSTR,DMA2_RST))
#define HAL_RCC_CRCR_RST()  (SET_BIT(RCC->AHB1RSTR,CRCR_RST))

/*-------RCC_AHB2RSTR-----*/
#define HAL_RCC_USBOTGFS_RST() (SET_BIT(RCC->AHB2RSTR,USB_OTGFSRST))
/*-------RCC_APB1RSTR-----*/
#define HAL_RCC_TIM2_RST()   (SET_BIT(RCC->APB1RSTR,TIM2_RST))
#define HAL_RCC_TIM3_RST()   (SET_BIT(RCC->APB1RSTR,TIM3_RST))
#define HAL_RCC_TIM4_RST()   (SET_BIT(RCC->APB1RSTR,TIM4_RST))
#define HAL_RCC_TIM5_RST()   (SET_BIT(RCC->APB1RSTR,TIM5_RST))
#define HAL_RCC_WWDG_RST()   (SET_BIT(RCC->APB1RSTR,WWDG_RST))
#define HAL_RCC_SPI2_RST()   (SET_BIT(RCC->APB1RSTR,SPI2_RST))
#define HAL_RCC_SPI3_RST()   (SET_BIT(RCC->APB1RSTR,SPI3_RST))
#define HAL_RCC_USART2_RST() (SET_BIT(RCC->APB1RSTR,USART2_RST))
#define HAL_RCC_I2C1_RST()   (SET_BIT(RCC->APB1RSTR,I2C1_RST))
#define HAL_RCC_I2C2_RST()   (SET_BIT(RCC->APB1RSTR,I2C2_RST))
#define HAL_RCC_I2C3_RST()   (SET_BIT(RCC->APB1RSTR,I2C3_RST))
#define HAL_RCC_PWR_RST()    (SET_BIT(RCC->APB1RSTR,PWR_RST))
/*-------RCC_APB2RSTR-----*/
#define HAL_RCC_TIM1_RST()     (SET_BIT(RCC->APB2RSTR,TIM1_RST))
#define HAL_RCC_USART1_RST()   (SET_BIT(RCC->APB2RSTR,USART1_RST))
#define HAL_RCC_USART6_RST()   (SET_BIT(RCC->APB2RSTR,USART6_RST))
#define HAL_RCC_ADC1_RST()     (SET_BIT(RCC->APB2RSTR,ADC1_RST))
#define HAL_RCC_SDIO_RST()     (SET_BIT(RCC->APB2RSTR,SDIO_RST))
#define HAL_RCC_SPI1_RST()     (SET_BIT(RCC->APB2RSTR,SPI1_RST))
#define HAL_RCC_SPI4_RST()     (SET_BIT(RCC->APB2RSTR,SPI4_RST))
#define HAL_RCC_SYSCFG_RST()   (SET_BIT(RCC->APB2RSTR,SYSCFG_RST))
#define HAL_RCC_TIM9_RST()     (SET_BIT(RCC->APB2RSTR,TIM9_RST))
#define HAL_RCC_TIM10_RST()    (SET_BIT(RCC->APB2RSTR,TIM10_RST))
#define HAL_RCC_TIM11_RST()    (SET_BIT(RCC->APB2RSTR,TIM11_RST))


/*-------RCC_AHB1ENR-----*/
#define HAL_RCC_GPIOA_EN() { if(!(READ_BIT(RCC->AHB1ENR,GPIOA_RST))) {(SET_BIT(RCC->AHB1ENR,GPIOA_RST));}}
#define HAL_RCC_GPIOB_EN() { if(!(READ_BIT(RCC->AHB1ENR,GPIOB_RST))) {(SET_BIT(RCC->AHB1ENR,GPIOB_RST));}}
#define HAL_RCC_GPIOC_EN() { if(!(READ_BIT(RCC->AHB1ENR,GPIOC_RST))) {(SET_BIT(RCC->AHB1ENR,GPIOC_RST));}}
#define HAL_RCC_GPIOD_EN() { if(!(READ_BIT(RCC->AHB1ENR,GPIOD_RST))) {(SET_BIT(RCC->AHB1ENR,GPIOD_RST));}}
#define HAL_RCC_GPIOE_EN() { if(!(READ_BIT(RCC->AHB1ENR,GPIOE_RST))) {(SET_BIT(RCC->AHB1ENR,GPIOE_RST));}}
#define HAL_RCC_GPIOH_EN() { if(!(READ_BIT(RCC->AHB1ENR,GPIOH_RST))) {(SET_BIT(RCC->AHB1ENR,GPIOH_RST));}}
#define HAL_RCC_DMA1_EN()  { if(!(READ_BIT(RCC->AHB1ENR,DMA1_RST)))  {(SET_BIT(RCC->AHB1ENR,DMA1_RST));}}
#define HAL_RCC_DMA2_EN()  { if(!(READ_BIT(RCC->AHB1ENR,DMA2_RST)))  {(SET_BIT(RCC->AHB1ENR,DMA2_RST));}}
#define HAL_RCC_CRCR_EN()  { if(!(READ_BIT(RCC->AHB1ENR,DMA2_RST)))  {(SET_BIT(RCC->AHB1ENR,CRCR_RST));}}

/*-------RCC_AHB2ENR-----*/
#define HAL_RCC_USBOTGFS_EN() (SET_BIT(RCC->AHB2ENR,USB_OTGFSRST));
/*-------RCC_APB1ENR-----*/
#define HAL_RCC_TIM2_EN()   { if(!(READ_BIT(RCC->AHB1ENR,TIM2_RST))) {(SET_BIT(RCC->APB1ENR,TIM2_RST));}}
#define HAL_RCC_TIM3_EN()   { if(!(READ_BIT(RCC->AHB1ENR,TIM3_RST))) {(SET_BIT(RCC->APB1ENR,TIM3_RST));}}
#define HAL_RCC_TIM4_EN()   { if(!(READ_BIT(RCC->AHB1ENR,GPIOA_RST))) {(SET_BIT(RCC->APB1ENR,TIM4_RST));}}
#define HAL_RCC_TIM5_EN()   { if(!(READ_BIT(RCC->AHB1ENR,TIM4_RST))) {(SET_BIT(RCC->APB1ENR,TIM5_RST));}}
#define HAL_RCC_WWDG_EN()   { if(!(READ_BIT(RCC->AHB1ENR,WWDG_RST))) {(SET_BIT(RCC->APB1ENR,WWDG_RST));}}
#define HAL_RCC_SPI2_EN()   { if(!(READ_BIT(RCC->AHB1ENR,SPI2_RST))) {(SET_BIT(RCC->APB1ENR,SPI2_RST));}}
#define HAL_RCC_SPI3_EN()   { if(!(READ_BIT(RCC->AHB1ENR,SPI3_RST))) {(SET_BIT(RCC->APB1ENR,SPI3_RST));}}
#define HAL_RCC_USART2_EN() { if(!(READ_BIT(RCC->AHB1ENR,USART2_RST))) {(SET_BIT(RCC->APB1ENR,USART2_RST));}}
#define HAL_RCC_I2C1_EN()   { if(!(READ_BIT(RCC->AHB1ENR,I2C1_RST))) {(SET_BIT(RCC->APB1ENR,I2C1_RST));}}
#define HAL_RCC_I2C2_EN()   { if(!(READ_BIT(RCC->AHB1ENR,I2C2_RST))) {(SET_BIT(RCC->APB1ENR,I2C2_RST));}}
#define HAL_RCC_I2C3_EN()   { if(!(READ_BIT(RCC->AHB1ENR,I2C3_RST))) {(SET_BIT(RCC->APB1ENR,I2C3_RST));}}
#define HAL_RCC_PWR_EN()    { if(!(READ_BIT(RCC->AHB1ENR,PWR_RST))) {(SET_BIT(RCC->APB1ENR,PWR_RST));}}
/*-------RCC_APB2RENR-----*/
#define HAL_RCC_TIM1_EN()     { if(!(READ_BIT(RCC->AHB1ENR,TIM2_RST))) {(SET_BIT(RCC->APB2ENR,TIM1_RST));}}
#define HAL_RCC_USART1_EN()   { if(!(READ_BIT(RCC->AHB1ENR,TIM2_RST))) {(SET_BIT(RCC->APB2ENR,USART1_RST));}}
#define HAL_RCC_USART6_EN()   { if(!(READ_BIT(RCC->AHB1ENR,TIM2_RST))) {(SET_BIT(RCC->APB2ENR,USART6_RST));}}
#define HAL_RCC_ADC1_EN()     { if(!(READ_BIT(RCC->AHB1ENR,TIM2_RST))) {(SET_BIT(RCC->APB2ENR,ADC1_RST));}}
#define HAL_RCC_SDIO_EN()     { if(!(READ_BIT(RCC->AHB1ENR,TIM2_RST))) {(SET_BIT(RCC->APB2ENR,SDIO_RST));}}
#define HAL_RCC_SPI1_EN()     { if(!(READ_BIT(RCC->AHB1ENR,TIM2_RST))) {(SET_BIT(RCC->APB2ENR,SPI1_RST));}}
#define HAL_RCC_SPI4_EN()     { if(!(READ_BIT(RCC->AHB1ENR,TIM2_RST))) {(SET_BIT(RCC->APB2ENR,SPI4_RST));}}
#define HAL_RCC_SYSCFG_EN()   { if(!(READ_BIT(RCC->AHB1ENR,TIM2_RST))) {(SET_BIT(RCC->APB2ENR,SYSCFG_RST));}}
#define HAL_RCC_TIM9_EN()     { if(!(READ_BIT(RCC->AHB1ENR,TIM2_RST))) {(SET_BIT(RCC->APB2ENR,TIM9_RST));}}
#define HAL_RCC_TIM10_EN()    { if(!(READ_BIT(RCC->AHB1ENR,TIM2_RST))) {(SET_BIT(RCC->APB2ENR,TIM10_RST));}}
#define HAL_RCC_TIM11_EN()    { if(!(READ_BIT(RCC->AHB1ENR,TIM2_RST))) {(SET_BIT(RCC->APB2ENR,TIM11_RST));}}

/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/
typedef struct
{
	__IO uint32_t CR;
	__IO uint32_t PLLCFGR;
	__IO uint32_t CFGR;
	__IO uint32_t CIR;
	__IO uint32_t AHB1RSTR;
	__IO uint32_t AHB2RSTR;
	__IO uint32_t RES_1[2];
	__IO uint32_t APB1RSTR;
	__IO uint32_t APB2RSTR;
	__IO uint32_t RES_2[2];
	__IO uint32_t AHB1ENR;
	__IO uint32_t AHB2ENR;
	__IO uint32_t RES_3[2];
	__IO uint32_t APB1ENR;
	__IO uint32_t APB2ENR;
	__IO uint32_t RES_4[2];
	__IO uint32_t AHB1LPENR;
	__IO uint32_t AHB2LPENR;
	__IO uint32_t RES_5[2];
	__IO uint32_t APB1LPENR;
	__IO uint32_t APB2LPENR;
	__IO uint32_t RES_6[2];
	__IO uint32_t BDCR;
	__IO uint32_t CSR;
	__IO uint32_t SSCGR;
	__IO uint32_t PLLI2SCFGR;
	__IO uint32_t RES_7;
	__IO uint32_t DCKCFGR;
}RCC_Typedef;

#if HAL_MCO1_PIN_EN == EN
typedef struct
{
	uint8_t MCO1_SRC;
	uint8_t MC01_PRE;
}MCO1_Cfg;
#endif

#if HAL_MCO2_PIN_EN == EN
typedef struct
{
	uint8_t MCO2_SRC;
	uint8_t MC02_PRE;
}MCO2_cfg;
#endif

#if RTC_EN == EN
typedef struct
{
	uint8_t RTC_SRC;
}RTC_Cfg;
#endif

typedef struct
{
	uint8_t MSTR_CLK_SRC;
	uint8_t PLL_SRC;
	uint8_t PLL_P;
	uint8_t PLL_Q;
	uint8_t PLL_M;
	uint16_t PLL_N;
	uint8_t APB1_PRE;
	uint8_t APB2_PRE;
	uint8_t AHB_PRE;
#if HAL_MCO1_PIN_EN == EN
	MCO1_Cfg MC1;
#endif
#if HAL_MCO2_PIN_EN == EN
	MCO2_cfg MC2;
#endif
#if RTC_EN == EN
	RTC_Cfg RTC;
#endif
}RCC_Config_t;

/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/
ERR 	 HAL_RCC_Init(RCC_Config_t *RCC_C);
uint32_t HAL_RCC_GET_AHBFREQ();
uint32_t HAL_RCC_GET_APB1FREQ();
uint32_t HAL_RCC_GET_APB2FREQ();
uint32_t HAL_RCC_GET_SYSCLKFREQ();
/*-----------Software Interfaces End--------------*/

#endif /* HAL_RCC_H_ */
