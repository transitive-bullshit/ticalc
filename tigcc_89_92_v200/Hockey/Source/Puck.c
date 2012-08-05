// C Source File
// Created 9/14/2003; 3:09:22 PM
#include "All.h"
static inline void Draw_Puck(short, short);

/* Move the Puck and check for Collisions/Goals scored */
void Update_Puck(void) {
	/* If the puck is being controlled by a player, the player will be the center of the camera */
	if (g->Is_Controlled == FALSE) {
		// Friction with the Ice that gradually makes the puck stop
		if (g->Friction != FALSE) {
			g->addx += ((g->addx < 0) - (g->addx > 0)) * 2;
			g->addy += ((g->addy < 0) - (g->addy > 0)) * 2;
		}
		
		g->x += (g->addx >> 5);
		g->y += (g->addy >> 5);
		
		// Check if puck hit Left Goal
		if (Puck_Hit_Goal1(g->x, g->y)) {
			if (g->y < 135 && g->y >= 82) {
				if (g->addx > 0 && g->x < 58) {
					g->x = 46;
					g->addx = -ABS(g->addx);
				} else if (g->addx < 0 && g->x > 56) {  /* !!!GOAL!!! */
					g->Status = Level_Scored;
				}
			}
			if (Puck_Hit_Goal1(g->x, g->y) && g->Status != Level_Scored) {
				if (g->addy > 0 && g->y < 86) {
					g->y = 80;
					g->addy = -ABS(g->addy);
				} else if (g->addy < 0 && g->y > 132) {
					g->y = 138;
					g->addy = ABS(g->addy);
				}
			}
		} else if (Puck_Hit_Goal2(g->x, g->y)) {  // Check if puck hit Right Goal
			if (g->y < 135 && g->y >= 82) {
				if (g->addx > 0 && g->x < 407) {  /* !!!GOAL!!! */
					g->Status = Level_Scored;
				} else if (g->addx < 0 && g->x > 404) {
					g->x = 414;
					g->addx = ABS(g->addx);
				}
			}
			if (Puck_Hit_Goal2(g->x, g->y) && g->Status != Level_Scored) {
				if (g->addy > 0 && g->y < 86) {
					g->y = 80;
					g->addy = -ABS(g->addy);
				} else if (g->addy < 0 && g->y > 132) {
					g->y = 138;
					g->addy = ABS(g->addy);
				}
			}
		}
		
		// Check for collisions with the puck and the sides of the Rink
		if (g->x < 7) {
			g->x = 7;
			g->addx = ABS(g->addx);
		} else if (g->x > Max_x + 11) {
			g->x = Max_x + 11;
			g->addx = -ABS(g->addx);
		} if (g->y < 15) {
			g->y = 15;
			g->addy = ABS(g->addy);
		} else if (g->y > Max_y + 15) {
			g->y = Max_y + 15;
			g->addy = -ABS(g->addy);
		}
	}
	
	Draw_Puck(g->x-(g->camera_x<<4)-g->softx, g->y-(g->camera_y<<4)-g->softy);
}

/* Draws the puck at a given x,y coordinate with no clipping */
static inline void Draw_Puck(short x, short y) {
	//register long           offset = (y<<5)-(y<<1)+((x>>3)&0x1e);
	register long           offset = ((y+y)<<4)-(y+y)+((x>>4)+(x>>4));
	register long           addr1  = (long)Hiddenl+offset;
	register long           addr2  = (long)Hiddend+offset;
	register unsigned short cnt    = 24-(x&15);
	
	*(long*)addr1|=(long)(0x70)<<cnt;
	*(long*)addr2|=(long)(0x70)<<cnt;
	addr1+=30, addr2+=30;
	*(long*)addr1|=(long)(0xF8)<<cnt;
	*(long*)addr2|=(long)(0xF8)<<cnt;
	addr1+=30, addr2+=30;
	*(long*)addr1|=(long)(0xF8)<<cnt;
	*(long*)addr2|=(long)(0xF8)<<cnt;
	addr1+=30, addr2+=30;
	*(long*)addr1|=(long)(0x70)<<cnt;
	*(long*)addr2|=(long)(0x70)<<cnt;
}