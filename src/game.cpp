#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

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
void clearArray(char Map[MAP_WIDTH][MAP_HEIGHT]);

unsigned int time_counter=0;

void draw( char Map[MAP_WIDTH][MAP_HEIGHT], point_t apples[MAX_APPLES] ) 
{
    placeSnakeOnMap();

    for (int i = 0; i < MAX_APPLES; i++) 
    {   
        if ( apples[i].x != -1)
        {
            Map[apples[i].x][apples[i].y] = APPLE_SYMBOL;
        }
    }

   mapDraw();
}


void update(char input, point_t apples[MAX_APPLES])
{
    point_t snake_postition = cbGetHead();
    bool grow = false;

    for ( int i = 0; i < MAX_APPLES; i++)
    {
        if (apples[i].x == snake_postition.x && apples[i].y == snake_postition.y) 
        {
            applesEaten += 1;
            appleDelete(apples[i]);
            appleAdd(calculateApplePos());
            grow = true;
        } 
    } 

    //handleInput(input, grow);
}

char getInput(void)
{
    char key = controllerGetInput();

    if( key != NO_KEY ) {
            return key;
    }

    return NO_KEY;
}

void handleInput(char input, bool grow) 
{
    if (input == 'a')
    {
        snakeMove( LEFT, grow );

    } else if (input == 'd')
    {
        snakeMove( RIGHT, grow );

    } else if (input == 'w')
    {
        snakeMove( UP, grow );

    } else if (input == 's')
    {
        snakeMove( DOWN, grow );
    } else
    {
        snakeMove( NONE, grow );
    }
}


void gameLoop()
{
    mapClear();
    update(getInput(), apples);
    draw(Map, apples);
    delay(frameDelay); // TODO is this correct delay?
}

void gameInit(void)
{
    controllerInit();
    srand(time(NULL));
    applesClear();
    snakeInit();
    appleAdd(calculateApplePos());
}




