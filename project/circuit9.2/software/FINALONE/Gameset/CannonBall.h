#ifndef CANNONBALL_H_
#define CANNONBALL_H_
#include "objectsType.h"

void cannonBallInit();
void cannonBallCharging();
void cannonBallFiring(int next_player, int posture, int player_x, int player_y, struct player* playerStruct);
void cannonBallAngle(int keycode);

int isLegalCannonBall();
extern struct cannon_Ball cannonBall;
#endif