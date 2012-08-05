// C Source File
// Created 12/14/2003; 4:59:36 PM
/* ------------------------------------------------------------------------- */
/* Defining this, will show the fps after each Race												   */
//#define BENCHMARK
/* ------------------------------------------------------------------------- */
#include "All.h"
/* Inlining Functions which are called each Frame will produce a slight speed increase */
static inline void Start_Race(void);
static inline void Initiate_HUD(void);
static inline void Update_HUD(void);
//static inline void Draw_HUD(void);
static inline void Heat_Bar(unsigned char*, short);

void MAIN_LOOP(void) {
	short Starting = TRUE;
	#ifdef BENCHMARK
	unsigned long cycles = 0;
	#endif
	
	if (g->Cheats & C_QUARTER)    // Quarter Timer Speed
		*(volatile unsigned char*)0x600017 = Original_Rate / 4;
		//PRG_setStart(Original_Rate / 4);
		//pokeIO(0x600017, (257 - (g->HW1 ? 79 : 53)) / 4);
	else if (g->Cheats & C_HALF)  // Half Timer Speed
		*(volatile unsigned char*)0x600017 = Original_Rate / 2;
		//PRG_setStart(Original_Rate / 2);
		//pokeIO(0x600017, (257 - (g->HW1 ? 79 : 53)) / 2);
	
	do 
	{
		
		if (g->bikes[g->Camera_Bike].y < 0) {
			g->softy = g->bikes[g->Camera_Bike].y;
			
			if (g->softy <= -94) {  // Bike is -Completely- off of the screen
				//GrayClearScreen2B_R(Hiddenl, Hiddend);
				memset(Hiddenl + 150, g->Back0, LCD_SIZE - 150);
				memset(Hiddend + 150, g->Back1, LCD_SIZE - 150);
				memcpy(Hiddenl, Terrainl, 150);
				memcpy(Hiddend, Terraind, 150);
			} else {
				register unsigned short offset = -((g->softy << 5) - g->softy - g->softy);  // -30 * g->softy
				/*if (g->softx & 1) {
					memset(Hiddenl + 150, (g->Back0 >> 1) | ((g->Back0 & 1) << 7), offset);
					memset(Hiddend + 150, (g->Back1 >> 1) | ((g->Back1 & 1) << 7), offset);
				} else {*/
				memset(Hiddenl + 150, g->Back0, offset);
				memset(Hiddend + 150, g->Back1, offset);
				short No_Lines = g->lcd_height + g->softy - 6;
				
				//											%1								 %2								 %3								 %4
				unsigned char *src0 = Terrainl, *src1 = Terraind, *dest0 = Hiddenl, *dest1 = Hiddend;
				// Copy the Terrain planes (and HUD) onto the Hidden Gray Planes with an offset because 
				// the Camera bike is off the top of the screen
				asm volatile("
				movem.l %%d0-%%d7,-(%%sp);
				
				| Copy the HUD onto the Hidden Gray Planes
				moveq #4,%%d0;
				0:;
				movem.l (%1)+,%%d1-%%d7;  | Light
				movem.l %%d1-%%d7,(%3);
				movem.l (%2)+,%%d1-%%d7;  | Dark
				movem.l %%d1-%%d7,(%4);
				lea (%3,28),%3;
				lea (%4,28),%4;
				move.w (%1)+,(%3)+;  			| Extra 2 Bytes on the Right of the Screen
				move.w (%2)+,(%4)+;
				
				dbra %%d0,0b;             | Loop until we copy all 5 Lines of the HUD
				
				movem.l  (%%sp)+,%%d0-%%d7;
				movem.l %%d0-%%d7,-(%%sp);
				add.l %5,%3;
				add.l %5,%4;
				
				| Copy the Terrain with an offset onto the Hidden Gray Planes
				1:;
				movem.l (%1)+,%%d1-%%d7;  | Light
				movem.l %%d1-%%d7,(%3);
				movem.l (%2)+,%%d1-%%d7;  | Dark
				movem.l %%d1-%%d7,(%4);
				lea (%3,28),%3;
				lea (%4,28),%4;
				move.w (%1)+,(%3)+;   		| Extra 2 Bytes on the Right of the Screen
				move.w (%2)+,(%4)+;
				
				dbra %0,1b;             	| Loop until we copy each Line
				
				movem.l  (%%sp)+,%%d0-%%d7;"
				: "=g" (No_Lines),"=a" (src0),"=a" (src1),"=a" (dest0),"=a" (dest1), "=g" (offset)
				: "0"  (No_Lines),"1"  (src0),"2"  (src1),"3"  (dest0),"4"  (dest1), "5"  (offset));
			}
		} else {  // No Vertical Shifting necessary
			g->softy = 0;
			// Copy Terrain buffers into Secondary, Hidden Gray Planes
			CopyGrayScreens(Terrainl, Terraind, Hiddenl, Hiddend);
			
			//CopyTerrain_ToHidden(Terrainl, Terraind, Hiddenl, Hiddend);
		}
		
		if (_KeyCheck(PAUSEKEY)) {
			g->Old_Status = g->Status;
			g->Status = Game_Paused;
		}
		
		if (_KeyCheck (ESCKEY)) {
			if (g->Status == End_Wheelie) {
				if (g->Link == FALSE) g->Status = Race_Finished;
			} else g->Status = QUIT;
		}
		if (_KeyCheck (TEACHERKEY))
			g->Status = Fast_Quit;
		
		#ifdef BENCHMARK
		g->bikes[0].addx = 66;
		g->bikes[0].attr &= ~(B_CRASHING | B_OVERHEAT);
		#endif
		Update_HUD();
		Update_Bikes();
		
		if (Starting) {
			if (Starting == TRUE)
				Start_Race();
			else Dissolve();
			clock->Running = TRUE;
			Starting = FALSE;
		} else {
			// From my benchmarks, just using two plane virtual planes and a custom 
			// Copyscreen function to "switch" the hidden and active plane contents 
			// is much faster than using the built-in Doublebuffering.
			Toggle_Screens_Fast();
		}
		
		#ifndef BENCHMARK
		if (g->Status == Game_Paused) {
			Pause_Game();
			if (g->Status == Normal) {
				if (g->Link == TRUE)
					Starting = 2;
				else clock->Running = TRUE;
			}
		}
		#endif
		/*if (_keytest (RR_CLEAR)) {  // For Debugging Purposes
			while(_keytest(RR_CLEAR));
			while(!_keytest(RR_CLEAR));
			while(_keytest(RR_CLEAR));
		}*/
		
		#ifdef BENCHMARK
		cycles++;
		#endif
	} while(Race_is_Running #ifdef BENCHMARK 
	&& clock->Seconds < 40
	#endif
	);
	
	clock->Running = FALSE;
	*(volatile unsigned char*)0x600017 = Original_Rate;
	//PRG_setStart(Original_Rate);
	//pokeIO(0x600017, 257 - (g->HW1 ? 79 : 53));  // Restore normal timer speed
	
	if (g->Status == Fast_Quit)
		LeadOut();
	
	// Print the fps (Should be fairly accurate)
	#ifdef BENCHMARK
	char buffer[50];
	sprintf(buffer, "%lu cycles", cycles);
	GrayClearScreen2B_R(Hiddenl, Hiddend);
	FS_DrawGrayString(0, 0, buffer, A_NORMAL, F_6x8);
	//sprintf(buffer, "%g", (float)cycles/(float)((float)60 * (float)clock->Minutes + 
	//(float)clock->Seconds + ((float).19) * (float)clock->Mseconds50));
	sprintf(buffer, "%g", (float)cycles/40);
	FS_DrawGrayString(0, 10, buffer, A_NORMAL, F_6x8);
	/*if (clock->Seconds > 9)
		sprintf(buffer, "Time:  %d:%d:%d", clock->Minutes, clock->Seconds, clock->Mseconds50);
	else sprintf(buffer, "Time:  %d:0%d:%d", clock->Minutes, clock->Seconds, clock->Mseconds50);*/
	sprintf(buffer, "Time - %d:%02d:%02d", clock->Minutes, clock->Seconds, (clock->Mseconds50 << 2));
	
	FS_DrawGrayString(TI89?54:86, g->lcd_height - 5, buffer, A_NORMAL, F_4x6);
	Toggle_Screens();
	WaitKey();
	#endif
	
	if (g->Status == Race_Finished) {
		if (g->Link) {
			unsigned short Time = (60 * clock->Minutes + clock->Seconds) | (clock->Mseconds50 << 11);
			
			// Exchange times after a Linkplay race
			Transfer(&g->Track[g->Width + 2], (char*)&Time, sizeof(short));
		}
		
		Post_Race(FALSE);
	} else g->Game_Progress = QUIT;
}

void Initialize_Race(short No_Bikes) {
	unsigned short Seed = random(3) + 2;
	register unsigned short a;
	
	clock->Reset_Time = TRUE;
	GrayClearScreen2B_R(Terrainl, Terraind);
	g->camera_x    = 0;
	g->softx       = 0;
	g->Old_x       = 0;
	g->No_Bikes    = No_Bikes;
	g->Status      = Normal;
	g->Max_xspeed  = 2 + ((g->Cheats & C_FASTBIKES) != FALSE);  // Fast Bikes Cheat
	g->delay       = 0;
	g->First       = 0;
	g->Animation   = 0;
	g->text[0]     = 0;
	g->OldMinutes  = 0;
	g->OldSeconds  = 0;
	g->OldMseconds50 = 0;
	Initiate_HUD();
	
	static const unsigned char _backs_[6] = {
// Light, Dark
	0x55, 0x00, 
	0x00, 0xFF, 
	0xFF, 0xFF, 
	};
	
	if (g->Cheats & C_BACKGROUND) {
		a = 2*random(3);
		g->Back0 = _backs_[a];
		g->Back1 = _backs_[a + 1];
	} else {
		g->Back0 = g->Back1 = FALSE;
	}
	
	for(a = g->No_Bikes; a--;) {
		memset(&g->bikes[a], 0, sizeof(Bike));
		g->bikes[a].Control = Seed + a;
		if (g->bikes[a].Control > AI_FAST)
			g->bikes[a].Control -= 3;
		
		g->bikes[a].x       = 13 + (a << 4);
		g->bikes[a].y       = g->lcd_height - 29 - (12 * a);
		g->bikes[a].attr    = a | ((g->bikes[a].Control == AI_FAST) * B_TURBO);
		g->bikes[a].Tilt    = None;
		g->bikes[a].Temp    = Low_Temp;
		g->bikes[a].Bike_NO = a;
		g->jumpdat[a].Original_x = -8;
		g->jumpdat[a].Last_Crash = -8;
	}
	
	if (g->Link) {
		g->Camera_Bike      = g->Calc;
		g->bikes[0].Control = Host;
		g->bikes[1].Control = Join;
	} else {
		g->Camera_Bike      = 0;
		g->bikes[0].Control = Host;
		g->Calc             = Host;
	}
	
	// Draw Initial Terrain
	for(; g->camera_x < (LCD_WIDTH>>3) + (TI89); g->camera_x++) {
		Tile tile = Tiles[g->Track[g->camera_x]];
		CGraySprite8_OR_R(g->camera_x<<3, g->lcd_height - tile.Height, tile.Height, 
			tile.PtrLight, tile.PtrDark);
		
		if (g->camera_x & 1 && Tiles[g->Track[g->camera_x]].Height <= 56)
			CGraySprite8_OR_R(g->camera_x << 3, g->lcd_height - 64, 8, Hedge, Hedge + 8);
	}
	
	if (g->Cheats & C_BACKGROUND) {
		register short b;
		
		for(a = TI89 ? 47 : (47+28); a--;) {
			for(b = TI89 ? 21 : 30; b--;) {
				unsigned char temp1 = ~(*(Terrainl + 150 + 30*a+b)), temp2 = ~(*(Terraind + 150 + 30*a+b));
				
				*(Terrainl + 150 + 30*a+b) |= (g->Back0 & temp2);
				*(Terraind + 150 + 30*a+b) |= (g->Back1 & temp1);
			}
		}
	}
	
	if (TI89)
		g->camera_x--;
	
	// Draw the 4 Starting Blocks
	for(a = 4; a--;) {
		GraySprite8_MASK_R(35 + (a << 4), g->lcd_height - 13 - (12 * a), 10, Starting_Blocks[0], 
			Starting_Blocks[0] + 10, Mask, Mask, Terrainl, Terraind);
	}
}

static inline void Start_Race(void) {
	short Time, a = 3;
	
	for(; a--;)
		GrayClipSprite16_MASK_R(g->Zerox + 44 + 8 + 24 * a, Zeroy + 10, 16, Beep[0], Beep[0] + 16, 
			Beep_Mask, Beep_Mask, Hiddenl, Hiddend);
	
	Dissolve();
	for(a = 0; a < 3; a++) {
		for(Time = 158; Time--;) {
			pokeIO(0x600005,0b11101); /* low power mode, wake up CPU only on AI 1 
		                                (grayscale), 3 (AMS clock), 4 (Link), 5 
		                                (AMS timer base, needed for APD) and 6 (ON, 
		                                always wakes up the CPU)
		                                -- Saves Battery Power -- */
		if (_KeyCheck (ESCKEY))
			g->Status = QUIT;
		else if (_KeyCheck(TEACHERKEY))
			g->Status = Fast_Quit;
		else continue;
		
		if (g->Link == FALSE)
			return;
	}
		if (TI89) {
			GrayClipSprite16_MASK_R(g->Zerox + 44 + 8 + 24 * a, Zeroy + 10, 16, Beep[a + 1], 
				Beep[a + 1] + 16, Beep_Mask, Beep_Mask, Terrainl, Terraind);
		}
		GrayClipSprite16_MASK_R(g->Zerox + 44 + 8 + 24 * a, Zeroy + 10, 16, Beep[a + 1], 
			Beep[a + 1] + 16, Beep_Mask, Beep_Mask, Activel, Actived);
	}
	
	for(a = 4; a--;)
		GraySprite8_MASK_R(35 + (a << 4), g->lcd_height - 13 - (12 * a), 10, Starting_Blocks[1], 
			Starting_Blocks[1] + 10, Mask, Mask, Terrainl, Terraind);
}

#define EXT_SETBLACKPIX(x,y,p0,p1)		EXT_SETPIX(p0,x,y); EXT_SETPIX(p1,x,y)
static inline void Initiate_HUD(void) {
	char buf[40];
	
	// -- Time Limit -- //
	short Seconds = Best_Seconds + 8;
	short Minutes = Best_Minutes;
	if (Seconds >= 60) {
		Seconds -= 60;
		Minutes++;
	} else if (Seconds == 8 && Minutes == 0)
		Seconds = 0;
	sprintf(buf, EFG("%d:%02d:%02d - 3rd", "%d:%02d:%02d - 3°", "%d:%02d:%02d - 3er"), Minutes, Seconds, 
		Best_Mseconds50);
	
	FS_DrawString(0, 0, buf, Terrainl, F_4x6);
	FS_DrawString(0, 0, buf, Terraind, F_4x6);
	FS_DrawString(g->Zerox + g->Zerox + EFG(111, 114, 111), 0, EFG("Time -", "Temps", "Zeit -"), Terrainl, 
		F_4x6);
	FS_DrawString(g->Zerox + g->Zerox + EFG(111, 114, 111), 0, EFG("Time -", "Temps", "Zeit -"), Terraind, 
		F_4x6);
	
	// -- Temp -- //
	Heat_Bar(Terrainl + (TI89 ? 30 + 8 : 30 + 13), Max_Temp >> 1);
	
	// Draw the Heat Box
	EXT_SETPIX(Terraind, g->Zerox + 63, 1);
	EXT_SETPIX(Terraind, g->Zerox + 63 + 33, 1);
	EXT_SETPIX(Terraind, g->Zerox + 63, 2);
	EXT_SETPIX(Terraind, g->Zerox + 63 + 33, 2);
	EXT_SETPIX(Terraind, g->Zerox + 63, 3);
	EXT_SETPIX(Terraind, g->Zerox + 63 + 33, 3);
	const unsigned char Offset = TI89 ? 8 : 13;
	unsigned char a = 4;
	static const unsigned char hud[2][4] = { // (long) 0xEF7BDEF7, 0xD6B5AD6B
		{ 0xD6, 0xB5, 0xAD, 0x6B }, 
		{ 0xEF, 0x7B, 0xDE, 0xF7 }
	};
	
	for(; a--;) {
		*((char*)Terrainl + Offset + a) = hud[0][a];
		*((char*)Terrainl + Offset + 120 + a) = hud[0][a];
		*((char*)Terraind + Offset + a) = hud[1][a];
		*((char*)Terraind + Offset + 120 + a) = hud[1][a];
	}
}

static inline void Update_HUD(void) {
	// -- Temp -- //
	Heat_Bar(Hiddend + (TI89 ? 30 + 8 : 30 + 13), g->bikes[g->Camera_Bike].Temp >> 1);
	
	// -- Race Time -- //
	char buf[40];
	sprintf(buf, "%d:%02d:%02d", clock->Minutes, clock->Seconds, (clock->Mseconds50 << 2));
	FS_DrawGrayString(LCD_WIDTH - DrawStrWidth(buf,F_4x6) - 1, 0, buf, A_NORMAL, F_4x6);
}

/*static inline void Draw_HUD(void) {
	char buf[40];
	
	// -- Time Limit -- //
	sprintf(buf, "%d:%02d:%02d - 3rd", g->Time_Limit[2], g->Time_Limit[1], g->Time_Limit[0]);
	FS_DrawGrayString(0, 0, buf, A_NORMAL, F_4x6);
	
	// -- Temp -- //
	Heat_Bar(Hiddenl + (TI89 ? 30 + 8 : 30 + 13), Max_Temp >> 1);
	Heat_Bar(Hiddend + (TI89 ? 30 + 8 : 30 + 13), g->bikes[g->Camera_Bike].Temp >> 1);
	// Draw the Heat Box
	EXT_SETPIX(Hiddend, (TI89 ? 63 : 103), 1);
	EXT_SETPIX(Hiddend, (TI89 ? 33 + 63 : 33 + 103), 1);
	EXT_SETPIX(Hiddend, (TI89 ? 63 : 103), 2);
	EXT_SETPIX(Hiddend, (TI89 ? 33 + 63 : 33 + 103), 2);
	EXT_SETPIX(Hiddend, (TI89 ? 63 : 103), 3);
	EXT_SETPIX(Hiddend, (TI89 ? 33 + 63 : 33 + 103), 3);
	*(long*)(Hiddend + (TI89 ? 8 : 13)) = 0xEF7BDEF7;
	*(long*)(Hiddenl + (TI89 ? 8 : 13)) = 0xD6B5AD6B;
	*(long*)(Hiddend + (TI89 ? 8 + 120 : 13 + 120)) = 0xEF7BDEF7;
	*(long*)(Hiddenl + (TI89 ? 8 + 120 : 13 + 120)) = 0xD6B5AD6B;
	
	// -- Race Time -- //
	sprintf(buf, "Time - %d:%02d:%02d", clock->Minutes, clock->Seconds, (clock->Mseconds50 << 2));
	FS_DrawGrayString(LCD_WIDTH - DrawStrWidth(buf,F_4x6) - 1, 0, buf, A_NORMAL, F_4x6);
}*/

static inline void Heat_Bar(unsigned char *Dest, short Len) {
	register const unsigned char Extra = 0xFF << (8 - (Len & 7));
	register short a = Len >> 3;
	
	for(; a--; Dest++) {
		*Dest = 0xFF;
		*(Dest + 30) = 0xFF;
		*(Dest + 60) = 0xFF;
	}
	
	// Remaining Bits
	*Dest |= Extra;
	*(Dest + 30) |= Extra;
	*(Dest + 60) |= Extra;
}

//static inline void CopyTerrain_ToHidden(unsigned char *src0 asm("%a0"), unsigned char *src1 asm("%a1"), 
//unsigned char *dest0 asm("%a2"), unsigned char *dest1 asm("%a3")) __attribute__((__regparm__));
/*static inline void CopyTerrain_ToHidden2(unsigned char *src0 asm("%a0"), unsigned char *src1 asm("%a1"), 
unsigned char *dest0 asm("%a2"), unsigned char *dest1 asm("%a3"), 
short offset, register short h) __attribute__((__regparm__));*/

// Copy the Terrain planes onto the Hidden Gray planes, while adding a 'background texture'
/*static inline void __attribute__((__regparm__)) CopyTerrain_ToHidden(unsigned char *src0 asm("%a0"), 
unsigned char *src1 asm("%a1"), unsigned char *dest0 asm("%a2"), unsigned char *dest1 asm("%a3")) {
	register short h = g->lcd_height - 48 - 6;
	register unsigned long Back0 = 0xAAAAAAAA;
	register unsigned long Back1 = 0x55555555;
	
	asm("
		movem.l %%d0-%%d5,-(%%sp);
		
	  moveq #4,%%d0;
	  0:;
	  movem.l (%0)+,%%d1-%%d5;		| Plane 1
	  movem.l %%d1-%%d5,(%2);
	  movem.l (%1)+,%%d1-%%d5;		| Plane 2
	  movem.l %%d1-%%d5,(%3);
	  lea (%2,30),%2;
	  adda.w #10,%0;
	  lea (%3,30),%3;
	  adda.w #10,%1;
	  
	  dbra %%d0,0b;							| Loop until we do all 100 Lines
	  
	  movem.l  (%%sp)+,%%d0-%%d5;	| Restore previous register contents"
	  : "=a" (src0),"=a" (src1),"=a" (dest0),"=a" (dest1)
		: "0"  (src0),"1"  (src1),"2"  (dest0),"3"  (dest1)
	);
	
	for(;h--;src0+=30,src1+=30,dest0+=30,dest1+=30) {
		// Light Layer
		*(long*)(dest0 + 0)  = (Back0 & ~*(long*)(src1 + 0)) | *(long*)(src0 + 0);
		*(long*)(dest0 + 4)  = (Back0 & ~*(long*)(src1 + 4)) | *(long*)(src0 + 4);
		*(long*)(dest0 + 8)  = (Back0 & ~*(long*)(src1 + 8)) | *(long*)(src0 + 8);
		*(long*)(dest0 + 12) = (Back0 & ~*(long*)(src1 + 12)) | *(long*)(src0 + 12);
		*(long*)(dest0 + 16) = (Back0 & ~*(long*)(src1 + 16)) | *(long*)(src0 + 16);
		*(long*)(dest0 + 20) = (Back0 & ~*(long*)(src1 + 20)) | *(long*)(src0 + 20);
		*(long*)(dest0 + 24) = (Back0 & ~*(long*)(src1 + 24)) | *(long*)(src0 + 24);
		*(short*)(dest0 + 28) = (Back0 & ~*(short*)(src1 + 28)) | *(short*)(src0 + 28);
		
		// Dark Layer
		*(long*)(dest1 + 0)  = (Back1 & ~*(long*)(src0 + 0)) | *(long*)(src1 + 0);
		*(long*)(dest1 + 4)  = (Back1 & ~*(long*)(src0 + 4)) | *(long*)(src1 + 4);
		*(long*)(dest1 + 8)  = (Back1 & ~*(long*)(src0 + 8)) | *(long*)(src1 + 8);
		*(long*)(dest1 + 12) = (Back1 & ~*(long*)(src0 + 12)) | *(long*)(src1 + 12);
		*(long*)(dest1 + 16) = (Back1 & ~*(long*)(src0 + 16)) | *(long*)(src1 + 16);
		*(long*)(dest1 + 20) = (Back1 & ~*(long*)(src0 + 20)) | *(long*)(src1 + 20);
		*(long*)(dest1 + 24) = (Back1 & ~*(long*)(src0 + 24)) | *(long*)(src1 + 24);
		*(short*)(dest1 + 28) = (Back1 & ~*(short*)(src0 + 28)) | *(short*)(src1 + 28);
		
		// Scroll the BackGround Right and wrap the right edge back to the left
		Back0 = (Back0 >> 1) | ((Back0 & 1) << 31);
		Back1 = (Back1 >> 1) | ((Back1 & 1) << 31);
	}
	
	// Copy the last 48 lines regularly (fast)
	asm volatile("
	movem.l %%d0-%%d7,-(%%sp);
	moveq #47,%%d0;
	0:;
	movem.l (%0)+,%%d1-%%d7;  | Light
	movem.l %%d1-%%d7,(%1);
	movem.l (%2)+,%%d1-%%d7;  | Dark
	movem.l %%d1-%%d7,(%3);
	lea (%1,28),%1;
	lea (%3,28),%3;
	move.w (%0)+,(%1)+;       | Extra 2 Bytes on the Right of the Screen
	move.w (%2)+,(%3)+;
	dbra %%d0,0b;             | Loop until we do all 48 Lines
	movem.l  (%%sp)+,%%d0-%%d7"
	: "=a" (src0),"=a" (dest0),"=a" (src1),"=a" (dest1)
	: "0" (src0),"1" (dest0),"2" (src1),"3" (dest1));
}*/