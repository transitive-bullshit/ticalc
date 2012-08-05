// C Source File
// Created 9/7/2003; 12:05:46 PM
#include "All.h"

/*	Waits until the user press a key and then releases the key	*/
inline void WaitKey(void) {
	while(_rowread(0)) continue;
	while(!_rowread(0)) continue;
	while(_rowread(0)) continue;
}

void KeyScan(void) {
	/* KeyScan Overview:
	(g->Keys is a 16 bit unsigned short variable)
	--- ----					---
	Bit Mask					Key
	--- ----					---
	1)	0b000000001		Up
	2)	0b000000010		Left
	3)	0b000000100		Down
	4)	0b000001000		Right
	5)	0b000010000		'Shoot'  (2nd, F1, Hand) - also used as a selection key in menus
	6)	0b000100000		'Pass'  (Shift, F5, Q)
	7)	0b001000000		Enter Key (also multiple Enter Keys on the TI-92+ and TI-v200)
	8)	0b010000000		Teacher Key  (F5, F4, F3)
	9)	0b100000000		Escape Key
	*/
	
	if (TI89) {
		register const unsigned short row1 = _rowread(~0x0002);
		
		g->keys = ((row1 & 0x01) << 8) | ((row1 & (1 << 7)) << 2) | 
		((_rowread(~((short) (1 << 6))) & 0x01) << 10);
	} else {
		register const unsigned short row6 = _rowread(~((short) (1 << 6)));
		register const unsigned short row9 = _rowread(~0x0200);
		
		g->keys = ((row9 & 0x02) << 7) | ((row6 & 0x40) << 2) | ((_rowread(~0x0100) & 0x40) << 4) | 
		(TI92PLUS ? ((row9 & 0x10) << 5) : (_rowread(~((short) (1 << 2))) & (1 << 4)) << 5);
	}
	
	register short a = 7;
	for(; a--;)
		g->keys |= (_custom_keytest(a) << a);
}

// Wait until key is released
void while_keypress(unsigned short key) {
	do { KeyScan(); } while(_KeyCheck(key));
}

/* Plots a Grayscale Circle at CX, CY, with a given Radius using a fast Bresenham Type Algorithm
	The Algorithm only calculates the points for 45 degrees of the Circle and manipulates the rest 
	It now includes clipping.  It much faster than calling DrawClipEllipse to draw a circle */
/*__attribute__((always_inline)) 
inline void GrayCircle_Clipped(const short CX,const short CY,const short Radius) {
	short Xchange = 1 - (Radius + Radiu), Ychange = 1, x = Radius, y = 0, RadiusError = 0;
	
	while(x >= y) {
		
		Clip_Gray_Pix(CX+x, CY+y);  // point in octant 1
		Clip_Gray_Pix(CX-x, CY+y);  // point in octant 4
		Clip_Gray_Pix(CX-x, CY-y);  // point in octant 5
		Clip_Gray_Pix(CX+x, CY-y);  // point in octant 8
		Clip_Gray_Pix(CX+y, CY+x);  // point in octant 2
		Clip_Gray_Pix(CX-y, CY+x);  // point in octant 3
		Clip_Gray_Pix(CX-y, CY-x);  // point in octant 6
		Clip_Gray_Pix(CX+y, CY-x);  // point in octant 7
		
		y++;
		RadiusError += Ychange;
		Ychange += 2;
		
		if ((RadiusError+RadiusError) + Xchange > 0) {
			x--;
			RadiusError += Xchange;
			Xchange += 2;
		}
	}
}*/

/*	Original by Thomas Nussbaumer (TICT)	*/
void Dissolve(void) {
	unsigned short seq = 1, offset;
	unsigned char mask;
	
	do 
	{
		offset = seq >> 3;
		mask   = 1 << (seq & 7);
		
		/* ---------------------------------------------------------------------
		   The fade will act like a strange double buffer toggle.  The contents 
		   of the Hidden planes will be faded onto the Active gray planes! 
		   --------------------------------------------------------------------- */
		
		unsigned char *ptr_light = (unsigned char*)(Activel + offset);
		*ptr_light = (*ptr_light & ~mask) | (*(unsigned char*)(Hiddenl + offset) & mask);
		
		unsigned char *ptr_dark = (unsigned char*)(Actived + offset);
		*ptr_dark = (*ptr_dark & ~mask) | (*(unsigned char*)(Hiddend + offset) & mask);
	  
	  do 
	  {
			seq = (seq >> 1) ^ ((seq & 1) ? 0x6000 : 0);
		} while(seq >= (LCD_SIZE * 8));
	} while(seq != 1);  // Loop
}

/* Fills outside of Screen on Ti-92+/v200, so 160x100 screens look nice on a 240x128 screen */
void Fill_Outside(void) {
	register short a;
	
	ClearGrayScreen2B_R(Hiddenl, Hiddend);
	if (TI89)
		return;
	memset(Hiddenl, 0xFF, LCD_SIZE);
	for(a = 14; a < 114; a++)
		memset(Hiddenl + (30 * a) + 5, 0x0, 20);  // Lightgray
}

void Show_Title(void) {
	// Clear the Hidden Gray planes and display the NHL TitleScreen
	Fill_Outside();
	
	SlowerSpriteX8_OR_R(Zerox, Zeroy, 100, NHLl, 20, Hiddenl);
	SlowerSpriteX8_OR_R(Zerox, Zeroy, 100, NHLd, 20, Hiddend);
	
	Distorted_Screen();
	/*// Dissolve in the TitleScreen
	Dissolve();
	// Wait for the User to press a key
	while(_rowread(0)) {
		if (_KeyCheck(ESCKEY) || _KeyCheck(TEACHERKEY))
			LeadOut();
	}
	while(!_rowread(0)) continue;
	while(_rowread(0)) {
		if (_KeyCheck(ESCKEY) || _KeyCheck(TEACHERKEY))
			LeadOut();
	}*/
}

/* Author:  Thomas Nussbaumer (From the FAT SDK)
// Integer Square Root function
// Contributors include Arne Steinarson for the basic approximation idea, Dann
// Corbit and Mathew Hendry for the first cut at the algorithm, Lawrence Kirby
// for the rearrangement, improvments and range optimization and Paul Hsieh
// for the round-then-adjust idea.
*/
unsigned long Fastsqrt(unsigned long x) {
	static const unsigned char sqq_table[] = {
		0,  16,  22,  27,  32,  35,  39,  42,  45,  48,  50,  53,  55,  57,
		59,  61,  64,  65,  67,  69,  71,  73,  75,  76,  78,  80,  81,  83,
		84,  86,  87,  89,  90,  91,  93,  94,  96,  97,  98,  99, 101, 102,
		103, 104, 106, 107, 108, 109, 110, 112, 113, 114, 115, 116, 117, 118,
		119, 120, 121, 122, 123, 124, 125, 126, 128, 128, 129, 130, 131, 132,
		133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 144, 145,
		146, 147, 148, 149, 150, 150, 151, 152, 153, 154, 155, 155, 156, 157,
		158, 159, 160, 160, 161, 162, 163, 163, 164, 165, 166, 167, 167, 168,
		169, 170, 170, 171, 172, 173, 173, 174, 175, 176, 176, 177, 178, 178,
		179, 180, 181, 181, 182, 183, 183, 184, 185, 185, 186, 187, 187, 188,
		189, 189, 190, 191, 192, 192, 193, 193, 194, 195, 195, 196, 197, 197,
		198, 199, 199, 200, 201, 201, 202, 203, 203, 204, 204, 205, 206, 206,
		207, 208, 208, 209, 209, 210, 211, 211, 212, 212, 213, 214, 214, 215,
		215, 216, 217, 217, 218, 218, 219, 219, 220, 221, 221, 222, 222, 223,
		224, 224, 225, 225, 226, 226, 227, 227, 228, 229, 229, 230, 230, 231,
		231, 232, 232, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 238,
		239, 240, 240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246,
		246, 247, 247, 248, 248, 249, 249, 250, 250, 251, 251, 252, 252, 253,
		253, 254, 254, 255
	};
	
	unsigned long xn;
	
	if (x >= 0x10000) {
		if (x >= 0x1000000) {
			if (x >= 0x10000000) {
				if (x >= 0x40000000) {
					if (x >= 65535UL*65535UL) return 65535;
						xn = sqq_table[x>>24] << 8;
					} else {
						xn = sqq_table[x>>22] << 7;
					}
				} else if (x >= 0x4000000)
					xn = sqq_table[x>>20] << 6;
        else xn = sqq_table[x>>18] << 5;
			} else {
				if (x >= 0x100000) {
					if (x >= 0x400000)
						xn = sqq_table[x>>16] << 4;
					else xn = sqq_table[x>>14] << 3;
			} else {
				if (x >= 0x40000)
					xn = sqq_table[x>>12] << 2;
				else xn = sqq_table[x>>10] * 2;
			}
			goto nr1;
		}
	} else if (x >= 0x100) {
		if (x >= 0x1000) {
			if (x >= 0x4000)
				xn = (sqq_table[x>>8] >> 0) + 1;
			else xn = (sqq_table[x>>6] >> 1) + 1;
		} else if (x >= 0x400)
			xn = (sqq_table[x>>4] >> 2) + 1;
		else xn = (sqq_table[x>>2] >> 3) + 1;
		
		goto adj;
	} else {
		return sqq_table[x] >> 4;
	}
	
	xn = (xn + 1 + x / xn) >> 1;
nr1:
	xn = (xn + 1 + x / xn) >> 1;
adj:
	
	return xn;
}

// Writes a string of messages onto the NHL 2004 background and dissolves them in
void Message(const char *const Str[], const short Flag) {
	short Pos = 0;
	Fill_Outside();  // For Ti-92+/v200 Models
	SlowerSpriteX8_OR_R(Zerox, Zeroy, 100, NHLd, 20, Hiddenl);  // Background
	
	// Print all messages onto the hidden planes
	do 
	{
		SpriteString(0, Zeroy + 10 * Pos, (char*)Str[Pos], Hiddenl, Hiddend, A_CENTERED);
	} while (Str[++Pos] != NULL);
	
	// Draw the Two Calculators Linking at the bottom of the screen
	if (g->Link) {
		GraySprite32_MASK_R(Zerox + 70, Zeroy + 79, 21, LinkPlay, LinkPlay + 21, 
		LinkPlay + 42, LinkPlay + 42, Hiddenl, Hiddend);
	}
	
	if (Flag == -1) {  // Sometimes, you don't want a dissolve effect
		Toggle_Screens();
	} else Dissolve();
	if (Flag > 0)
		WaitKey();  // Wait for the user to press a key
}

// Shows a little status screen and waits for the user to resume the game
short Pause_Game(void) {
	char buffer[20], Redo = 0, Receive = 0, Signal = FALSE;
	if (g->Link)
		OSLinkOpen();
	
	do 
	{
		// Draw the Background
		Fill_Outside();
		SlowerSpriteX8_OR_R(Zerox, Zeroy, 100, NHLd, 20, Hiddenl);
		// Print the Game's status
		SpriteString(0, Zeroy, (char*)"- game paused -", Hiddenl, Hiddend, A_CENTERED);
		sprintf(buffer, "%d -- %d", Team1->score, Team2->score);
		SpriteString(0, Zeroy + 13, buffer, Hiddenl, Hiddend, A_CENTERED);
		SpriteString(0, Zeroy + 30, ((Team1->Conference)?(Eastern[Team1->No].TeamName):
			(Western[Team1->No].TeamName)), Hiddenl, Hiddend, A_CENTERED);
		SpriteString(0, Zeroy + 40, (char*)"vs", Hiddenl, Hiddend, A_CENTERED);
		if (g->Mode == PRACTICE)  // No Opponent in Practice Mode
			SpriteString(0, Zeroy + 50, (char*)"- -", Hiddenl, Hiddend, A_CENTERED);
		else {
			SpriteString(0, Zeroy + 50, Team2->Conference?Eastern[Team2->No].TeamName:Western[Team2->No].TeamName, 
				Hiddenl, Hiddend, A_CENTERED);
		}
		
		// Print the User's key options
		FS_DrawGrayString(0, Zeroy + 87, "APPS shuts calc off, F1 opens the Options Menu", 
		A_NORMAL|A_SHADOWED|A_CENTERED, F_4x6);
		FS_DrawGrayString(0, Zeroy + 94, "Press any other key to continue...", 
		A_NORMAL|A_SHADOWED|A_CENTERED, F_4x6);
		
		if (Redo != 2)  // Only dissolve the screen in, if the calc is not turning back on after off();
			Dissolve();
		Redo = FALSE;
		while(_rowread(0)) continue;
		while(!_rowread(0)) {
			// Check for signals from the other calc
			if (g->Link && OSReadLinkBlock(&Receive, 1) == 1) {
				if (Receive == QUIT) { // Other calc has exited the Pause Menu and quit the games
					Message((const char *const[]){"other calc has", "exited the game", NULL}, 1);
					return QUIT;
				}
				if (g->Calc == Join && Receive == HOST_NO)  // Other calc is ready and waiting
					Signal = TRUE;
			}
		}
		if (_keytest (RR_APPS)) {  // Turn calc off
			off();
			Redo = 2;
		} else if (_keytest (RR_F1)) {  // Go to Options Menu
			Options();
			Redo = 1;
		}
	} while (Redo);
	while(_rowread(0)) {
		KeyScan();
		
		if (_KeyCheck(ESCKEY | TEACHERKEY)) {
			// Notify other calc that you are exiting the game
			Exit_Linkplay();
			return QUIT;
		}
	}
	
	if (g->Link) { 
		if (Signal == TRUE) {  // Other calc is already waiting
			Receive = JOIN_NO;
			OSWriteLinkBlock(&Receive, 1);
		} else if (Align_Calcs() == QUIT)  // Wait for other calc to resume the games
			return QUIT;
	}
	return 0;
}

/*===========================================================================*/
/* utility function to draw a string to both planes                          */
/* NOTE: there exists to new styles which are handled as flags               */
/*       A_CENTERED ... the given font will be used and the string will be   */
/*                      drawn centered on the screen                         */
/*       A_SHADOWED ... if this flag is set in the attributes a lightgray    */
/*                      shadow will be drawn with +1 pixel offset in all     */
/*                      two directions                                       */
/*===========================================================================*/
void FS_DrawGrayString(short x,short y,const char* s,short attr,short font) {
    if (attr & A_CENTERED)
      x = (LCD_WIDTH-DrawStrWidth(s,font))>>1;
		
		FS_DrawString(x, y, s, Hiddend, font);  // Very Fast!!! (by TICT)
		FS_DrawString(x, y, s, Hiddenl, font);

    if (attr & A_SHADOWED)
    	FS_DrawString(x + 1, y + 1, s, Hiddenl, font);
}

/* Gets Text Input from the User */
void Get_Input(const char *Question, char *buffer, short MaxLen) {
	short key, pos = 0, Old_pos, width;
	void *kbq = kbd_queue();
	while(buffer[pos]) {
		if (pos++ > MaxLen) {
			pos = 0;
			buffer[0] = 0;
		}
	}
	unsigned char temp[MaxLen + 1], a = 0;
	while(a < MaxLen) {
		temp[a++] = 'G';
		temp[a]   = 0;
	}
	width = DrawStrWidth(temp, F_6x8) + 6;
	
	while(_rowread(0));
	GraySetInt1Handler(interrupt1);  // Temporarily reinstall the original auto_int_1
	
	Draw_User_Input(Question, buffer, width);
	Dissolve();
	
	do 
	{
		Old_pos = pos;
		
		while (OSdequeue(&key, kbq)) continue;  // Get a keypress
		key = key & 0xf7ff;  // Mask out the repeat flag
		
		if (pos < MaxLen && isprint(key))
			buffer[pos++] = key;
		
		if (key == KEY_BACKSPACE && pos)
			pos--;
		if (key == KEY_CLEAR)
			pos = 0;
		
		if (pos != Old_pos) {
			buffer[pos] = 0;
			Draw_User_Input(Question, buffer, width);  // Draws the text with a box around it
			Toggle_Screens();
		}
	} while (key != KEY_ENTER && key != KEY_ESC);
	
	GraySetInt1Handler(DUMMY_HANDLER);
	while(_rowread(0));
}

void Draw_User_Input(const char *Question, char *buffer, short width) {
	Fill_Outside();
	SlowerSpriteX8_OR_R(Zerox, Zeroy, 100, NHLd, 20, Hiddenl);  // Background
	
	DrawGrayRect2B((LCD_WIDTH - width)/2, Zeroy + 41, (LCD_WIDTH - width)/2 + width, Zeroy + 55, COLOR_WHITE, 
		RECT_FILLED, Hiddenl, Hiddend);
	DrawGrayRect2B((LCD_WIDTH - width)/2, Zeroy + 41, (LCD_WIDTH - width)/2 + width, Zeroy + 55, COLOR_BLACK, 
		RECT_EMPTY, Hiddenl, Hiddend);
	DrawGrayRect2B((LCD_WIDTH - width)/2 + 1, Zeroy + 42, (LCD_WIDTH - width)/2 + width - 1, Zeroy + 54, 
		COLOR_DARKGRAY, RECT_EMPTY, Hiddenl, Hiddend);
	DrawGrayRect2B((LCD_WIDTH - width)/2 + 2, Zeroy + 43, (LCD_WIDTH - width)/2 + width - 2, Zeroy + 53, 
		COLOR_LIGHTGRAY, RECT_EMPTY, Hiddenl, Hiddend);
	
	FS_DrawGrayString(0, Zeroy + 45, buffer, A_NORMAL|A_SHADOWED|A_CENTERED, F_6x8);
	FS_DrawGrayString(0, Zeroy, Question, A_NORMAL|A_SHADOWED|A_CENTERED, F_6x8);
	FS_DrawGrayString(Zerox, Zeroy + 94, "Press Enter when done", A_NORMAL|A_SHADOWED, F_4x6);
}

void BubbleSort_Teams(Names *Teams[], short Max) {
	unsigned char LowerCase[Max + 1][13];
	unsigned char *Ptr, *Original;
	register int i, j;
	
	for(i = Max; i--;) {
		Original = (*Teams)[i].TeamName;
		Ptr = LowerCase[i];
		while((*Ptr++ = tolower(*Original++)));  // Convert one String to LowerCase
	}
	
	for(i = Max - 1; i > 0; i--) {
		for(j = 0; j < i; j++) {
			// Compare two TeamNames
			if(strcmp(LowerCase[j], LowerCase[j + 1]) > 0) {
				Swap_Teams(&(*Teams)[j], &(*Teams)[j + 1]);
				
				/* Swap the two LowerCase Teams also */
				strcpy(LowerCase[Max], LowerCase[j]);
				strcpy(LowerCase[j], LowerCase[j + 1]);
				strcpy(LowerCase[j + 1], LowerCase[Max]);
			}
		}
	}
}

/* Swap two Teams */
void Swap_Teams(Names *team1, Names *team2) {
	Names temp;
	
	memcpy(&temp, team1, sizeof(Names));
	memcpy(team1, team2, sizeof(Names));
	memcpy(team2, &temp, sizeof(Names));
}

short Distorted_Screen(void) {
	short Offset = 0;
	long Wait;
	
	Dissolve();
	
	do 
	{
		if (++Offset > 15)
			Offset = 0;
		
		//Distort_ScreenSafe(100, Offset, 20, Hiddenl + 425 * (!TI89), Activel + 425 * (!TI89));
		Distort_ScreenSafe(50, Offset, 20, Hiddend + 425 * (!TI89), Actived + 425 * (!TI89));
		Distort_ScreenSafe(50, Offset, 20, Hiddend + 425 * (!TI89) + 50 * 30, Actived + 425 * (!TI89) + 50 * 30);
		
		for(Wait = 16000; Wait--;);  // Slow Down
	} while(!_rowread(0));
	KeyScan();
	
	if (_KeyCheck(TEACHERKEY))
		LeadOut();
	if (_KeyCheck(ESCKEY)) {
		while_keypress(ESCKEY);
		return QUIT;
	}
	
	while(_rowread(0));
	return 0;
}

/* Copies the Distorted buffer pointed to by Source into the buffer Destination */
/* This routine will give a Dreamy, Wavy look to the Distorted Buffer */
/* NOTE:  Source, Destination, and ByteWidth MUST all be Even!  Use Distort_ScreenSafe 
	 if this is not the case.  Distort_ScreenSafe is slower, though*/
/*void Distort_Screen(unsigned short height, unsigned short Offset, unsigned char ByteWidth, 
unsigned char *Source, unsigned char *Destination) {
	register unsigned short *Dest = (unsigned short*)Destination;
	register unsigned short *Src  = (unsigned short*)Source;
	register unsigned short Temp, Temp2;
	register short Off;
	register short x;
	
	if (ByteWidth & 1)  // If ByteWidth is Odd
		ByteWidth--;
	
	for(; height--;) {  // Loop until we finish each Line
		Off = 16 - ((height + Offset) & 15);
		Off = (4-Off) + (Off > 8) * (Off + Off - 16);  // -4 >= Off <= 4
		
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
		*/
		/* NOTE:
			This Routine assumes that both Dest and Src are regular sized buffers.  
		If you would like to use this routine on a different sized buffer, Change 
		the 30's in this line to the ByteWidth of your custom buffer */
/*		(char*)Dest += (30 - x), (char*)Src += (30 - x);
	}
}*/

/* Copies the Distorted buffer pointed to by Source into the buffer Destination */
/* This routine will give a Dreamy, Wavy look to the Distorted Buffer */
void Distort_ScreenSafe(unsigned short height, unsigned short Offset, unsigned char ByteWidth, 
unsigned char *Source, unsigned char *Destination) {
	register unsigned char *Dest = Destination;
	register unsigned char *Src  = Source;
	register unsigned char Temp, Temp2;
	register short Off;
	register short x;
	
	for(; height--;) {  // Loop until we finish each Line
		Off = 16 - ((height + Offset) & 15);
		Off = (4-Off) + (Off > 8) * (Off + Off - 16);  // -4 >= Off <= 4
		//Off  = 12 - ((height + Offset) & 15);
		//Off -= ((Off > 0) << 3);
		
		if (Off >= 0) {				// Shift one line to the Right
			Temp = (*(Src + ByteWidth - 1) & (0x0F >> (4 - Off))) << (8 - Off);
			for(x = 0; x < ByteWidth; x++) {
				Temp2 = (*Src & (0x0F >> (4 - Off))) << (8 - Off);
				*Dest = (*Src >> Off) | Temp;
				Temp = Temp2;
				Dest++, Src++;
			}
		} else {							// Shift one line to the Left
			Off = -Off;
			Temp = (*Src & (0xF0 << (4 - Off))) >> (8 - Off);
			Src += ByteWidth - 1, Dest += ByteWidth - 1;
			for(x = 0; x < ByteWidth; x++) {
				Temp2 = (*Src & (0xF0 << (4 - Off))) >> (8 - Off);
				*Dest = (*Src << Off) | Temp;
				Temp = Temp2;
				Dest--, Src--;
			}
			Src += ByteWidth + 1, Dest += ByteWidth + 1;
		}
		
		/* NOTE:
			This Routine assumes that both Dest and Src are regular sized buffers.  
		If you would like to use this routine on a different sized buffer, Change 
		the 30's in this line to the ByteWidth of your custom buffer */
		Dest += (30 - x), Src += (30 - x);
	}
}