#include "Player.h"

void playersInit(){
    // red player

    redPlayer.x_pos = RED_PLAYER_X_INIT_POS;
    redPlayer.y_pos = RED_PLAYER_Y_INIT_POS;
    // redPlayer.x_pos = 151;
    // redPlayer.y_pos = 201;
    // printf("%d y\n",redPlayer.y_pos);
	// printf("%d x\n",redPlayer.x_pos);
    // printf("%d yy\n",RED_PLAYER_Y_INIT_POS);
	// printf("%d xx\n",RED_PLAYER_X_INIT_POS);
    redPlayer.state = 0;
    redPlayer.pos_state = 0;
    redPlayer.health = PLAYER_MAX_HEALTH;
    redPlayer.direction = 1;   // red player standing at left, facing right
    redPlayer.x_prev = RED_PLAYER_X_INIT_POS;
    redPlayer.y_prev = RED_PLAYER_Y_INIT_POS;
    redPlayer.x_left = RED_PLAYER_X_INIT_POS - 10;
    redPlayer.y_left = RED_PLAYER_Y_INIT_POS - 10;
    redPlayer.x_right = RED_PLAYER_X_INIT_POS + 10;
    redPlayer.y_right = RED_PLAYER_Y_INIT_POS - 10;
    redPlayer.v_y = 0;
    redPlayer.magic = 0;
    
    // blue player
    bluePlayer.x_pos = BLUE_PLAYER_X_INIT_POS;
    bluePlayer.y_pos = BLUE_PLAYER_Y_INIT_POS;
    bluePlayer.state = 0;
    bluePlayer.pos_state = 0;
    bluePlayer.health = PLAYER_MAX_HEALTH;
    bluePlayer.direction = 0;  // blue player standing at right, facing left
    bluePlayer.x_prev = BLUE_PLAYER_X_INIT_POS;
    bluePlayer.y_prev = BLUE_PLAYER_Y_INIT_POS;
    bluePlayer.x_left = BLUE_PLAYER_X_INIT_POS - 10;
    bluePlayer.y_left = BLUE_PLAYER_Y_INIT_POS - 10;
    bluePlayer.x_right = BLUE_PLAYER_X_INIT_POS + 10;
    bluePlayer.y_right = BLUE_PLAYER_Y_INIT_POS - 10;
    bluePlayer.v_y = 0;
    bluePlayer.magic = 0;

    return;
}

void playerMovement(int keycode, int player){
    // only basic movement for week 1 demo.

    // if(keycode == KEY_LEFT && redPlayer.x_pos > RED_PLAYER_X_INIT_POS){
    //     redPlayer.x_pos--;
    // } 
    //  if(keycode == KEY_RIGHT && redPlayer.x_pos < 600){
    //     redPlayer.x_pos++;
    // } 
    // printf("%d x\n" , redPlayer.x_pos);

    int x_pos = (player == 0) ? redPlayer.x_pos: bluePlayer.x_pos;
    int y_pos = (player == 0) ? redPlayer.y_pos: bluePlayer.y_pos;
    int direction;
    int another_x = (player == 1) ? redPlayer.x_pos: bluePlayer.x_pos;
    int another_y = (player == 1) ? redPlayer.y_pos: bluePlayer.y_pos;

    int player_state = 0;

    if(keycode == KEY_LEFT){
        player_state = 0;
        if(((x_pos-1) >= RED_PLAYER_X_INIT_POS) && (((x_pos-1) != another_x) || (another_y != y_pos))){
            x_pos -= PLAYER_STEP;
        }
        direction = 0; // face left
    }
    else{
        player_state = 1;
        if(((x_pos+1) <= GAME_RIGHTEDGE) && (((x_pos+1) != another_x) || (another_y != y_pos))){
            x_pos += PLAYER_STEP;
        }
        direction = 1; // face left
    }

    if(player == 0){
        redPlayer.x_pos = x_pos;
        redPlayer.y_pos = y_pos;
        redPlayer.direction = direction;
    }
    else{
        bluePlayer.x_pos = x_pos;
        bluePlayer.y_pos = y_pos;
        bluePlayer.direction = direction;
    }
    // printf("%d player in move \n" , player);
    //printf("%d x\n" , redPlayer.x_pos);
    printf("The player position: x: %d; y: %d\n", x_pos, y_pos);
    return;
}


// calculate the left right foot coordinate x y position
void updateDownhillLeftRightxy(struct player* playerStruct){
    if(playerStruct->direction == 4){  // if it is downhill left
        playerStruct->x_left = playerStruct->x_pos - 14; 
        playerStruct->x_right = playerStruct->x_pos - 4;
        playerStruct->y_left = playerStruct->y_pos - 3;
        playerStruct->y_right = playerStruct->y_pos - 13;
    }
    else{  // if it is downhill right
        playerStruct->x_left = playerStruct->x_pos - 14; 
        playerStruct->x_right = playerStruct->x_pos - 4;
        playerStruct->y_left = playerStruct->y_pos - 3;
        playerStruct->y_right = playerStruct->y_pos - 13;
    }
    return;
}
// calculate the left right foot coordinate x y position
void updateGroundLeftRightxy(struct player* playerStruct){
    playerStruct->x_left = playerStruct->x_pos - 10;
    playerStruct->x_right = playerStruct->x_pos + 10;
    playerStruct->y_left = playerStruct->y_pos - 10 - 1;  // minus 1 for special use: the img should be above the ground while the foot should be in the ground for validation
    playerStruct->y_right = playerStruct->y_pos - 10 - 1;
    return;
}
// calculate the left right foot coordinate x y position
void updateUphillLeftRightxy(struct player* playerStruct){
    if(playerStruct->direction == 2){  // if it is uphill left
        playerStruct->x_left = playerStruct->x_pos + 3; 
        playerStruct->x_right = playerStruct->x_pos + 13;
        playerStruct->y_left = playerStruct->y_pos - 14;
        playerStruct->y_right = playerStruct->y_pos - 4;
    }
    else{  // if it is uphill right
        playerStruct->x_left = playerStruct->x_pos + 3; 
        playerStruct->x_right = playerStruct->x_pos + 13;
        playerStruct->y_left = playerStruct->y_pos - 14;
        playerStruct->y_right = playerStruct->y_pos - 4;
    }
    return;
}


// when the player transit to another position: e.g from ground position to uphill or downhill position, we should update his position
// choice: 0 --  ground -> right up to uphill;  1 -- ground -> left down to uphill; 2 -- air -> uphill
void convertCoordinateUpHill(struct player* playerStruct, int choice){
    int mid_x, mid_y;
    mid_x = (playerStruct->x_left + playerStruct->x_right)/2;
    mid_y = (playerStruct->y_left + playerStruct->y_right)/2;
    switch(choice){
        case 0: 
            playerStruct->x_pos = (mid_x - 1) - 8; // right position
            playerStruct->y_pos = mid_y + 9;
            break;
        case 1:
            playerStruct->x_pos = (mid_x + 1) - 8; // left position
            playerStruct->y_pos = mid_y + 9;
            break;
        case 2:
            playerStruct->x_pos = mid_x - 8;    // convert mid point to center point
            playerStruct->y_pos = mid_y + 9; 
            break;
    }
    updateUphillLeftRightxy(playerStruct);
    return;
}

// when the player transit to another position: e.g from ground position to uphill or downhill position, we should update his position
// choice: 0 --  ground -> right down to downhill;  1 -- ground -> left up to downhill; 2 -- air -> downhill
void convertCoordinateDownHill(struct player* playerStruct, int choice){
    int mid_x, mid_y;
    mid_x = (playerStruct->x_left + playerStruct->x_right)/2;
    mid_y = (playerStruct->y_left + playerStruct->y_right)/2;
    switch(choice){
        case 0: 
            playerStruct->x_pos = (mid_x - 1) + 9;    // convert mid point to center point
            playerStruct->y_pos = mid_y + 8; 
            break;
        case 1:
            playerStruct->x_pos = (mid_x + 1) + 9;    // convert mid point to center point
            playerStruct->y_pos = (mid_y) + 8; 
            break;
        case 2:
            playerStruct->x_pos = mid_x + 9;    // convert mid point to center point
            playerStruct->y_pos = mid_y + 8; 
            break;
    }
    updateDownhillLeftRightxy(playerStruct);
    return;
}

// when the player transit to another position: e.g from ground position to uphill or downhill position, we should update his position
// choice: 0 -- downhill -> left up to ground;  1 -- downhill -> right down to ground; 2 -- uphill -> right up to ground; 3 -- uphill -> left down to ground; 4 -- air -> ground
void convertCoordinateGround(struct player* playerStruct, int choice){
    int mid_x, mid_y;
    mid_x = (playerStruct->x_left + playerStruct->x_right)/2;
    mid_y = (playerStruct->y_left + playerStruct->y_right)/2;
    switch(choice){
        case 0: 
            playerStruct->x_pos = (mid_x + 1);    // convert mid point to center point
            playerStruct->y_pos = (mid_y - 1) + 11;
            break;
        case 1:
            playerStruct->x_pos = (mid_x - 1);    // convert mid point to center point
            playerStruct->y_pos = (mid_y + 1) + 11;
            break;
        case 2:
            playerStruct->x_pos = (mid_x - 1);    // convert mid point to center point
            playerStruct->y_pos = (mid_y - 1) + 11;
            break;
        case 3:
            playerStruct->x_pos = (mid_x + 1);    // convert mid point to center point
            playerStruct->y_pos = (mid_y + 1) + 11; 
            break;
        case 4:
            playerStruct->x_pos = mid_x;    // convert mid point to center point
            playerStruct->y_pos = mid_y + 11; 
            break;
    }
    updateGroundLeftRightxy(playerStruct);
    return;
}


