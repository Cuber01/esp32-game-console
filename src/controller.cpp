#include <stdio.h>
#include "controller.h"

#include <esp32-hal-gpio.h>

void controllerInit()
{
    pinMode(INPUT_FORWARD, INPUT_PULLUP);
    pinMode(INPUT_BACKWARD, INPUT_PULLUP);
    pinMode(INPUT_RIGHT, INPUT_PULLUP);
    pinMode(INPUT_LEFT, INPUT_PULLUP);
    pinMode(INPUT_PRESS, INPUT_PULLUP);
}

InputKeys controllerGetInput()
{
    if (digitalRead(INPUT_FORWARD)  == LOW) return INPUT_FORWARD;
    if (digitalRead(INPUT_BACKWARD) == LOW) return INPUT_BACKWARD;
    if (digitalRead(INPUT_RIGHT)    == LOW) return INPUT_LEFT;
    if (digitalRead(INPUT_LEFT)     == LOW) return INPUT_RIGHT;
    if (digitalRead(INPUT_PRESS)    == LOW) return INPUT_PRESS;

    return INPUT_NONE;
}


