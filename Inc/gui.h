/*
 * gui.h
 *
 *  Created on: Aug 22, 2023
 *      Author: Nguyen Duc Phu
 */

#ifndef GUI_H_
#define GUI_H_

// 0b1000xxxx : this is a screen
// 0b1xxxxxxx : this is a screen options
// 0bxxxxxxxx : other utility options
// (x) don't care

#define GUI_MAIN_SCREEN					0x80U

#define GUI_SCREEN_SETTINGS				0x81U
#define SETTING_SOUND					0x91U
#define SETTING_CONTRAST				0xA1U

#define SETTING_OPTION_INCREMENT		0x03U	// 0b00000011
#define SETTING_OPTION_DECREMENT		0x05U	// 0b00000101

#define GUI_SCREEN_GAMES				0x82U
#define GAME_SNAKE						0x92U

#define SELECT_OPTIONS_MAX				0x0AU
#define SCREEN_HISTORY_MAX				0x0AU

void GUI_ScreenSelection(void);
void GUI_MainScreen(void);
void GUI_GamesScreen(void);
void GUI_SettingsScreen(void);
void GUI_SnakeGame(void);

void GUI_UpdateLCDScreenHeader(char *headerStr);
void GUI_UpdateLCDOption(char *graphicStr[], const uint8_t *options);
void GUI_UpdateLCDPointer(void);
void GUI_UpdatePointers(void);
void GUI_ClearSelectOptions(void);
void GUI_FillCurrentScreenOptions(const uint8_t *optionsArr);

void GUI_SetGraphicStatus(void);
void GUI_ClearGraphicStatus(void);



#endif /* GUI_H_ */
