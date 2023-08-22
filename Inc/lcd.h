/*
 * lcd.h
 *
 *  Created on: Aug 12, 2023
 *      Author: Nguyen Duc Phu
 */

#ifndef LCD_H_
#define LCD_H_

/************ DESCRIPTION *************
 * LCD Instructions
***************************************/
// Function Set
#define CHIP_ACTIVE					0x00U
#define CHIP_POW_DOWN 				0x04U
#define HORIZONTAL_ADDR 			0x00U
#define VERTICAL_ADDR 				0x02U
#define BASIC_INSTRUCTION 			0x00U
#define EXTENDED_INSTRUCTION 		0x01U
// Display CTRL
#define DISPLAY_BLANK 				0x00U
#define NORMAL_MODE 				0x04U
#define ALL_SEGMENT_ON 				0x01U
#define INVERSE_MODE				0x05U
// Command
#define LCD_COMMAND_DISPLAY_CTRL	0x08U 	// Format for display control
#define LCD_COMMAND_VOP				0x80U	// Format for setting VOP (LCD contrast)
#define LCD_COMMAND_TEMP_CTRL 		0x04U	// Format for temperature coefficient
#define LCD_COMMAND_FUNC_SET		0x20U	// Format for function set
#define LCD_COMMAND_VOL_BIAS		0x10U	// Format for setting voltage bias
#define LCD_COMMAND_X_POS			0x80U	// Format for setting X position
#define LCD_COMMAND_Y_POS			0x40U	// Format for setting Y position
// Others
#define LCD_MAX_X_PIXEL				0x53U	// pixel width 83
#define LCD_MAX_Y_PIXEL				0x2FU	// pixel height 47
#define LCD_MAX_Y_BANK				0x05U	// bank height 5
#define LCD_MAX_CONTRAST			0x7FU	// contrast of LCD

extern uint8_t bufferLCD[48][84];

extern uint8_t contrastLCD;

extern uint8_t offsetX;
extern uint8_t offsetY;

extern uint8_t prevSetX;
extern uint8_t prevSetY;

void LCD_Init(void);
void LCD_WriteCommand(const uint8_t command);
void LCD_WriteData(const uint8_t data);
void LCD_GUI(void);
void LCD_ClearScreen(void);
void LCD_SetPosition(uint8_t Xpos, uint8_t Ybank);
void LCD_UpdatePosOffset(void);
void LCD_UpdateBuffer(const uint8_t data);
void LCD_DrawPixel(uint8_t Xpos, uint8_t Ypos);
void LCD_ErasePixel(uint8_t Xpos, uint8_t Ypos);
void LCD_WriteString(char *string);

uint8_t LCD_IncrementContrast(void);
uint8_t LCD_DecrementContrast(void);

#endif /* LCD_H_ */
