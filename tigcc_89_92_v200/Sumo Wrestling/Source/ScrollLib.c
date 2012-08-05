// C Source File
// Created 9/7/2003; 12:06:05 PM
#include "All.h"
void Redraw_Background(wrestler*);

/* ---------------------------------------CAMERA----------------------------------------
		The Camera always follows the puck and will stay centered on it scrolling vertically.  
		Horizontal scrolling is not centered on the puck, but off to one side so you can view
		more of what's coming better
   ------------------------------------------------------------------------------------- */
void Center_Camera(void) {
	register short Cur_x = g->x_off;
	// Store the wrestlers's coordinates of the left and right edges of their 
	// sprites for quick calculations
	short Left_0  = g->wrestlers[0].x + Wrestlers[g->wrestlers[0].Graphic].x_off - Cur_x;
	short Right_0 = Left_0 + Wrestlers[g->wrestlers[0].Graphic].Width;
	short Left_1  = g->wrestlers[1].x + Wrestlers[g->wrestlers[1].Graphic].x_off - Cur_x;
	short Right_1 = Left_1 + Wrestlers[g->wrestlers[1].Graphic].Width;
	short Update  = TRUE;
	
	// Assure that if a wrestler moves offscreen, the camera will follow as long as 
	// the other wrestler isn't cut off too.  There is a 5 pixel boundary on the 
	// left and right sides where the camera will scroll so wrestlers aren't too 
	// close to the edge
	if ((Left_0 < 5 && Right_1 <= g->lcd_width - 6) || (Left_1 < 5 && Right_0 <= g->lcd_width - 6))
		Shift_Horizontal(-1);
	else if ((Right_0 > g->lcd_width - 5 && Left_1 >= 6) || (Right_1 > g->lcd_width - 5 && Left_0 >= 6))
		Shift_Horizontal(1);
	else if (Cur_x < 40 && Left_0 > 12 && Left_1 > 12) {
		Check_Bounds(0);
		/*if (Left_0 < Left_1) {
			if (Left_0 > 12)
				Check_Bounds(0);
				//Shift_Horizontal(1);
				//if ((g->camera_x << 4) + g->softx > 40) {
				//	g->camera_x = 2;
				//	g->softx    = 8;
				//}
		} else if (Left_1 > 12)
			Check_Bounds(0);*/
	} else if (Cur_x > 40 && Right_0 < (g->lcd_width - 12) && Right_1 < (g->lcd_width - 12)) {
		Check_Bounds(1);
		
		/*if (Right_0 > Right_1) {
			if (Right_0 < (LCD_WIDTH - 12))
				Check_Bounds(1);
		} else if (Right_1 < (LCD_WIDTH - 12))
			Check_Bounds(1);*/
	} else Update = FALSE;
	
	if (g->Force_Update == FALSE && Update)
		g->Force_Update = TRUE;
	
	g->x_off = (g->camera_x << 4) + g->softx;
}

// 'Reset' Camera position to its default position, centered on the Ring 
// if no wrestler is outside of that center
void Check_Bounds(const short sign) {
	Shift_Horizontal(1 - sign - sign);
	
	if (((short)((g->camera_x << 4) + g->softx - 40)) >> 15 == sign) {
		g->camera_x = 2;
		g->softx    = 8;
	}
}

// Move the Camera Horizontally
void Shift_Horizontal(const short No_Lines) {
	if (!g->calculator) {
		g->softx += No_Lines;
		
		if (g->softx >= 16) {
			if (g->camera_x < 4) {
				++g->camera_x;
				g->softx -= 16;
			} else g->softx = 16;
		} else {
			if (g->softx <= -16 && g->camera_x > 0) {
				--g->camera_x;
				g->softx += 16;
			}
			
			if (g->softx + (g->camera_x << 4) < 0)
				g->softx = 0;
		}
	}
}

//-//-// //-//-// //-//-// Much Faster than GrayDBufToggleSync(); //-//-// //-//-// //-//-//
/*void __attribute__((__regparm__)) CopyScreens89(unsigned char *src0 asm("%a0"), 
unsigned char *src1 asm("%a1"), unsigned char *dest0 asm("%a2"), unsigned char *dest1 asm("%a3"));
asm("
.data
.globl CopyScreens89
.even

CopyScreens89:
	movem.l %d0-%d5/%a0-%a3,-(%sp);
  moveq #99,%d0;
  0:;
  movem.l (%a0)+,%d1-%d5;		| Plane 1
  movem.l %d1-%d5,(%a2);
  movem.l (%a1)+,%d1-%d5;		| Plane 2
  movem.l %d1-%d5,(%a3);
  lea (%a2,30),%a2;
  adda.w #10,%a0;
  lea (%a3,30),%a3;
  adda.w #10,%a1;
  
  dbra %d0,0b;							| Loop until we do all 100 Lines
  
  movem.l  (%sp)+,%d0-%d5/%a0-%a3;		| Restore previous register contents
  rts
");*/

void Redraw_Background(wrestler *w) {
	register unsigned char gfx = w->Graphic;
	
	short y = w->y + Wrestlers[gfx].y_off - g->y_off;
	short h = Wrestler_Height(gfx), width = Wrestler_Width(gfx);
	if (y < 0) {  // Vertical clipping for top of screen
		if (y + h < 0)
			return;
		
		h += y;
		y  = 0;
	}
	
	GraySpriteX8_BLIT(w->x + Wrestlers[gfx].x_off - g->x_off, y, h, g->buf[NO(w)], g->buf[2 + NO(w)], 
		g->p_maskval, width, Hiddenl, Hiddend);
}

void DrawBackground(void) {
	// Update Animated tiles (crowd/audience)
	if (g->Animation && --g->Anim_Delay <= FALSE) {
		g->Anim_Delay = g->Delay_Max + random(4);
		g->Plane = !g->Plane;
		g->Force_Update = TRUE;
	}
	
	// Update EarthQuake screen-bouncing effect
	if (g->EarthQuake >= 0) {
		g->E_i += g->E_add;
		
		if ((g->E_add < 0 && 4 - g->EarthQuake == g->E_i) || (g->E_add > 0 && 5 + g->EarthQuake == g->E_i)) {
			g->E_add = -g->E_add;
			if (g->E_add == -1 && (g->E_double = !g->E_double) && --g->EarthQuake < 0) {
				g->softy = g->Old_softy;
				g->y_off = (g->camera_y << 4) + g->softy;
			}
		}
		
		if (g->EarthQuake >= 0) {
			g->softy = g->Old_softy + (g->E_i > 4 ? _EarthQuake_[9 - g->E_i] : -_EarthQuake_[g->E_i]);
			g->y_off = (g->camera_y << 4) + g->softy;
		}
		g->Force_Update = TRUE;
	}
	
	// Do not redraw the background every frame, instead only when the screen scrolls, we must 
	// animate the crowd, or we have drawn blood/sweat gfx
	if (g->Force_Update == FALSE) {
		wrestler *w0 = &g->wrestlers[0];
		wrestler *w1 = &g->wrestlers[1];
		
		if (g->Mixed_Gfx > 0) {
			Redraw_Background(w0);
		} else if (w0->attr & DRAW_FIRST) {
			Redraw_Background(w1);
			Redraw_Background(w0);
		} else {
			Redraw_Background(w0);
			Redraw_Background(w1);
		}
		
		return;
	}
	
	ClearGrayScreen2B_R(Hiddenl, Hiddend);
	register unsigned char *Tile = ((g->Status == Game_Paused || g->Animation == -1) ? 
	(unsigned char*)Ring_Matrix : (unsigned char*)Ring[g->Plane]) + (g->camera_x - (g->softx < 0));
	register short x, y = 0;
	short max  = 8;
	const short ti89 = !g->calculator;
	if (g->softy < 0 && (ti89 || g->softy <= -12)) {
		max = 7;
		y = -ti89;
	} else Tile += (ti89 << 4) - ti89;
	
	// Draw all Foreground Tiles
	for(; y < max; ++y) {
		
		asm volatile("
			tst.w %0
			smi.b %0 | if negative fill lower byte with 1s, else with 0s
			ext.w %0 | copy what is in the lower byte into the upper byte"
			: "=d" (x)
			: "0" (g->softx)
		);  // Thanks to Samuel Stearly
		
		for(; x < g->plane_width; ++x, ++Tile) {
			GrayTile_Clipped((x<<4) - g->softx, (y<<4) - g->softy, Tiles[*Tile]);
			
			//GrayClipSprite16_OR_R((x<<4) - g->softx, (y<<4) - g->softy, 16, Tiles[*Tile], Tiles[*Tile]+16, 
			//	Hiddenl, Hiddend);
		}
		Tile += map_width - (g->plane_width + (g->softx < 0));
	}
}

// Original by Thomas Nussbaumer - from the Game Construction Kit
void GrayTile_Clipped(short x, short y, unsigned short* sprite1) {
	register long            addr1   = (long) Hiddenl;
	register long            addr2   = (long) Hiddend;
	register unsigned short* sprite2 = sprite1+16;
	register short           h       = 16;
	const short              bottom  = (g->lcd_height - 4);
	register unsigned short  cnt;
	register long            offset;
	
	if (y < -15 || y > bottom || x < -15)
		return;  // Completely off screen
	
	if (y < 0) {  // Clip Above
		sprite1 -= y;
		sprite2 -= y;
		h       += y;
		y        = 0;
	} else if (y > bottom - 17)  // Clip Below
		h = bottom - y;
	
	offset = ((y+y)<<4)-(y+y);
	if (x < 0) {  // Clip Left
		cnt = -x+16;
	} else {  // No Horizontal Clipping Necessary
		cnt = 16-(x&15);
		offset += ((x>>4)+(x>>4));
	}
	
	addr1 += offset;
	addr2 += offset;
	for (;h;--h,addr1+=30,addr2+=30) {
		*(long*)addr1|=(long)(*sprite1++)<<cnt;
		*(long*)addr2|=(long)(*sprite2++)<<cnt;
	}
}


// Move the Camera Vertically
/*void Shift_Vertical(const short No_Lines) {
	if (TI89) {
		g->softy += No_Lines;
		
		if (g->softy > 12) {
			if (g->camera_y > 0)
				g->softy = 12;
			else if (g->softy >= 16) {
				g->camera_y = 1;
				g->softy -= 16;
			}
		} else {
			if (g->softy <= -16 && g->camera_y > 0) {
				--g->camera_y;
				g->softy += 16;
			}
			
			if (g->softy + (g->camera_y << 4) < 0)
				g->softy = 0;
		}
	}
}*/