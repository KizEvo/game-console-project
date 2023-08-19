/*
 * snake_game.h
 *
 *  Created on: Aug 19, 2023
 *      Author: Nguyen Duc Phu
 */

#ifndef SNAKE_GAME_H_
#define SNAKE_GAME_H_

#define GAMESNAKE_MOVEUP		0x00
#define GAMESNAKE_MOVEDOWN		0x01
#define GAMESNAKE_TURNLEFT		0x02
#define GAMESNAKE_TURNRIGHT		0x03

void GameSnake_GUI(void);
void GameSnake_Start(void);

uint8_t GameSnake_UpdatePositionHead(uint8_t *dirFlag, uint8_t *Xpos, uint8_t *Ypos);
void GameSnake_UpdatePositionTail(uint8_t *Xpos, uint8_t *Ypos);

void GameSnake_SavePathTraversal(uint8_t *Xpos, uint8_t *Ypos, uint8_t *Xidx, uint8_t *Yidx);

#endif /* SNAKE_GAME_H_ */
