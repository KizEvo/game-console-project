/*
 * timer.c
 *
 *  Created on: Aug 10, 2023
 *      Author: Nguyen Duc Phu
 */

#include <stm32f1xx.h>
#include "timer.h"

/************ DESCRIPTION *************
 * Configure Timer clock
***************************************/

void Timer2_Init(void){
	// Enable Timer2 clock
	RCC->APB1ENR |= (1 << 0);
}

void Timer1_Init(void){
	// Enable Timer1 clock
	RCC->APB2ENR |= (1 << 11);
}

/************ DESCRIPTION *************
 * Configure Timer2 delay of 1ms
***************************************/

void Timer2_Delay(int timeInMs){
	// Enable pre-load ARR buffer
	TIM2->CR1 |= (1 << 7);

	// ARR value
	TIM2->ARR = 10;

	// Pre-scaler value
	TIM2->PSC = 4799;

	// Clear previous update interrupt flag UIF
	TIM2->SR &= ~(1 << 0);

	// Enable counter
	TIM2->CR1 |= (1 << 0);

	// Delay
	while(timeInMs-- > 0)
	{
		while((TIM2->SR & 1 << 0) == 0);
		// Clear update interrupt flag UIF
		TIM2->SR &= ~(1 << 0);
	}

	// Disable counter
	TIM2->CR1 &= ~(1 << 0);
}

/************ DESCRIPTION *************
 * Configure Timer2 delay of 1us
***************************************/

void Timer2_DelayMicro(int timeInMicrosecond){
	// Enable pre-load ARR buffer
	TIM2->CR1 |= (1 << 7);

	// ARR value
	TIM2->ARR = 12;

	// Pre-scaler value
	TIM2->PSC = 3;

	// Clear previous update interrupt flag UIF
	TIM2->SR &= ~(1 << 0);

	// Enable counter
	TIM2->CR1 |= (1 << 0);

	// Delay
	while(timeInMicrosecond-- > 0)
	{
		while((TIM2->SR & 1 << 0) == 0);
		// Clear update interrupt flag UIF
		TIM2->SR &= ~(1 << 0);
	}

	// Disable timer counter
	TIM2->CR1 &= ~(1 << 0);
}

/************ DESCRIPTION *************
 * Configure Timer1 CH1 (PA8) in PWM mode
 * Polarity active high
 * Edge-aligned mode, up counting
 * Parameters: frequency, duty cycle percentage
***************************************/

void Timer1_EnablePWM(uint16_t frequency, uint8_t dutyCyclePercent){

	uint16_t prescalerValue = 1200.0;
	// 48*10^6 = APB2ENR Timer clock (check rcc.c for more details)
	double arrValue = (1.0 / frequency) * (48000000.0 / prescalerValue);

	// ARR value (frequency)
	TIM1->ARR = (uint16_t)arrValue;
	// Pre-scaler value
	TIM1->PSC = prescalerValue - 1;
	// CCR1 value (duty cycle)
	TIM1->CCR1 = (uint16_t)(arrValue * (dutyCyclePercent / 100.0));
	// PWM mode 1 in CH1
	TIM1->CCMR1 |= (0x6 << 4);
	// Enable CH1 pre-load register
	TIM1->CCMR1 |= (1 << 3);
	// Auto-reload pre-load enable
	TIM1->CR1 |= (1 << 7);

	// Initialize all the registers
	TIM1->EGR |= (1 << 0);

	// Capture/Compare 1 output enable
	TIM1->CCER |= (1 << 0);
	// Set MOE bit
	TIM1->BDTR |= (1 << 15);

	// Enable counter
	TIM1->CR1 |= (1 << 0);
}

void Timer1_DisablePWM(void){
	// Capture/Compare 1 output disable
	TIM1->CCER &= ~(1 << 0);
	// Clear MOE bit
	TIM1->BDTR &= ~(1 << 15);
	// Disable timer counter
	TIM1->CR1 &= ~(1 << 0);
}
