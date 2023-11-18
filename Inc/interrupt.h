/*
 * interrupt.h
 *
 *  Created on: Aug 17, 2023
 *      Author: Nguyen Duc Phu
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include <stdint.h>

extern uint8_t flagBtnAction0;	// SELECT / OK / Others // PA15
extern uint8_t flagBtnAction1;	// Others				// PA1
extern uint8_t flagBtnAction2;	// Others				// PA2
extern uint8_t flagBtnAction3;	// Others				// PB3
extern uint8_t flagBtnAction4;	// Others				// PB4
extern uint8_t flagBtnAction5;	// Others				// PB5

void EXTI_Init(void);

void NVIC_Enable(void);
void NVIC_SetInterruptPriority(uint32_t interruptPos, uint32_t priority);

void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

void EXTI_ClearIRQFlag(uint8_t *flag);
void EXTI_ClearAllIRQFlag(void);

#endif /* INTERRUPT_H_ */
