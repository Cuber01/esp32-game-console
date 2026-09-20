#ifndef __MAIN_H
#define __MAIN_H

typedef struct
{
    int x;
    int y;
} Point;

void gameLoop();
void gameInit();
void EndGame();
bool IsRunning();

#endif
