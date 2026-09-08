#include <cstdio>

#include "game.h"
#include "snake.h"
#include "cyclic_buffer.h"
#include "map.h"
#include "controller.h"

int snake_vel_x = 1;
int snake_vel_y = 0;

bool snakeCollision( point_t value, bool head_omit);

void snakeClear()
{
    snakeInit();
}

point_t SnakeGet()
{
    return cbGetHead();
}

void placeSnakeOnMap()
{
    point_t point;
    int rv;

    cbIterateReset();

    while (true)
    {
        rv = cbIterateGet(&point);
        if (rv)
            break;

        // TODO what??
        if (!isInBounds(point.x,point.y)) {
            gameRunning = false;
            break;
        }
        mapSet(point.x, point.y, SNAKE_SYMBOL);
    }

}

void snakeInit()
{
    cbClear();

    cbAdd((point_t){.x = 9, .y = 3});
    cbAdd((point_t){.x = 8, .y = 3});
    cbAdd((point_t){.x = 7, .y = 3});
    cbAdd((point_t){.x = 6, .y = 3});
}

void snakeMove(InputKeys direction, bool grow )
{
    static InputKeys current_direction = INPUT_NONE;

    point_t currentPostition = cbGetHead();
    point_t nextPostition = currentPostition;

    if(direction == INPUT_NONE)
    {
        direction = current_direction;
    }

    switch (direction)
    {
    case INPUT_LEFT:
        nextPostition.x -= 1;
        break;
    case INPUT_RIGHT:
        nextPostition.x += 1;
        break;
    case INPUT_FORWARD:
        nextPostition.y -= 1;
        break;
    case INPUT_BACKWARD:
        nextPostition.y += 1;
        break;
    default:
        break;
    };

    if (current_direction != INPUT_NONE) {
        if (snakeCollision( nextPostition, false )) {
            gameRunning = false;
        }
    }


    cbAdd(nextPostition);

    if (!grow)
    {
        cbDelete();
    } 
        
    current_direction = direction;
    
}

bool snakeCollision( point_t value, bool head_omit )
{
    point_t current_value;
    bool head_flag = head_omit;

    cbIterateReset();

    while (true)
    {
        bool stop = cbIterateGet(&current_value);

        if(stop)
        {
            return false;
        }
        if(!head_flag)
        {
            if(current_value.x == value.x && current_value.y == value.y)
            {
                return true;
            } 
            head_flag = false;
        }

    }

    return false;
}
