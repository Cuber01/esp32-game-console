#ifndef __APPLES_H
#define __APPLES_H

#include "game.h"

extern int ApplesEaten;
extern point_t CurrentApple;

point_t calculateApplePos(void);

void createApple(point_t applePos);
void appleRemove();

#endif