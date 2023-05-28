#ifndef PLAYER_H_
#define PLAYER_H_
#include "objectsType.h"


void playersInit();
void playerMovement(int keycode, int player);



extern volatile struct player redPlayer;
extern volatile struct player bluePlayer;

void convertCoordinateUpHill(struct player* playerStruct, int choice);
void convertCoordinateDownHill(struct player* playerStruct, int choice);
void convertCoordinateGround(struct player* playerStruct, int choice);

void updateDownhillLeftRightxy(struct player* playerStruct);
void updateGroundLeftRightxy(struct player* playerStruct);
void updateUphillLeftRightxy(struct player* playerStruct);
#endif
