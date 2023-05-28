#ifndef DRAWBUFFER_H_
#define DRAWBUFFER_H_

#define COLUMNS 80
#define ROWS 30
#define BUF_SIZE 480*640

#define OFFSET 0
#define CRATER_OFFSET 200
// #define CRATER_OFFSET 16   // just for temporary use

#include <system.h>
#include <alt_types.h>
#include "Ground.h"
#include "CannonBall.h"
#include "gameState.h"
#include "LifeBar.h"
#include "Player.h"

volatile struct player redPlayer;
volatile struct player bluePlayer;

struct cannon_Ball cannonBall;

struct attackedGround ground;
struct TEXT_VGA_STRUCT {
	alt_u8 VRAM [200*4]; //Week 2 - extended VRAM   ROWS*COLUMNS*2
	alt_u32 colorCombined;
	alt_u32 counter;
	alt_u32 crater_counter; // the offset starting from the beginning of crater memory;
	//modify this by adding const bytes to skip to palette, or manually compute palette
} ;

//you may have to change this line depending on your platform designer
static volatile struct TEXT_VGA_STRUCT* vga_ctrl = VGA_TEXT_MODE_CONTROLLER_0_BASE;
extern volatile struct TEXT_VGA_STRUCT IngameBuffer;
extern volatile struct TEXT_VGA_STRUCT InitialgameBuffer;

void display_init();
int yCoordinateConvert(int y);
void PackArray(struct TEXT_VGA_STRUCT* buffer, int itemId, int x, int y, int size);
void PackArrayCrater(struct TEXT_VGA_STRUCT* buffer, int itemId, int x, int y);
void SetZeroArray(struct TEXT_VGA_STRUCT* buffer, int skipNUM);

void endingFill(struct TEXT_VGA_STRUCT* buffer);


//struct COLOR{
//	char name [20];
//	alt_u8 red;
//	alt_u8 green;
//	alt_u8 blue;
//};


//you may have to change this line depending on your platform designer


//CGA colors with names
//static struct COLOR colors[]={
//    {"black",          0x0, 0x0, 0x0},
//	{"white",          0xf, 0xf, 0xf},
//	{"red",            0xa, 0x0, 0x0},
//	{"blue",           0x0, 0x0, 0xa},
//	{"cyan",           0x0, 0xa, 0xa},
//	{"green",          0x0, 0xa, 0x0},
//	{"magenta",        0xa, 0x0, 0xa},
//	{"brown",          0xa, 0x5, 0x0},
//	{"light gray",     0xa, 0xa, 0xa},
//    {"dark gray",      0x5, 0x5, 0x5},
//	{"light blue",     0x5, 0x5, 0xf},
//    {"light green",    0x5, 0xf, 0x5},
//	{"light cyan",     0x5, 0xf, 0xf},
//    {"light red",      0xf, 0x5, 0x5},
//	{"light magenta",  0xf, 0x5, 0xf},
//    {"yellow",         0xf, 0xf, 0x5}
//
//};


//
//void setColorPalette (alt_u8 color, alt_u8 red, alt_u8 green, alt_u8 blue); //Fill in this code
//
//void textVGAColorScreenSaver(); //Call this for your demo

// ITEM ID

// CRATER 1
// CANNON 2
// EXPLODE 3

// Blue LEFT 4
// Blue RIGHT 5
// BLUE UPHILL LEFT 6
// BLUE UPHILL RIGHT 7
// BLUE DOWNHILL LEFT 8
// BLUE DOWNHILL RIGHT 9

// RED  LEFT 10
// RED  RIGHT 11
// RED  UPHILL  LEFT 12
// RED  UPHILL  RIGHT 13
// RED  DOWNHILL LEFT 14
// RED  DOWNHILL RIGHT 15

// ARROW 16

// HEALTHFilled 17
// HEALTHunfilled 18
// CHARGINGFilled 19
// CHARGINGunfilled 20
// STRENGTHfilled 21
// STRENGTHunfilled 22

// CHARACTER ......
// ANGLE 23
// ENTER 26
 
// flying plane 27
// VS 28
// Win 29
// 指示


#endif
