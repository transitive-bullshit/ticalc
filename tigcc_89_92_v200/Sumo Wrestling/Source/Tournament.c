// C Source File
// Created 7/10/2005; 9:33:19 PM
#include "All.h"
#include "Storyline.h"
static inline void Initiate_Tournament(BOOL*, short);
static inline short Run_Tournament(void);
static inline void Draw_Tournament(void);
static inline void Draw_Sumos(void);
static inline void Showdown(void);
void Handle_EndofGame(const char *const);
static inline void Handle_Parallax(const short);

#define T_START							1
#define CONVERT_SCORE(s)		(((s) - T_START) * 40)
enum Parallax_Types { SCROLL_UP, SCROLL_DOWN, SCROLL_LEFT, SCROLL_RIGHT, SCROLL_UPLEFT, 
											SCROLL_DOWNLEFT, SCROLL_UPRIGHT, SCROLL_DOWNRIGHT, RANDOM_PARALLAX };

void Tournament(short Loaded) {
	static const char *const Save_Menu[] = {  // Save Menu
		"Exit", "Save and Exit", "Save and Continue", "Continue", NULL, 
	};
	
	BOOL Tournament_World;
	Initiate_Tournament(&Tournament_World, Loaded);
	const short Random_Start = random(NO_DEFAULT_WRESTLERS);
	
	do 
	{
		if (Tournament_World) {
			while(Run_Tournament() == QUIT) {  // Run the Walkway sequence
				short Choice = Menu(EFG("- Tournament -"), Save_Menu, 0, TRUE, 0);
				
				if (Choice == 1 || Choice == 2) {
					if (Save_Tournament() != TRUE) {
						Message((const char *const[]){ "- Error -", "", "Game could not", "be saved", NULL }, 
						A_DISSOLVE | A_WAITKEY);
					}
					
					if (Choice == 1)  // Save and Exit
						return;
				} else if (Choice != 3) {
					Handle_EndofGame("End of Game");
					return;
				}
			}
		}
		
		g->wrestlers[1].c = g->Characters[(g->Progress + Random_Start) & 15];
		if (Pre_Match() == QUIT) { // Handle Pre_Match screen and smack talk
			Tournament_World = FALSE;
			continue;
		}
		MAIN_LOOP();  // Wrestle the match
		if (!(Tournament_World = (g->Status == Match_Won_0))) {  // Player failed to continue
			short calc = g->calculator;
			if (_keytest(5-2*calc, !calc))
				continue;  // Try again (continue tournament)
			
			Handle_EndofGame("You failed!");
			return;
			/*short Choice = Menu(EFG("- Tournament -"), Lose_menu, 0, TRUE, 4);
			
			if (Choice == 1)  // Exit Tournament
				return;
			if (Choice == 2)  // Restart Tournament
				Initiate_Tournament(&Tournament_World);
			
			continue;  // Try again (continue tournament)*/
		}
		
		g->Progress++;  // Each defeated opponent
	} while(1);
}

// Setup Tournament Variables
static inline void Initiate_Tournament(BOOL *Tournament_World, short Loaded) {
	// Setup Parallax scrolling tiles
	g->Parallax = random(RANDOM_PARALLAX);
	g->delay = 0;
	
	*Tournament_World = TRUE;
	
	if (!Loaded) {
		g->Progress = T_START;
		g->x = 0;
		
		// Display (purposefully) crappy storyline D)
		ScrollText(&story, NO_STORY_STRINGS);
	}
}

// Run the Walkway/Passage portion of the Tournament
static inline short Run_Tournament(void) {
	short Starting = TRUE;
	// Initiate parallax scrolling routines
	memcpy(g->T_Parallax, T_Parallax_Tile, TILE_SIZE);
	if (++g->Parallax >= RANDOM_PARALLAX)
		g->Parallax = SCROLL_UP;
	if (g->Progress > RANDOM_PARALLAX) {
		g->Parallax = RANDOM_PARALLAX;
		g->Difficulty = TRUE;
	}
	
	do {
		Draw_Tournament();
		if (++g->delay >= 16)  // For animating Stars
			g->delay = 0;
		
		KeyScan();  // Update Keypresses
		if (_KeyCheck(RIGHTKEY)) {
			g->x++;
			
			// Hit detection check with opponents
			if (g->x + 45 > 240 * g->Progress) {
				Showdown();
				
				return 0;
			}
		}
		Draw_Sumos();
		
		if (Starting) {
			Dissolve();
			Starting = FALSE;
		} else Toggle_Screens();
		
		KeyScan();
		if (_KeyCheck(TEACHERKEY))
			LeadOut();
	} while(!_KeyCheck(ESCKEY));
	
	return QUIT;
#if 0	
Tournament Ideas:
Long passage (bridge), cooridor-like, 2 planes scrolling distinct speeds (back slower)
Left to Right, have maze included
Wrestler opponents, have them Stomp when confronted, then go into Pummel with you, then 
Dissolve into Pre_Match.  After match, either throw them to the Left or toss them up and 
have them fall down
#endif
}

// Display the background walkway and scrolling tiles
static inline void Draw_Tournament(void) {
	ClearGrayScreen2B_R(Hiddenl, Hiddend);
	Handle_Parallax((g->Parallax == RANDOM_PARALLAX ? random(RANDOM_PARALLAX) : g->Parallax));
	const short max = (6 + 2 * g->calculator), softx = (g->x > 30 ? g->x & 15 : 0);
	register short x, y = max;
	unsigned short *Tile = Walkway[0];
	
	// Draw all Background Tiles
	for(; y--;) {
		
		asm volatile("
			tst.w %0
			smi.b %0 | if negative fill lower byte with 1s, else with 0s
			ext.w %0 | copy what is in the lower byte into the upper byte"
			: "=d" (x)
			: "0" (softx)
		);  // Thanks to Samuel Stearly
		
		for(; x < g->plane_width; x++)
			GrayTile_Clipped((x << 4) - softx, (y << 4), Tile);
		
		if (y == max - 1)
			Tile = Walkway[1];
		if (y == max - 2)
			Tile = g->T_Parallax;
	}
	
	// Display animated Stars (which show the player's progression)
	y = (g->delay >> 2);
	for(x = g->Progress - T_START; x--;) {
		const short offset = (x / (g->lcd_width >> 4)), index = ((x + y + offset) & 3);
		if (offset < (g->lcd_height >> 4)) {
			WRESTLER *star = &Wrestlers[Star1 + index];
			const unsigned char *mask = ((index & 1) ? star->Mask : maskstar[index >> 1]);
			
			// Draw one star masked over the background
			GraySpriteX8_MASK((x << 4) - g->lcd_width * offset, (offset << 4), 16, star->Light, star->Dark, 
				mask, mask, 2, Hiddenl, Hiddend);
		}
	}
	
	// Display the Score
	char buf[24];
	sprintf(buf, "Score: %d ", CONVERT_SCORE(g->Progress));
	DrawGrayString(0, g->lcd_height - 5, buf, A_REPLACE | A_CENTERED, F_4x6);
}

// Draw "Steve" and opponent on Walkway, as well as the Sign in the beginning
static inline void Draw_Sumos(void) {
	short gfx = Walk2 + 2 * ((g->x & 15) < 8), x = (g->x <= 30), zero = 39 + 2 * g->Zeroy;
	if (x) { // Draw sign pointing right on Walkway
		GraySpriteX8_MASK(140, zero, 32, Wrestler_Light(Sign), Wrestler_Dark(Sign), Wrestler_Mask(Sign), 
		Wrestler_Mask(Sign), 2, Hiddenl, Hiddend);
	}
	zero += 3;
	
	// Draw "Steve" on Walkway
	GraySpriteX8_MASK((x ? g->x : 30), zero, Wrestler_Height(gfx), Wrestler_Light(gfx), 
		Wrestler_Dark(gfx), Wrestler_Mask(gfx), Wrestler_Mask(gfx), Wrestler_Width(gfx), Hiddenl, Hiddend);
	
	// Draw opponent on Walkway
	x = 240 * g->Progress - g->x;
	if (x < g->lcd_width)
		GraySpriteX8_MASK(x, zero, Wrestler_Height(Walk1), Wrestler_Light(Walk1), 
			Wrestler_Dark(Walk1), Wrestler_Mask(Walk1), Wrestler_Mask(Walk1), Wrestler_Width(Walk1), 
			Hiddend, Hiddenl);
}

// When wrestlers collide, they go into a pummel before initiating the Pre_Match smack talk
static inline void Showdown(void) {
	GraySpriteX8_MASK(30, 41, Wrestler_Height(Pummel1), Wrestler_Light(Pummel1), 
		Wrestler_Dark(Pummel1), Wrestler_Mask(Pummel1), Wrestler_Mask(Pummel1), Wrestler_Width(Pummel1), 
		Hiddend, Hiddenl);
	
	Toggle_Screens();
	
	short a = 15;
	for(; a--;)
		pokeIO(0x600005,0b11101);
}

void Handle_EndofGame(const char *const message) {
	const unsigned short Score = CONVERT_SCORE(g->Progress);
	char buf[16];
	short a, b;
	
	sprintf(buf, "%d", Score);
	// Display End of Tournament Message
	Message((const char *const[]){ EFG("- Tournament -"), "", message, "", "Score:", buf, NULL }, 
		A_DISSOLVE | A_WAITKEY);
	
	// Update High Scores
	for(a = 5; a--;) { // Check if new Score is greater than any of the previous high scores
		if (Score > g->high[a].score) {  // New High Score!!!
			buf[0] = '\0';
			if (Get_Input("New High Score!", buf, 14, NULL) != QUIT) {  // Input player's name
				for(b = 0; b < a; b++)  // Shift old high scores down
					memcpy(&g->high[b], &g->high[b + 1], sizeof(HSC));
				
				// Insert new high score data
				strcpy(g->high[a].name, buf);
				g->high[a].score = Score;
			}
			
			break;
		}
	}
}

/* Parallax Scrolling Tile Routines */
static inline void Handle_Parallax(const short Type) {
	register unsigned short a = 15, *Ptr = g->T_Parallax, Templ, Tempd;
	
	// Note:  Compatible sprites are stored in the format 16 short Light, followed by 16 short Dark data
	if (Type == SCROLL_UP) {
		/* Scroll a 16x16 Sprite One Pixel Up */
		/* ---------------------------------- */
		
		Templ = *Ptr;
		Tempd = *(Ptr + 16);
		
		for(; a--; Ptr++) {
			*Ptr = *(Ptr + 1);
			*(Ptr + 16) = *(Ptr + 17);
		}
		
		*Ptr = Templ;
		*(Ptr + 16) = Tempd;
	} else if (Type == SCROLL_DOWN) {
		/* Scroll a 16x16 Sprite One Pixel Down */
		/* ------------------------------------ */
		
		Ptr += 15;
		Templ = *Ptr;
		Tempd = *(Ptr + 16);
		
		for(; a--; Ptr--) {
			*Ptr = *(Ptr - 1);
			*(Ptr + 16) = *(Ptr + 15);
		}
		
		*Ptr = Templ;
		*(Ptr + 16) = Tempd;
	} else if (Type == SCROLL_LEFT) {
		/* Scroll a 16x16 Sprite One Pixel to the Left */
		/* ------------------------------------------- */
		
		for(a = 16; a--; Ptr++) {
			*Ptr = (*Ptr << 1) | ((*Ptr & 0xFFFE) >> 15);
			*(Ptr + 16) = (*(Ptr + 16) << 1) | ((*(Ptr + 16) & 0xFFFE) >> 15);
		}
	} else if (Type == SCROLL_RIGHT) {
		/* Scroll a 16x16 Sprite One Pixel to the Right */
		/* -------------------------------------------- */
		
		for(a = 16; a--; Ptr++) {
			*Ptr = (*Ptr >> 1) | ((*Ptr & 0x1) << 15);
			*(Ptr + 16) = (*(Ptr + 16) >> 1) | ((*(Ptr + 16) & 0x1) << 15);
		}
	} else if (Type == SCROLL_UPLEFT) {
		/* Scroll a 16x16 Sprite One Pixel to the Left and One Pixel Up */
		/* ------------------------------------------------------------ */
		
		Templ = *Ptr << 1;
		Tempd = *(Ptr + 16) << 1;
		
		for(; a--; Ptr++) {
			*Ptr = (*(Ptr + 1) << 1) | ((*(Ptr + 1) & 0x8000) >> 15);
			*(Ptr + 16) = (*(Ptr + 17) << 1) | ((*(Ptr + 17) & 0x8000) >> 15);
		}
		
		*Ptr = Templ | ((*Ptr & 0x8000) >> 15);
		*(Ptr + 16) = Tempd | ((*(Ptr + 16) & 0x8000) >> 15);
	} else if (Type == SCROLL_DOWNLEFT) {
		/* Scroll a 16x16 Sprite One Pixel to the Left and One Pixel Down */
		/* -------------------------------------------------------------- */
		
		Ptr += 15;
		Templ = *Ptr << 1;
		Tempd = *(Ptr + 16) << 1;
		
		for(; a--; Ptr--) {
			*Ptr = (*(Ptr - 1) << 1) | ((*(Ptr - 1) & 0x8000) >> 15);
			*(Ptr + 16) = (*(Ptr + 15) << 1) | ((*(Ptr + 15) & 0x8000) >> 15);
		}
		
		*Ptr = Templ | ((*Ptr & 0x8000) >> 15);
		*(Ptr + 16) = Tempd | ((*(Ptr + 16) & 0x8000) >> 15);
	} else if (Type == SCROLL_UPRIGHT) {
		/* Scroll a 16x16 Sprite One Pixel to the Right and One Pixel Up */
		/* ------------------------------------------------------------- */
		
		Templ = *Ptr >> 1;
		Tempd = *(Ptr + 16) >> 1;
		
		for(; a--; Ptr++) {
			*Ptr = (*(Ptr + 1) >> 1) | ((*(Ptr + 1) & 0x1) << 15);
			*(Ptr + 16) = (*(Ptr + 17) >> 1) | ((*(Ptr + 17) & 0x1) << 15);
		}
		
		*Ptr = Templ | ((*Ptr & 0x1) << 15);
		*(Ptr + 16) = Tempd | ((*(Ptr + 16) & 0x1) << 15);
	} else if (Type == SCROLL_DOWNRIGHT) {
		/* Scroll a 16x16 Sprite One Pixel to the Right and One Pixel Down */
		/* --------------------------------------------------------------- */
		
		Ptr += 15;
		Templ = *Ptr >> 1;
		Tempd = *(Ptr + 16) >> 1;
		
		for(; a--; Ptr--) {
			*Ptr = (*(Ptr - 1) >> 1) | ((*(Ptr - 1) & 0x1) << 15);
			*(Ptr + 16) = (*(Ptr + 15) >> 1) | ((*(Ptr + 15) & 0x1) << 15);
		}
		
		*Ptr = Templ | ((*Ptr & 0x1) << 15);
		*(Ptr + 16) = Tempd | ((*(Ptr + 16) & 0x1) << 15);
	}
}