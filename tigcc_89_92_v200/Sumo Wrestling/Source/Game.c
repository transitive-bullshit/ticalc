// C Source File
// Created 6/8/2004; 7:52:37 PM
#include "All.h"
/* ------------------------------------------------------------------------- */
/* Defining this, will show the fps after each Match											   */
//#define BENCHMARK
/* ------------------------------------------------------------------------- */
static inline void Initiate_Match(void);
void Distort_Screen(unsigned short, unsigned short, unsigned char, unsigned char*, 
unsigned char*);
static inline void Verify_Exit(void);

static inline void Draw_HUD(void) {
	short offset = (g->calculator ? 3750 : 2910);
	unsigned char *hiddenl = Hiddenl + offset;
	unsigned char *hiddend = Hiddend + offset;
	
	// Wrestler One's Health Meter
	short Health = g->wrestlers[0].Health;
	// Make the Health bar slowly change instead of changing instantly
	if (Health != g->Cur_Health_0 || g->Force_Update) {
		g->Cur_Health_0 += (g->Cur_Health_0 != Health) - ((Health < g->Cur_Health_0) * 2);
		
		Health_Bar(hiddenl, 72, 1);
		memset(hiddend, 0x00, (72/8));
		memset(hiddend+30, 0x00, (72/8));
		memset(hiddend+60, 0x00, (72/8));
		if (g->Cur_Health_0 > 0)
			Health_Bar(hiddend, g->Cur_Health_0, 1);
	}
	
	// Wrestler Two's Health Meter
	Health = g->wrestlers[1].Health;
	if (Health != g->Cur_Health_0 || g->Force_Update) {
		// Make the Health bar slowly change instead of changing instantly
		g->Cur_Health_1 += (g->Cur_Health_1 != Health) - ((Health < g->Cur_Health_1) * 2);
		
		short offset2 = (g->calculator ? (168/8) : (88/8));//((g->lcd_width - 72) / 8);
		Health_Bar(hiddenl + offset2, 72, 1);
		hiddend += offset2;
		memset(hiddend, 0x00, (72/8));
		memset(hiddend+30, 0x00, (72/8));
		memset(hiddend+60, 0x00, (72/8));
		if (g->Cur_Health_1 > 0)
			Health_Bar(hiddend + (72/8) - 1, g->Cur_Health_1, -1);
	}
	
	if (g->Force_Update || clock->Seconds != clock->Old_Seconds) {
		unsigned char timebuf[5];
		
		clock->Old_Seconds = clock->Seconds;
		itoa_ushort_10(timebuf, clock->Seconds);
		/*asm volatile("
				subq.l   #6,%%sp
        movea.l  %%sp,%%a1
        moveq    #10,%%d1
        cmp.w    %%d1,%%d0
        bcc.s    __ius10_clear_upper_part_of_d0__

        addi.b   #'0',%%d0            | 8 clocks, 4 bytes.
        move.b   %%d0,(%%a0)+          | 8 clocks, 2 bytes.
|       move.b   0(%%a2,%%d0.w),(%%a1)+ | 18 clocks, 4 bytes.

        moveq    #1,%%d0
        bra.s    __ius10_cleanup_and_bye__
__ius10_clear_upper_part_of_d0__:
        swap     %%d0
        clr.w    %%d0
        swap     %%d0
__ius10_loop__:
        divu.w   %%d1,%%d0
| Take the remainder.
        swap     %%d0
        addi.b   #'0',%%d0
        move.b   %%d0,(%%a1)+
| Clear the remainder.
        clr.w    %%d0
| Swap back the register.
        swap     %%d0
| If whole register is less than 10, that's over.
        cmp.w    %%d1,%%d0
        bcc.s    __ius10_loop__

        addi.b   #'0',%%d0
        move.b   %%d0,(%%a0)+
__ius10_prepare_copy__:
        move.l   %%a1,%%d1
        sub.l    %%sp,%%d1
        move.w   %%d1,%%d0
        addq.b   #1,%%d0
        subq.b   #1,%%d1
__ius10_copy__:
        move.b   -(%%a1),(%%a0)+
        dbf      %%d1,__ius10_copy__
__ius10_cleanup_and_bye__:
        clr.b    (%%a0)
        addq.l   #6,%%sp"
				: "=a" (timebuf), "=g" (clock->Seconds)
				: "0"  (timebuf), "1"  (clock->Seconds)
		);*/
		DrawGrayString(0, g->lcd_height - 5, timebuf, A_REPLACE | A_CENTERED, F_4x6);
	}
	
	g->Force_Update = FALSE;
}

void MAIN_LOOP(void) {
	short Starting = TRUE;
	#ifdef BENCHMARK
	unsigned long cycles = 0;
	#else
	short Delay;
	#endif
	Initiate_Match();
	count = FALSE;
	
	do {
		
		if (_keytest(RR_APPS)) {  // Pause Key
			g->Old_Status = g->Status;
			g->Status = Game_Paused;
			g->Force_Update = TRUE;
		}
		if (clock->Time_Limit && clock->Seconds == FALSE)
			g->Status = Time_Expired;
		
		DrawBackground();
		Draw_HUD();
		
		if (g->Distortion && g->Status != Game_Paused) {
			if (g->Distortion++ > 15)  // Distortion :)
				g->Distortion = 1;
			
			Distort_Screen(g->calculator ? 123 : 93, g->Distortion - 1, (g->lcd_width >> 3), Hiddend, Hiddend);
			g->Force_Update = TRUE; // Background must be updated
		}
		
		KeyScan();  // Update Keypresses
		if (_KeyCheck(ESCKEY)) {
			if (g->Status >= M_Finish) {
				short winner = ((g->wrestlers[0].attr & W_ANIM) != FALSE);
				if (g->Status == PummelLift2)
					winner = !winner;
				g->Status = Match_Won_0 + winner;
			} else {
				g->Old_Status = g->Status;
				g->Status = QUIT;
			}
		}
		if (_KeyCheck(TEACHERKEY))
			g->Status = Fast_Quit;
		
		// Handle and Draw both Wrestlers
		Update_Wrestlers();
		
		if (Starting) {  // For the first frame of a match, use a dissolve transition effect
			Dissolve();
			count = g->Old_count * (Starting >= 2);
			if (Starting == 3)
				clock->Running = TRUE;
			Starting = FALSE;
		} else if (g->Distortion && g->Status != Game_Paused) {
			Distort_Screen(g->calculator ? 123 : 93, g->Distortion - 1, (g->lcd_width >> 3), Hiddenl, Activel);
			
			const unsigned short offset = (g->calculator ? 3690 : 2850);
			memcpy((char*)Activel + offset, (char*)Hiddenl + offset, 150);
			FastCopyScreen_R(Hiddend, Actived);
		} else {
			if (g->calculator) {
				CopyScreens92Plus(Hiddenl, Hiddend, Activel, Actived);
			} else {
				unsigned char *src0 = Hiddenl, *src1 = Hiddend, *dest0 = Activel, *dest1 = Actived;
				
				// Copy content of TI89-sized Hidden gray planes onto the Active gray planes
				asm volatile(" | Inlined for optimum speed/size
					movem.l %%d0-%%d5,-(%%sp);
				  moveq #99,%%d0;
				  0:;
				  movem.l (%0)+,%%d1-%%d5;		| Plane 1
				  movem.l %%d1-%%d5,(%2);
				  movem.l (%1)+,%%d1-%%d5;		| Plane 2
				  movem.l %%d1-%%d5,(%3);
				  lea (%2,30),%2;
				  adda.w #10,%0;
				  lea (%3,30),%3;
				  adda.w #10,%1;
				  
				  dbra %%d0,0b;								| Loop until we do all 100 Lines
				  
				  movem.l  (%%sp)+,%%d0-%%d5;		| Restore previous register contents"
				  : "=a" (src0), "=a" (src1), "=a" (dest0), "=a" (dest1)
				  : "0"  (src0), "1"  (src1), "2"  (dest0), "3"  (dest1)
				);
			}
		}
		
		
		//Toggle_Screens_Fast();  // Copy content of Hidden gray planes onto the Active gray planes
		
		
		#ifndef BENCHMARK
		// Slow down Match by putting calc into an idle state
		// -- conserves batteries and allows user to configure a speed preference
		for(Delay = MAX_GAME_SPEED - g->Game_Speed; Delay--;) {
			pokeIO(0x600005,0b11101); /* low power mode, wake up CPU only on AI 1 
	                                 (grayscale), 3 (AMS clock), 4 (Link), 5 
	                                 (AMS timer base, needed for APD) and 6 (ON, 
	                                 always wakes up the CPU) 
	                                 do not wake up CPU for AI 2 (keypress)
	                                 -- Saves Battery Power -- */
    }
    #else
    cycles++;
		#endif
		
		/*memset(Actived, 0, 6*30);
		PortSet(Actived, 239, 127);
		printf_xy(0, 0, "%d", g->Distortion - 1);
		WaitKey();*/
		//print out info
		
		/*if (g->Status > M_Normal) {
			clock->Running = FALSE;
			WaitKey();
			clock->Running = TRUE;
		}*/
		if (g->Status != M_Start)
			Center_Camera();
		
		if (g->Status == Game_Paused) {
			// Handle Pausing of a Match
			Pause_Game();
			if (g->Link)
				Starting = 2 + (g->Status == M_Normal);
			else if (g->Status == M_Normal)
				clock->Running = TRUE;
			
			count = g->Old_count;
		} else if (g->Status == QUIT) {
			Verify_Exit();
			Starting = 2 + (g->Status == M_Normal);
			
			count = g->Old_count;
		}
	} while(Normal_Match_Process #ifdef BENCHMARK 
	&& clock->Seconds < 30
	#endif
	);
	
	clock->Running = FALSE;
	if (g->Status == Fast_Quit)
		LeadOut();
	
	#ifdef BENCHMARK
	char buffer[50];
	sprintf(buffer, "%lu cycles", cycles);
	GrayClearScreen2B_R(Hiddenl, Hiddend);
	DrawGrayString(0, 0, buffer, A_NORMAL, F_6x8);
	//sprintf(buffer, "%g", (float)cycles/(float)((float)60 * (float)clock->Minutes + 
	//(float)clock->Seconds + ((float).19) * (float)clock->Mseconds50));
	sprintf(buffer, "%g", (float)cycles/40);
	DrawGrayString(0, 10, buffer, A_NORMAL, F_6x8);
	sprintf(buffer, "Time - %02d:%02d", clock->Seconds, (clock->Mseconds50 << 2));
	
	DrawGrayString(g->calculator?86:54, g->lcd_height - 5, buffer, A_NORMAL, F_4x6);
	Toggle_Screens();
	WaitKey();
	#endif
	
	if (g->Status > M_Normal)
		Post_Match();  // Show Post Match Summary Screen
	
	Reset_Time();
}

static inline void Initiate_Match(void) {
	register const char ti_89 = !g->calculator;
	
	//map.x = 40 * ti_89;
	//map.y = (ti_89 << 4) + ti_89;
	
	clock->Time_Limit = g->Time_Limit;
	Reset_Time();
	g->camera_x       = ti_89 + ti_89;  // TI89 ? 2 : 0;
	g->camera_y       = ti_89;          // TI89 ? 1 : 0;
	g->softx          = (ti_89 << 3);   // TI89 ? 8 : 0;
	g->softy          = ti_89;          //  TI89 ? 1 : 0;
	g->x_off          = (g->camera_x << 4) + g->softx;
	g->y_off          = (g->camera_y << 4) + g->softy;
	g->Force_Update   = TRUE;
	g->Animation      = TRUE;  // -1 to turn the animated crowd off
	g->Plane          = 0;
	g->Delay_Max      = 13;
	g->Anim_Delay     = 13;
	g->EarthQuake     = -1;
	g->Status         = M_Start;
	g->delay          = 0;
	g->Mixed_Gfx      = FALSE;
	g->Cur_Health_0   = 0;
	g->Cur_Health_1   = 0;
	g->keys           = FALSE;
	memset(salt_y, 24, sizeof(salt_y));
	
	unsigned char a = 0;
	for(; a < 2; a++) {
		//const short Ritual = (g->Link ? g->Random[a + a + 1] : random(3)) << 2;
		register wrestler *w = &g->wrestlers[a];
		
		w->x       = 0;
		w->y       = 0;
		#ifdef CAPTURE_SREENSHOT
		w->attr    = (a * W_NO) | ((!a) * W_DIRECTION) | (a << 2) | W_MOVEABLE;
		#else
		w->attr    = (a * W_NO) | ((!a) * W_DIRECTION) | (random(3) << 2) | W_MOVEABLE;
		#endif
		w->Graphic = 0;
		w->Counter = 0;
		w->Health  = 41 + w->c->Health;
		w->flag    = FALSE;
		memset(salt_x[a], 86 + 60 * a, 8);
		salt_x[a][8] = 0;
	}
	
	// Initiate a New, Random Audience
	for(a = map_height; a--;) {
		unsigned short b;
		for(b = map_width; b--;) {
			unsigned char tile = Ring_Matrix[a][b];
			unsigned short c = 2;
			
			const unsigned char t = (tile == Insi);
			if (t || tile == Outs) {
				const unsigned short Random = random(4 + t);
				
				if (Random == 4) {
					tile = Aud9;
					c = 1;
				} else tile = Audience1 + t * (Aud1 - Audience1) + (Random << 2) + (b > (map_width/2));
			} else if (tile >= Smal) {
				tile = Face1 + (tile == Smal2 ? (Face5 - Face1) : (random(2) << 1));
				
				c = 1;
			}	else if (tile == Cir1) {
				tile = Eye1;
			} else if (tile == Cir1M) {
				tile = Eye1M;
			}	else if (tile == Ci14) {
				tile = Eye3;
			} else if (tile == Ci14M) {
				tile = Eye3M;
			} else {
				c = 0;
				
				if (tile == Camera)
					tile = PicRight - (b > (map_width/2));
			}
			
			// Place Tile into two Ring_Matrix sized buffers
			unsigned char i = random(2);
			Ring[i][a][b]  = tile;
			Ring[!i][a][b] = tile + c;
		}
	}
}

void Health_Bar(unsigned char *Dest, const short Len, const short Dir) {
	register short a = Len >> 3;
	for(; a--; Dest+=Dir) {
		*Dest = 0xFF;
		*(Dest + 30) = 0xFF;
		*(Dest + 60) = 0xFF;
	}
	
	register unsigned char Extra = (8 - (Len & 7));
	Extra = (Dir > 0) ? 0xFF << Extra : 0xFF >> Extra;
	
	// Remaining Bits
	*Dest |= Extra;
	*(Dest + 30) |= Extra;
	*(Dest + 60) |= Extra;
}

/*void Shift_Horizontal(const short No_Lines) {
	if (TI89) {
		map.x += No_Lines;
		if (map.x < 0)
			map.x = 0;
		else if (map.x > 80)
			map.x = 80;
	}
}

void Shift_Vertical(const short No_Lines) {
	if (TI89) {
		map.y += No_Lines;
		if (map.y < 0)
			map.y = 0;
		else if (map.y > 28)
			map.y = 28;
	}
}*/

/* Copies the Distorted buffer pointed to by Source into the buffer Destination */
/* This routine will give a Dreamy, Wavy look to the Distorted Buffer */
/* NOTE:  Source, Destination, and ByteWidth MUST all be Even!  Use Distort_ScreenSafe 
	 if this is not the case.  Distort_ScreenSafe is slower, though */
void Distort_Screen(unsigned short Height, unsigned short Offset, unsigned char ByteWidth, 
unsigned char *Source, unsigned char *Destination) {
	register unsigned short *Dest = (unsigned short*)Destination;
	register unsigned short *Src  = (unsigned short*)Source;
	register unsigned short Temp, Temp2;
	register short Off;
	register short x;
	
	//ByteWidth -= (ByteWidth & 1)  // If ByteWidth is Odd
	
	/* NOTE:
			This Routine assumes that both Dest and Src are regular sized buffers.  
		If you would like to use this routine on a different sized buffer, Change 
		the 30's in this line to the ByteWidth of your custom buffer */
	for(; Height--; (char*)Dest += (30 - x), (char*)Src += (30 - x)) {  // Loop until we finish each Line
		Off = 16 - ((Height + Offset) & 15);
		Off = (Off > 8 ? Off - 12 : 4 - Off);  // -4 >= Off <= 4
		//Off = (Off < 16 ? 28 - Off : 12 + Off);
		
		//16 - x
		
		//16 - x = 4 - y
		
		//16 - x = -y + 28
		
		//Off  = 12 - ((Height + Offset) & 15);
		//Off -= ((Off > 0) << 3);  // -4 >= Off <= 4
		//Off = (4-Off) + (Off > 8) * (Off + Off - 16);
		
		if (Off >= 0) {				// Shift one line to the Right
			Temp = ((*(short*)((char*)Src + ByteWidth - 2)) & (0x000F >> (4 - Off))) << (16 - Off);
			for(x = 0; x < ByteWidth; x += 2) {
				Temp2 = (*Src & (0x000F >> (4 - Off))) << (16 - Off);
				*Dest = (*Src >> Off) | Temp;
				Temp = Temp2;
				Dest++, Src++;
			}
		} else {							// Shift one line to the Left
			Off = -Off;
			Temp = (*Src & (0xF000 << (4 - Off))) >> (16 - Off);
			(char*)Src += ByteWidth - 2, (char*)Dest += ByteWidth - 2;
			for(x = 0; x < ByteWidth; x += 2) {
				Temp2 = (*Src & (0xF000 << (4 - Off))) >> (16 - Off);
				*Dest = (*Src << Off) | Temp;
				Temp = Temp2;
				Dest--, Src--;
			}
			(char*)Src += ByteWidth + 2, (char*)Dest += ByteWidth + 2;
		}
	}
}

static inline void Verify_Exit(void) {
	clock->Running  = FALSE;  // Stop the Clock
	g->Old_count    = count;
	g->Force_Update = TRUE;
	
	if (Menu("- Exit Match -", (const char *const[]){ "Confirm", "Cancel", NULL }, 0, TRUE, 0) == 1 && (
	g->Link == FALSE || Align_Calcs() != QUIT)) {
		g->Status = g->Old_Status;
	} else Exit_Linkplay();
}