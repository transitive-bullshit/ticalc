// C Source File
// Created 5/31/2004; 12:21:45 PM
#include "All.h"

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
	
	/*count = 0;  (380)
	
	GrayClearScreen2B_R(Activel, Actived);
	PortSet(Actived, 239, 127);
	printf("%d", count);
	WaitKey();*/
}

// Update Keypresses (used extensively throughout project)
void KeyScan(void) {
	g->keys = ((g->keys & (0b00110000)) << 2);
	register const unsigned short row0 = _rowread(~0x0001);
	register const unsigned short row6 = _rowread(~((short) (1 << 6)));
	
	/* KeyScan Overview:
	(g->Keys is a 16 bit unsigned short variable)
	--- ----						---
	Bit Mask						Key
	--- ----						---
	1)	0b00000000001		Up
	2)	0b00000000010		Left
	3)	0b00000000100		Down
	4)	0b00000001000		Right
	5)	0b00000010000		'A'  (2nd, F1, Hand) - also used as a selection key in menus
	6)	0b00000100000		'B'  (Shift, F5, Q)
	7)	0b00001000000		Repeat Value of 'A' key from last frame
	8)	0b00010000000		Repeat Value of 'B' key from last frame
	9)  0b00100000000		Enter Key (also multiple Enter Keys on the TI-92+ and TI-v200)
	10) 0b01000000000		Teacher Key  (F5, F4, F3)
	11) 0b10000000000		Escape Key
	*/
	
	// A little cryptic, but it gets the job done and is very small
	if (!g->calculator) {  // if (TI89)
		register const unsigned short row1 = _rowread(~0x0002);
		g->keys |= (row0 & 0b00111111) | ((row1 & 0x01) << 8) | ((row1 & (1 << 7)) << 2) | ((row6 & 0x01) << 10);
	} else {
		register const unsigned short row9 = _rowread(~0x0200);
		
		g->keys |= ((row0 & (0x20 | 0x80)) >> 5) | ((row0 & (0x10 | 0x40)) >> 3) | 
		((row9 & 0x02) << 7) | ((row6 & 0x40) << 2) | 
		((_rowread(~0x0100) & 0x40) << 4) | // ESC, row8
		(TI92PLUS ? 
		(row6 & (1 << 4)) | ((_rowread(~((short) (1 << 7))) & 0x10) << 1) | ((row9 & 0x10) << 5) : 
		// v200
		((row0 & 0x08) << 1) | ((row9 & (1 << 3)) << 2) | ((_rowread(~((short) (1 << 2))) & (1 << 4)) << 5));
	}
}

// Wait for key(s) to be released
void while_keypress(unsigned short keys) {
	do { KeyScan(); pokeIO(0x600005,0b11101); } while(_KeyCheck(keys));
}

/*void Gallery(void) {
	GrayClearScreen2B_R(Activel, Actived);
	short a = 0, x = 0, y = 0, Big_y = 0;
	
	do {
		if (Tiles[a] != NULL)
			GraySpriteX8_BLIT(x, y, 16, (char*)Tiles[a], (char*)(Tiles[a] + 16), g->p_maskval, 2, Activel, Actived);
		if ((x += 16) >= g->lcd_width - 15) {
			x = 0;
			if ((y += 16) >= g->lcd_height - 15) {
				y = 0;
				WaitKey();
				GrayClearScreen2B_R(Activel, Actived);
			}
		}
	} while(++a < MAX_RING_SPRITES);
	WaitKey();
	//memset(Activel, 0xFF, LCD_SIZE); memset(Actived, 0xFF, LCD_SIZE);
	GrayClearScreen2B_R(Activel, Actived);
	a = x = y = 0;
	
	while(a < NO_SPRITES) {
		if (x + Wrestler_Width(a) > (g->lcd_width >> 3)) {
			x = 0;
			y += Big_y;
		}
		if (y + Wrestler_Height(a) > g->lcd_height) {
			x = y = Big_y = 0;
			WaitKey();
			GrayClearScreen2B_R(Activel, Actived);
		}
		if (Wrestler_Height(a) >= Big_y)
			Big_y = Wrestler_Height(a);
		
		//Draw_Wrestler(x << 3, y, a, Activel, Actived);
		GraySpriteX8_BLIT(x << 3, y, Wrestler_Height(a), Wrestler_Light(a), Wrestler_Dark(a), g->p_maskval, 
			Wrestler_Width(a), Activel, Actived);
		
		//printf_xy(0, 0, "%d) %d %d %d %d", a, x, y, Wrestler_Height(a), Wrestler_Width(a));
		//WaitKey();
		x += Wrestler_Width(a);
		
		a++;
	}
	
	WaitKey();
}*/

// Display Background design and possibly the text box
void Display_Interface_Design(short Invert) {
	// Display Background design in menues (broken into mirrored fourths)
	if (g->calculator) {
		memset(Hiddenl, 0xFF, LCD_SIZE * 2);
		
		register unsigned short a = 14 * 30 + 5;
		for(; a < (114 * 30 + 5); a+=30) {  // White
			memset(Hiddenl + a, 0x0, 20);
			memset(Hiddend + a, 0x0, 20);
		}
	} else ClearGrayScreen2B_R(Hiddenl, Hiddend);
	
	const char *p_maskval = g->p_maskval;
	GraySpriteX8_BLIT(g->Zerox,  g->Zeroy, 50, Wrestler_Light(Pre_Match0), Wrestler_Dark(Pre_Match0), 
		p_maskval, 10, Hiddenl, Hiddend);
	GraySpriteX8_BLIT(g->Zerox + 80,  g->Zeroy, 50, Wrestler_Light(Pre_Match1), Wrestler_Dark(Pre_Match1), 
		p_maskval, 10, Hiddenl, Hiddend);
	GraySpriteX8_BLIT(g->Zerox, g->Zeroy + 50, 50, Wrestler_Light(Pre_Match2), Wrestler_Dark(Pre_Match2), 
		p_maskval, 10, Hiddenl, Hiddend);
	GraySpriteX8_BLIT(g->Zerox + 80, g->Zeroy + 50, 50, Wrestler_Light(Pre_Match3), Wrestler_Dark(Pre_Match3), 
		p_maskval, 10, Hiddenl, Hiddend);
	
	if (Invert) {
		register const unsigned short offset = ((g->Zeroy + 7) * 30 + 3 + (g->Zerox >> 3));
		register unsigned char *dest0 = Hiddenl + offset;
		register unsigned char *dest1 = Hiddend + offset;
		short a = 86;
		for(; a--; dest0 += 30, dest1 += 30) {
			memset(dest0, 0xFF, 14);
			memset(dest1, 0xFF, 14);
		}
	} else {  // Display the text box at the bottom of the design
		GraySpriteX8_BLIT(g->Zerox + 24, g->Zeroy + 72, 21, Wrestler_Light(Text_Box0), Wrestler_Dark(Text_Box0), 
			p_maskval, 7, Hiddenl, Hiddend);
		GraySpriteX8_BLIT(g->Zerox + 80, g->Zeroy + 72, 21, Wrestler_Light(Text_Box1), Wrestler_Dark(Text_Box1), 
			p_maskval, 7, Hiddenl, Hiddend);
	}
	
	// Display the text box at the bottom of the design
	/*if (g->box_active > 0) {
		short y = 93 - g->box_active, h = g->box_active;
		
		GraySpriteX8_OR(24, y, h, Wrestler_Light(Text_Box0), Wrestler_Dark(Text_Box0), 7, Hiddenl, Hiddend);
		GraySpriteX8_OR(80, y, h, Wrestler_Light(Text_Box1), Wrestler_Dark(Text_Box1), 7, Hiddenl, Hiddend);
	}*/
}

// Show the Paused Game Screen
void Pause_Game(void) {
	short Redo, anim = 0, wait = 129, x = 48, dir = 2;
	clock->Running = FALSE;  // Stop the Clock
	g->Old_count   = count;
	DrawGrayString(0, 41 - g->y_off, EFG("(APPS shuts calc off)"/*, "(APPS éteint la TI)", 
		"(Apps schaltet den TI aus)"*/), A_REVERSE | A_CENTERED | A_SHADOWED, F_4x6);
	DrawGrayString(0, 24 - g->y_off, EFG("* Paused *"/*, "* Pause *", "* Pause *"*/), 
		A_REVERSE | A_CENTERED | A_SHADOWED, F_8x10);
	
	do 
	{
		Redo = FALSE;
		
		while(_rowread(0));
		OSTimerRestart(APD_TIMER);
		
		while(!_rowread(0)) {
			pokeIO(0x600005,0b11111);
			// Check for APD
			if(OSTimerExpired(APD_TIMER)) {
				off();
				Redo = TRUE;
				break;
			}
			
			if (++wait == 130) {
				anim = !anim;
				if (dir == -2 + (anim << 2))
					x += 4 * dir;
				else x -= dir;
			} else if (wait > 139) {
				wait = 0;
				x += dir;
				
				//add 8, then add 2
				//-2, then add 2
				if (x < 48) {
					x = 48;
					dir = 2;
				}
				if (x > 168) {
					x = 168;
					dir = -2;
				}
			} else continue;
			
			unsigned char gfx = SweepingLeft + anim;
			Toggle_Screens();
			
			GraySpriteX8_MASK(x - g->x_off, 49 - g->y_off, 32, Wrestler_Light(gfx), Wrestler_Dark(gfx), 
				Wrestler_Mask(gfx), Wrestler_Mask(gfx), 3, Activel, Actived);
		}
		if (_keytest(RR_APPS)) {  // Turn calc off by tapping Pause (default APPS) twice
			off();
			Redo = TRUE;
		}
		KeyScan();
		if (_KeyCheck(ESCKEY | TEACHERKEY)) {
			Exit_Linkplay();
			g->Status = QUIT;
			return;
		}
	} while (Redo == TRUE);
	while(_rowread(0));
	
	g->Status = g->Old_Status;
	g->Force_Update = TRUE;
	// Align the calcs in Linkplay before continuing the game
	if (g->Link == TRUE && Align_Calcs() == QUIT)
		g->Status = QUIT;
}

// Wait for the user to press and release a key
void WaitKey(void) {
	short a;
	for(a = 2; a--;) {
		while(_rowread(0)) {
			KeyScan();
			if (_KeyCheck(TEACHERKEY))
				LeadOut();
			
			pokeIO(0x600005,0b11101);
		}
		if (a) {
			while(!_rowread(0)) { pokeIO(0x600005,0b11111); };
		}
	}
}

// Displays a list of messages
void Message(const char *const Str[], const short Flag) {
	Display_Interface_Design(TRUE);
	short Pos = 0;
	
	// Print all messages onto the hidden planes
	do 
	{
		DrawGrayString(0, g->Zeroy + 7 + 10 * Pos, Str[Pos], A_SHADOWED|A_XOR|A_CENTERED, F_6x8);
	} while (Str[++Pos] != NULL);
	Display_Author();
	
	if (Flag & A_DISSOLVE)
		Dissolve();
	else Toggle_Screens();
	if (Flag & A_WAITKEY)
		WaitKey();
}

void Display_Author(void) {
	const char *const Author = EFG("By: Travis Fischer"/*, "Par Travis Fischer", "Von Travis Fischer"*/);
	DrawGrayString(g->Zerox + 135 - DrawStrWidth(Author, F_4x6), g->Zeroy + 88, Author, A_SHADOWED | A_XOR, 
		F_4x6);
}

/* Gets Text Input from the User */
short Get_Input(const char *Question, char *buf, short MaxLen, short *Num_Only) {
	short key, pos = strlen(buf), Old_pos, width = (MaxLen >= 52 ? 108 : 6 * (MaxLen + 1));
	INT_HANDLER save_int_1_counter = GrayGetInt1Handler();
	void *kbq = kbd_queue();
	char buffer[MaxLen + 1];
	strcpy(buffer, buf);
	
	while(_rowread(0));
	GraySetInt1Handler(interrupt1);  // Temporarily reinstall the original AMS auto_int_1
	
	Draw_User_Input(Question, buffer, width);
	Dissolve();
	
	do 
	{
		Old_pos = pos;
		
		while (OSdequeue(&key, kbq)) continue;  // Get a keypress
		key = key & 0xf7ff;  // Mask out the repeat flag
		
		if (pos < MaxLen && key >= ' ' && key <= '~' && 
		(Num_Only ? isdigit(key) : (pos > 0 || (key >= 'A' && key <= 'z'))))
			buffer[pos++] = key;
		
		//if (pos < MaxLen && ((Num_Only == NULL && toupper(key) >= 'A' && toupper(key) <= 'z') || 
		//((pos > 0 || Num_Only != NULL) && isdigit(key))))
		//	buffer[pos++] = key;
		
		if (key == KEY_BACKSPACE && pos)
			pos--;
		if (key == KEY_CLEAR)
			pos = 0;
		if (key == KEY_F5)
			LeadOut();
		
		if (pos != Old_pos) {
			buffer[pos] = 0;
			if ((MaxLen == 52 && DrawStrWidth(buffer, F_4x6) > 52) || (MaxLen == 3 && atoi(buffer) > 127)) {
				pos = Old_pos;
				buffer[pos] = 0;
			} else {
				Draw_User_Input(Question, buffer, width);  // Draws the text with a box around it
				Toggle_Screens();
			}
		}
	} while (key != KEY_ESC && (pos <= 0 || key != KEY_ENTER));
	
	GraySetInt1Handler(save_int_1_counter);
	while_keypress(ENTERKEY | ESCKEY);
	if (key == KEY_ESC)
		return QUIT;
	
	if (Num_Only != NULL)
		*Num_Only = atoi(buffer);
	else strcpy(buf, buffer);
	return 0;
	//return -(key == KEY_ESC);
}

void Draw_User_Input(const char *Question, const char *buffer, const short width) {
	Display_Interface_Design(TRUE);
	Display_Author();
	
	const short x = ((g->lcd_width - width) >> 1);
	short y = g->Zeroy;
	GrayDrawRect2B(x + 1, y + 44, x + width - 1, y + 56, COLOR_WHITE, RECT_FILLED, Hiddenl, Hiddend);
	GrayDrawRect2B(x + 1, y + 44, x + width - 1, y + 56, COLOR_DARKGRAY, RECT_EMPTY, Hiddenl, Hiddend);
	GrayDrawRect2B(x + 2, y + 45, x + width - 2, y + 55, COLOR_LIGHTGRAY, RECT_EMPTY, Hiddenl, Hiddend);
	
	const short small = (DrawStrWidth(buffer, F_6x8) > width - 6);
	DrawGrayString(0, y + 7, Question, A_XOR|A_SHADOWED|A_CENTERED, F_6x8);
	DrawGrayString(0, y + 47 + 2 * small, buffer, A_NORMAL|A_SHADOWED|A_CENTERED, (small ? F_4x6 : F_6x8));
	if (g->Mode == TOURNAMENT) {
		DrawGrayString(g->Zerox + 26, y + 59, "Input Name", A_XOR|A_CENTERED, F_4x6);
		y += 7;
	}
	DrawGrayString(g->Zerox + 26, y + 59, EFG("Press Enter when done"), A_XOR|A_CENTERED, F_4x6);
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
void DrawGrayString(short x,short y,const char* s,short attr,short font) {
	short attr2   = attr & ~(A_CENTERED | A_SHADOWED/* | A_SPECIAL*/);
	//short oldfont = FontGetSys();
	FontSetSys(font);
	
	if (attr & A_CENTERED)
	  x = (g->lcd_width - DrawStrWidth(s, font)) >> 1;
	
	/*if (attr & A_SPECIAL) {
		PortSet(Hiddenl,239,127);
		DrawStr(x, y, s, attr2);
		return;
	}*/
	
	PortSet(Hiddend,239,127);
	DrawStr(x, y, s, attr2);
	PortSet(Hiddenl,239,127);
	DrawStr(x, y, s, attr2);
	
	if (attr & A_SHADOWED)
		DrawStr(x + 1, y + 1, s, attr2);
	
	//FontSetSys(oldfont);
}

void Reset_Time(void) {
	clock->Seconds = clock->Old_Seconds = clock->Time_Limit;
	clock->Mseconds50 = 0;
}