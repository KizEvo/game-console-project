/*
 * gpio.c
 *
 *  Created on: Aug 10, 2023
 *      Author: Nguyen Duc Phu
 */

#include <stm32f1xx.h>
#include "gpio.h"

/************ DESCRIPTION *************
 * Configure GPIOx
***************************************/
void GPIOA_Init(void){
	// Enable GPIOA clock
	RCC->APB2ENR |= 1 << 2;

	// GPIOA, PIN 0 as output push-pull
	GPIOA->CRL = 0x1;

	// Set PIN 0
	GPIOA->BSRR |= 1 << 0;
}

void GPIOB_Init(void){
	// Enable GPIOB clock
	RCC->APB2ENR |= 1 << 3;

	// PIN 5 & 6 as GPIO output push-pull
	GPIOB->CRL = 0x02200000;

	// PIN 13 & 15 as SPI alternate function output mode
	// PIN 13: SCK, PIN 15: MOSI
	GPIOB->CRH = 0x90900000;

	// Set GPIOB PIN 5 & 6
	GPIOB->BSRR |= (0b11 << 5);
}

