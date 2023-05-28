#include "startInterface.h"


int moveX;
int cracterOriginal[100];
int cracterCounter;
int lastPress;


void gameInterface(int keycode){
    int game_state = gameState.state;
    switch (game_state) {
        case 0: // The game init state
            gameInitStateHandler(keycode);
            break;
        case 1: // The game has started
            gameStartStateHandler(keycode);
            break;
        case 2: // The game has ended
            gameEndStateHandler(keycode);
            break;

    }
    game_state = gameState.state;

    // update section
//    updatePlayer();
    
    

    // printf("%d sblyxx\n" , redPlayer.x_pos);
    // printf("%d sblyxgamestate\n" , gameState.state);
    // draw section
    // if((redPlayer.direction == 0) && (redPlayer.prev_dire == 0)){
    //     move_print(&redPlayer,red_player_left,left_shape,left_shape,red_player_left_buffer,red_player_left_buffer,trash);
    // }
    // else if((redPlayer.direction == 0) && (redPlayer.prev_dire == 1)){
    //     move_print(&redPlayer,red_player_left,left_shape,right_shape,red_player_left_buffer,red_player_right_buffer,trash);
    // }
    // else if((redPlayer.direction == 1) && (redPlayer.prev_dire == 0)){
    //     move_print(&redPlayer,red_player_right,right_shape,left_shape,red_player_right_buffer,red_player_left_buffer,trash);
    // }
    // else if((redPlayer.direction == 1) && (redPlayer.prev_dire == 1)){
    //     move_print(&redPlayer,red_player_right,right_shape,right_shape,red_player_right_buffer,red_player_right_buffer,trash);
    // }

    //  if((bluePlayer.direction == 0) && (bluePlayer.prev_dire == 0)){
    //     move_print(&redPlayer,blue_player_left,left_shape,left_shape,blue_player_left_buffer,blue_player_left_buffer,trash);
    // }
    // else if((bluePlayer.direction == 0) && (bluePlayer.prev_dire == 1)){
    //     move_print(&bluePlayer,blue_player_left,left_shape,right_shape,blue_player_left_buffer,blue_player_right_buffer,trash);
    // }
    // else if((bluePlayer.direction == 1) && (bluePlayer.prev_dire == 0)){
    //     move_print(&bluePlayer,blue_player_right,right_shape,left_shape,blue_player_right_buffer,blue_player_left_buffer,trash);
    // }
    // else if((bluePlayer.direction == 1) && (bluePlayer.prev_dire == 1)){
    //     move_print(&bluePlayer,blue_player_right,right_shape,right_shape,blue_player_right_buffer,blue_player_right_buffer,trash);
    // }
    


    // move_print(&redPlayer,red_player_left,left_shape,red_player_right_buffer,trash);
    // move_print(&bluePlayer,blue_player_right,right_shape,blue_player_right_buffer,trash);
    // cannon_move_print(&cannonBall, cannon_color, cannon_shape, cannon_buffer, cannonBall_trash);


//    drawBackground();
//    drawGround();
//    drawPlayer();
//    drawCannonBall();
//    drawExplosion();
//    drawLifeBar();
//    drawChargingBar();
//    drawAngleText();

    return;
}

void gameInitStateHandler(int keycode){
    if(keycode == KEY_ENTER){
        gameState.state = 1; 
    }
    IngameBuffer.counter = 0; // rebuld the lookup table;
    SetZeroArray(&IngameBuffer, 47); // 1 reserved
    PackArray(&IngameBuffer, 26, 320 + 12, 280, 0);
    PackArray(&IngameBuffer, 1 + 10, 320 - 80, 280, 10);
    PackArray(&IngameBuffer, 4, 320 + 80, 280, 10);
    memcpy((vga_ctrl->VRAM),(IngameBuffer.VRAM),sizeof(IngameBuffer.VRAM));
    return;
}

void gameStartStateHandler(int keycode){
    int now_player = (TurnState.next_player == 0) ? 1:0;
    if((lastPress == 1) && (keycode != KEY_F)){
        if(now_player == 0){
            redPlayer.magic = (redPlayer.magic == 0) ? 1:0;
        } 
        else{
            bluePlayer.magic = (bluePlayer.magic == 0) ? 1:0;
        }
        lastPress = 0;
    }

    //printf("%d Turn State 1 means freezing \n" , TurnState.turn);
    if(keycode == KEY_ESC){ // END GAME
        gameState.state = 2;
    }
    else{ 
        if(TurnState.turn != 1){ //When the mode is not freezing, the player can move
            if((keycode == KEY_DOWN) || (keycode ==KEY_UP)){  // When the player is inputed movement
                cannonBallAngle(keycode);
            }
            else if(((keycode == KEY_LEFT)|| (keycode == KEY_RIGHT)) && (playerstrength.strength != 0)){
               if(now_player == 0) updatedPlayerMovement(keycode, &redPlayer);
               else updatedPlayerMovement(keycode, &bluePlayer);
                //  playerMovement(keycode, now_player);
            }
            else if((keycode == KEY_A) || (keycode == KEY_D) || (keycode == KEY_S) || (keycode == KEY_W)){  // The player in turn is moving the camera
                if((keycode == KEY_A) && (moveX >= 5) ) moveX -= 5;
                if((keycode == KEY_D) && (moveX <= 155)) moveX += 5;
                updateMovementCracter(&IngameBuffer);
            } 
            else if(keycode == KEY_F){
                lastPress = 1;
            } 
            else if(keycode == KEY_SPACE){ // start charging
                cannonBallCharging();
            }
        }
        else{  // When the mode is freezing, and only action player can take is charging if ball state is charging. 
            //printf("%d CannonBall state \n" , cannonBall.state); 
            if(cannonBall.state == 1){  //the player has already been charging the cannon ball
                if(keycode == KEY_SPACE){  // KEEP CHARGING
                    cannonBallCharging(); // keep charge the cannonBall;
                }
                else{ // Other key is pressed, means charging end
                    if(now_player == 0) cannonBallFiring(TurnState.next_player, redPlayer.direction, redPlayer.x_pos, redPlayer.y_pos, &redPlayer); // fire the cannonBall
                    else cannonBallFiring(TurnState.next_player, bluePlayer.direction, bluePlayer.x_pos, bluePlayer.y_pos, &bluePlayer);
                }
            }
        }
    }
    if(now_player == 0) updateMagicMoving(&redPlayer);
    else updateMagicMoving(&bluePlayer);
    updateFalling(&bluePlayer);
    updateFalling(&redPlayer);
    if(bluePlayer.y_pos <= 0){
        bluePlayer.health = 0;
        gameState.state = 2;
    } 
    else if(redPlayer.y_pos <= 0){
        redPlayer.health = 0;
        gameState.state = 2;
    }
    updateCannonBall(); // update the position of cannonball in the list
    updatePlayerTurn();
    

    IngameBuffer.counter = 0; // rebuld the lookup table;
    // This is for test purpose
    // fillBufferPlayers();  // 2 objects
    // fillBufferCannon();   // 1 objects
    // fillBufferExplode();  // 1 objects

    // This is the correct one
    fillBufferArrow(now_player);    // 1 objects
    fillBufferAngle(now_player);    // 1 objects
    fillBufferWord();     // 3 objects
    SetZeroArray(&IngameBuffer, 1); // 1 reserved
    fillBufferStrength(now_player); // 10 objects
    fillBufferCharging(); // 10 objects
    SetZeroArray(&IngameBuffer, 1); // need to skip this 1 item;
    fillBufferFlying(now_player);
    SetZeroArray(&IngameBuffer, 8); // need to skip this 8 item;
    fillBufferHealth();   // 10 objects
    fillBufferExplode();  // 1 objects
    fillBufferCannon();   // 1 objects
    fillBufferPlayers();  // 2 objects
    
    endingFill(&IngameBuffer);
  
    return;
}

void updateMagicMoving(struct player* playerStruct){
    if((playerStruct->magic == 1) && (cannonBall.state == 2)){
        playerStruct->x_pos = cannonBall.x_pos;
        playerStruct->y_pos = cannonBall.y_pos;
    }
    return;
}


void updateRedPlayerCamera(){
    if(redPlayer.x_pos + 320 > 800){
        moveX = 800-640;
    }
    else if(redPlayer.x_pos - 320 < 0){
        moveX = 0;
    }
    else moveX = redPlayer.x_pos - 320;
    updateMovementCracter(&IngameBuffer);
    return;
}

void updateBluePlayerCamera(){
    if(bluePlayer.x_pos + 320 > 800){
        moveX = 800-640;
    }
    else if(bluePlayer.x_pos - 320 < 0){
        moveX = 0;
    }
    else moveX = bluePlayer.x_pos - 320;
    updateMovementCracter(&IngameBuffer);
    return;
}


void gameEndStateHandler(int keycode){
    if(keycode == KEY_ENTER){
        gameState.state = 0; 
        gameInit();
    }
    else{
        IngameBuffer.counter = 0; // rebuld the lookup table;
        SetZeroArray(&IngameBuffer, 46); // 1 reserved
        if(redPlayer.health == 0) PackArray(&IngameBuffer, 3, 320 - 80, 280, 40);
        else PackArray(&IngameBuffer, 3, 320 + 80, 280, 40);
        PackArray(&IngameBuffer, 26, 320 + 12, 280, 0);
        if(redPlayer.health == 0) {
            SetZeroArray(&IngameBuffer, 1); // red none
            PackArray(&IngameBuffer, 4, 320 + 80, 280, 10); // blue exist
        }
        else{
            PackArray(&IngameBuffer, 1 + 10, 320 - 80, 280, 10);
            SetZeroArray(&IngameBuffer, 1); // red none
        }
        memcpy((vga_ctrl->VRAM),(IngameBuffer.VRAM),sizeof(IngameBuffer.VRAM));
    }
    
    return;
}


void gameInit(){
    gameStateInit();
    playersInit();
    strengthBarInit();
	ground_init();
	playerTurnInit();
	cannonBallInit();
    moveX = 0;
    for(int i =0; i < 100; i++) cracterOriginal[i] = 0;
    cracterCounter = 0;
    IngameBuffer.counter = 0;
    for(int i = 4*50; i < 4*80; i++){
    	IngameBuffer.VRAM[i] = 0;
    }
    InitialgameBuffer.counter = 0;
    for(int i = 4*50; i < 4*80; i++){
		InitialgameBuffer.VRAM[i] = 0;
	}
    IngameBuffer.crater_counter = 0;
    InitialgameBuffer.crater_counter = 0;
    
    return;
}

void playerTurnInit(){
    TurnState.turn = 0; // start with player A
    TurnState.next_player = 1; // start with 0
}
void updatePlayerTurn(){
    int now_player;
    if(cannonBall.state != 0) TurnState.turn = 1; // freezing state, player cannot be controlled to move right or left.
    else if((TurnState.turn == 1) && (bluePlayer.pos_state != 1) && (redPlayer.pos_state != 1)){ // the cannonBall has ended exploding and two players are on the ground
        strengthBarInit();
        TurnState.turn = (TurnState.next_player == 1) ? 2:0;
        TurnState.next_player = (TurnState.next_player == 1) ? 0:1;
        now_player = (TurnState.next_player == 0) ? 1:0;
        redPlayer.magic = 0;
        bluePlayer.magic = 0;
        if(now_player == 0) {
            updateRedPlayerCamera();
            
        }
        else if(now_player == 1) {
            updateBluePlayerCamera();
            
        }
    }
    return;
}

void updateCannonBall(){
    if(cannonBall.state == 2){ // flying mode
    	cannonBall.x_pos += cannonBall.v_x;  // update the position
		cannonBall.y_pos += cannonBall.v_y;
		cannonBall.v_y -= GRAVITY;
		// if((cannonBall.fired_v_x) || (cannonBall.fired_v_y)){  // If the cannon is just fired
		//     cannonBall.v_x = cannonBall.fired_v_x;
		//     cannonBall.v_y = cannonBall.fired_v_y;
		//     cannonBall.fired_v_x = 0;
		//     cannonBall.fired_v_y = 0;
		// }
		// boundary judgement
		if(isLegalCannonBall() == 0){ // not legal
            if(cannonBall.magic == 1){
                if(cannonBall.target == 1){
                    redPlayer.magic = 0;
                    redPlayer.x_pos = redPlayer.x_prev;  // let him go back to the previous position
                    redPlayer.y_pos = redPlayer.y_prev;
                }
                else{
                    bluePlayer.magic = 0;
                    bluePlayer.x_pos = bluePlayer.x_prev;
                    bluePlayer.y_pos = bluePlayer.y_prev;
                }
            }
			cannonBallInit(); // erase the cannonball
		}
        else if(cannonBall.y_pos <= 239){
            if(ground.validGround[cannonBall.x_pos][cannonBall.y_pos] == 1){ // touch the valid ground, turn into explode
                
                if(cannonBall.magic == 1){
                    if(cannonBall.target == 1){
                        redPlayer.magic = 0;
                        redPlayer.x_pos = cannonBall.x_pos - cannonBall.v_x;  // let him go back to the previous position
                        redPlayer.y_pos = cannonBall.y_pos - cannonBall.v_y - GRAVITY;
                        if(ground.validGround[redPlayer.x_pos][redPlayer.y_pos + 12] == 0) redPlayer.y_pos += 12;
                        // if(cannonBall.v_y + GRAVITY >= 0) redPlayer.y_pos = cannonBall.y_pos;
                        // else redPlayer.y_pos = cannonBall.y_pos + 20;
                        redPlayer.pos_state = 1;
                    }
                    else{
                        bluePlayer.magic = 0;
                        // if(cannonBall.v_y + GRAVITY >= 0) bluePlayer.y_pos = cannonBall.y_pos;
                        // else bluePlayer.y_pos = cannonBall.y_pos + 20;
                        bluePlayer.x_pos = cannonBall.x_pos - cannonBall.v_x;  // let him go back to the previous position
                        bluePlayer.y_pos = cannonBall.y_pos - cannonBall.v_y - GRAVITY;
                        if(ground.validGround[bluePlayer.x_pos][bluePlayer.y_pos + 12] == 0) bluePlayer.y_pos += 12;
                        bluePlayer.pos_state = 1;
                    }
                    cannonBallInit();
                    return;
                }
                findexplosionPoint();
                cannonBall.state = 3;
                explodingHarm();
                attackingGround(cannonBall.x_pos, cannonBall.y_pos); // firstly assume that the exploding point is the pos of cannonball
                //printf("The crater position: x: %d; y: %d\n", cannonBall.x_pos, cannonBall.y_pos);
                cracterOriginal[cracterCounter] = cannonBall.x_pos;
                cracterCounter += 1; 
                fillBufferCrater(cannonBall.x_pos, cannonBall.y_pos);   // a new crater is formed fill as many as possible 
                
            }
        }

    }

    else if(cannonBall.state == 3){ // exploding mode
        if(cannonBall.explodingCount <= 0){ // has ended exploding
            cannonBallInit(); // erase the cannonball
        }
        else cannonBall.explodingCount -= 1; // count exploding
    }

    return;
}

void explodingHarm(){
    int player_x, player_y;
    int healthDec = 0;
    if(cannonBall.target == 0){
        player_x = redPlayer.x_pos;
        player_y = redPlayer.y_pos;
    }
    else{
        player_x = bluePlayer.x_pos;
        player_y = bluePlayer.y_pos;
    }
    if(((cannonBall.x_pos-player_x)*(cannonBall.x_pos-player_x) + (cannonBall.y_pos-player_y)*(cannonBall.y_pos-player_y)) <= (40)*(40)){ // IF THE PLAYER IS WITHIN THE RANGE
        healthDec = 1;
    }

    if(cannonBall.target == 0){
        redPlayer.health -= healthDec;
        if(redPlayer.health == 0) gameState.state = 2;
    }
    else{
        bluePlayer.health -= healthDec;
        if(bluePlayer.health == 0) gameState.state = 2;
    }
    
    return;
}

void attackingGround(int cent_x, int cent_y){
    if((cent_x<0)||(cent_x> 800) || (cent_y< 0) || (cent_y> 480) ){
		return;
	}
	int left = (cent_x < 40)?0:(cent_x - 40);
	int right =(cent_x > 760)?800: (cent_x+ 40);
    //draw the higher side of the explosion
	for ( int y = cent_y; y < cent_y + 25; y++){
		if (y > 240){
			break;
		}
		for (int x = left; x < right; x++){
			// value = pixel_color[(480 - y - 1 )*320 + (x/2)] ;
			// if (x%2){
			// 	value = (value & 0x0f) | 0x90;
			// }
			// else{
			// 	value = (value & 0xf0) | 0x09;
			// }
			// pixel_color[(480-y-1)*320 + (x/2)] = value;
			ground.validGround[x][y] = 0; 
			//ground[y*640 + x] = 0;

		}
		left = (left == 0)? 0:(left + 1);
		right = (right == 800)? 800:(right - 1);
	}
    
    left = (cent_x < 40)?0:(cent_x - 40);
	right =(cent_x > 760)?800: (cent_x+ 40);
    // draw the lower side of the explosion 
    for ( int y = cent_y; y > cent_y - 25; y--){
		if (y < 0){
			break;
		}
		for (int x = left; x < right; x++){
			// value = pixel_color[(480 - y - 1 )*320 + (x/2)] ;
			// if (x%2){
			// 	value = (value & 0x0f) | 0x90;
			// }
			// else{
			// 	value = (value & 0xf0) | 0x09;
			// }
			// pixel_color[(480-y-1)*320 + (x/2)] = value;
			ground.validGround[x][y] = 0; 
			//ground[y*640 + x] = 0;

		}
		left = (left == 0)? 0:(left +  1);
		right = (right == 800)? 800:(right - 1);
	}
    return;
}


void fillBufferPlayers(){  // 2 objects
    // red player:
    if((redPlayer.direction == 0) || (redPlayer.direction == 1)) PackArray(&IngameBuffer, redPlayer.direction + 10, redPlayer.x_pos - moveX, redPlayer.y_pos, 10);
    else PackArray(&IngameBuffer, redPlayer.direction + 10, redPlayer.x_pos - moveX, redPlayer.y_pos, 16);
    // blue player:
    if((bluePlayer.direction == 0) || (bluePlayer.direction == 1)) PackArray(&IngameBuffer, bluePlayer.direction + 4, bluePlayer.x_pos - moveX, bluePlayer.y_pos, 10);
    else PackArray(&IngameBuffer, bluePlayer.direction + 4, bluePlayer.x_pos - moveX, bluePlayer.y_pos, 16);
    return;
}
void fillBufferCannon(){   // 1 objects'
    if((cannonBall.state == 2) && (cannonBall.magic != 1)){ // when it is flying in the air
        PackArray(&IngameBuffer, 2, cannonBall.x_pos - moveX, cannonBall.y_pos, 3);
    }
    else SetZeroArray(&IngameBuffer, 1); // need to skip this item;
    return;
}

void fillBufferExplode(){  // 1 objects
    if(cannonBall.state == 3){ // when it is exploding in the air
        PackArray(&IngameBuffer, 3, cannonBall.x_pos - moveX, cannonBall.y_pos, 40);
    }
    else SetZeroArray(&IngameBuffer, 1); // need to skip this item;
    return;
}

void fillBufferHealth(){   // 10 objects
    // PackArray(&IngameBuffer, 12, cannonBall.x_pos, cannonBall.y_pos);
    for(int i = 0; i < redPlayer.health; i++){
        PackArray(&IngameBuffer, 17, 30 + 20 + i*30, 460, 0); 
    }
    for(int i = redPlayer.health; i < 5; i++){
        PackArray(&IngameBuffer, 18, 30 + 20 + i*30, 460, 0); 
    }

    for(int j = 0; j < bluePlayer.health; j++){
        PackArray(&IngameBuffer, 17, 30 + 470 + j*30, 460, 0); 
    }
    for(int j = bluePlayer.health; j < 5; j++){
        PackArray(&IngameBuffer, 18, 30 + 470 + j*30, 460, 0); 
    }

    // SetZeroArray(&IngameBuffer, 10); // need to skip this item;
    return;
}

void fillBufferAngle(int player){    // 1 objects
    int chargingTable[5] = {4*CHARGING_LEVEL_1+10, 4*CHARGING_LEVEL_2+10, 4*CHARGING_LEVEL_3+10, 4*CHARGING_LEVEL_4+10, 4*CHARGING_LEVEL_5+10};
    int intensity, posture, calX, calY;
    double convertedAngle;
    if(cannonBall.state == 0) intensity = 30;
    else intensity = chargingTable[cannonBall.chargingTimeCount / (CHARGING_COUNT_INTERVAL)];

    if(player == 0) posture = redPlayer.direction;
    else posture = bluePlayer.direction;
    switch(posture){
        case 0: // face left
            convertedAngle = cannonBall.angle*PI/180;
            // cannonBall.fired_v_x = -intensity*cos(convertedAngle);
            // cannonBall.fired_v_y = intensity*sin(convertedAngle);
            calX = -intensity*cos(convertedAngle);
            calY = intensity*sin(convertedAngle);
            break;
        case 1: // face right
            convertedAngle = cannonBall.angle*PI/180;
            // cannonBall.fired_v_x = intensity*cos(convertedAngle);
            // cannonBall.fired_v_y = intensity*sin(convertedAngle);
            calX = intensity*cos(convertedAngle);
            calY = intensity*sin(convertedAngle);
            break;
        case 2: // uphill left
            convertedAngle = (cannonBall.angle - 30)*PI/180;
            // cannonBall.fired_v_x = -intensity*cos(convertedAngle);
            // cannonBall.fired_v_y = intensity*sin(convertedAngle);
            calX = -intensity*cos(convertedAngle);
            calY = intensity*sin(convertedAngle);
            break;
        case 3: // uphill right
            convertedAngle = (cannonBall.angle + 30)*PI/180;
            // cannonBall.fired_v_x = intensity*cos(convertedAngle);
            // cannonBall.fired_v_y = intensity*sin(convertedAngle);
            calX = intensity*cos(convertedAngle);
            calY = intensity*sin(convertedAngle);
            break;
        case 4: // downhill left
            convertedAngle = (cannonBall.angle + 30)*PI/180;
            // cannonBall.fired_v_x = -intensity*cos(convertedAngle);
            // cannonBall.fired_v_y = intensity*sin(convertedAngle);
            calX = -intensity*cos(convertedAngle);
            calY = intensity*sin(convertedAngle);
            break;
        case 5: // downhill right
            convertedAngle = (cannonBall.angle - 30)*PI/180;
            // cannonBall.fired_v_x = intensity*cos(convertedAngle);
            // cannonBall.fired_v_y = intensity*sin(convertedAngle);
            calX = intensity*cos(convertedAngle);
            calY = intensity*sin(convertedAngle);
            break;
    }
        // pay attention!!! The coordinate here is the center coordinate of the player
    if(player == 0) {
        PackArray(&IngameBuffer, 23, redPlayer.x_pos + calX - moveX, redPlayer.y_pos + calY, 5);
    }
    else {
        PackArray(&IngameBuffer, 23, bluePlayer.x_pos + calX - moveX, bluePlayer.y_pos + calY, 5);
    }
    return;
}

void fillBufferCharging(){ // 20 objects   --- in real only 10 used
    int count;
    count = cannonBall.chargingTimeCount/2; // how many number out of 10 can it fill;
    for( int i = 0; i < count; i++){
        PackArray(&IngameBuffer, 19, 25 + 195 + i*25, 70, 0); // start with the filled one
    }
    for( int j = count; j < 10; j++){
        PackArray(&IngameBuffer, 20, 25 + 195 + j*25, 70, 0); // start with the filled one
    }
    
    return;
}

void fillBufferStrength(int player){ // 10 objects
    int offset, count;
    offset = 0;
    count = playerstrength.strength/20;
    if(player == 1) offset = 568; 
    for(int i = 0; i < count; i++){
        PackArray(&IngameBuffer, 21, 32 + 20 + offset, 56 + 16*i, 0); // start with the filled one
    }
    for(int j = count; j < 10; j++){
        PackArray(&IngameBuffer, 22, 32 + 20 + offset, 56 + 16*j, 0); 
    }
    

    // SetZeroArray(&IngameBuffer, 10); // need to skip this item;
    return;
}

void fillBufferFlying(int player){
    if(player == 0){
        if(redPlayer.magic == 1) PackArray(&IngameBuffer, 27, 188, 458, 0); // start with the filled one   188
        else SetZeroArray(&IngameBuffer, 1); // 1 reserved
    } 
    else{
        if(bluePlayer.magic == 1) PackArray(&IngameBuffer, 27, 470, 461, 0);
        else SetZeroArray(&IngameBuffer, 1); // 1 reserved 
    }
    return;
}

void fillBufferWord(){     // 3 objects
    SetZeroArray(&IngameBuffer, 3); // need to skip this item;
    return;
}

void fillBufferArrow(int player){    // 1 objects
    if(player == 1)  PackArray(&IngameBuffer, 16, bluePlayer.x_pos - moveX, bluePlayer.y_pos + 19, 4); 
    else PackArray(&IngameBuffer, 16, redPlayer.x_pos - moveX, redPlayer.y_pos + 19, 4); 
    return;
}

void fillBufferCrater(int x, int y){   // fill as many as possible
    PackArrayCrater(&IngameBuffer, 1, cannonBall.x_pos - moveX, cannonBall.y_pos);
    return;
}








// player Movement Part
void updatedPlayerMovement(int keycode,  struct player* playerStruct){
    if(playerStruct->pos_state == 1){ // the player is in the air, he cannot move but falling to the gournd
    }
    else{
        switch ((playerStruct->direction)/2) {  // update the position
            case 0: // face left and face right
                if(keycode == KEY_LEFT) {
                	if((playerStruct->x_pos -1)==10) return;
                    playerstrength.strength -= 1;
                    playerStruct->x_pos -= 1;
                    playerStruct->direction = 0;
                    updateGroundLeftRightxy(playerStruct); // update the left right coordinate 
                    if( (ground.validGround[(playerStruct->x_left + playerStruct->x_right)/2][(playerStruct->y_left + playerStruct->y_right)/2] == 0)){ // right foot is solid, half not)
                        playerStruct->direction = 2;
                        convertCoordinateUpHill(playerStruct, 1);
                    }
                    else if( (ground.validGround[(playerStruct->x_left + playerStruct->x_right)/2][(playerStruct->y_left + playerStruct->y_right)/2 + 1] == 1)){ // left foot is solid half is not
                        playerStruct->direction = 4; // down hill left
                        convertCoordinateDownHill(playerStruct, 1);
                    }
                    else{
                    }
                }
                else {
                	if((playerStruct->x_pos + 1) >= 799) return;
                    playerstrength.strength -= 1;
                    playerStruct->x_pos += 1;
                    playerStruct->direction = 1;
                    updateGroundLeftRightxy(playerStruct); // update the left right coordinate 
                    if((ground.validGround[(playerStruct->x_left + playerStruct->x_right)/2][(playerStruct->y_left + playerStruct->y_right)/2] == 0)){ // left foot is solid, half not
                        playerStruct->direction = 5;
                        convertCoordinateDownHill(playerStruct, 0);
                    }
                    else if( (ground.validGround[(playerStruct->x_left + playerStruct->x_right)/2][(playerStruct->y_left + playerStruct->y_right)/2 + 1] == 1)){ // left foot is solid half is not
                        playerStruct->direction = 3; // up hill left
                        convertCoordinateUpHill(playerStruct, 0);
                    }
                    else{
                    }
                }
                break;
            case 1: //up hill  
                if(keycode == KEY_LEFT) {
                    playerstrength.strength -= 1;
                    playerStruct->x_pos -= 1;
                    playerStruct->y_pos -= 1;
                    playerStruct->direction = 2;
                    updateUphillLeftRightxy(playerStruct); // update the left right coordinate
                    if( (ground.validGround[(playerStruct->x_left + playerStruct->x_right)/2][(playerStruct->y_left + playerStruct->y_right)/2 + 1] == 1)){ // left foot is solid, half is not
                        playerStruct->direction = 0;
                        convertCoordinateGround(playerStruct, 3);    
                    }
                    else{
                    }
                }
                else {
                    if(ground.validGround[playerStruct->x_right][playerStruct->y_right+1] != 1){
                        playerstrength.strength -= 1;
                        playerStruct->x_pos += 1;
                        playerStruct->y_pos += 1;
                        playerStruct->direction = 3;
                    }
                    updateUphillLeftRightxy(playerStruct); // update the left right coordinate
                    if((ground.validGround[(playerStruct->x_left + playerStruct->x_right)/2][(playerStruct->y_left + playerStruct->y_right)/2] == 0)){ // left foot still on the slope, half already in the air;
                        playerStruct->direction = 1;
                        convertCoordinateGround(playerStruct, 2);
                    }
                    
                }
                break;
            case 2: // downhill
                if(keycode == KEY_LEFT) {
                    if(ground.validGround[playerStruct->x_left][playerStruct->y_left+1] != 1){
                        playerstrength.strength -= 1;
                        playerStruct->x_pos -= 1;
                        playerStruct->y_pos += 1;
                        playerStruct->direction = 4;
                    }
                    updateDownhillLeftRightxy(playerStruct); // update the left right coordinate 
                    if((ground.validGround[(playerStruct->x_left + playerStruct->x_right)/2][(playerStruct->y_left + playerStruct->y_right)/2] == 0)){ // right foot on the slope, half in the air
                        playerStruct->direction = 0;
                        convertCoordinateGround(playerStruct, 0);
                    }
                    
                }
                else {
                    playerstrength.strength -= 1;
                    playerStruct->x_pos += 1;
                    playerStruct->y_pos -= 1;
                    playerStruct->direction = 5;
                    updateDownhillLeftRightxy(playerStruct); // update the left right coordinate 
                    if((ground.validGround[(playerStruct->x_left + playerStruct->x_right)/2][(playerStruct->y_left + playerStruct->y_right)/2 + 1] == 1)){ // left on the slope, half in the ground
                        playerStruct->direction = 1;
                        convertCoordinateGround(playerStruct, 1);
                    }
                    else{
                    }
                }
                break;
        }
    }    

    int faceDirection;
    if((playerStruct->direction == 0) || (playerStruct->direction == 2) || (playerStruct->direction == 4)){ // face left
        faceDirection = 0;   // face left
    }
    else faceDirection = 1;  // face right;

    return;
}

void updateMovementCracter(struct TEXT_VGA_STRUCT* buffer){
    int x_cal;
    int x_lowTwo;
    int x_high;
    int y_high;
    
    for(int i = 0; i < buffer->crater_counter;){
        y_high = buffer->VRAM[CRATER_OFFSET + i + 1] & 0x3f;
        // x_lowTwo = (buffer->VRAM[CRATER_OFFSET + i + 1] >> 6) & 0x03;
        // x_high = buffer->VRAM[CRATER_OFFSET + i + 2];
        x_cal = cracterOriginal[i/4] - moveX + 40;
        buffer->VRAM[CRATER_OFFSET + i + 1] = y_high | ((x_cal << 6) & 0xc0);
        buffer->VRAM[CRATER_OFFSET + i + 2] = (x_cal >> 2) & 0xff;
        i += 4;
    }
}
// void updatedPlayerMovement(int keycode,  struct player* playerStruct){

//     if(playerStruct->pos_state == 1){ // the player is in the air, he cannot move but falling to the gournd
//     }
//     else{
//         switch ((playerStruct->direction)/2) {  // update the position
//             case 0: // face left and face right
//                 if(keycode == KEY_LEFT) {
//                     playerStruct->x_pos -= 1;
//                     playerStruct->direction = 0;
//                     updateGroundLeftRightxy(playerStruct); // update the left right coordinate 
//                     if((ground.validGround[playerStruct->x_right][playerStruct->y_right] == 1) && (ground.validGround[(playerStruct->x_left + playerStruct->x_right)/2][(playerStruct->y_left + playerStruct->y_right)/2] == 0)){ // right foot is solid, half not)
//                         playerStruct->direction = 2 ;
//                         convertCoordinateUpHill(playerStruct, 1);
//                     }
//                     else if((ground.validGround[playerStruct->x_right][playerStruct->y_right + 1] == 0) && (ground.validGround[(playerStruct->x_left + playerStruct->x_right)/2][(playerStruct->y_left + playerStruct->y_right)/2 + 1] == 1)){ // left foot is solid half is not
//                         playerStruct->direction = 4; // down hill left
//                         convertCoordinateDownHill(playerStruct, 1);
//                     }
//                     else{
//                     }
//                 }
//                 else {
//                     playerStruct->x_pos += 1;
//                     playerStruct->direction = 1;
//                     updateGroundLeftRightxy(playerStruct); // update the left right coordinate 
//                     if((ground.validGround[playerStruct->x_left][playerStruct->y_left] == 1) && (ground.validGround[(playerStruct->x_left + playerStruct->x_right)/2][(playerStruct->y_left + playerStruct->y_right)/2] == 0)){ // left foot is solid, half not
//                         playerStruct->direction = 5;
//                         convertCoordinateDownHill(playerStruct, 0);
//                     }
//                     else if((ground.validGround[playerStruct->x_left][playerStruct->y_left + 1] == 0) && (ground.validGround[(playerStruct->x_left + playerStruct->x_right)/2][(playerStruct->y_left + playerStruct->y_right)/2 + 1] == 1)){ // left foot is solid half is not
//                         playerStruct->direction = 3; // up hill left
//                         convertCoordinateUpHill(playerStruct, 0);
//                     }
//                     else{
//                     }
//                 }
//                 break;
//             case 1: //up hill  
//                 if(keycode == KEY_LEFT) {
//                     playerStruct->x_pos -= 1;
//                     playerStruct->y_pos -= 1;
//                     playerStruct->direction = 2;
//                     updateUphillLeftRightxy(playerStruct); // update the left right coordinate
//                     if((ground.validGround[playerStruct->x_right][playerStruct->y_right + 1] == 0) && (ground.validGround[(playerStruct->x_left + playerStruct->x_right)/2][(playerStruct->y_left + playerStruct->y_right)/2 + 1] == 1)){ // left foot is solid, half is not
//                         playerStruct->direction = 0;
//                         convertCoordinateGround(playerStruct, 3);    
//                     }
//                     else{
//                     }
//                 }
//                 else {
//                     if(ground.validGround[playerStruct->x_right][playerStruct->y_right+1] != 1){
//                         playerStruct->x_pos += 1;
//                         playerStruct->y_pos += 1;
//                         playerStruct->direction = 3;
//                     }
//                     updateUphillLeftRightxy(playerStruct); // update the left right coordinate
//                     if((ground.validGround[playerStruct->x_left][playerStruct->y_left] == 1) && (ground.validGround[(playerStruct->x_left + playerStruct->x_right)/2][(playerStruct->y_left + playerStruct->y_right)/2] == 0)){ // left foot still on the slope, half already in the air;
//                         playerStruct->direction = 1;
//                         convertCoordinateGround(playerStruct, 2);
//                     }
                    
//                 }
//                 break;
//             case 2: // downhill
//                 if(keycode == KEY_LEFT) {
//                     if(ground.validGround[playerStruct->x_left][playerStruct->y_left+1] != 1){
//                         playerStruct->x_pos -= 1;
//                         playerStruct->y_pos += 1;
//                         playerStruct->direction = 4;
//                     }
//                     updateDownhillLeftRightxy(playerStruct); // update the left right coordinate 
//                     if((ground.validGround[playerStruct->x_right][playerStruct->y_right] == 1) && (ground.validGround[(playerStruct->x_left + playerStruct->x_right)/2][(playerStruct->y_left + playerStruct->y_right)/2] == 0)){ // right foot on the slope, half in the air
//                         playerStruct->direction = 0;
//                         convertCoordinateGround(playerStruct, 0);
//                     }
                    
//                 }
//                 else {
//                     playerStruct->x_pos += 1;
//                     playerStruct->y_pos -= 1;
//                     playerStruct->direction = 5;
//                     updateDownhillLeftRightxy(playerStruct); // update the left right coordinate 
//                     if((ground.validGround[playerStruct->x_left][playerStruct->y_left + 1] == 0) && (ground.validGround[(playerStruct->x_left + playerStruct->x_right)/2][(playerStruct->y_left + playerStruct->y_right)/2 + 1] == 1)){ // left on the slope, half in the ground
//                         playerStruct->direction = 1;
//                         convertCoordinateGround(playerStruct, 1);
//                     }
//                     else{
//                     }
//                 }
//                 break;
//         }
//     }    

//     int faceDirection;
//     if((playerStruct->direction == 0) || (playerStruct->direction == 2) || (playerStruct->direction == 4)){ // face left
//         faceDirection = 0;   // face left
//     }
//     else faceDirection = 1;  // face right;

//     return;
// }

void updateFalling(struct player* playerStruct){
    if(playerStruct->magic == 1){
        return;
    }
    if(playerStruct->pos_state == 1){
        playerStruct->y_pos -= playerStruct->v_y;
        playerStruct->v_y += 1;

		updateGroundLeftRightxy(playerStruct); // update the left right coordinate
		int fallyleft, fallymid, fallyright, fallxleft, fallxmid, fallxright;
		fallyleft = playerStruct->y_left;
		fallyright = playerStruct->y_right;
		fallymid = (fallyleft + fallyright)/2;
		fallxleft = playerStruct->x_left;
		fallxright = playerStruct->x_right;
		fallxmid = (fallxleft + fallxright)/2;

		if((ground.validGround[fallxmid][fallymid] == 1) && ( (ground.validGround[fallxmid][fallymid] == 1) || (ground.validGround[fallxright][fallyright] == 1) )){ // have touch the ground
            updatefallingPoint(playerStruct);
        
        }


		if((ground.validGround[fallxleft][fallyleft] == 1 ) && (ground.validGround[fallxmid][fallymid] == 1) && (ground.validGround[fallxright][fallyright] == 0)){ // The left and the mid side both touching ground downHill position
			convertCoordinateDownHill(playerStruct, 2);
			playerStruct->pos_state = 0; // update the status to be not in air;
            playerStruct->v_y = 0;
			if(playerStruct->direction == 0) playerStruct->direction = 4; // if face left, update to downhill left;
			else playerStruct->direction = 5; // otherwise update to downhill right;
		}
		else if((ground.validGround[fallxleft][fallyleft] == 0) && (ground.validGround[fallxmid][fallymid] == 1) && (ground.validGround[fallxright][fallyright] == 1)){  // The right and the mid side both touching ground upHill position
			convertCoordinateUpHill(playerStruct, 2);
			playerStruct->pos_state = 0; // update the status to be not in air;
            playerStruct->v_y = 0;
			if(playerStruct->direction == 0) playerStruct->direction = 2; // if face left, update to uphill left;
			else playerStruct->direction = 3; // otherwise update to uphill right;
		}
		else if((ground.validGround[fallxleft][fallyleft] == 1) && (ground.validGround[fallxmid][fallymid] == 1) && (ground.validGround[fallxright][fallyright] == 1)){ // All the part has touched the ground ground position
			convertCoordinateGround(playerStruct, 4);
			playerStruct->pos_state = 0; // update the status to be not in air;
            playerStruct->v_y = 0;
			if(playerStruct->direction == 0) playerStruct->direction = 0; // if face left, update to face left;
			else playerStruct->direction = 1; // otherwise update to face right;
		}
    }

    if((ground.validGround[playerStruct->x_left][playerStruct->y_left] == 0) && (ground.validGround[playerStruct->x_right][playerStruct->y_right] == 0) && (ground.validGround[(playerStruct->x_left + playerStruct->x_right)/2][(playerStruct->y_left + playerStruct->y_right)/2] == 0)){   // there are space under the feet
        playerStruct->pos_state = 1;
        if((playerStruct->direction == 0) || (playerStruct->direction == 2) || (playerStruct->direction == 4)){ // face left
            playerStruct->direction = 0;   // face left
        }
        else playerStruct->direction = 1;  // face right;
	}
    return;
} 


void updatefallingPoint(struct player* playerStruct){
    int fally = playerStruct->y_pos + playerStruct->v_y - 1;
    int cury = playerStruct->y_pos;
    int fallyleft, fallymid, fallyright, fallxleft, fallxmid, fallxright;
    for(int deltaY = fally; deltaY >= cury; deltaY--){
        playerStruct->y_pos = deltaY;
        updateGroundLeftRightxy(playerStruct); // update the left right coordinate
        fallyleft = playerStruct->y_left;
        fallyright = playerStruct->y_right;
        fallymid = (fallyleft + fallyright)/2;
        fallxleft = playerStruct->x_left;
        fallxright = playerStruct->x_right;
        fallxmid = (fallxleft + fallxright)/2;
        if((ground.validGround[fallxmid][fallymid] == 1) && ( (ground.validGround[fallxmid][fallymid] == 1) || (ground.validGround[fallxright][fallyright] == 1) )){ // have touch the ground
            break;
        }
    }
    return;
}





// binary search the explosion point.
// void findexplosionPoint(){
//     int air_x, air_y, ground_x, ground_y, mid_x, mid_y;
//     air_x = cannonBall.x_pos - cannonBall.v_x;
//     air_y = cannonBall.y_pos - cannonBall.v_y + GRAVITY;
//     ground_x = cannonBall.x_pos;
//     ground_y = cannonBall.y_pos;
//     while(1){
//         mid_x = (ground_x + air_x)/2;
//         mid_y = (ground_y + air_y)/2;
//         if(((mid_x == ground_x) && (mid_y == ground_y)) || ((mid_x == air_x) && (mid_y == air_y))){
//             mid_x = ground_x;
//             mid_y = ground_y;
//             break;
//         } 
//         if(ground.validGround[mid_x][mid_y] == 0){
//             air_x = mid_x;
//             air_y = mid_y;
//         }
//         else{
//             ground_x = mid_x;
//             ground_y = mid_y;
//         }
//     }

// 	cannonBall.x_pos = mid_x;
// 	cannonBall.y_pos = mid_y;
// 	return;
// }


void findexplosionPoint(){
    int air_x, air_y, ground_x, ground_y, mid_x, mid_y;
    air_x = cannonBall.x_pos - cannonBall.v_x;
    air_y = cannonBall.y_pos - cannonBall.v_y + GRAVITY;
    
    ground_x = cannonBall.x_pos;
    ground_y = cannonBall.y_pos;
    // printf("Air x %d, Air y %d \n", air_x, air_y);
    // printf("Ground x %d, Ground y %d \n", ground_x, ground_y);
    double slope;
    slope = (air_x - ground_x) / (air_y - ground_y);
    int delta_y, delta_x;
    delta_y = ((air_y - ground_y) >= 0) ? (air_y -ground_y) : (ground_y - air_y);
    for( int y = 0; y <= delta_y; y++){
        delta_x = y * slope;
        // printf("The checked x %d, check y %d \n", (air_x - delta_x), (air_y - y));
        if((ground.validGround[air_x - delta_x][air_y - y]) == 1){
            cannonBall.x_pos = air_x - delta_x;
	        cannonBall.y_pos = air_y - y;
            break;
        }
    }
	return;
}
