#ifndef __SNAKE_H
#define __SNAKE_H

#include "game.h"
#include "map.h"
#include <cstdint>

#include "controller.h"

#define MAX_SNAKES (MAP_WIDTH * MAP_HEIGHT)

extern point_t snakeParts[MAX_SNAKES];

extern int snake_vel_x;
extern int snake_vel_y;

typedef struct
{
    uint32_t head_index;
    uint32_t tail_index;
} snakeElementState;

typedef struct
{
    snakeElementState state;
    point_t snakeParts[MAX_SNAKES];
} snake_t;

void snakeInit(void);
void snakeClear(void);
void snakeMove(InputKeys direction, bool grow);
void placeSnakeOnMap(void);

point_t SnakeGet(void);

#endif