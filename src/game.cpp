#include "game.h"

#include <FreeRTOSConfig.h>
#include <portmacro.h>

#include "controller.h"
#include "apples.h"
#include "snake.h"
#include "map.h"

bool gameRunning = true;

void handleInput(char input, bool grow);

void draw()
{
   placeSnakeOnMap();
   mapSet(CurrentApple.x, CurrentApple.y, APPLE_SYMBOL);
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
    update(ControllerGetLastKeyPressed());
    draw();
}

void gameInit()
{
    ControllerInit();
    snakeInit();
    createApple(calculateApplePos());
}

bool IsRunning() {
    return gameRunning;
}

void EndGame() {
    gameRunning = false;
}




