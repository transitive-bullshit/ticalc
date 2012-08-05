// C Source File
// Created 8/5/2004; 2:26:56 PM
#include "All.h"
static inline unsigned char Display_Character(const short);

// Initiate a new Text Message
void New_Text(const char *str, short pause) {
	g->text_delay = count + TEXT_DELAY;
	g->text_pause = pause;
	g->text_i = 0;
	g->text_x = 0;
	g->text_y = 1;
	memset(g->box0, 0x00, 2 * BOX_SIZE);
	
	if (str == NULL) {
		SafeFree(g->text);
	} else {
		unsigned long New_Size = strlen(str);
		if (strlen(g->text) < New_Size) {
			if (g->text)
				free(g->text);
			if ((g->text = malloc(New_Size + 1)) == NULL)
				return;
		}
		
		strcpy(g->text, str);
	}
	
	/*if (g->text[0] == 0) {
			GrayClearScreen2B_R(Activel, Actived);
			PortSet(Actived, 239, 127);
			short k = strlen(g->text) - 1;
			printf("%s %d %d", g->text, g->text_i, k);
			WaitKey();
	}*/
	
	//Activate_TextBox();
	
	/*if ((g->text != NULL && strlen(g->text) < New_Size) || realloc(g->text, New_Size + 1) != NULL)
		strcpy(g->text, str);*/
}

void Run_Text(void) {
	/*if (g->box_active < 21 && g->box_active >= 0) {
		if (count >= g->text_delay + TEXT_DELAY) {
			g->box_active += g->box_move;
			short y = 93 - g->box_active, h = g->box_active;
			g->text_delay = count;
			
			
			GraySpriteX8_BLIT(24, y, h, Wrestler_Light(Text_Box0), Wrestler_Dark(Text_Box0), 
				g->p_maskval, 7, Hiddenl, Hiddend);
			GraySpriteX8_BLIT(80, y, h, Wrestler_Light(Text_Box1), Wrestler_Dark(Text_Box1), 
				g->p_maskval, 7, Hiddenl, Hiddend);
		}
	} else */
	
	if (g->text != NULL) {  // Check to see if there is text currently being processed
		
		if (_keytest(RR_APPS))
			g->text_delay = count;
		else if (count >= g->text_delay + TEXT_DELAY) {
		// Text output is synchronized with AI_1 to provide a smoother, timed display
			if (g->text_y > 7) {  // Scroll text box up one line
				g->text_y--;
				memmove(g->box0, g->box0 + 30, BOX_SIZE - 30);
				memmove(g->box1, g->box1 + 30, BOX_SIZE - 30);
			}
			
			const short Str_Length = strlen(g->text) - 1;
			if (g->text_i > Str_Length) {  // All text has been shown in this message
				if (g->text_pause && count >= g->text_delay + g->text_pause)  // Pause before clearing the Text Box
					New_Text(NULL, FALSE);
			} else {
				g->text_delay = count;
				while(Display_Character(Str_Length) == ' ');
			}
		}
		
		/* Copy Text Box to the Hidden Gray Planes */
		register const unsigned short offset = ((g->Zeroy + 76) * 30 + 4 + (g->Zerox >> 3));
		register unsigned char *dest0 = Hiddenl + offset;
		register unsigned char *dest1 = Hiddend + offset;
		register unsigned char *src0  = g->box0;
		register unsigned char *src1  = g->box1;
		unsigned char a = 13;
		for(; a--;) {
			unsigned char b = (96/8);
			for(; b--;) {
				*dest0++ = *src0++;
				*dest1++ = *src1++;
			}
			dest0 += (30 - (96/8));
			dest1 += (30 - (96/8));
			src0  += (30 - (96/8));
			src1  += (30 - (96/8));
		}
	}
}

/*void Activate_TextBox(void) {
	g->text_delay = count;
	if (g->box_active < 0)
		g->box_active = 0;
	
	g->box_move = 1;
}

void Deactivate_TextBox(void) {
	g->text_delay = count;
	if (g->box_active == 21)
		g->box_active = 20;
	
	g->box_move = -1;
}*/

// Display one character and update text box cursor
static inline unsigned char Display_Character(const short Str_Length) {
	unsigned short ch, Char_Width;
	FontSetSys(F_4x6);
	
	do 
	{
		if (g->text_i > Str_Length)
			return FALSE;
		
		ch = g->text[g->text_i];
		Char_Width = FontCharWidth(ch);
		short Width = g->text_x, a = g->text_i++;
		const unsigned char Line_Break = (ch == 0x0A);
		
		if (Line_Break || Width + Char_Width > 94) {
			g->text_x  = 0;
			if (g->text_y <= 7)
				g->text_y += 6;
			
			if (Line_Break)
				return ch;
		} else {
			/* Implement word wrap such that words will begin on the next line instead of being split in two */
			do 
			{
				const short c = g->text[a];
				
				if (c == ' ' || c == '.' || c == ',' || c == '-' || c == '!' || c == '/' || c == 0x0A)
					break;  // We've come to the end of the current word (0x0A is a new line marker)
				
				Width += FontCharWidth(c);
				if (Width > 94) {  // This word spills onto the next line
					g->text_x  = 0;
					g->text_y += 6;
					break;
				}
			} while(++a < Str_Length);
		}
	} while(g->text_x == FALSE && ch == ' ');  // Do not want to display leading spaces on left side
	
	// Output current character to the screen
	PortSet(g->box1, 239, 127);
	DrawChar(g->text_x, g->text_y, ch, A_NORMAL);
	PortSet(g->box0, 239, 127);
	DrawChar(g->text_x, g->text_y, ch, A_NORMAL);
	
	g->text_x += Char_Width;
	return ch;
}