#include "drawBuffer.h"


int yCoordinateConvert(int y){
    return 480 - y - 1;
}


    // I5 | I4 | I3 | I2 | I1 | I0 |  0 |  0 |   
    // x9 | x8 | x7 | x6 | x5 | x4 | x3 | x2 | 
    // x1 | x0 | y9 | y8 | y7 | y6 | y5 | y4 |
    // y3 | y2 | y1 | y0 |  0 |  0 |  0 |  0 |

    // itemId:  6  bit;
    // x:       10 bit;
    // y:       10 bit;
void PackArray(struct TEXT_VGA_STRUCT* buffer, int itemId, int x, int y, int size){
    int converted_Y, converted_X;
    converted_Y = yCoordinateConvert(y + size);
    converted_X = x + size;
    buffer->VRAM[OFFSET + buffer->counter] = (converted_Y << 4) & 0xf0;
    buffer->counter++;
    buffer->VRAM[OFFSET + buffer->counter] = ((converted_Y >> 4) & 0x3f) | ((converted_X << 6) & 0xc0) ;
    buffer->counter++;
    buffer->VRAM[OFFSET + buffer->counter] = (converted_X >> 2) & 0xff;
    buffer->counter++;
    buffer->VRAM[OFFSET + buffer->counter] = (itemId << 2) & 0xfc;
    buffer->counter++;
//	printf("%d | %d | %d | %d\n", ((itemId << 2) & 0xfc), ((x >> 2) & 0xff), (((converted_Y >> 4) & 0x3f) | ((x << 6) & 0xc0)), ((converted_Y << 4) & 0xf0));
    return;
}

void SetZeroArray(struct TEXT_VGA_STRUCT* buffer, int skipNUM){ // skip num is the number of item to skip
    while(skipNUM > 0){
        buffer->VRAM[OFFSET + buffer->counter] = 0;
        buffer->counter++;
        buffer->VRAM[OFFSET + buffer->counter] = 0;
        buffer->counter++;
        buffer->VRAM[OFFSET + buffer->counter] = 0;
        buffer->counter++;
        buffer->VRAM[OFFSET + buffer->counter] = 0;
        buffer->counter++;
        skipNUM -= 1;
    }
}

void PackArrayCrater(struct TEXT_VGA_STRUCT* buffer, int itemId, int x, int y){
    int converted_Y, converted_X;
    
    
    converted_Y = yCoordinateConvert(y + 24);
    converted_X = x + 40;
    buffer->VRAM[CRATER_OFFSET + buffer->crater_counter] = (converted_Y << 4) & 0xf0;
    buffer->crater_counter++;
    buffer->VRAM[CRATER_OFFSET + buffer->crater_counter] = ((converted_Y >> 4) & 0x3f) | ((converted_X << 6) & 0xc0) ;
    buffer->crater_counter++;
    buffer->VRAM[CRATER_OFFSET + buffer->crater_counter] = (converted_X >> 2) & 0xff;
    buffer->crater_counter++;
    buffer->VRAM[CRATER_OFFSET + buffer->crater_counter] = (itemId << 2) & 0xfc;
    buffer->crater_counter++;

    
//	printf("%d | %d | %d | %d\n", ((itemId << 2) & 0xfc), ((x >> 2) & 0xff), (((converted_Y >> 4) & 0x3f) | ((x << 6) & 0xc0)), ((converted_Y << 4) & 0xf0));
    return;
}








void endingFill(struct TEXT_VGA_STRUCT* buffer){   // fill ending words 0xFFFF and copy it to the vga_ctrl

//    buffer->VRAM[CRATER_OFFSET + buffer->crater_counter] = 0xff;
//
//    buffer->VRAM[CRATER_OFFSET + buffer->crater_counter + 1] = 0xff;
//
//    buffer->VRAM[CRATER_OFFSET + buffer->crater_counter + 2] = 0xff;
//
//    buffer->VRAM[CRATER_OFFSET + buffer->crater_counter + 3] = 0xff;

    // memcopy to the ctrl

    memcpy((vga_ctrl->VRAM),(buffer->VRAM),sizeof(buffer->VRAM));

//    for(int i = 0; i < 4*10;){
//		printf("%d | %d | %d | %d\n", vga_ctrl->VRAM[OFFSET + i + 3], vga_ctrl->VRAM[OFFSET + i + 2], vga_ctrl->VRAM[OFFSET + i + 1], vga_ctrl->VRAM[OFFSET + i]);
//		if((vga_ctrl->VRAM[OFFSET + i] == 0xff) && (vga_ctrl->VRAM[OFFSET + i + 1] == 0xff) && (vga_ctrl->VRAM[OFFSET + i + 2] == 0xff) && (vga_ctrl->VRAM[OFFSET + i + 3] == 0xff)) break;
//		i += 4;
//    }
//
//    printf("------------------------------------\n");

    
    return;
}


//void setColorPalette (alt_u8 color, alt_u8 red, alt_u8 green, alt_u8 blue)
//{
//	red = red & 0x0f;
//	green = green & 0x0f;
//	blue = blue & 0x0f;
//	if(color%2 == 0){
//		vga_ctrl->colorCombined = ((red << 8) | (green << 4) | blue) & 0xFFF ;
//	}
//	else{
//		alt_u32 temp = ((((red << 8) | (green << 4) | blue)<< 12) | vga_ctrl->colorCombined) << 1;
//		alt_u8 temp1 = temp & 0xff;
//		alt_u8 temp2 = (temp >> 8) & 0xff;
//		alt_u8 temp3 = (temp >> 16) & 0xff;
//		alt_u8 temp4 = (temp >> 24) & 0xff;
//		vga_ctrl->VRAM[0x400+2*(color-1)+0] = temp1;
//		vga_ctrl->VRAM[0x400+2*(color-1)+1] = temp2;
//		vga_ctrl->VRAM[0x400+2*(color-1)+2] = temp3;
//		vga_ctrl->VRAM[0x400+2*(color-1)+3] = temp4;
//	}
//	//fill in this function to set the color palette starting at offset 0x0000 2000 (from base)
//}
//
//
//void textVGAColorScreenSaver()
//{
//	//initialize palette
//	for (int i = 0; i < 16; i++)
//	{
//		setColorPalette (i, colors[i].red, colors[i].green, colors[i].blue);
//	}
//
//}
