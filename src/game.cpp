#include "game.h"

#include <esp32-hal.h>

#include "controller.h"
#include "apples.h"
#include "snake.h"
#include "map.h"
#include "fixed_list.h"

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
    Point& snakePostition = SnakeParts.GetHead();
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
    delay(frameDelay);
}

void gameInit()
{
    controllerInit();
    snakeInit();
    createApple(calculateApplePos());
}

bool IsRunning() {
    return gameRunning;
}

void EndGame() {
    gameRunning = false;
}




