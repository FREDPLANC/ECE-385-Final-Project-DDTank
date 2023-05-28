#include "Ground.h"

// void attactedGroundInit(){
//     // ground initialization
//     for(int i = 0; i < GROUND_HEIGHT*GROUND_WIDTH; i++){
//         ground.validGround[i] = 1;
//     }
    
//     return;
// }
void ground_init(){
		// printf("b\n");
	int x,y,value;
	for (y = 0; y < 240; y++){
		for (x = 0; x < 800; x++){
			// value = (480-y-1)*320 + (x/2);
			// pixel_color[value] = 0xAA;//0x66;
			ground.validGround[x][y] = 1;
		}
	}
	for (y = 240; y < 480; y++){
		for (x = 0; x < 800; x++){
			ground.validGround[x][y] = 0;
		}
	}
	return;
}