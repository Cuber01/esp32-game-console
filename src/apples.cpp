#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "map.h"
#include "apples.h"

#define RV_ERROR 1
#define RV_OK 0

point_t apples[MAX_APPLES];
int appleMultiplier = 1;

point_t calculateApplePos(void)
{
    int apple_x = rand() % MAP_WIDTH + 0;
    int apple_y = rand() % MAP_HEIGHT + 0;

    return (point_t){ .x = apple_x, .y = apple_y };
}

void applesClear()
{
    for (uint16_t i = 0; i < MAX_APPLES; i++)
    {
        apples[i] = (point_t){.x = -1, .y = -1};
    }
}

int appleAdd(point_t applePos)
{
    for (int i = 0; i < MAX_APPLES; i++)
    {
        if (apples[i].x == -1)
        {
            apples[i] = applePos;
            return RV_OK;
        }
    }

    return RV_ERROR;
}

int appleDelete(point_t applePos)
{
    for (uint16_t i = 0; i < MAX_APPLES; i++)
    {
        if (apples[i].x == applePos.x && apples[i].y == applePos.y)
        {
            apples[i] = (point_t){.x = -1, .y = -1};
            return RV_OK;
        }
    }

    return RV_ERROR;
}

int appleContains(point_t applePos)
{
    for (uint16_t i = 0; i < MAX_APPLES; i++)
    {
        if (apples[i].x == applePos.x && apples[i].y == applePos.y)
        {
            return true;
        }
    }

    return false;
}
