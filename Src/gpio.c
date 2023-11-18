/*
 * gpio.c
 *
 *  Created on: Aug 10, 2023
 *      Author: Nguyen Duc Phu
 */

#include <stm32f1xx.h>
#include "gpio.h"
#include "timer.h"

/************ DESCRIPTION *************
 * Configure a GPIOx Pin
***************************************/

void GPIO_Config(GPIO_TypeDef *GPIO, uint8_t pin, uint8_t mode, uint32_t afioMap, uint32_t remapOption){
	uint32_t controlReg = mode & 0xF;

	// AFIO re-map
	if(afioMap != NO_REMAP){
		uint32_t tempAfioMap = afioMap;
		uint8_t position = 0;
		while((afioMap & 1) == 0){
			afioMap = afioMap >> 1;
			position++;
		}
		if(!((AFIO->MAPR & tempAfioMap) == (remapOption << position))){
			RCC->APB2ENR |= (1 << 0);
			AFIO->MAPR &= ~tempAfioMap;
			AFIO->MAPR |= (remapOption << position);
		}
	}

	// Check if pin is in low or high register
	if(pin <= 7){
		GPIO->CRL &= ~(0xF << (pin * 4));
		GPIO->CRL |= (controlReg << (pin * 4));
	} else {
		GPIO->CRH &= ~(0xF << ((pin - 8) * 4));
		GPIO->CRH |= (controlReg << ((pin - 8) * 4));
	}

	// Check if mode is input pull-up/pull-down then pull-up/pull-down accordingly
	if((mode & 0x3) == 0 && !((mode & 1 << 3) == 0)){
		if(!((mode & 1 << 4) == 0)) GPIO->BSRR |= (1 << SET_PIN(pin));
		else GPIO->BSRR |= (1 << RESET_PIN(pin));
	}
}

void GPIO_EnableClock(uint8_t portPos){
	RCC->APB2ENR |= (1 << portPos);
}

/************ DESCRIPTION *************
 * Initialize GPIOx
***************************************/

void GPIOA_Init(void){
	// Enable GPIOA clock
	GPIO_EnableClock(RCC_APB2ENR_IOPAEN_Pos);

	// PIN 1, 2 & 15 as floating input mode
	GPIO_Config(GPIOA, 1, INPUT_FLOATING, NO_REMAP, NO_REMAP);
	GPIO_Config(GPIOA, 2, INPUT_FLOATING, NO_REMAP, NO_REMAP);
	GPIO_Config(GPIOA, 15, INPUT_FLOATING, AFIO_MAPR_SWJ_CFG, 2);

	// PIN 5 & 7 as alternate function output push-pull
	GPIO_Config(GPIOA, 5, ALT_OUTPUT_PUSHPULL | OUTPUT_SPD_MED, NO_REMAP, NO_REMAP);
	GPIO_Config(GPIOA, 7, ALT_OUTPUT_PUSHPULL | OUTPUT_SPD_MED, NO_REMAP, NO_REMAP);
}

void GPIOB_Init(void){
	// Enable GPIOB clock
	GPIO_EnableClock(RCC_APB2ENR_IOPBEN_Pos);

	// PIN 0, 1 & 2 as GPIO output push-pull
	GPIO_Config(GPIOB, 0, GP_OUTPUT_PUSHPULL | OUTPUT_SPD_MED, NO_REMAP, NO_REMAP);
	GPIO_Config(GPIOB, 1, GP_OUTPUT_PUSHPULL | OUTPUT_SPD_MED, NO_REMAP, NO_REMAP);
	GPIO_Config(GPIOB, 2, GP_OUTPUT_PUSHPULL | OUTPUT_SPD_MED, NO_REMAP, NO_REMAP);
	// PIN 3, 4 & 5 as floating input mode
	GPIO_Config(GPIOB, 3, INPUT_FLOATING, AFIO_MAPR_SWJ_CFG, 2);
	GPIO_Config(GPIOB, 4, INPUT_FLOATING, AFIO_MAPR_SWJ_CFG, 2);
	GPIO_Config(GPIOB, 5, INPUT_FLOATING, NO_REMAP, NO_REMAP);
	// PIN 15 as alternate function output push-pull
	GPIO_Config(GPIOB, 15, ALT_OUTPUT_PUSHPULL | OUTPUT_SPD_LOW, NO_REMAP, NO_REMAP);

	// Pre-Set/Clear pins
	GPIOB->BSRR |= (1 << SET_PIN(10));
	GPIOB->BSRR |= (1 << SET_PIN(1));
	GPIOB->BSRR |= (1 << RESET_PIN(0));
}

void GPIOC_Init(void){
	// Enable GPIOC clock
	GPIO_EnableClock(RCC_APB2ENR_IOPCEN_Pos);
	// PIN 13 as GPIO output push-pull
	GPIO_Config(GPIOC, 13, GP_OUTPUT_PUSHPULL | OUTPUT_SPD_LOW, NO_REMAP, NO_REMAP);
	// Set PIN 13
	GPIOB->BSRR |= (1 << SET_PIN(13));
}

/************ DESCRIPTION *************
 * Warning signal when user-defined error occurred
 * Not really effective but at least it will tell something have went wrong and needed attentions
 * Only needed in development
***************************************/

void GPIOC_Warning(void){
	for(uint8_t i = 0; i < 10; i++){
		GPIOC->BSRR |= (1 << SET_PIN(13));
		Timer2_Delay(150);
		GPIOC->BSRR |= (1 << RESET_PIN(13));
		Timer2_Delay(150);
		GPIOC->BSRR |= (1 << SET_PIN(13));
		Timer2_Delay(500);
		GPIOC->BSRR |= (1 << RESET_PIN(13));
		Timer2_Delay(200);
	}

	while(1);
}
