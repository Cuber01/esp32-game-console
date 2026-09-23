#ifndef GAME_CONSOLE_APPLES_H
#define GAME_CONSOLE_APPLES_H

#include "game.h"

extern int ApplesEaten;
extern Point CurrentApple;

Point calculateApplePos(void);

void createApple(Point applePos);
void appleRemove();

#endif // GAME_CONSOLE_APPLES_H