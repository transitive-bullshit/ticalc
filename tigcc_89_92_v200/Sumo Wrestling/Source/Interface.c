// C Source File
// Created 8/14/2004; 8:22:34 PM
#include "All.h"

/*	Returns the option chosen by the user	 */
short Menu(const char *title, const char *const Options[], short pos, short Fading, char index) {
	//const short Is_Main_Menu = (index == 1);//!strcmp(EFG("- Sumo Wrestling -"), title);
	const short Is_Exhibition_Menu = (index == 2);//!strcmp(EFG("- Exhibition -"), title);
	const short Is_Options_Menu = (index == 3);
	//!strcmp(EFG("- Options -"/*, "- Options -", "- Optionen -"*/), title);
	short width = 0, No_Options = 0, Main_Pos = 0, font = F_6x8;
	short a, keys[2] = { 0, 0 };
	if (pos == -1) {
		font = F_4x6;
		pos = 0;
	}
	
	while_keypress(ESCKEY);
	do 
	{
		short cur_width = DrawStrWidth(Options[No_Options], font);
		
		if (cur_width > width)
			width = cur_width;
	} while(Options[++No_Options] != NULL);
	
	do {
		
		// Draw the Menu's background
		Display_Interface_Design(TRUE);
		if (Is_Options_Menu == FALSE)
			Display_Author();
		else if (pos < 5)
			DrawGrayString(0, g->Zeroy + 89, EFG("(Left/Right modifies selection)"), A_XOR | A_CENTERED, F_4x6);
		
		// Display the Menu's Title
		DrawGrayString(0, g->Zeroy + 7, title, A_XOR | A_CENTERED | A_SHADOWED, F_6x8);
		// Loop through and display each option in the Menu
		for(a = (No_Options > 6 ? 7 : No_Options); a--;)
			DrawGrayString(g->Zerox + 27, g->Zeroy + 19 + 10 * a, Options[Main_Pos + a], A_XOR | (a == pos) * 
			A_SHADOWED/* | A_SPECIAL * (Is_Main_Menu && (a == 1))*/, font);
		
		// Highlight the selected Option
		GrayInvertRect2B(g->Zerox + 26, g->Zeroy + 18 + 10 * pos, g->Zerox + 28 + width, g->Zeroy + 26 + 
			10 * pos, Hiddenl, Hiddend);
		
		// Display linkplay graphic if Multiplayer mode is selected
		if (Is_Exhibition_Menu && pos == 2) {
			GraySpriteX8_MASK(g->Zerox + 28, g->Zeroy + 70, 22, (char*)LinkPlay[0], (char*)LinkPlay[1], 
				(char*)LinkPlay[2], (char*)LinkPlay[2], 4, Hiddenl, Hiddend);
		}
		
		if (Fading) {  // Make a new menu screen fade in
			Dissolve();
			Fading = FALSE;
		} else Toggle_Screens();
		
		do {
			
			KeyScan();
			if (g->keys) {
				// Allows for any size menues to be scrolled easily
				if (_KeyCheck(UPKEY)) {
				 	if (keys[0] == FALSE || ++keys[0] > 11) {
						keys[0] = TRUE;
						if (--pos < 0) {
							if (--Main_Pos < 0) {
								pos = (No_Options > 6 ? 6 : No_Options - 1);
								Main_Pos = (No_Options > 6) * (No_Options - 7);
							} else pos = 0;
						}
						
						//pos -= (Is_Main_Menu && (pos == 1));
						break;
					}
				} else keys[0] = FALSE;
				
				if (_KeyCheck(DOWNKEY)) {
					if (keys[1] == FALSE || ++keys[1] > 11) {
						keys[1] = TRUE;
						if (++pos >= (No_Options > 6 ? 7 : No_Options)) {
							if (++Main_Pos >= (No_Options > 6) * (No_Options - 6))
								pos = Main_Pos = 0;
							else pos = 6;
						}
						
						//pos += (Is_Main_Menu && (pos == 1));
						break;
					}
				} else keys[1] = FALSE;
				
				if (_KeyCheck(TEACHERKEY))
					Exit_Linkplay();
				if (_KeyCheck(ENTERKEY | A_KEY)) {
					while_keypress(ENTERKEY | A_KEY);
					return Main_Pos + pos;  // Return the option chosen by the user
				}
				if (_KeyCheck(ESCKEY))
					return QUIT;  // Return -1
				
				// Allow for Backwards scrolling in the Options Menu (Time Limit, Game Speed, Smack Talk, Distortion)
				if (Is_Options_Menu && pos < 5) {
					/*register const short ti89 = TI89;
					char buf[30];
					if (_keytest(1 + 3 * ti89, 1 + ((!ti89) << 2))) {
						g->Distortion = !g->Distortion;
						
						//  Wavy, dreamy, disorienting screen effect
						strcpy(buf, g->Distortion ? "Enabled" : "Disabled");
						Message((const char *const[]){ "- Cheat -", "", "Distortion", buf, NULL },A_DISSOLVE|A_WAITKEY);
						Fading = TRUE;
						break;
					}*/
					
					if (_KeyCheck(LEFTKEY)) {
						if (pos == FALSE)
							g->Time_Limit -= 10;
						else if (pos == 1 && (g->Game_Speed -= 2) < 0)
							g->Game_Speed += MAX_GAME_SPEED;
						
						//while_keypress(LEFTKEY);
						return pos;
					}
					if (_KeyCheck(RIGHTKEY)) {
						//while_keypress(RIGHTKEY);
						return pos;
					}
				}
			} else { keys[0] = keys[1] = FALSE; }
			
			pokeIO(0x600005,0b11111);
		} while(1);
	} while (1);
}

// Scrolls Text from the bottom to the top of the screen
void ScrollText(void *Text, const short NO_STRINGS) {
	short pos = 0, nul = FALSE, y = 8, time = 26, Wait;
	const short offset_x = 3 + (g->Zerox >> 3);
	const short offset_y = (6 + g->Zeroy) * 30;
	Display_Interface_Design(TRUE);
	Dissolve();
	memset(Hiddenl, 0xFF, LCD_SIZE * 2);  // Fill the hidden planes Black
	
	do {
		
		if (++y > 8) {
			y = 0;
			memset(Hiddenl+2640, 0xFF, 270);  // Fill in another row of 9 black lines for new text
			memset(Hiddend+2640, 0xFF, 270);
			
			if (nul == FALSE) {
				if (pos >= NO_STRINGS) {
					nul = TRUE;
				} else {
					/*unsigned char *str_address;
					// Inline version of function to access strings:
					// str_address is the (char*) address the accessed string will be stored to
					// str_address may then be used to manipulate/draw the string just like regular C strings
					// Index is the number of the string you want to access (info_text[Index])
					
					asm("
					move.w %1,%%d0
					add.w %%d0,%%d0
					lea info_text(%%pc),%%a0
					move.w 0(%%a0,%%d0.w),%0
					adda.l %%a0,%0"
					: "=a"(str_address) // output
					: "g"(pos++)        	// input
					: "d0","a0"         // clobbered registers
					);*/
					// try dereferencing array in STRADDR_MASTER
					DrawGrayString(0, 88, STRADDR_MASTER(pos++, Text), A_XOR | A_CENTERED, F_6x8);
				}
				
				//if (str[pos] == NULL)
				//	nul = TRUE;
				//else DrawGrayString(0, 88, (char*)str[pos++], A_XOR | A_CENTERED, F_6x8);
			} else if (nul++ > 10) break;
		}
		if (_keytest (RR_PLUS) && time > 1)  // Speed up or slow down text
			time--;
		if (_keytest (RR_MINUS) && time < 100)
			time++;
		
		// Scroll up and Copy virtual screens to Hidden gray planes
		ScrollUp240_R((short*) Hiddenl, 97);
		ScrollUp240_R((short*) Hiddend, 97);
		unsigned short offset2 = offset_x + 2610, a = 88;
		for(; a--; offset2-=30) {
			memcpy(Activel + offset2 + offset_y, Hiddenl + offset2, 14);
			memcpy(Actived + offset2 + offset_y, Hiddend + offset2, 14);
		}
		
		// Slow Down the Scrolling
		for(Wait = time; Wait--;)
			pokeIO(0x600005,0b11111);
		KeyScan();
	} while (_KeyCheck(ESCKEY | TEACHERKEY | (ENTERKEY * (g->Mode == TOURNAMENT))) == FALSE);
	
	if (_KeyCheck(TEACHERKEY))
		LeadOut();
}

/*if (_KeyCheck(UPKEY)) {
 	if (Changing == FALSE) {
		Changing = TRUE;
		if (--pos < 0) {
			if (--Main_Pos < 0) {
				pos = (No_Options > 6 ? 6 : No_Options - 1);
				Main_Pos = (No_Options > 6) * (No_Options - 7);
			} else pos = 0;
		}
		break;
	}
} else if (_KeyCheck(DOWNKEY)) {
	if (Changing == FALSE) {
		Changing = TRUE;
		if (++pos >= (No_Options > 6 ? 7 : No_Options)) {
			if (++Main_Pos >= (No_Options > 6) * (No_Options - 6))
				pos = Main_Pos = 0;
			else pos = 6;
		}
		break;
	}
} else Changing = FALSE;*/