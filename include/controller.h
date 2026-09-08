#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#define CONTROLLER_FORWARD 32
#define CONTROLLER_BACKWARD 33
#define CONTROLLER_RIGHT 26
#define CONTROLLER_LEFT 25
#define CONTROLLER_PRESS 27

void controllerInit();
char controllerGetInput(void);

#endif
