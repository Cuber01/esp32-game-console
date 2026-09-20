#ifndef __MAP_H
#define __MAP_H

#include "graphics.h"
#include "game.h"
#include <cstdint>

#define GROUND_SYMBOL ' '
#define APPLE_SYMBOL '@'
#define SNAKE_SYMBOL '#'

#define MAP_HEIGHT ((SCREEN_HEIGHT/MAP_BLOCK_SIZE) -1)
#define MAP_WIDTH (SCREEN_WIDTH/MAP_BLOCK_SIZE)

extern char Map[MAP_HEIGHT][MAP_WIDTH];

void mapClear();
void gameDraw();
void mapSet(int8_t x, int8_t y, char value);
char mapGetDefault(int8_t x, int8_t y, char defaultVal);
char mapGet(int8_t x, int8_t y);
bool IsInBounds(int8_t x, int8_t y);
bool IsInBounds(Point p);

#endif