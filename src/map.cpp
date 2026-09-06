#include <stdio.h>

#include "map.h"

char Map[MAP_WIDTH][MAP_HEIGHT];

void mapClear(void)
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            Map[x][y] = GROUND_SYMBOL;
        };
    };
}

void mapSet(uint8_t x, uint8_t y, char value)
{
    Map[x][y] = value;
}

char mapGet(uint8_t x, uint8_t y)
{
    return (Map[x][y]);
}

bool checkBounds(uint8_t x, uint8_t y)
{
    if (x > MAP_WIDTH - 1 || y > MAP_HEIGHT - 1)
    {
        return false;
    }
    return true;
}
