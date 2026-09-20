#include "game.h"
#include "snake.h"

#include <HardwareSerial.h>

#include "fixed_circular_queue.h"
#include "map.h"
#include "controller.h"

FixedCircularQueue<Point, MAX_SNAKES> SnakeParts = FixedCircularQueue<Point, MAX_SNAKES>();

bool snakeCollision(Point& nextPos);

void placeSnakeOnMap()
{
    for (size_t i = 1; i < SnakeParts.Size(); i++) {
        const Point& point = SnakeParts[i];
        if (!IsInBounds(point.x, point.y)) {
            EndGame();
            break;
        }
        mapSet(point.x, point.y, SNAKE_SYMBOL);
    }
}

void snakeInit()
{
    SnakeParts.PushHead((Point){.x = 1, .y = 3});
    SnakeParts.PushHead((Point){.x = 2, .y = 3});
    SnakeParts.PushHead((Point){.x = 3, .y = 3});
    SnakeParts.PushHead((Point){.x = 4, .y = 3});
    SnakeParts.PushHead((Point){.x = 5, .y = 3});
    SnakeParts.PushHead((Point){.x = 6, .y = 3});
    SnakeParts.PushHead((Point){.x = 7, .y = 3});
    SnakeParts.PushHead((Point){.x = 8, .y = 3});
}

void snakeMove(InputKeys direction, bool grow)
{
    static InputKeys current_direction = INPUT_NONE;

    Point nextPosition = SnakeParts.GetHead();

    // Fix for edge case bug that happens when starting the game
    if (current_direction == INPUT_NONE && direction == INPUT_LEFT) {
        direction = INPUT_NONE;
    }

    if     (direction == INPUT_NONE
        || (direction == INPUT_PRESS)
        || (direction == INPUT_RIGHT && current_direction == INPUT_LEFT)
        || (direction == INPUT_LEFT && current_direction == INPUT_RIGHT)
        || (direction == INPUT_FORWARD && current_direction == INPUT_BACKWARD)
        || (direction == INPUT_BACKWARD && current_direction == INPUT_FORWARD))
    {
        direction = current_direction;
    }

    switch (direction)
    {
    case INPUT_LEFT:
        nextPosition.x -= 1;
        break;
    case INPUT_RIGHT:
        nextPosition.x += 1;
        break;
    case INPUT_FORWARD:
        nextPosition.y -= 1;
        break;
    case INPUT_BACKWARD:
        nextPosition.y += 1;
        break;
    default:
        break;
    };

    if (direction != INPUT_NONE)
    {
        if (snakeCollision(nextPosition)) {
            EndGame();
        }

        SnakeParts.PushHead(nextPosition);

        if (!grow)
        {
            SnakeParts.PopTail();
        }

        current_direction = direction;
    }
}

bool snakeCollision(Point& nextPos)
{
    for (size_t i = 1; i < SnakeParts.Size(); i++) {
        Point& part = SnakeParts[i];
        if (&part != &SnakeParts.GetHead() && nextPos.x == part.x && nextPos.y == part.y) {
            return true;
        }
    }

    return false;
}
