/*
 * rcc.c
 *
 *  Created on: Aug 10, 2023
 *      Author: Nguyen Duc Phu
 */

#include <stm32f1xx.h>
#include <rcc.h>

/************ DESCRIPTION *************
 * Configure clock pre-scalers
 * Setup PLL as system clock
***************************************/

void Clock_Init(void){

	// AHB prescaler: no division
	RCC->CFGR &= ~(1 << 7);

	// APB1 prescaler: divided by 2
	RCC->CFGR |= (0x4 << 8);

	// APB2 prescaler: no division
	RCC->CFGR &= ~(1 << 13);

	// PLL clock source input is (HSI / 2)
	RCC->CFGR &= ~(1 << 16);

	// PLL multiplication factor : x12
	RCC->CFGR |= (0xA << 18);

	// Enable the PLL and wait for it to become ready
	RCC->CR |= (1 << 24);
	while(!((RCC->CR & 1 << 25) == 0));

	// System clock switch to PLL and wait for it to become ready
	RCC->CFGR |= (0x2 << 0);
	while(!((RCC->CFGR & 2 << 2) == 8));
}

