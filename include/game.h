#ifndef __MAIN_H
#define __MAIN_H

extern int applesEaten;
extern bool gameRunning;

typedef struct
{
    int x;
    int y;
} point_t;

extern point_t apples[];

void gameLoop();
void gameInit();

#endif
