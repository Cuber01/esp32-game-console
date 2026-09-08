#include "game.h"

#include <esp32-hal.h>

#include "controller.h"
#include "apples.h"
#include "snake.h"
#include "map.h"
#include "cyclic_buffer.h"

bool gameRunning = true;
int frameDelay = 100;

int applesEaten;

void handleInput(char input, bool grow);

unsigned int time_counter=0;

void draw(point_t applePositions[MAX_APPLES])
{
    placeSnakeOnMap();

    for (int i = 0; i < MAX_APPLES; i++) 
    {   
        if (applePositions[i].x != -1)
        {
            mapSet(applePositions[i].x, applePositions[i].y, APPLE_SYMBOL);
        }
    }

   mapDraw();
}


void update(InputKeys input, point_t applePositions[MAX_APPLES])
{
    point_t snakePostition = cbGetHead();
    bool grow = false;

    for (int i = 0; i < MAX_APPLES; i++)
    {
        if (applePositions[i].x == snakePostition.x && applePositions[i].y == snakePostition.y)
        {
            applesEaten += 1;
            appleDelete(applePositions[i]);
            appleAdd(calculateApplePos());
            grow = true;
        } 
    } 

    snakeMove(input, grow);
}

void gameLoop()
{
    mapClear();
    update(controllerGetInput(), apples);
    draw(apples);
    delay(frameDelay); // TODO is this correct delay?
}

void gameInit()
{
    controllerInit();
    applesClear();
    snakeInit();
    appleAdd(calculateApplePos());
}




