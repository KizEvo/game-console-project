/*
 * interrupt.c
 *
 *  Created on: Aug 17, 2023
 *      Author: Nguyen Duc Phu
 */

#include <stm32f1xx.h>
#include "interrupt.h"

uint8_t flagBtnAction0 = 0;
uint8_t flagBtnAction1 = 0;
uint8_t flagBtnAction2 = 0;
uint8_t flagBtnAction3 = 0;

/************ DESCRIPTION *************
 * Initialize hardware external interrupt
 * Pin 0, 1, 2 & 3 of port A
***************************************/

void EXTI_Init(void){
	// Enable AFIO clock
	RCC->APB2ENR |= (1 << 0);
	// EXTI on pin 0, 1, 2 & 3 of port A
	AFIO->EXTICR[0] &= ~(0xFFFF);

	// Interrupt on falling edge on line 0, 1, 2 & 3
	EXTI->FTSR |= 0xF << 0;

	// Set bit mask of 0, 1, 2 & 3
	EXTI->IMR |= 0xF << 0;

}

/************ DESCRIPTION *************
 * Enable NVIC for line 0, 1, 2 & 3 external interrupt
 * Default interrupt priority is 0 (highest)
***************************************/

void NVIC_Enable(void){
	NVIC->ISER[0] = (0xF << 6);
}

/************ DESCRIPTION *************
 * IRQ Handlers
 * Get called whenever there is an external interrupt
***************************************/

void EXTI0_IRQHandler(void){
	flagBtnAction0 = 1;
	// Clear pending register
	EXTI->PR |= 1 << 0;
}

void EXTI1_IRQHandler(void){
	flagBtnAction1 = 1;
	// Clear pending register
	EXTI->PR |= 1 << 1;
}

void EXTI2_IRQHandler(void){
	flagBtnAction2 = 1;
	// Clear pending register
	EXTI->PR |= 1 << 2;
}

void EXTI3_IRQHandler(void){
	flagBtnAction3 = 1;
	// Clear pending register
	EXTI->PR |= 1 << 3;
}

void EXTI_ClearIRQFlag(uint8_t *flag){
	*flag = 0;
}
