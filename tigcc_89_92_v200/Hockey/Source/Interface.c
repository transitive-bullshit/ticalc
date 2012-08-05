// C Source File
// Created 10/4/2003; 3:29:28 PM
#include "All.h"
#include "asm_strings.h"
#include "Font.h"
static inline void Adjust_Contrast(void);
static inline void Redefine_Keys(void);
static inline short Get_Key(short);

void Main_Menu(void) {
	short Choice;
	
	/*	All menu items are in lower case because the Font only has one set of letters, but the 
	SpriteString Function converts the strings to lower case just in case	 */
	static menu Main[7] = { {"exhibition"}, {"tournament"}, {"shootout"}, {"practice"}, {"options"}, 
												 {"info"}, {"exit"} };
	typedef void (*P2Func)(void);
	static P2Func Menu_Funcs[] = 
	{&Exhibition, &Tournament, &ShootOut, &Practice, &Options, &Info, &LeadOut};
	
	do 
	{
		g->Link = FALSE;
		if ((Choice = Menu("- hockey 68k -", (char*)Main, 7, 0, TRUE)) == QUIT)
			break;
		if (Choice != 4)
			g->Status = Normal;
		else g->Status = Level_Setup;
		Menu_Funcs[Choice]();
	} while (1);  // Loop until Escape is pressed or the user chooses to Exit
	
	LeadOut();
}

/*	Returns the option chosen by the user	 */
short Menu(const char *Title, char Options[], char No_Options, char Start, short Fading) {
	static short Offset = 15;
	short pos = 0, width = 0;
	short x, y, a, Changing = 0;
	//, Flag;
	
	while_keypress(ESCKEY);
	for(a = No_Options; a--;) {
		short new_width = (strlen(Options + (Max_Length * a)) << 3);
		
		if (new_width > width)
			width = new_width;
	}
	
	//width -= (width%8) - 8;
	//x = (LCD_WIDTH-width)/2;// - 2;
	//width--;
	/*Flag = (x%8);
	x -= Flag;
	Flag = (Flag > 3);*/
	x = (LCD_WIDTH-width)/2 - 2;
	width += 2;
	pos = Start;
	
	do {
		
		Fill_Outside();
		SlowerSpriteX8_OR_R(Zerox, Zeroy, 100, NHLd, 20, Hiddenl);
		Distort_ScreenSafe(100, Offset, 20, Hiddenl + 425 * (!TI89), Hiddenl + 425 * (!TI89));
		//Distort_ScreenSafe(50, Offset, 20, Hiddenl + 50 * 30, Hiddenl + 50 * 30);
		
		SpriteString(0, Zeroy, (char*)Title, Hiddenl, Hiddend, A_CENTERED);
		for(a = No_Options; a--;) {
			if ((y = 13+12*(a-(pos-6)*(pos>6))) > 12 && y < 90)
				SpriteString(0, Zeroy + y, (char*)Options + (Max_Length * a), Hiddenl, Hiddend, A_CENTERED);
		}
		y = Zeroy + 12+12*(pos-(pos-6)*(pos>6));
		
		if (++Offset > 15)
			Offset = 0;
		// Make the selected Option Squiggly
		/*Distort_ScreenSafe(10, Offset, (width>>3)+Flag, Hiddenl + (x>>3) + 30 * y, 
			Hiddenl + (x>>3) + 30 * y);
		Distort_ScreenSafe(10, Offset, (width>>3)+Flag, Hiddend + (x>>3) + 30 * y, 
			Hiddend + (x>>3) + 30 * y);*/
		// Highlight the selected Option
		InvertGrayRect2B(x, y, x+width/*-1+(Flag<<3)*/, 9 + y, Hiddenl, Hiddend);
		
		// Draw the Two Calculators (Modified Gameboys) Linking at the bottom of the screen
		if (pos == 2 && !strcmp((char*)Options + (Max_Length * pos), "two player")) {
			GraySprite32_MASK_R(Zerox + 70, Zeroy + 79, 21, LinkPlay, LinkPlay + 21, 
				LinkPlay + 42, LinkPlay + 42, Hiddenl, Hiddend);
		}
		
		if (Fading) {  // Make the New Menu screen Fade in
			Dissolve();
			Fading = FALSE;
		} else {
			Toggle_Screens();
		}
		
		KeyScan();
		if (_KeyCheck(UPKEY)) {
		 	if (Changing == FALSE) {
				Changing = TRUE;
				if (--pos < 0)
					pos = No_Options - 1;
			}
		} else if (_KeyCheck(DOWNKEY)) {
			if (Changing == FALSE) {
				Changing = TRUE;
				if (++pos >= No_Options)
					pos = 0;
			}
		} else Changing = 0;
		
		/* Allow for the Backwards scrolling of the Time Limit in the Options Menu */
		if (((pos == 1 && g->Status == Level_Setup) || (pos == 2 && (g->Status == Level_Setup || 
		g->Link == FALSE))) && !strcmp(Title, "- Options -")) {
			char *Ptr;
			
			if (pos == 1)
				Ptr = &g->Time_Limit;
			else {
				Ptr = &g->Speed;
				if (_keytest(1 + 3 * (TI89), 1 + ((CALCULATOR > 0) << 2))) {
					if (g->Speed < 1) {
						g->Speed = 7;
						sprintf(Options + Max_Length * 2, "speed %d", g->Speed);
						Message((const char *const[]){"-- cheat --", "", "slow motion", "disabled", NULL}, 1);
					} else {
						g->Speed = -32;
						strcpy(Options + Max_Length * 2, "slow motion");
						Message((const char *const[]){"-- cheat --", "", "slow motion", "enabled", NULL}, 1);
					}
					Fading = TRUE;
				} else if (_keytest(3 - (CALCULATOR > 0) - (CALCULATOR > 0), 6 - 5 * (TI89))) {
					g->Friction = !g->Friction;
					Fading = TRUE;
					if (g->Friction)
						Message((const char *const[]){"-- cheat --", "", "friction enabled", NULL}, 1);
					else Message((const char *const[]){"-- cheat --", "", "friction disabled", NULL}, 1);
				} else if (_keytest(2 - (!TI89), 1 + 6 * (CALCULATOR > 0))) {
					g->Distortion = !g->Distortion;
					Fading = TRUE;
					if (g->Distortion)
						Message((const char *const[]){"-- cheat --", "", "distortion enabled", NULL}, 1);
					else Message((const char *const[]){"-- cheat --", "", "distortion disabled", NULL}, 1);
				}
			}
			
			if (_KeyCheck(LEFTKEY)) {
				if ((*Ptr -= 2) < 0)
					*Ptr += 15;
				while_keypress(LEFTKEY);
				return pos;
			} else if (_KeyCheck(RIGHTKEY)) {
				while_keypress(RIGHTKEY);
				return pos;
			}
		}
		
		if (_KeyCheck(ESCKEY | TEACHERKEY)) {
			if (_KeyCheck(TEACHERKEY) && (g->Mode != Game_Paused || g->Link == FALSE))
				LeadOut();
			while_keypress(ESCKEY);
			return QUIT;
		}
	} while (!_KeyCheck(SHOTKEY | ENTERKEY));
	while(_rowread(0));
	
	return pos;
}

// The Options Menu
void Options(void) {
	short Choice = 0, Fading = TRUE;
	
	menu Option_Menu[] = { {"level     "}, {"time            "}, {"speed       "}, {"redefine keys"}, 
												 {"team editor"}, {"adjust contrast"}, {"- back -"} };
	
	while(1) {
		strcpy(Option_Menu[0].Item, g->Difficulty ? "level hard" : "level easy");
		if (g->Speed < 1)
			strcpy(Option_Menu[2].Item, "slow motion");
		else sprintf(Option_Menu[2].Item, "speed %d", g->Speed);
		if (g->Status == Level_Setup) {
			sprintf(Option_Menu[1].Item, "time %d minutes", g->Time_Limit);
		} else {
			strcpy(Option_Menu[1].Item, "- -");
			strcpy(Option_Menu[4].Item, "- -");
			if (g->Link)
				strcpy(Option_Menu[2].Item, "- -");
		}
		
		Choice = Menu("- Options -", (char*)Option_Menu, 7, Choice, Fading);
		Fading = FALSE;
		
		switch(Choice) {
			case 0:								// Change Difficulty
				g->Difficulty = !g->Difficulty;
				break;
			case 1:								// Change Game Time
				if (g->Status == Level_Setup && ++g->Time_Limit > 15)
					g->Time_Limit = 1;
				break;
			case 2:
				if ((g->Status == Level_Setup || g->Link == FALSE) && (g->Speed < 1 || ++g->Speed > 15))
					g->Speed = 1;
				break;
			case 3:								// Redefine Keys
				Redefine_Keys();
				Fading = TRUE;
				break;
			case 4:								// Edit Teams
				if (g->Status == Level_Setup) {
					Editor_Menu();
					Fading = TRUE;
				}
				break;
			case 5:								// Adjust Contrast
				Adjust_Contrast();
				Fading = TRUE;
				break;
			default:							// Back
				return;
		}
	}
}

static inline void Adjust_Contrast(void) {
	short Choice = 0, Starting = TRUE;
	
	while(1) {
		Choice = Menu("- adjust contrast -", (char*)((menu[]){ {"contrast up"}, {"contrast down"}, 
		{"- back -"} }), 3, Choice, Starting);
		Starting = FALSE;
		
		if (Choice == 0)
			OSContrastUp();				// Contrast Up
		else if (Choice == 1)
			OSContrastDn();				// Contrast Down
		else return;						// Back
	}
}

/*TI89 ? "2nd shoots" : (TI92PLUS ? "f1 shoots" : "hand shoots"), 
	"", 
	TI89 ? "diamond switches" : (TI92PLUS ? "f2 switches" : "q switches"), 
	"plrs on defense", 
	"", 
	TI89 ? "shift passes" : (TI92PLUS ? "f5 passes" : "f1 passes"), */

/*"ice hockey", "v2.75", "--", "", "programmed by", "- fisch -", "", "", "released in", 
	"december 2003", "", "", "thanks to", "", "thomas nussbaumer,", "everyone at tict,", 
	"the tigcc team,", "apogee software,", "nintendo,", "jesystems,", "open source,", 
	"mr. mobius,", "malcolm smith,", "zeljko juric,", "kevin kofler,", "sebastian reichelt,", 
	"greg dietsche,", "scott noveck,", "lionel debroux,", "", 
	"and cisco, my cat", "--", "", "", 
	"default keys", "", 
	"- ti89 -", "2nd shoots", "diamond switches", "plrs on defense", "shift passes", 
	"f5 teacher key", 
	"", 
	"- ti92plus -", "f1 shoots", "f2 switches", "plrs on defense", "f5 passes", 
	"f4 teacher key", 
	"", 
	"- v200 -", 
	"plrs on defense", "hand shoots", "q switches", "plrs on defense", "f1 passes", 
	"f3 teacher key"
	"", "escape quits", "", "apps pauses", "--", "", "", "beta testers", "", 
	"clawsonia,", "deadbird,", "malcolm smith,", "kevin kofler,", "stoopidguy,", 
	"adrian bader,", "digital,", "jordan krage,", "lionel debroux,", "henry barrow,", 
	"Dan Barr", "--",*/


// Shows the Game Credits and Keys
void Info(void) {
	ScrollText(78);
}

/* Allow the User to Reconfigure the In-Game Key Layout */
static inline void Redefine_Keys(void) {
	unsigned short Cur_Key, Starting = TRUE;
	char buffer[40];
	static const char *const Keys[8] = {
		"Up", "Left", "Down", "Right", "Shoot", "Pass", "Switch Plrs", "Pause"
	};

/*	
	static const char *const TI89_rowread[7][8] = { 
		{"Up", "Left", "Down", "Right", "2nd", "Shift", "Diamond", "Alpha"}, 
		{"Enter", "+", "-", "*", "/", "^", "Clear", "F5"}, 
	  {"(-)", "3", "6", "9", ",", "T", "<-", "F4"}, 
	  {".", "2", "5", "8", ")", "Z", "CATALOG", "F3"}, 
	  {"0", "1", "4", "7", "(", "Y", "MODE", "F2"}, 
	  {"APPS", "STO", "EE", "|", "=", "X", "HOME", "F1"}, 
	  {"", "", "", "", "", "", "", "ESC"} 
	};
	
	static const char *const TI92plus_rowread[10][8] = {
	  {"2nd", "Diamond", "Shift", "Hand", "Left", "Up", "Right", "Down"}, 
	  {"", "Z", "S", "W", "F8", "1", "2", "3"}, 
	  {"", "X", "D", "E", "F3", "4", "5", "6"}, 
	  {"STO", "C", "F", "R", "F7", "7", "8", "9"}, 
	  {"Space", "V", "G", "T", "F2", "(", ")", ","}, 
	  {"/", "B", "H", "Y", "F6", "sin", "cos", "tan"}, 
	  {"^", "N", "J", "U", "F1", "ln", "Enter2", "P"}, 
	  {"=", "M", "K", "I", "F5", "Clear", "APPS", "*"}, 
	  {"BckSpc", "theta", "L", "O", "+", "MODE", "ESC", ""}, 
	  {"-", "Enter1", "A", "Q", "F4", "0", ".", "(-)"}, 
	};
*/	
	while(1) {
		
		Fill_Outside();  // For Ti-92+/v200 Models
		SlowerSpriteX8_OR_R(Zerox, Zeroy, 100, NHLd, 20, Hiddenl);  // Background
		
		for(Cur_Key = 8; Cur_Key--;) {
			strcpy(buffer, Keys[Cur_Key]);
			FS_DrawGrayString(Zerox + ((100-DrawStrWidth(buffer, F_6x8))>>1), Zeroy + (Cur_Key + 1) * 10, 
			buffer, A_NORMAL|A_SHADOWED, F_6x8);
			
			sprintf(buffer, ": %s", STRADDR((g->Keys[Cur_Key].Row << 3) + g->Keys[Cur_Key].Key, 
			 (CALCULATOR ? &keys_92plus : &keys_89)));
			
			FS_DrawGrayString(Zerox + 100, Zeroy + (Cur_Key + 1) * 10, buffer, A_NORMAL|A_SHADOWED, F_6x8);
		}
		SpriteString(0, Zeroy, (char*)"- keys -", Hiddenl, Hiddend, A_CENTERED);
		FS_DrawGrayString(0, Zeroy + 88, "Enter Redefines Keys", A_NORMAL|A_SHADOWED|A_CENTERED, F_4x6);
		FS_DrawGrayString(0, Zeroy + 95, "APPS sets Default Keys", A_NORMAL|A_CENTERED, F_4x6);
		
		if (Starting == TRUE) {
			Dissolve();
		} else {
			Toggle_Screens();
			Starting = TRUE;
		}
		
		while(_rowread(0));
		while(!_rowread(0));
		KeyScan();
		if (_KeyCheck(TEACHERKEY) && (g->Mode != Game_Paused || g->Link == FALSE)) {
			LeadOut();
		} else if (_keytest(RR_APPS)) {
			Compatibility();
			Starting = FALSE;
			continue;
		} else if (!_KeyCheck(ENTERKEY))
			return;
		
		// Loop through each Key and allow the User to change its value
		for(Cur_Key = 0; Cur_Key < 8; Cur_Key++) {
			Message((const char *const[]){"press and hold key", NULL}, -(Cur_Key != 0));
			
			strcpy(buffer, Keys[Cur_Key]);
			SpriteString(0, Zeroy + 46, buffer, Activel, Actived, A_CENTERED);
			if (Get_Key(Cur_Key) == QUIT)
				break;
		}
	}
}

/* Majority of function written by Malcolm Smith, from Oblivion */
/* Retrieve one new Reconfigured Key */
static inline short Get_Key(short Cur_Key) {
	short Row, Column;
	
	while(_rowread(0));
	while(!_rowread(0));
	KeyScan();
	if (_KeyCheck(TEACHERKEY)) {
		if (g->Mode != Game_Paused || g->Link == FALSE)
			LeadOut();
		return QUIT;
	} else if (_KeyCheck(ESCKEY))
		return QUIT;
	
	for(Row = (CALCULATOR ? 10 : 7); Row--;) {
		BEGIN_KEYTEST
			for(Column = 8; Column--;) {
				if (_keytest_optimized (Row, Column)) {
					g->Keys[Cur_Key].Row = Row;
					g->Keys[Cur_Key].Key = Column;
					Message((const char *const[]){"release key", NULL}, -1);
				}
			}
		END_KEYTEST
	}
	while(_rowread(0));
	return 0;
}