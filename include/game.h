#ifndef __MAIN_H
#define __MAIN_H

extern bool gameRunning;

typedef struct
{
    int x;
    int y;
} point_t;

void gameLoop();
void gameInit();

#endif
