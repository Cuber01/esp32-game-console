#ifndef GAME_CONSOLE_GAME_H
#define GAME_CONSOLE_GAME_H

typedef struct
{
    int x;
    int y;
} Point;

void gameLoop();
void gameInit();
void EndGame();
bool IsRunning();

#endif // GAME_CONSOLE_GAME_H
