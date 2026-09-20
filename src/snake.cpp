#include "game.h"
#include "snake.h"

#include <HardwareSerial.h>

#include "fixed_circular_queue.h"
#include "map.h"
#include "controller.h"

FixedCircularQueue<Point, MAX_SNAKES> SnakeParts = FixedCircularQueue<Point, MAX_SNAKES>((Point){.x = 9, .y = 3});

bool snakeCollision();

void placeSnakeOnMap()
{
    // TODO: fix iterator for when head < tail
    for (size_t i = 0; i < SnakeParts.Size(); i++) {
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
    SnakeParts.PushHead((Point){.x = 8, .y = 3});
    SnakeParts.PushHead((Point){.x = 7, .y = 3});
    SnakeParts.PushHead((Point){.x = 6, .y = 3});
    SnakeParts.PushHead((Point){.x = 5, .y = 3});
    SnakeParts.PushHead((Point){.x = 4, .y = 3});
    SnakeParts.PushHead((Point){.x = 3, .y = 3});
    SnakeParts.PushHead((Point){.x = 2, .y = 3});
    SnakeParts.PushHead((Point){.x = 1, .y = 3});
}

void snakeMove(InputKeys direction, bool grow )
{
    static InputKeys current_direction = INPUT_NONE;

    Point& currentPosition = SnakeParts.GetHead();
    Point nextPosition = currentPosition;

    if(direction == INPUT_NONE)
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

    if (current_direction != INPUT_NONE) {
        if (snakeCollision()) {
            EndGame();
        }

        SnakeParts.PushHead(nextPosition);

        if (!grow)
        {
            SnakeParts.PopTail();
        }
    }




        
    current_direction = direction;
}

bool snakeCollision()
{
    Point& head = SnakeParts.GetHead();
    for (size_t i = 0; i < SnakeParts.Size() - 1; i++) {
        Point& item = SnakeParts[i];
        if (head.x == item.x && head.y == item.y) {
            return true;
        }
    }

    return false;
}
