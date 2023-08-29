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

	// PIN 0, 1, 2 & 3 as floating input mode
	// PIN 7 & 5 as alternate function output push-pull
	GPIOA->CRL = 0x90904444;
}

void GPIOB_Init(void){
	// Enable GPIOB clock
	RCC->APB2ENR |= 1 << 3;

	// Enable AFIO clock
	RCC->APB2ENR |= (1 << 0);
	// Re-map PIN 4
	AFIO->MAPR |= (1 << 24);

	// PIN 0, 1 & 10 as GPIO output push-pull
	// PIN 4, 5 as floating input mode
	GPIOB->CRL = 0x00440011;
	GPIOB->CRH = 0x00000100;

	GPIOB->BSRR |= (1 << SET_PIN(10));
	GPIOB->BSRR |= (1 << SET_PIN(1));
	GPIOB->BSRR |= (1 << RESET_PIN(0));
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
