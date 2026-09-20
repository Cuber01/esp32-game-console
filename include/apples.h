#ifndef __APPLES_H
#define __APPLES_H

#include "game.h"

extern int ApplesEaten;
extern Point CurrentApple;

Point calculateApplePos(void);

void createApple(Point applePos);
void appleRemove();

#endif