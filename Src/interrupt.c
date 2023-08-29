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
uint8_t flagBtnAction4 = 0;
uint8_t flagBtnAction5 = 0;

/************ DESCRIPTION *************
 * Initialize hardware external interrupt
 * Pin 0, 1, 2 & 3 of port A
***************************************/

void EXTI_Init(void){
	// Enable AFIO clock
	RCC->APB2ENR |= (1 << 0);
	// EXTI on pin 0, 1, 2 & 3 of port A
	AFIO->EXTICR[0] &= ~(0xFFFF);
	// EXTI on pin 4 & 5 of port B
	AFIO->EXTICR[1] |= 0x0011;

	// Interrupt on falling edge on line 0, 1, 2, 3, 4 & 5
	EXTI->FTSR |= 0x3F << 0;

	// Set bit mask of 0, 1, 2, 3, 4 & 5
	EXTI->IMR |= 0x3F << 0;

}

/************ DESCRIPTION *************
 * Enable NVIC for line 0, 1, 2, 3 & 4 external interrupt
 * Default interrupt priority is 0 (highest)
***************************************/

void NVIC_Enable(void){
	uint32_t line = 0;
	// Set line 0 to 4
	line |= (0x1F << 6);
	// Set line 5 (5-9)
	line |= (1 << 23);
	// Write to ISER register
	NVIC->ISER[0] = line;
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

void EXTI4_IRQHandler(void){
	flagBtnAction4 = 1;
	// Clear pending register
	EXTI->PR |= 1 << 4;
}

void EXTI9_5_IRQHandler(void){
	flagBtnAction5 = 1;
	// Clear pending register
	EXTI->PR |= 1 << 5;
}

void EXTI_ClearIRQFlag(uint8_t *flag){
	*flag = 0;
}
