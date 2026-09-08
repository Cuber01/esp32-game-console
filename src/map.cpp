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

void mapSet(uint8_t x, uint8_t y, char value)
{
    Map[y][x] = value;
}

char mapGet(uint8_t x, uint8_t y)
{
    return Map[y][x];
}

bool isInBounds(uint8_t x, uint8_t y)
{
    if (x < MAP_WIDTH-1 && y < MAP_HEIGHT-1 && y > 0 && x > 0)
    {
        return true;
    }
    return false;
}
