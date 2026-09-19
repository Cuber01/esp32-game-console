#include <cstdio>

#include "map.h"

char Map[MAP_HEIGHT][MAP_WIDTH];

void mapClear()
{
    for (auto &row : Map)
    {
        for (auto &cell : row)
        {
            cell = GROUND_SYMBOL;
        }
    }
}

void mapSet(int8_t x, int8_t y, char value)
{
    Map[y][x] = value;
}

char mapGet(int8_t x, int8_t y)
{
    return Map[y][x];
}

bool isInBounds(int8_t x, int8_t y)
{
    // y is offset from the top by "apples: x" si
    if (x < MAP_WIDTH && y < MAP_HEIGHT && y > 0 && x > -1)
    {
        return true;
    }
    return false;
}
