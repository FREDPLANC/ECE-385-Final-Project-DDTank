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



void setColorPalette (alt_u8 color, alt_u8 red, alt_u8 green, alt_u8 blue)
{
	red = red & 0x0f;
	green = green & 0x0f;
	blue = blue & 0x0f;
	if(color%2 == 0){	
		vga->colorCombined = ((red << 8) | (green << 4) | blue) & 0xFFF ;
	}
	else{
		alt_u32 temp = ((((red << 8) | (green << 4) | blue)<< 12) | vga->colorCombined) << 1;
		alt_u8 temp1 = temp & 0xff;
		alt_u8 temp2 = (temp >> 8) & 0xff;
		alt_u8 temp3 = (temp >> 16) & 0xff;
		alt_u8 temp4 = (temp >> 24) & 0xff;
		vga->VRAM[0x400+2*(color-1)+0] = temp1;
		vga->VRAM[0x400+2*(color-1)+1] = temp2;
		vga->VRAM[0x400+2*(color-1)+2] = temp3;
		vga->VRAM[0x400+2*(color-1)+3] = temp4;
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


