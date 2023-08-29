/*
 * interrupt.h
 *
 *  Created on: Aug 17, 2023
 *      Author: Nguyen Duc Phu
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include <stdint.h>

extern uint8_t flagBtnAction0;	// SELECT / OK / Others
extern uint8_t flagBtnAction1;	// Others
extern uint8_t flagBtnAction2;	// Others
extern uint8_t flagBtnAction3;	// Others
extern uint8_t flagBtnAction4;	// Others
extern uint8_t flagBtnAction5;	// Others

void EXTI_Init(void);

void NVIC_Enable(void);

void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);

void EXTI_ClearIRQFlag(uint8_t *flag);

#endif /* INTERRUPT_H_ */
