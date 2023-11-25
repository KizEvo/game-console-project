/*
 * sound.c
 *
 *  Created on: Nov 19, 2023
 *      Author: Nguyen Duc Phu
 */

#include <stm32f1xx.h>
#include "sound.h"
#include "timer.h"
#include "gpio.h"

uint8_t muteSound = 1;

void Sound_PlayNote(uint16_t noteFrequency){
	Timer1_EnablePWM(noteFrequency, 20);
}

void Sound_Config(uint8_t option){
	if (option == SOUND_ON || option == SOUND_OFF)
		muteSound = option;
	else
		GPIOC_Warning();
}

void Sound_Disable(void){
	Timer1_DisablePWM();
}

void Sound_MenuNavigate(void){
	if(!muteSound) return;
	Sound_PlayNote(NOTE_D4);
	Timer2_Delay(90);
	Sound_Disable();
}

void Sound_LostGame(void){
	if(!muteSound) return;
	Sound_PlayNote(NOTE_B5);
	Timer2_Delay(238);

	Sound_Disable();
	Timer2_Delay(84);

	Sound_PlayNote(NOTE_F5);
	Timer2_Delay(223);

	Sound_Disable();
	Timer2_Delay(590);

	Sound_PlayNote(NOTE_D4);
	Timer2_Delay(585);

	Sound_Disable();
}

void Sound_SnakeEatApple(void){
	if(!muteSound) return;
	Sound_PlayNote(NOTE_D4);
	Timer2_Delay(50);

	Sound_Disable();
	Timer2_Delay(17);

	Sound_PlayNote(NOTE_B4);
	Timer2_Delay(183);

	Sound_Disable();
	Timer2_Delay(197);

	Sound_PlayNote(NOTE_A5);
	Timer2_Delay(90);

	Sound_Disable();
}
