/*
 * text_mode_vga_color.c
 * Minimal driver for text mode VGA support
 * This is for Week 2, with color support
 *
 *  Created on: Oct 25, 2021
 *      Author: zuofu
 */

#include <system.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alt_types.h>
#include "text_mode_vga_color.h"

void textVGAColorClr()
{
	for (int i = 0; i<(ROWS*COLUMNS) * 2; i++)
	{
		vga_ctrl->VRAM[i] = 0x00;
	}
}

void textVGADrawColorText(char* str, int x, int y, alt_u8 background, alt_u8 foreground)
{
	int i = 0;
	while (str[i]!=0)
	{
		vga_ctrl->VRAM[(y*COLUMNS + x + i) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(y*COLUMNS + x + i) * 2 + 1] = str[i];
		i++;
	}
}

void setColorPalette (alt_u8 color, alt_u8 red, alt_u8 green, alt_u8 blue)
{
	red = red & 0x0f;
	green = green & 0x0f;
	blue = blue & 0x0f;
	if(color%2 == 0){	
		vga_ctrl->colorCombined = ((red << 8) | (green << 4) | blue) & 0xFFF ;
	}
	else{
		alt_u32 temp = ((((red << 8) | (green << 4) | blue)<< 12) | vga_ctrl->colorCombined) << 1;
		alt_u8 temp1 = temp & 0xff;
		alt_u8 temp2 = (temp >> 8) & 0xff;
		alt_u8 temp3 = (temp >> 16) & 0xff;
		alt_u8 temp4 = (temp >> 24) & 0xff;
		vga_ctrl->VRAM[0x400+2*(color-1)+0] = temp1;
		vga_ctrl->VRAM[0x400+2*(color-1)+1] = temp2;
		vga_ctrl->VRAM[0x400+2*(color-1)+2] = temp3;
		vga_ctrl->VRAM[0x400+2*(color-1)+3] = temp4;
	}
	//fill in this function to set the color palette starting at offset 0x0000 2000 (from base)
}


void textVGAColorScreenSaver()
{
	//This is the function you call for your week 2 demo

//	textVGAColorClr();
	//initialize palette
	for (int i = 0; i < 16; i++)
	{
		setColorPalette (i, colors[i].red, colors[i].green, colors[i].blue);
	}
	// background_init();
	// ground_init();
	IngameBuffer.VRAM[0] = 0x10;
	IngameBuffer.VRAM[1] = 0x00;
	IngameBuffer.VRAM[2] = 0x00;
	IngameBuffer.VRAM[3] = 0x10;
	IngameBuffer.VRAM[4] = 0x00;
	IngameBuffer.VRAM[5] = 0x00;
	IngameBuffer.VRAM[6] = 0x77;
	IngameBuffer.VRAM[7] = 0x1C;
	IngameBuffer.VRAM[8] = 0x80;
	IngameBuffer.VRAM[9] = 0x0C;
	IngameBuffer.VRAM[10] = 0x00;
	IngameBuffer.VRAM[11] = 0x04;
	IngameBuffer.VRAM[12] = 0x00;
	IngameBuffer.VRAM[13] = 0x01;
	IngameBuffer.VRAM[14] = 0x00;
	IngameBuffer.VRAM[15] = 0x20;

	memcpy((vga_ctrl->VRAM),(IngameBuffer.VRAM),sizeof(vga_ctrl->VRAM));

//	vga_ctrl->VRAM[0] = 0x10;
//	vga_ctrl->VRAM[1] = 0x00;
//	vga_ctrl->VRAM[2] = 0x00;
//	vga_ctrl->VRAM[3] = 0x10;
//	vga_ctrl->VRAM[4] = 0x00;
//	vga_ctrl->VRAM[5] = 0x00;
//	vga_ctrl->VRAM[6] = 0x77;
//	vga_ctrl->VRAM[7] = 0x1C;
//	vga_ctrl->VRAM[8] = 0x80;
//	vga_ctrl->VRAM[9] = 0x0C;
//	vga_ctrl->VRAM[10] = 0x00;
//	vga_ctrl->VRAM[11] = 0x04;
//	vga_ctrl->VRAM[12] = 0x00;
//	vga_ctrl->VRAM[13] = 0x01;
//	vga_ctrl->VRAM[14] = 0x00;
//	vga_ctrl->VRAM[15] = 0x20;

	// while (1)
	// {
	// 	fg = rand() % 16;
	// 	bg = rand() % 16;
	// 	while (fg == bg)
	// 	{
	// 		fg = rand() % 16;
	// 		bg = rand() % 16;
	// 	}
	// 	sprintf(color_string, "Drawing %s text with %s background", colors[fg].name, colors[bg].name);
	// 	x = rand() % (80-strlen(color_string));
	// 	y = rand() % 30;
	// 	textVGADrawColorText (color_string, x, y, bg, fg);
	// 	usleep (1000000);
	// }
}

void background_init(){
	int x,y;
	for (y = 0; y < 480; y++){
		for (x = 0; x < 640; x = x+2){
			vga_ctrl->VRAM[y*320 + (x/2)] = 0x99;//0x66;
			//printf("%d\n",pixel_color[y*640 + x]);
		}	
	}
}



void ground_init(){
		printf("b\n");
	int x,y,value;
	for (y = 0; y < 240; y++){
		for (x = 0; x < 640; x = x+2){
			value = (480-y-1)*320 + (x/2);

			vga_ctrl->VRAM[value] = 0xAA;//0x66;
			// ground.validGround[y*640 + x] = 1;
			// ground.validGround[y*640 + x + 1] = 1;
		}
	}

}
