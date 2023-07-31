/*
 * Hal_GPIO.c
 *
 *  Created on: Jul 25, 2023
 *      Author: Seif pc
 */
#include "../../Inc/HAL/Hal_GPIO.h"

#if GPIO_EXTI_LINE0_EN == EN
static void (*GPIO0_Callback)(void);
#endif

#if GPIO_EXTI_LINE1_EN == EN
static void (*GPIO1_Callback)(void);
#endif

#if GPIO_EXTI_LINE2_EN == EN
static void (*GPIO2_Callback)(void);
#endif

#if GPIO_EXTI_LINE3_EN == EN
static void (*GPIO3_Callback)(void);
#endif

#if GPIO_EXTI_LINE4_EN == EN
static void (*GPIO4_Callback)(void);
#endif

#if GPIO_EXTI_LINE5_EN == EN
static void (*GPIO5_Callback)(void);
#endif

#if GPIO_EXTI_LINE6_EN == EN
static void (*GPIO6_Callback)(void);
#endif

#if GPIO_EXTI_LINE7_EN == EN
static void (*GPIO7_Callback)(void);
#endif

#if GPIO_EXTI_LINE8_EN == EN
static void (*GPIO8_Callback)(void);
#endif

#if GPIO_EXTI_LINE9_EN == EN
static void (*GPIO9_Callback)(void);
#endif

#if GPIO_EXTI_LINE10_EN == EN
static void (*GPIO10_Callback)(void);
#endif

#if GPIO_EXTI_LINE11_EN == EN
static void (*GPIO11_Callback)(void);
#endif

#if GPIO_EXTI_LINE12_EN == EN
static void (*GPIO12_Callback)(void);
#endif

#if GPIO_EXTI_LINE13_EN == EN
static void (*GPIO13_Callback)(void);
#endif

#if GPIO_EXTI_LINE14_EN == EN
static void (*GPIO14_Callback)(void);
#endif

#if GPIO_EXTI_LINE15_EN == EN
static void (*GPIO15_Callback)(void);
#endif

static ERR HAL_PULL_SET(GPIO_Typedef * GPIO_PORT,uint8_t PULL,uint8_t Shift)
{
	ERR Err_State = HAL_OK;
	switch (PULL) {
		case GPIO_NOPULL:
			GPIO_PORT->PUPDR &= ~(PUPDR_CLEAR << Shift);
			GPIO_PORT->PUPDR |= (PUPDR_NOPULL << Shift);
			break;
		case GPIO_PULLDOWN:
			GPIO_PORT->PUPDR &= ~(PUPDR_CLEAR << Shift);
			GPIO_PORT->PUPDR |= (PUPDR_PULLDOWN << Shift);
			break;
		case GPIO_PULLUP:
			GPIO_PORT->PUPDR &= ~(PUPDR_CLEAR << Shift);
			GPIO_PORT->PUPDR |= (PUPDR_PULLUP << Shift);
			break;
		default:
			Err_State = HAL_ERR;
	}
	return Err_State;
}

static ERR HAL_Speed_SET(GPIO_Typedef * GPIO_PORT,uint8_t Speed,uint8_t Shift)
{
	ERR Err_State = HAL_OK;
	switch (Speed) {
		case GPIO_Speed_2MHz:
			GPIO_PORT->OSPEEDR &= ~(PUPDR_CLEAR << Shift);
			GPIO_PORT->OSPEEDR |= (OSPEEDR_LOW << Shift);
			break;
		case GPIO_Speed_25MHz:
			GPIO_PORT->OSPEEDR &= ~(PUPDR_CLEAR << Shift);
			GPIO_PORT->OSPEEDR |= (OSPEEDR_MED << Shift);
			break;
		case GPIO_Speed_50MHz:
			GPIO_PORT->OSPEEDR &= ~(PUPDR_CLEAR << Shift);
			GPIO_PORT->OSPEEDR |= (OSPEEDR_HIGH << Shift);
			break;
		case GPIO_Speed_100MHz:
			GPIO_PORT->OSPEEDR &= ~(PUPDR_CLEAR << Shift);
			GPIO_PORT->OSPEEDR |= (OSPEEDR_VHIGH << Shift);
			break;
		default:
			Err_State = HAL_ERR;
	}
	return Err_State;
}

static ERR Hal_Gpio_Pin_Config(GPIO_Typedef * GPIO_PORT, GPIO_InitStruct * GPIO_S,uint8_t Shift,uint8_t Pin_Num)
{
	uint8_t Reg_Number = (Pin_Num>>EXTI_SYSCFG_DIV);
	uint32_t *Exti_Reg = (Reg_Number == 0)? (uint32_t *)&SYSCFG->EXTICR1 : (Reg_Number == 1)? (uint32_t *)&SYSCFG->EXTICR2 :(Reg_Number == 2)?(uint32_t *)&SYSCFG->EXTICR3:(uint32_t *)&SYSCFG->EXTICR4;
	uint32_t  Pin_Number = (((Pin_Num & 0x07)<<2)&0x0F);
	uint8_t   EXTI_FLAG = 0x00;
	ERR Err_State = HAL_OK;
	if (GPIO_PORT == GPIOA)
		EXTI_FLAG = 0b0000;
	else if (GPIO_PORT == GPIOB)
		EXTI_FLAG = 0b0001;
	else if (GPIO_PORT == GPIOC)
		EXTI_FLAG = 0b0010;
	else if (GPIO_PORT == GPIOD)
		EXTI_FLAG = 0b0011;
	else if (GPIO_PORT == GPIOE)
		EXTI_FLAG = 0b0100;
	else if (GPIO_PORT == GPIOH)
		EXTI_FLAG = 0b0111;
	switch(GPIO_S->mode)
	{
		case GPIO_MODE_INPUT:
			GPIO_PORT->MODER &= ~(PUPDR_CLEAR<<Shift);
			GPIO_PORT->MODER |= (MODERX_MInput<<Shift);
			HAL_PULL_SET(GPIO_PORT,GPIO_S->pull,Shift);
			break;
		case GPIO_MODE_OUTPUT_PP:
			GPIO_PORT->MODER &= ~(PUPDR_CLEAR<<Shift);
			GPIO_PORT->MODER |= (MODERX_MGPOUT<<Shift);
			HAL_PULL_SET(GPIO_PORT,GPIO_S->pull,Shift);
			HAL_Speed_SET(GPIO_PORT,GPIO_S->Speed,Shift);
			break;
		case GPIO_MODE_OUTPUT_OD:
			GPIO_PORT->MODER &= ~(PUPDR_CLEAR<<Shift);
			GPIO_PORT->MODER |= (MODERX_MGPOUT << Shift);
			HAL_PULL_SET(GPIO_PORT,GPIO_S->pull,Shift);
			HAL_Speed_SET(GPIO_PORT,GPIO_S->Speed,Shift);
			break;
		case GPIO_MODE_AF_PP:
			GPIO_PORT->MODER &= ~(PUPDR_CLEAR<<Shift);
			GPIO_PORT->MODER |= (MODERX_MALT<<Shift);
			HAL_PULL_SET(GPIO_PORT,GPIO_S->pull,Shift);
			HAL_Speed_SET(GPIO_PORT,GPIO_S->Speed,Shift);
			break;
		case GPIO_MODE_AF_OD:
			GPIO_PORT->MODER &= ~(PUPDR_CLEAR<<Shift);
			GPIO_PORT->MODER |= (MODERX_MALT<<Shift);
			HAL_PULL_SET(GPIO_PORT,GPIO_S->pull,Shift);
			HAL_Speed_SET(GPIO_PORT,GPIO_S->Speed,Shift);
			break;
		case GPIO_MODE_AN_INPUT:
			GPIO_PORT->MODER &= ~(PUPDR_CLEAR<<Shift);
			GPIO_PORT->MODER |= (MODERX_MANA<<Shift);
			break;
#if GPIO_ENT_EN
		__disable_irq();
		case GPIO_MODE_IT_RISING:
			GPIO_PORT->MODER &= ~(PUPDR_CLEAR << Shift);
			GPIO_PORT->MODER |= (MODERX_MInput << Shift);
			HAL_PULL_SET(GPIO_PORT, GPIO_S->pull, Shift);
			EXTI->IMR  |= (1<<Pin_Num);
			EXTI->RTSR |= (1<<Pin_Num);
			*Exti_Reg  &= ~(PUPDR_CLEAR<<Pin_Number);
			*Exti_Reg  |= (EXTI_FLAG<<Pin_Number);
			break;
		case GPIO_MODE_IT_FALLING:
			GPIO_PORT->MODER &= ~(PUPDR_CLEAR << Shift);
			GPIO_PORT->MODER |= (MODERX_MInput << Shift);
			HAL_PULL_SET(GPIO_PORT, GPIO_S->pull, Shift);
			EXTI->IMR  |= (1<<Pin_Num);
			EXTI->FTSR |= (1<<Pin_Num);
			*Exti_Reg  &= ~(PUPDR_CLEAR<<Pin_Number);
			*Exti_Reg  |= (EXTI_FLAG<<Pin_Number);
			break;
		case GPIO_MODE_IT_RISING_FALLING:
			GPIO_PORT->MODER &= ~(PUPDR_CLEAR << Shift);
			GPIO_PORT->MODER |= (MODERX_MInput << Shift);
			HAL_PULL_SET(GPIO_PORT, GPIO_S->pull, Shift);
			EXTI->IMR  |= (1<<Pin_Num);
			EXTI->RTSR |= (1<<Pin_Num);
			EXTI->FTSR |= (1<<Pin_Num);
			*Exti_Reg  &= ~(PUPDR_CLEAR<<Pin_Number);
			*Exti_Reg  |= (EXTI_FLAG<<Pin_Number);
			break;
		case GPIO_MODE_EVT_RISING:
			GPIO_PORT->MODER &= ~(PUPDR_CLEAR << Shift);
			GPIO_PORT->MODER |= (MODERX_MInput << Shift);
			HAL_PULL_SET(GPIO_PORT, GPIO_S->pull, Shift);
			EXTI->EMR |= (1<<Pin_Num);
			EXTI->RTSR |= (1<<Pin_Num);
			*Exti_Reg  &= ~(PUPDR_CLEAR<<Pin_Number);
			*Exti_Reg  |= (EXTI_FLAG<<Pin_Number);
			break;
		case GPIO_MODE_EVT_FALLING:
			GPIO_PORT->MODER &= ~(PUPDR_CLEAR << Shift);
			GPIO_PORT->MODER |= (MODERX_MInput << Shift);
			HAL_PULL_SET(GPIO_PORT, GPIO_S->pull, Shift);
			EXTI->EMR |= (1<<Pin_Num);
			EXTI->FTSR |= (1<<Pin_Num);
			*Exti_Reg  &= ~(PUPDR_CLEAR<<Pin_Number);
			*Exti_Reg  |= (EXTI_FLAG<<Pin_Number);
			break;
		case GPIO_MODE_EVT_RISING_FALLING:
			GPIO_PORT->MODER &= ~(PUPDR_CLEAR << Shift);
			GPIO_PORT->MODER |= (MODERX_MInput << Shift);
			HAL_PULL_SET(GPIO_PORT, GPIO_S->pull, Shift);
			EXTI->EMR |= (1<<Pin_Num);
			EXTI->RTSR |= (1 << Pin_Num);
			EXTI->FTSR |= (1 << Pin_Num);
			*Exti_Reg  &= ~(PUPDR_CLEAR<<Pin_Number);
			*Exti_Reg  |= (EXTI_FLAG<<Pin_Number);
			break;
		__enable_irq();
#endif
	}
	return Err_State;
}

static ERR Hal_write_Pin(GPIO_Typedef * GPIO_PORT,uint16_t GPIO_PIN,GPIO_State State)
{
	ERR Err_State = HAL_OK;
	switch(State)
	{
		case GPIO_PIN_SET:
			GPIO_PORT->BSRR |= (1<<GPIO_PIN);
			break;
		case GPIO_PIN_RESET:
			GPIO_PORT->BSRR |= (1<<(GPIO_PIN+16));
			break;
		default:
			Err_State = HAL_ERR;
	}
	return Err_State ;
}

ERR  HAL_GPIO_Init(GPIO_Typedef * GPIO_PORT, GPIO_InitStruct * GPIO_S)
{
	ERR Err_State = HAL_OK;
	if( GPIO_PORT == NULL || GPIO_S ==NULL)
		Err_State = HAL_ERR;
	else{
		/* Enable Clock To the GPIO Port it is naturally disabled to reduce Power Consumption*/
		if (GPIO_PORT == GPIOA)
		{
			HAL_RCC_GPIOA_EN();
		}
		else if (GPIO_PORT == GPIOB)
		{
			HAL_RCC_GPIOB_EN();
		}
		else if (GPIO_PORT == GPIOC)
		{
			HAL_RCC_GPIOC_EN();
		}
		else if (GPIO_PORT == GPIOD)
		{
			HAL_RCC_GPIOD_EN();
		}
		else if (GPIO_PORT == GPIOE)
		{
			HAL_RCC_GPIOE_EN();
		}
		else if (GPIO_PORT == GPIOH)
		{
			HAL_RCC_GPIOH_EN();
		}
		/*Configure GPIO*/
		/*Since Function inits multiple pins a check must be done to configure a ceratin pin instead of using For Loop*/
		if (GPIO_S->Pin & GPIO_PIN_0) {
			/*Init PIN0*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_0,PIN_0);
#if GPIO0_INT_EN
			GPIO0_Callback = GPIO_S->GPIO0_Callbck;
			NVIC_EnableIRQ(EXTI0_IRQ);
#endif
		}
		if (GPIO_S->Pin & GPIO_PIN_1) {
			/*Init PIN1*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_1,PIN_1);
#if GPIO1_INT_EN
			GPIO1_Callback = GPIO_S->GPIO1_Callbck;
			NVIC_EnableIRQ(EXTI1_IRQ);
#endif
		}
		if (GPIO_S->Pin & GPIO_PIN_2) {
			/*Init PIN2*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_2,PIN_2);
#if GPIO2_INT_EN
			GPIO2_Callback = GPIO_S->GPIO2_Callbck;
			NVIC_EnableIRQ(EXTI2_IRQ);
#endif
		}
		if (GPIO_S->Pin & GPIO_PIN_3) {
			/*Init PIN3*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_3,PIN_3);
#if GPIO3_INT_EN
			GPIO3_Callback = GPIO_S->GPIO3_Callbck;
			NVIC_EnableIRQ(EXTI3_IRQ);
#endif
		}
		if (GPIO_S->Pin & GPIO_PIN_4) {
			/*Init PIN4*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_4,PIN_4);
#if GPIO4_INT_EN
			GPIO4_Callback = GPIO_S->GPIO4_Callbck;
			NVIC_EnableIRQ(EXTI4_IRQ);
#endif
		}
		if (GPIO_S->Pin & GPIO_PIN_5) {
			/*Init PIN5*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_5,PIN_5);
#if GPIO5_INT_EN
			GPIO5_Callback = GPIO_S->GPIO5_Callbck;
			NVIC_EnableIRQ(EXTI9_5_IRQ);
#endif
		}
		if (GPIO_S->Pin & GPIO_PIN_6) {
			/*Init PIN6*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_6,PIN_6);
#if GPIO6_INT_EN
			GPIO6_Callback = GPIO_S->GPIO6_Callbck;
			NVIC_EnableIRQ(EXTI9_5_IRQ);
#endif
		}
		if (GPIO_S->Pin & GPIO_PIN_7) {
			/*Init PIN7*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_7,PIN_7);
#if GPIO7_INT_EN
			GPIO7_Callback = GPIO_S->GPIO7_Callbck;
			NVIC_EnableIRQ(EXTI9_5_IRQ);
#endif
		}
		if (GPIO_S->Pin & GPIO_PIN_8) {
			/*Init PIN8*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_8,PIN_8);
#if GPIO8_INT_EN
			GPIO8_Callback = GPIO_S->GPIO8_Callbck;
			NVIC_EnableIRQ(EXTI9_5_IRQ);
#endif
		}
		if (GPIO_S->Pin & GPIO_PIN_9) {
			/*Init PIN9*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_9,PIN_9);
#if GPIO9_INT_EN
			GPIO9_Callback = GPIO_S->GPIO9_Callbck;
			NVIC_EnableIRQ(EXTI9_5_IRQ);
#endif
		}
		if (GPIO_S->Pin & GPIO_PIN_10) {
			/*Init PIN10*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_10,PIN_10);
#if GPIO10_INT_EN
			GPIO10_Callback = GPIO_S->GPIO10_Callbck;
			NVIC_EnableIRQ(EXTI15_10_IRQ);
#endif
		}
		if (GPIO_S->Pin & GPIO_PIN_11) {
			/*Init PIN11*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_11,PIN_11);
#if GPIO11_INT_EN
			GPIO11_Callback = GPIO_S->GPIO11_Callbck;
			NVIC_EnableIRQ(EXTI15_10_IRQ);
#endif
		}
		if (GPIO_S->Pin & GPIO_PIN_12) {
			/*Init PIN12*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_12,PIN_12);
#if GPIO12_INT_EN
			GPIO12_Callback = GPIO_S->GPIO12_Callbck;
			NVIC_EnableIRQ(EXTI15_10_IRQ);

#endif
		}
		if (GPIO_S->Pin & GPIO_PIN_13) {
			/*Init PIN13*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_13,PIN_13);
#if GPIO13_INT_EN
			GPIO13_Callback = GPIO_S->GPIO13_Callbck;
			NVIC_EnableIRQ(EXTI15_10_IRQ);
#endif
		}
		if (GPIO_S->Pin & GPIO_PIN_14) {
			/*Init PIN14*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_14,PIN_14);
#if GPIO14_INT_EN
			GPIO14_Callback = GPIO_S->GPIO14_Callbck;
			NVIC_EnableIRQ(EXTI15_10_IRQ);
#endif
		}
		if (GPIO_S->Pin & GPIO_PIN_15) {
			/*Init PIN15*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_15,PIN_15);
#if GPIO15_INT_EN
			GPIO15_Callback = GPIO_S->GPIO15_Callbck;
			NVIC_EnableIRQ(EXTI15_10_IRQ);
#endif
		}
	}
	return Err_State;
}

ERR  HAL_GPIO_WritePin(GPIO_Typedef * GPIO_PORT,uint16_t GPIO_PIN,GPIO_State State)
{
	ERR Err_State = HAL_OK;
	if (GPIO_PORT == NULL )
		Err_State = HAL_ERR;
	else {
		GPIO_PORT->MODER = 0x00000000U;
		if (GPIO_PIN & GPIO_PIN_0) {
			Err_State = Hal_write_Pin(GPIO_PORT, 0,State);
		}
		if (GPIO_PIN & GPIO_PIN_1) {
			Err_State = Hal_write_Pin(GPIO_PORT, 1,State);
		}
		if (GPIO_PIN & GPIO_PIN_2) {
			Err_State = Hal_write_Pin(GPIO_PORT, 2,State);
		}
		if (GPIO_PIN & GPIO_PIN_3) {
			Err_State = Hal_write_Pin(GPIO_PORT, 3,State);
		}
		if (GPIO_PIN & GPIO_PIN_4) {
			Err_State = Hal_write_Pin(GPIO_PORT, 4,State);
		}
		if (GPIO_PIN & GPIO_PIN_5) {
			Err_State = Hal_write_Pin(GPIO_PORT, 5,State);
		}
		if (GPIO_PIN & GPIO_PIN_6) {
			Err_State = Hal_write_Pin(GPIO_PORT, 6,State);
		}
		if (GPIO_PIN & GPIO_PIN_7) {
			Err_State = Hal_write_Pin(GPIO_PORT, 7,State);
		}
		if (GPIO_PIN & GPIO_PIN_8) {
			Err_State = Hal_write_Pin(GPIO_PORT, 8,State);
		}
		if (GPIO_PIN & GPIO_PIN_9) {
			Err_State = Hal_write_Pin(GPIO_PORT, 9,State);
		}
		if (GPIO_PIN & GPIO_PIN_10) {
			Err_State = Hal_write_Pin(GPIO_PORT, 10,State);
		}
		if (GPIO_PIN & GPIO_PIN_11) {
			Err_State = Hal_write_Pin(GPIO_PORT, 11,State);
		}
		if (GPIO_PIN & GPIO_PIN_12) {
			Err_State = Hal_write_Pin(GPIO_PORT, 12,State);
		}
		if (GPIO_PIN & GPIO_PIN_13) {
			Err_State = Hal_write_Pin(GPIO_PORT, 13,State);
		}
		if (GPIO_PIN & GPIO_PIN_14) {
			Err_State = Hal_write_Pin(GPIO_PORT, 14,State);
		}
		if (GPIO_PIN & GPIO_PIN_15) {
			Err_State = Hal_write_Pin(GPIO_PORT, 15,State);
		}
	}
	return Err_State;
}

GPIO_State HAL_GPIO_ReadPin(GPIO_Typedef * GPIO_PORT, uint16_t GPIO_PIN)
{
	ERR Err_State = HAL_OK;
	GPIO_State State = GPIO_PIN_RESET;
	if (GPIO_PORT == NULL)
		Err_State = HAL_ERR;
	else {
		switch(GPIO_PIN)
		{
			case GPIO_PIN_0:
				State =READ_BIT(GPIO_PORT->IDR,0);
				break;
			case GPIO_PIN_1:
				State =READ_BIT(GPIO_PORT->IDR,1);
				break;
			case GPIO_PIN_2:
				State =READ_BIT(GPIO_PORT->IDR,2);
				break;
			case GPIO_PIN_3:
				State =READ_BIT(GPIO_PORT->IDR,3);
				break;
			case GPIO_PIN_4:
				State =READ_BIT(GPIO_PORT->IDR,4);
				break;
			case GPIO_PIN_5:
				State =READ_BIT(GPIO_PORT->IDR,5);
				break;
			case GPIO_PIN_6:
				State =READ_BIT(GPIO_PORT->IDR,6);
				break;
			case GPIO_PIN_7:
				State =READ_BIT(GPIO_PORT->IDR,7);
				break;
			case GPIO_PIN_8:
				State =READ_BIT(GPIO_PORT->IDR,8);
				break;
			case GPIO_PIN_9:
				State =READ_BIT(GPIO_PORT->IDR,9);
				break;
			case GPIO_PIN_10:
				State =READ_BIT(GPIO_PORT->IDR,10);
				break;
			case GPIO_PIN_11:
				State =READ_BIT(GPIO_PORT->IDR,11);
				break;
			case GPIO_PIN_12:
				State =READ_BIT(GPIO_PORT->IDR,12);
				break;
			case GPIO_PIN_13:
				State =READ_BIT(GPIO_PORT->IDR,13);
				break;
			case GPIO_PIN_14:
				State =READ_BIT(GPIO_PORT->IDR,14);
				break;
			case GPIO_PIN_15:
				State =READ_BIT(GPIO_PORT->IDR,15);
				break;
		}
	}
	return State;
}

ERR  HAL_GPIO_TogglePin(GPIO_Typedef * GPIO_PORT, uint16_t GPIO_PIN)
{
	ERR Err_State = HAL_OK;
	if (GPIO_PORT == NULL)
		Err_State = HAL_ERR;
	else {
		GPIO_PORT->ODR ^= GPIO_PIN;
	}
	return Err_State;
}


#if GPIO_EXTI_LINE0_EN == EN
void EXTI0_IRQHandler(void)
{
	EXTI->PR |= (1<<PIN_0);
	if(GPIO0_Callback)
		GPIO0_Callback();
}
#endif

#if GPIO_EXTI_LINE1_EN == EN
void EXTI1_IRQHandler(void)
{
	EXTI->PR |= (1<<PIN_1);
	if(GPIO1_Callback)
		GPIO1_Callback();
}
#endif

#if GPIO_EXTI_LINE2_EN == EN
void EXTI2_IRQHandler(void)
{
	EXTI->PR |= (1<<PIN_2);
	if(GPIO2_Callback)
		GPIO2_Callback();
}
#endif

#if GPIO_EXTI_LINE3_EN == EN
void EXTI3_IRQHandler(void)
{
	EXTI->PR |= (1<<PIN_3);
	if(GPIO3_Callback)
		GPIO3_Callback();
}
#endif

#if GPIO_EXTI_LINE4_EN == EN
void EXTI4_IRQHandler(void)
{
	EXTI->PR |= (1<<PIN_4);
	if(GPIO4_Callback)
		GPIO4_Callback();
}
#endif

#if GPIO5_INT_EN || GPIO6_INT_EN ||GPIO7_INT_EN ||GPIO8_INT_EN ||GPIO9_INT_EN
void EXTI9_5_IRQHandler(void)
{
#if  GPIO5_INT_EN
	EXTI->PR |= (1<<PIN_5);
	if(GPIO5_Callback)
		GPIO5_Callback();
#endif
#if GPIO6_INT_EN
	EXTI->PR |= (1<<PIN_6);
	if(GPIO6_Callback)
		GPIO6_Callback();
#endif
#if GPIO7_INT_EN
	EXTI->PR |= (1<<PIN_7);
	if(GPIO7_Callback)
		GPIO7_Callback();
#endif
#if GPIO8_INT_EN
	EXTI->PR |= (1<<PIN_8);
	if(GPIO8_Callback)
		GPIO8_Callback();
#endif
#if GPIO9_INT_EN
	EXTI->PR |= (1<<PIN_9);
	if(GPIO9_Callback)
		GPIO9_Callback();
#endif
}
#endif

#if GPIO10_INT_EN || GPIO11_INT_EN || GPIO12_INT_EN || GPIO13_INT_EN||GPIO14_INT_EN|| GPIO15_INT_EN
void EXTI15_10_IRQHandler(void)
{
#if  GPIO10_INT_EN
	EXTI->PR |= (1<<PIN_10);
	if(GPIO10_Callback)
		GPIO10_Callback();
#endif
#if GPIO11_INT_EN
	EXTI->PR |= (1<<PIN_11);
	if(GPIO11_Callback)
		GPIO11_Callback();
#endif
#if GPIO12_INT_EN
	EXTI->PR |= (1<<PIN_12);
	if(GPIO12_Callback)
		GPIO12_Callback();
#endif
#if GPIO13_INT_EN
	EXTI->PR |= (1<<PIN_13);
	if(GPIO13_Callback)
		GPIO13_Callback();
#endif
#if GPIO14_INT_EN
	EXTI->PR |= (1<<PIN_14);
	if(GPIO14_Callback)
		GPIO14_Callback();
#endif
#if GPIO15_INT_EN
	EXTI->PR |= (1<<PIN_15);
	if(GPIO15_Callback)
		GPIO15_Callback();
#endif
}
#endif
