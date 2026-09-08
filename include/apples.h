#ifndef __APPLES_H
#define __APPLES_H

#define MAX_APPLES 10
extern point_t apples[MAX_APPLES];


void PlaceApple(point_t applePos, point_t apples[MAX_APPLES]);
point_t calculateApplePos(void);

bool appleAdd(point_t applePos);
bool appleDelete(point_t applePos);
void applesClear();

#endif