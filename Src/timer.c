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

	// Disable counter
	TIM2->CR1 &= ~(1 << 0);
}
