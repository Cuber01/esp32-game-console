#ifndef __MAP_H
#define __MAP_H

#include "graphics.h"
#include <cstdint>

#define GROUND_SYMBOL ' '
#define APPLE_SYMBOL '@'
#define SNAKE_SYMBOL '#'

#define MAP_HEIGHT (SCREEN_WIDTH/MAP_BLOCK_SIZE)
#define MAP_WIDTH (SCREEN_HEIGHT/MAP_BLOCK_SIZE)

extern char Map[MAP_HEIGHT][MAP_WIDTH];

void mapClear();
void mapDraw();
void mapSet(uint8_t x, uint8_t y, char value);
char mapGet(uint8_t x, uint8_t y);
bool isInBounds(uint8_t x, uint8_t y);

#endif