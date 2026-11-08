#include <stdio.h>
#include "esp_random.h"
#include "game.h"
#include "map.h"
#include "apples.h"
#include "graphics.h"

Point CurrentApple = (Point){.x = -1, .y = -1};
int ApplesEaten = 0;

Point calculateApplePos()
{
    uint32_t apple_x = 0;
    uint32_t apple_y = 0;
    do {
        apple_x = esp_random() % MAP_WIDTH + 0;
        apple_y = (esp_random() % (MAP_HEIGHT-1) ) + 1;
    } while (mapGet(static_cast<int8_t>(apple_x), static_cast<int8_t>(apple_y)) == SNAKE_SYMBOL);

    assert(IsInBounds((int)apple_x, (int)apple_y));
    return (Point){ .x = (int)apple_x, .y = (int)apple_y };
}

void createApple(Point applePos)
{
    CurrentApple = applePos;
}

void appleRemove()
{
    CurrentApple = (Point){.x = -1, .y = -1};
}
