// C Source File
// Created 1/26/2004; 5:25:56 PM
#include "All.h"

/*	Original by Thomas Nussbaumer, added fading options	 */
void Dissolve(void) {
	unsigned short seq = 1, offset;
	unsigned char mask;
	
	do 
	{
		mask   = 0x80 >> (seq & 0x7);
		offset = seq >> 3;
		
		if (seq & 1)
			seq = (seq>>1)^0x6000;
    else seq = seq>>1;
    
		if (offset >= LCD_SIZE)
			continue;
		
		/* ---------------------------------------------------------------------
		   The fade will act like a strange double buffer toggle.  The contents 
		   of the Hidden planes will be faded onto the Active gray planes! 
		   --------------------------------------------------------------------- */
		
		// Light
		if (*(unsigned char*)(Hiddenl + offset) & mask)
			*(unsigned char*)(Activel + offset) |= mask;
		else *(unsigned char*)(Activel + offset) &= ~mask;
		// Dark
		if (*(unsigned char*)(Hiddend + offset) & mask)
			*(unsigned char*)(Actived + offset) |= mask;
		else *(unsigned char*)(Actived + offset) &= ~mask;
    
	} while (seq != 1);
}

// Writes a string of messages onto the NHL 2004 background and dissolves them in
void Message(const char *const Str[], const short Flag) {
	short Pos = 0;
	memset(Hiddenl, 0xFF, LCD_SIZE + LCD_SIZE);
	GraySpriteX8_AND(g->Zerox + 8, 0, 16, ExciteBike_Title[1], ExciteBike_Title[0], 18, 
		Hiddenl, Hiddend);
	
	// Print all messages onto the hidden planes
	do 
	{
		FS_DrawGrayString(0, 28 + 10 * Pos, (char*)Str[Pos], A_SHADOWED|A_XOR|A_CENTERED, F_6x8);
	} while (Str[++Pos] != NULL);
	Display_Author();
	
	if (Flag & A_DISSOLVE) {  // Sometimes, the dissolve effect is not needed
		Dissolve();
	} else Toggle_Screens();
	if (Flag & A_WAITKEY) {  // Wait for the user to press a key
		while(_rowread(0)) {
			if (_KeyCheck(TEACHERKEY))
				LeadOut();
		}
		while(!_rowread(0)) continue;
		while(_rowread(0)) {
			if (_KeyCheck(TEACHERKEY))
				LeadOut();
		}
	}
}

/* Gets Text Input from the User */
short Get_Input(const char *Question, char *buffer, short MaxLen) {
	short key, pos = 0, Old_pos, width;
	void *kbq = kbd_queue();
	unsigned char temp[MaxLen + 1], a = 0;
	while(a < MaxLen)
		temp[a++] = 'G';
	
	temp[a]   = 0;
	width = DrawStrWidth(temp, F_6x8) + 6;
	buffer[0] = 0;
	
	while(_rowread(0));
	GraySetInt1Handler(interrupt1);  // Temporarily reinstall the original auto_int_1
	
	Draw_User_Input(Question, buffer, width);
	Dissolve();
	
	do 
	{
		Old_pos = pos;
		
		while (OSdequeue(&key, kbq)) continue;  // Get a keypress
		key = key & 0xf7ff;  // Mask out the repeat flag
		
		if (pos < MaxLen && ((toupper(key) >= 'A' && toupper(key) <= 'z') || (pos > 0 && isdigit(key))))
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
	} while (key != KEY_ESC && (pos <= 0 || key != KEY_ENTER));
	
	GraySetInt1Handler(DUMMY_HANDLER);
	if (key == KEY_ESC)
		return QUIT;
	while(_rowread(0));
	
	return 0;
}

void Draw_User_Input(const char *Question, const char *buffer, const short width) {
	memset(Hiddenl, 0xFF, LCD_SIZE + LCD_SIZE);
	GraySpriteX8_AND(g->Zerox + 8, 0, 16, ExciteBike_Title[1], ExciteBike_Title[0], 18, 
		Hiddenl, Hiddend);
	
	DrawGrayRect2B(((LCD_WIDTH - width) >> 1) + 1, 52, ((LCD_WIDTH - width) >> 1) + width - 1, 
		64, COLOR_WHITE, RECT_FILLED, Hiddenl, Hiddend);
	DrawGrayRect2B(((LCD_WIDTH - width) >> 1) + 1, 52, ((LCD_WIDTH - width) >> 1) + width - 1, 
		64, COLOR_DARKGRAY, RECT_EMPTY, Hiddenl, Hiddend);
	DrawGrayRect2B(((LCD_WIDTH - width) >> 1) + 2, 53, ((LCD_WIDTH - width) >> 1) + width - 2, 
		63, COLOR_LIGHTGRAY, RECT_EMPTY, Hiddenl, Hiddend);
	
	FS_DrawGrayString(0, 55, buffer, A_SHADOWED|A_CENTERED, F_6x8);
	FS_DrawGrayString(0, 22, Question, A_XOR|A_SHADOWED|A_CENTERED, F_6x8);
	FS_DrawGrayString(0, g->lcd_height - 6, "Press Enter when done", A_XOR|A_SHADOWED, F_4x6);
}

void DecompressRLE(unsigned char *Dest, unsigned char *Src, short Size) {
	register unsigned char a;
	// RLE = Flag (0x91), Value, Number of Repititions
	
	while (Size > 0) {
		// Decompress one run
		if (*Src == 0x91) {
			Src++;
			for(a = *(Src + 1); a--;)
				*Dest++ = *Src;
			Size -= *(++Src);
		} else if (*Src == 0x92) {
			Src++;
			for(a = 0; a < *(Src + 1); a++)
				*Dest++ = (*Src) + a;
			Size -= *(++Src);
		} else {
			*Dest++ = *Src;
			Size--;
		}
		Src++;
	}
}

short CompressRLE(unsigned char *Buf, unsigned short Size) {
	register unsigned char *Ptr   = Buf;
	const unsigned char *Original = Buf;
	register unsigned short a;
	unsigned char Val;
	
	while (Size > 0) {
		a = 1;
		Val = *Ptr;
		while(Size - a > 0 && Val == *(Ptr + a)) a++;
		if (a > 3) {  // We have a run to compress (this run looks like { x, x, x, x... })
			*Buf++ = 0x91;  // Flag
			*Buf++ = Val;   // Value being repeated
			*Buf++ = a;     // Number of Repititions
			Ptr   += a;
			Size  -= a;
		} else {
			a = 1;
			while(Size - a > 0 && Val + a == *(Ptr + a)) a++;
			if (a > 3) {  // We have a secondary run to compress (this run looks like { x, x+1, x+2, x+3... })
				*Buf++ = 0x92;  // Second Flag
				*Buf++ = Val;   // Value being repeated
				*Buf++ = a;     // Number of Repititions
				Ptr   += a;
				Size  -= a;
			} else {
				*Buf++ = Val;
				Ptr   += 1;
				Size  -= 1;
			}
		}
	}
	
	return (Buf - Original);  // New size of Compressed Buffer
}

void Display_Author(void) {
	const char *const Author = EFG("By: Travis Fischer", "Par Travis Fischer", "Von Travis Fischer");
	FS_DrawGrayString(LCD_WIDTH - DrawStrWidth(Author, F_4x6) - 1, g->lcd_height - 6, Author, 
		A_SHADOWED|A_XOR, F_4x6);
}

// Show the Paused Game Screen
void Pause_Game(void) {
	short Redo;
	clock->Running = FALSE;  // Stop the Clock
	FS_DrawGrayString(0, 7, EFG("(APPS shuts calc off)", "(APPS éteint la TI)", "(Apps schaltet den TI aus)"), 
		A_SHADOWED, F_4x6);
	FS_DrawGrayString(0, 18, EFG("* Paused *", "* Pause *", "* Pause *"), A_SHADOWED|A_CENTERED, F_8x10);
	
	Toggle_Screens();
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
		}
		if (_KeyCheck(PAUSEKEY)) {  // Turn calc off by tapping Pause (default APPS) twice
			off();
			Redo = TRUE;
		}
		if (_KeyCheck(ESCKEY) || _KeyCheck(TEACHERKEY)) {
			Exit_Linkplay();
			g->Status = QUIT;
			return;
		}
	} while (Redo == TRUE);
	while(_rowread(0));
	g->Status = g->Old_Status;
	
	// Align the calcs in Linkplay before continuing the game
	if (g->Link == TRUE && Align_Calcs() == QUIT)
		g->Status = QUIT;
}

unsigned char Get_Number(const char *Question, short No, short Min, short Max) {
	char buf[4] = { 0, 0, 0, 0 }, Fading = A_DISSOLVE;
	
	do
	{
		sprintf(buf, "%d", No);
		Message((const char *const[]){ Question, NULL }, Fading);
		FS_DrawGrayString(0, 40, buf, A_SHADOWED|A_CENTERED|A_XOR, F_8x10);
		FS_DrawGrayString(0, g->lcd_height - 6, EFG("Up/Dn", "H/B", "Oben/Unten"), A_SHADOWED|A_XOR, F_4x6);
		Toggle_Screens();
		
		// Delay Calc in Grayscale Low Power Mode
		while(_rowread(0) == FALSE) { buf[2] = buf[3] = FALSE; pokeIO(0x600005,0b11111); }
		
		if (_KeyCheck(UPKEY)) {
			if (buf[2] == FALSE) {
				buf[2] = TRUE;
				if (++No > Max) No = Min;
			}
		} else buf[2] = FALSE;
		if (_KeyCheck(DOWNKEY)) {
			if (buf[3] == FALSE) {
				buf[3] = TRUE;
				if (--No < Min) No = Max;
			}
		} else buf[3] = FALSE;
		
		Fading = FALSE;
		if (_KeyCheck(TEACHERKEY))
			LeadOut();
		if (_KeyCheck(ESCKEY))
			return FALSE;
	} while ((_KeyCheck(CONFIRMKEY) || (_keytest(RR_ENTER) || (CALCULATOR && _keytest(RR_ENTER1)) || 
	(CALCULATOR && _keytest(RR_ENTER2)))) == FALSE);
	
	return No;
}

/*short Delay(short Time) {
	for(; Time--;) {
		pokeIO(0x600005,0b11111);*/ /* low power mode, wake up CPU only on AI 1 
     	                          (grayscale), 2 (Keypress), 3 (AMS clock), 4 
      	                         (Link), 5 (AMS timer base, needed for APD) 
        	                       and 6 (ON, always wakes up the CPU)
         	                      -- Saves Battery Power -- *//*
    if (_rowread(0))
    	break;
	}
	
	return time;
}*/