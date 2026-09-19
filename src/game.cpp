#include "game.h"

#include <esp32-hal.h>

#include "controller.h"
#include "apples.h"
#include "snake.h"
#include "map.h"
#include "cyclic_buffer.h"

bool gameRunning = true;
int frameDelay = 100;

void handleInput(char input, bool grow);

unsigned int time_counter=0;

void draw()
{
   placeSnakeOnMap();
   mapSet(CurrentApple.x, CurrentApple.y, APPLE_SYMBOL);
   gameDraw();
}


void update(InputKeys input)
{
    point_t snakePostition = cbGetHead();
    bool grow = false;

    if (CurrentApple.x == snakePostition.x && CurrentApple.y == snakePostition.y) {
        ApplesEaten += 1;
        appleRemove();
        createApple(calculateApplePos());
        grow = true;
    }

    snakeMove(input, grow);
}

void gameLoop()
{
    mapClear();
    update(controllerGetInput());
    draw();
    delay(frameDelay); // TODO is this correct delay?
}

void gameInit()
{
    controllerInit();
    snakeInit();
    createApple(calculateApplePos());
}




