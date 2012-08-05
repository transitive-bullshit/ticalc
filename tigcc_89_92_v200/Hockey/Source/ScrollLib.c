// C Source File
// Created 9/7/2003; 12:06:05 PM
#include "All.h"
static inline void Shift_Vertical(const short);

/* ---------------------------------------CAMERA----------------------------------------
		The Camera always follows the puck and will stay centered on it scrolling vertically.  
		Horizontal scrolling is not centered on the puck, but off to one side so you can view
		more of what's coming better
   ------------------------------------------------------------------------------------- */
void Center_Camera(void) {
	register short Cur_x = (g->camera_x << 4) + g->softx;
	register short Xdif = g->x - g->Oldx;
	register short Ydif = g->y - g->Oldy;
	
	if (Xdif > 0) {					// Puck is moving Right
		if (Cur_x > g->x - g->Camerax_1 && g->x > g->Camerax_2)
			Shift_Horizontal(Xdif);
		else if (Cur_x + Xdif + Xdif > g->x - g->Camerax_1)  // > puckx - 60
			Shift_Horizontal((g->x - g->Camerax_1) - Cur_x);
		else Shift_Horizontal(Xdif + Xdif);
	} else if (Xdif < 0) {	// Puck is moving Left
		if (Cur_x < g->x - g->Camerax_3 && g->x < Max_x - g->Camerax_4)
			Shift_Horizontal(Xdif);
		else if (Cur_x + Xdif + Xdif < g->x - g->Camerax_3)
			Shift_Horizontal((g->x - g->Camerax_3) - Cur_x);
		else Shift_Horizontal(Xdif + Xdif);
	}
	
	if ((g->y > g->Cameray_1 && Ydif > 0) || (g->y < Max_y-g->Cameray_2 && Ydif < 0))
		Shift_Vertical(Ydif);
}

// Move the Camera Horizontally
void Shift_Horizontal(const short No_Lines) {
	
	g->softx += No_Lines;
	
	if (g->softx >= 16 && g->camera_x < Width - g->SPRWIDTH) {
		++g->camera_x;
		g->softx -= 16;
	} else if (g->softx <= -16 && g->camera_x > 0) {
		--g->camera_x;
		g->softx += 16;
	}
	
	if (g->softx > 16)
		g->softx = 16;
	else if (g->softx + (g->camera_x << 4) < 0)
		g->softx = 0;
}

// Move the Camera Vertically
static inline void Shift_Vertical(const short No_Lines) {
	
	g->softy += No_Lines;
	/*if (g->Distortion != FALSE && g->softy + (g->camera_y << 4) > 0) {
		g->Distortion += No_Lines;
		if (g->Distortion < 1)
			g->Distortion = 16;
		else if (g->Distortion > 16)
			g->Distortion = 1;
	}*/
	
	if (g->softy <= -16 && g->camera_y > 0) {
		--g->camera_y;
		g->softy +=16;
	} else if (g->softy >=16 && g->camera_y < Height - g->SPRHEIGHT) {
		++g->camera_y;
		g->softy -= 16;
	}
	
	if (g->softy > 16)
		g->softy = 16;
	else if (g->softy + (g->camera_y << 4) < 0)
		g->softy = 0;
}

static inline void GrayTile_Clipped(short, short, unsigned short*);
void DrawMap(void) {
	ClearGrayScreen2B_R(Hiddenl, Hiddend);
	
	// Shifting a word (2 Bytes or a short) 15 bits to the right will tell you the sign of the Number
	register unsigned char *Tile = (unsigned char*)LevelData + Width * (g->camera_y - (g->softy < 0)) + 
	(g->camera_x - (g->softx < 0));
	register short x, y;
	asm volatile("
		tst.w %0
		smi.b %0 | if negative fill lower byte with 1s, else with 0s
		ext.w %0 | copy what is in the lower byte into the upper byte"
		: "=d" (y)										// Output
		: "0" (g->softy)						// Input
	);
	// Draw all Foreground Tiles
	for(; y < g->SPRHEIGHT; ++y) {
		asm volatile("
			tst.w %0
			smi.b %0 | if negative fill lower byte with 1s, else with 0s
			ext.w %0 | copy what is in the lower byte into the upper byte"
			: "=d" (x)
			: "0" (g->softx)
		);  // Thanks to Samuel Stearley
		for(; x < g->SPRWIDTH; ++x, ++Tile) {
			if (*Tile)
				GrayTile_Clipped((x<<4) - g->softx, (y<<4) - g->softy, Blocks[*Tile]);
		}
		Tile += Width - (g->SPRWIDTH + (g->softx < 0));
	}
}

// Original by Thomas Nussbaumer - from the Game Construction Kit
static inline void GrayTile_Clipped(short x, short y, unsigned short* sprite1) {
	register long            addr1   = (long) Hiddenl;
	register long            addr2   = (long) Hiddend;
	register unsigned short* sprite2 = sprite1+16;
	register short           h       = 16;
	register unsigned short  cnt;
	register long            offset;
	
	if (y < -15 || y > g->PlaneHeight || x < -15 || x > LCD_WIDTH)
		return;  // Completely off screen
	
	if (y < 0) {  // Clip Above
		sprite1 -= y;
		sprite2 -= y;
		h       += y;
		y        = 0;
	} else if (y > g->PlaneHeight - 1 - h)  // Clip Below
		h = g->PlaneHeight - y;
	
	if (x + 16 > LCD_WIDTH) {  // Clip Right
		register unsigned short clip=0xffff<<(16-(LCD_WIDTH-(x)));
		cnt    = 16-(x&15);
		offset = (((y)+(y))<<4)-((y)+(y))+(((x)>>4)+((x)>>4));  // Suggested change by Bremen
		addr1  += offset;
		addr2  += offset;
		for (;h;--h,addr1+=30,addr2+=30) {
			*(long*)addr1|=(long)(*sprite1++ & clip)<<cnt;
			*(long*)addr2|=(long)(*sprite2++ & clip)<<cnt;
		}
	} else {
		if (x < 0) {  // Clip Left
			cnt = -x+16;
			offset = (y<<5)-(y+y);
		} else {  // No Horizontal Clipping Necessary
			cnt = 16-(x&15);
			offset = (((y)+(y))<<4)-((y)+(y))+(((x)>>4)+((x)>>4));  // Suggested change by Bremen
		}
		addr1  += offset;
		addr2  += offset;
		for (;h;--h,addr1+=30,addr2+=30) {
			*(long*)addr1|=(long)(*sprite1++)<<cnt;
			*(long*)addr2|=(long)(*sprite2++)<<cnt;
		}
	}
}