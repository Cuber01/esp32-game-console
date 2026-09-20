#include <cstdio>

#include "map.h"

#include "game.h"

char Map[MAP_HEIGHT][MAP_WIDTH];

void mapClear()
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            Map[i][j] = GROUND_SYMBOL;
        }
    }
}

void mapSet(int8_t x, int8_t y, char value)
{
    assert(IsInBounds(x, y));
    Map[y][x] = value;
}

char mapGetDefault(int8_t x, int8_t y, char defaultVal) {
    if (IsInBounds(x, y)) {
        return Map[y][x];
    }
    return defaultVal;
}

char mapGet(int8_t x, int8_t y)
{
    assert(IsInBounds(x, y));
    return Map[y][x];
}

bool IsInBounds(int8_t x, int8_t y)
{
    // y is offset from the top by "apples: x" si
    if (x < MAP_WIDTH && y < MAP_HEIGHT && y > 0 && x > -1)
    {
        return true;
    }
    return false;
}

bool IsInBounds(Point p)
{
    return IsInBounds(p.x, p.y);
}
