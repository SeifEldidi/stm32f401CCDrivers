/*
 * Hal_GPIO.c
 *
 *  Created on: Jul 25, 2023
 *      Author: Seif pc
 */
#include "../../Inc/HAL/Hal_GPIO.h"

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

static ERR Hal_Gpio_Pin_Config(GPIO_Typedef * GPIO_PORT, GPIO_InitStruct * GPIO_S,uint8_t Shift)
{
	ERR Err_State = HAL_OK;
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
		case GPIO_MODE_AF_INPUT:
			GPIO_PORT->MODER &= ~(PUPDR_CLEAR<<Shift);
			GPIO_PORT->MODER |= (MODERX_MInput<<Shift);
			HAL_PULL_SET(GPIO_PORT,GPIO_S->pull,Shift);
			break;
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
		/*Since Function inits multiple pins a check must be done to configure a ceratin pin*/
		if (GPIO_S->Pin & GPIO_PIN_0) {
			/*Init PIN0*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_0);
		}
		if (GPIO_S->Pin & GPIO_PIN_1) {
			/*Init PIN1*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_1);
		}
		if (GPIO_S->Pin & GPIO_PIN_2) {
			/*Init PIN2*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_2);
		}
		if (GPIO_S->Pin & GPIO_PIN_3) {
			/*Init PIN3*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_3);
		}
		if (GPIO_S->Pin & GPIO_PIN_4) {
			/*Init PIN4*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_4);
		}
		if (GPIO_S->Pin & GPIO_PIN_5) {
			/*Init PIN5*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_5);
		}
		if (GPIO_S->Pin & GPIO_PIN_6) {
			/*Init PIN6*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_6);
		}
		if (GPIO_S->Pin & GPIO_PIN_7) {
			/*Init PIN7*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_7);
		}
		if (GPIO_S->Pin & GPIO_PIN_8) {
			/*Init PIN8*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_8);
		}
		if (GPIO_S->Pin & GPIO_PIN_9) {
			/*Init PIN9*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_9);
		}
		if (GPIO_S->Pin & GPIO_PIN_10) {
			/*Init PIN10*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_10);
		}
		if (GPIO_S->Pin & GPIO_PIN_11) {
			/*Init PIN11*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_11);
		}
		if (GPIO_S->Pin & GPIO_PIN_12) {
			/*Init PIN12*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_12);
		}
		if (GPIO_S->Pin & GPIO_PIN_13) {
			/*Init PIN13*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_13);
		}
		if (GPIO_S->Pin & GPIO_PIN_14) {
			/*Init PIN14*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_14);
		}
		if (GPIO_S->Pin & GPIO_PIN_15) {
			/*Init PIN15*/
			Err_State = Hal_Gpio_Pin_Config(GPIO_PORT,GPIO_S,MODERX_PIN_15);
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
