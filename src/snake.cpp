#include <stdio.h>

#include "game.h"
#include "snake.h"
#include "cyclic_buffer.h"
#include "map.h"

int snake_vel_x = 1;
int snake_vel_y = 0;

bool snakeCollision( point_t value, bool head_omit);

void snakeClear(void)
{
    snakeInit();
}

point_t SnakeGet(void)
{
    return cbGetHead();
}

void placeSnakeOnMap(void)
{
    point_t point;
    int rv;

    cbIterateReset();

    while (true)
    {
        rv = cbIterateGet(&point);
        if (rv)
            break;

        if (!checkBounds(point.x,point.y)) {
            // TODO GAME OVER
        }
        mapSet(point.x, point.y, SNAKE_SYMBOL);
    }

}

void snakeInit(void)
{
    cbClear();

    cbAdd((point_t){.x = 9, .y = 3});
    cbAdd((point_t){.x = 8, .y = 3});
    cbAdd((point_t){.x = 7, .y = 3});
    cbAdd((point_t){.x = 6, .y = 3});
}

void snakeMove(direction_t direction, bool grow )
{
    static direction_t current_direction = NONE;

    point_t current_postition = cbGetHead();
    point_t next_postition = current_postition;

    if( direction == NONE ){
        direction = current_direction;
    }

    switch (direction)
    {
    case LEFT:
        next_postition.x -= 1;        
        break;
    case RIGHT:
        next_postition.x += 1;
        break;
    case UP:
        next_postition.y -= 1;
        break;
    case DOWN:
        next_postition.y += 1;    
        break;
    case NONE:    
        break;
    };

    if( current_direction != NONE ){
        if (snakeCollision( next_postition, false )) {
              gameRunning = false;
        }    
    }

    cbAdd(next_postition);


    if (!grow)
    {
        cbDel();
    } 
        
    current_direction = direction;
    
}

bool snakeCollision( point_t value, bool head_omit )
{
    point_t current_value;
    bool stop;       
    bool head_flag = head_omit;

    cbIterateReset();

    while (true)
    {
        stop = cbIterateGet( &current_value );
        if( stop ){
            return false;
        }
        if( ! head_flag ){
            if( current_value.x == value.x && current_value.y == value.y) 
            {
                return true;
            } 
            head_flag = false;
        }

    }

    return false;
}
