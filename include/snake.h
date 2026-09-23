#ifndef GAME_CONSOLE_SNAKE_H
#define GAME_CONSOLE_SNAKE_H

#include "game.h"
#include "map.h"
#include "fixed_circular_queue.h"
#include "controller.h"

#define MAX_SNAKES (MAP_WIDTH * MAP_HEIGHT)

extern FixedCircularQueue<Point, MAX_SNAKES> SnakeParts;


typedef struct
{
    uint32_t head_index;
    uint32_t tail_index;
} snakeElementState;

typedef struct
{
    snakeElementState state;
    Point snakeParts[MAX_SNAKES];
} snake_t;

void snakeInit();
void snakeClear();
void snakeMove(InputKeys direction, bool grow);
void placeSnakeOnMap();


#endif // GAME_CONSOLE_SNAKE_H