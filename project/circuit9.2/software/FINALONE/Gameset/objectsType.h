#ifndef OBJECTSTYPE_H_
#define OBJECTSTYPE_H_



// CONST
#define PI 3.1415926535



// GAME BASIC INFOR
#define GAME_WIDTH 800
#define GAME_HEIGHT 480
#define GROUND_WIDTH 800
#define GROUND_HEIGHT 240
#define GAME_LEFTEDGE 0
#define GAME_RIGHTEDGE 800
#define GAME_DOWNEDGE 0

// PLAYER BASIC INFOR
#define RED_PLAYER_X_INIT_POS 100
#define RED_PLAYER_Y_INIT_POS GROUND_HEIGHT+(PLAYER_HEIGHT/2)
#define BLUE_PLAYER_X_INIT_POS GAME_WIDTH-RED_PLAYER_X_INIT_POS -1
#define BLUE_PLAYER_Y_INIT_POS GROUND_HEIGHT+(PLAYER_HEIGHT/2)

#define PLAYER_WIDTH 21
#define PLAYER_HEIGHT 21
#define CANNON_SIZE 9
#define PLAYER_MAX_HEALTH 5
#define EXPLOSION_DURATION 10

#define PLAYER_STEP 4

#define MAX_STRENGTH 200


// KEYCODE INFOR

    // VISION MOVEMENT
#define KEY_W 0x1A
#define KEY_A 0x04
#define KEY_S 0x16
#define KEY_D 0x07
    // PLAYER MOVEMENT
#define KEY_RIGHT 0x4F
#define KEY_LEFT 0x50
    // CANNON OPERATION
#define KEY_SPACE 0x2C
#define KEY_DOWN 0x51
#define KEY_UP 0x52
    // GAME START
#define KEY_ENTER 0x28
    // GAME END
#define KEY_ESC 0x29

#define KEY_F 0x09


//CANNON BALL BASIC INFO
    // Charging Level

#define CHARGING_MAXIMUM_COUNT 20
#define CHARGING_COUNT_INTERVAL CHARGING_MAXIMUM_COUNT/4
#define CHARGING_LEVEL_1 5
#define CHARGING_LEVEL_2 10
#define CHARGING_LEVEL_3 15
#define CHARGING_LEVEL_4 20
#define CHARGING_LEVEL_5 25

    // Firing Angle

#define FIRING_ANGLE_MIN 20
#define FIRING_ANGLE_MAX 70
#define FIRING_ANGLE_INTERVAL 5 // The unit change of angle 

    // Basic Size
#define CANNONBALL_SIZE 5 // the size of the ball

// Gravity
#define GRAVITY 3
struct player {
    int x_pos;
    int y_pos;
    int x_prev;
    int y_prev;
    int state;  // 0: do nothing; 1: Charge attack
    int pos_state;//0: not in air 1: Air
    int health;
    int direction; // 0: face left; 1: face right; 2: uphill_left; 3: uphill_right; 4: downhill_left; 5: downhill_right
    int prev_dire;
    int x_left;
    int x_right;
    int y_left;
    int y_right;
    int v_y;
    int magic;
};

struct attackedGround {
    int validGround[GROUND_WIDTH][480]; // 20 for special use, to avoid unnecessary boarder cross
};

struct playerTurningState {
    int turn;  
    int next_player;   // who will fire next?
};


struct strengthBar {
    int strength;
};

struct game_State {
    int state; //0: game initial; 1: game started; 2: end game
};

struct cannon_Ball {
    int y_pos;
    int x_pos;
    int x_prev;
    int y_prev;
    int v_x;
    int v_y;
    int state; //0: not exist; 1: charging 2: flying; 3: exploded
    int explodingCount;
    int chargingTimeCount;
    int fired_v_x; // used to wait one iteration to update the value
    int fired_v_y; // used to wait one iteration to update the value
    int target; // 0: only cause harm to red player; 1: only cause harm to blue player
    int angle;
    int magic;
};

// static volatile struct gameState gameState;
// static volatile struct lifeBar lifeBar;
// static volatile struct attackedGround ground;
// static volatile struct player redPlayer;
// static volatile struct player bluePlayer;
// static volatile struct cannonBall cannonBall;
#endif
