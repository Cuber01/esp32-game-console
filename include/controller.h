#ifndef __KEYBOARD_H
#define __KEYBOARD_H

typedef enum
{
    INPUT_NONE=-1,
    INPUT_FORWARD=32,
    INPUT_BACKWARD=33,
    INPUT_RIGHT=26,
    INPUT_LEFT=25,
    INPUT_PRESS=27
} InputKeys;

void controllerInit();
InputKeys controllerGetInput();

#endif
