#include <stdio.h>
#include "esp_random.h"
#include "game.h"
#include "map.h"
#include "apples.h"

Point CurrentApple = (Point){.x = -1, .y = -1};
int ApplesEaten = 0;

Point calculateApplePos()
{  
    uint32_t apple_x = esp_random() % MAP_WIDTH + 0;
    uint32_t apple_y = (esp_random() % (MAP_HEIGHT-1) ) + 1;
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
