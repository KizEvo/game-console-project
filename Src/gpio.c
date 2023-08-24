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
 * Configure GPIOx
***************************************/
void GPIOA_Init(void){
	// Enable GPIOA clock
	RCC->APB2ENR |= 1 << 2;

	// PIN 5 & 7 as alternate function output mode push-pull
	GPIOA->CRL = 0x90904444;

	// Set PIN 0
	GPIOA->BSRR |= (1 << SET_PIN(0));
}

void GPIOB_Init(void){
	// Enable GPIOB clock
	RCC->APB2ENR |= 1 << 3;

	// PIN 5, 6 & 7 as GPIO output push-pull
	GPIOB->CRL = 0x11100000;

	// Set PIN 5
	GPIOB->BSRR |= (1 << SET_PIN(5));
	// Set PIN 6
	GPIOB->BSRR |= (1 << SET_PIN(6));
	// Clear PIN 7
	GPIOB->BSRR |= (1 << RESET_PIN(7));
}

void GPIOC_Init(void){
	// Enable GPIOC clock
	RCC->APB2ENR |= 1 << 4;

	// PIN 13 as GPIO output push-pull
	GPIOC->CRH = 0x00200000;

	// Set PIN 13
	GPIOB->BSRR |= (1 << SET_PIN(13));
}

/************ DESCRIPTION *************
 * Warning signal when user-defined error occurred
 * Not really effective but at least it will tell something have went wrong and need attentions
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
