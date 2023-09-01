/*
 * timer.h
 *
 *  Created on: Aug 10, 2023
 *      Author: Nguyen Duc Phu
 */

#ifndef TIMER_H_
#define TIMER_H_

void Timer2_Init(void);
void Timer2_Delay(int timeInMs);
void Timer2_DelayMicro(int timeInMicrosecond);

void Timer1_Init(void);
void Timer1_EnablePWM(uint16_t frequency, uint8_t dutyCyclePercent);
void Timer1_DisablePWM(void);

#endif /* TIMER_H_ */
