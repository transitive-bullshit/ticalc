									/* ------------------------------------------------------------------ */
									/*																																		*/
									/*											 Sumo Wrestling v0.20													*/
									/*													- By Fisch -															*/
									/*												 Travis Fischer															*/
									/*																																		*/
									/*											 phishfud@elknet.net													*/
									/*																																		*/
									/*																																		*/
									/*	Start:		May 22, 2004																						*/
									/*	Beta:			February 13, 2005																				*/
									/*	Release:	May 14, 2005																						*/
									/*																																		*/
									/*	I hope the source is fairly readable and well structured.  If			*/
									/*	there is a problem/bug with any aspect of the Game, or you know		*/
									/*	a way to improve it in any way, your e-mail is very welcome.  I		*/
									/*	have learned so much working on this Game and I hope that the 		*/
									/*	source will help others as well.																	*/
									/*																																		*/
									/* ------------------------------------------------------------------ */


/*#define COMMENT_PROGRAM_NAME		"Sumo Wrestling"
#define COMMENT_STRING					"By Travis Fischer  v0.20"
#define COMMENT_GRAY_ICON \
	{0x00,0x00,0x1B,0xE2,0x3F,0xF4,0x7F,0xF8,0xFF,0x88,0xFF,0x64,0x7B,0x34,0xF1,0x1C}, \
	{0xF4,0x62,0xF2,0x31,0xF8,0x01,0xFC,0x06,0x74,0x3C,0x32,0x64,0x0A,0x0C,0x05,0xF8}*/
#include "All.h"
#include "SumoGfx.h"
#include "Ring.h"
#include "Wrestlers.h"
#include "Info_Text.h"

// Local Functions specific to this C source file
static inline void Initialize_Game(void);
void LeadOut2(void) __attribute__ ((noreturn));

/*	Globals	 */
INT_HANDLER interrupt1;
INT_HANDLER interrupt5;
Clock  *clock;
char   *Home;
void   *Bulk;
void   *Hiddenl;
void   *Hiddend;
unsigned char (*Ring)[8][15];
volatile short count;

// Main Function
void _main(void)
{
	
	Initialize_Game();  // Setup all necessary game components
	
	
	/* Support for unexpected errors (including Kern0 ESC+ON breaks) */
	TRY  // Has to come here because Initialize_Game sets up the global register 
	// variable 'g' (which is referenced inside the ONERR ENDTRY block).  All 
	// register values are reset to their previous values before TRY, so if 'g' 
	// wasn't initialized until after the TRY, it would be corrupted if there 
	// was an error.
	
	//Post_Match();
	
	#ifdef ENGLISH
	static const char *const Main_menu[] = {  // Main Menu
		"Exhibition", "Tournament", "Sumo Editor", "Information", "High Scores", "Options", "Exit", NULL, 
	};
	static const char *const modes[] = {  // Game Mode Menu
		"Plr vs Calc", "Calc vs Calc", "Two Player", "Practice", "- Back", NULL, 
	};
	static const char *const Load_menu[] = {  // Main Menu
		"Load Game", "New Game", "- Back", NULL, 
	};
	#elif defined(FRENCH)
	
	#elif defined(GERMAN)
	
	#endif
	
															//-----------------------------------------------------------
	do 													// Main Menu Loop
	{														//-----------------------------------------------------------
		
		short Fading = TRUE, option = 0, a;
		unsigned char high[5][19];
		g->Link = FALSE;
		g->Mode = NONE;
		
		switch(Menu(EFG("- Sumo Wrestling -"), Main_menu, 0, TRUE, 1)) {
															//-----------------------------------------------------------
			case 0:                 // Exhibition Mode
															//-----------------------------------------------------------
				
				choose_mode:
				option = Menu(EFG("- Exhibition -"), modes, 0, TRUE, 2);
				g->wrestlers[0].Control = Host;
				g->wrestlers[1].Control = AI;
				g->Calc = Host;
				Fading = TRUE;
				
				// Determine mode of play
				if (option == FALSE) {					// Plr  vs Calc
					g->Mode = EXHIBITION;
				} else if (option == 1) {				// Calc vs Calc
					g->Mode = EXHIBITION;
					g->wrestlers[0].Control = AI;
				} else if (option == 2) {				// Two Player
					g->wrestlers[1].Control = Join;
					if (Establish_Connection() == TRUE)  // Linkplay established
						MAIN_LOOP();  // Begin Linkplay Match
					
					break;
				} else if (option == 3) {				// Practice
					g->Mode = PRACTICE;
				} else break;
				
				choose1:  // Choose first wrestler
				if (Choose_Wrestler(option == 1 ? EFG("Select wrestler") : EFG("Choose your wrestler"), 0, FALSE, 
				Fading) == QUIT)
					goto choose_mode;
				
				Fading = FALSE;
				choose2:  // Choose second wrestler
				if (Choose_Wrestler(option == 1 ? EFG("Select wrestler") : EFG("Choose your opponent"), 1, FALSE, 
				FALSE) == QUIT)
					goto choose1;
				if (Pre_Match() == QUIT)  // Handle Pre_Match screen and smack talk
					goto choose2;
				
				MAIN_LOOP();  // Begin Match
				break;
															//-----------------------------------------------------------
			case 1:                 // Tournament Mode
															//-----------------------------------------------------------
				g->Mode = TOURNAMENT;
				g->wrestlers[0].Control = Host;
				g->wrestlers[1].Control = AI;
				g->wrestlers[1].c = NULL;
				g->Calc = Host;
				
				short Loaded = FALSE;
				if (Get_Data_Ptr("sumtourn", 0)) {
					option = Menu(EFG("- Tournament -"), Load_menu, 0, TRUE, 0);
					
					if (option == 0) {
						Load_Tournament();
						Loaded = TRUE;
					}
					if (option == 2)
						break;
				}
				
				
				if (Loaded == TRUE || Choose_Wrestler(EFG("Choose your wrestler"), 0, FALSE, Fading) != QUIT)
					Tournament(Loaded);
				/*#ifdef ENGLISH
				// If I get enough positive feedback regarding Sumo Wrestling, I will continue the 
				// development of a very large Tournament mode.  (phishfud@elknet.net)
				Message((const char *const[]){ "Tournament mode", "is still under", "development", "", 
				"Following is a gfx", "gallery instead", NULL }, A_DISSOLVE | A_WAITKEY);
				#elif defined(FRENCH)
				
				#elif defined(GERMAN)
				
				#endif
				
				Gallery();*/
				break;
															//-----------------------------------------------------------
			case 2:                 // Wrestler Editor
															//-----------------------------------------------------------
				Wrestler_Editor();
				break;
															//-----------------------------------------------------------
			case 3:                 // Information Screen
															//-----------------------------------------------------------
				ScrollText(&info_text, NO_INFO_STRINGS);
				break;
															//-----------------------------------------------------------
			case 4:                 // Display High Scores
															//-----------------------------------------------------------
				for(a = 5; a--;)
					sprintf(high[a], "%-15s%3d", g->high[a].name, g->high[a].score);
				
				Message((const char *const[]){ EFG("High Scores:"), "--", high[4], high[3], high[2], high[1], 
					high[0], NULL }, A_DISSOLVE | A_WAITKEY);
				break;
															//-----------------------------------------------------------
			case 5:                 // Options Menu
															//-----------------------------------------------------------
				
				do 
				{
					char options[5][18];
					
					// Copy current preferences into the Options Menu as defaults
					if (g->Time_Limit == FALSE)
						strcpy(options[0], EFG("Time: No Limit"));
					else sprintf(options[0], EFG("Time: %d seconds"), g->Time_Limit);
					//if (g->Game_Speed < 0)
					//	strcpy(options[1], EFG("Slow Motion"));
					#ifdef ENGLISH
					sprintf(options[1], "Speed: %d", g->Game_Speed);
					sprintf(options[2], "Smack Talk: %s", (g->Smack_Talk ? "On" : "Off"));
					sprintf(options[3], "Difficulty: %s", (g->Difficulty ? "Hard" : "Easy"));
					sprintf(options[4], "Distortion: %s", (g->Distortion ? "On" : "Off"));
					#elif defined(FRENCH)
					
					#elif defined(GERMAN)
					
					#endif
					
					// Retrieve selection chosen by user in the Options menu
					option = Menu(EFG("- Options -"), (const char *const[]){ options[0], options[1], options[2], 
						options[3], options[4], EFG("Adjust Contrast"), EFG("- Back"), NULL }, option, Fading, 3);
					Fading = FALSE;
					
																				//-------------------------------------------------
					if (option == 0) {  					// Adjust Time Limit
																				//-------------------------------------------------
						/*if (g->Time_Limit < 0)
							g->Time_Limit = 120;
						else if (g->Time_Limit == FALSE) {
							g->Time_Limit = 20;
						} else {
							g->Time_Limit += 5;
							if (g->Time_Limit < 20 || g->Time_Limit > 120)
								g->Time_Limit = FALSE;
						}*/
						g->Time_Limit += 5;
						if (g->Time_Limit < 0)
							g->Time_Limit = 120;
						else if (g->Time_Limit > 120)
							g->Time_Limit = FALSE;
					} 														//-------------------------------------------------
					else if (option == 1) {				// Adjust Game Speed
																				//-------------------------------------------------
						if (g->Game_Speed < 1 || ++g->Game_Speed > MAX_GAME_SPEED)
							g->Game_Speed = 1;
					} 														//-------------------------------------------------
					else if (option == 2) {				// Toggle Smack Talk On/Off
																				//-------------------------------------------------
						g->Smack_Talk = !g->Smack_Talk;
					} 														//-------------------------------------------------
					else if (option == 3) {				// Change Game Difficulty
						 														//-------------------------------------------------
						g->Difficulty = !g->Difficulty;
					} 														//-------------------------------------------------
					else if (option == 4) {				// Toggle Screen Distortion On/Off
						 														//-------------------------------------------------
						g->Distortion = !g->Distortion;
					} 														//-------------------------------------------------
					else if (option == 5) {				// Adjust Contrast
																				//-------------------------------------------------
						short Choice = 0, Starting = TRUE;
						static const char *const contrast_menu[4] = {
							EFG("Contrast up"), EFG("Contrast down"), EFG("- Back"), NULL, 
						};
						
						while(1) {
							Choice = Menu(EFG("- Adjust Contrast -"), contrast_menu, Choice, Starting, 0);
							Starting = FALSE;
							
							if (Choice == 0)
								OSContrastUp();				// Contrast Up
							else if (Choice == 1)
								OSContrastDn();				// Contrast Down
							else break;							// Return to Options Menu
						}
						Fading = TRUE;
					} else break;               // Return to Main Menu
				} while (1);  // Options Menu loop
				break;
															//-----------------------------------------------------------
			default:                // Exit
															//-----------------------------------------------------------
				LeadOut();
		}
	} while (1);  // Loop until Escape is pressed or the user chooses to Exit
	
	ONERR
	
	// If there was an unexpected error, make sure everything is cleaned up 
	// before exiting and having an AMS popup error message
	LeadOut2();
	
	ENDTRY
}

/* Initializes and Preps the Game */
static inline void Initialize_Game(void) {
	/*	Disable old interrupts and install our own dummy interrupts	 */
	interrupt1 = GetIntVec(AUTO_INT_1);
	interrupt5 = GetIntVec(AUTO_INT_5);
	SetIntVec(AUTO_INT_1, INT_1_COUNTER);
	/*	-----------------------------------------------------------	 */
	unsigned char *wr_mirror;
	Home = NULL;
	Bulk = NULL;
	
	randomize();
	if ((Home = malloc(LCD_SIZE)) == NULL) {
		ST_helpMsg(EFG("Error, Not Enough Memory.  Free up RAM"/*, "Pas assez de mémoire.", 
			"Nicht genug Speicher"*/));
		LeadOut();
	}
	LCD_save(Home);  // Save LCD HomeScreen Contents
	
	ST_helpMsg(EFG("Calculating sprites..."));
	unsigned long mirror_size = 0;
	short a;
	for(a = MAX_RING_SPRITES; a--;) {
		if (Tiles[a] == NULL)
			mirror_size += TILE_SIZE;
	}
	for(a = NO_SPRITES; a--;) {
		short m = a;
		while(Wrestler_Light(m) == NULL)
			m = (Wrestlers[m].Height ? : m - 1);
		
		mirror_size += ((a < NO_MASKS) + 2 * (m != a)) * (Wrestler_Width(m) * Wrestlers[m].Height);
	}
	
	if ((Bulk = malloc((BULK_SIZE) + mirror_size + 8)) == NULL)
		ERROR(MEMORY);
	
	// Initiate Pointers within the allocated Bulk block
	Hiddenl   = Bulk;
	Hiddend   = (void*)((char*)Hiddenl + LCD_SIZE);
	wr_mirror = (char*)((char*)Hiddend + LCD_SIZE);
	g         = (GlobalVars*)((((long)wr_mirror + mirror_size) & ~3) + 4);
	clock     = (Clock*)((char*)g + sizeof(GlobalVars));
	Ring      = (unsigned char (*)[8][15])((char*)clock + sizeof(Clock));
	Ring      = (unsigned char (*)[8][15])(((long)Ring & ~3) + 4);
	g->box0   = (unsigned char*)((char*)Ring + (2 * sizeof(Ring_Matrix)));
	g->box1   = (unsigned char*)((char*)g->box0 + BOX_SIZE);
	g->buf    = (unsigned char (*)[192])((char*)g->box1 + BOX_SIZE);
	g->T_Parallax = (unsigned short*)((char*)g->buf + TILE_SIZE);
	g->text   = NULL;
	
	clock->Running = FALSE;
	Reset_Time();
	SetIntVec(AUTO_INT_5, CLOCK_TIMER);
	g->wrestlers[0].c = NULL;
	g->wrestlers[1].c = NULL;
	g->p_maskval  = (char*)Tiles[0];
	g->lcd_width  = LCD_WIDTH;
	g->lcd_height = LCD_HEIGHT;
	g->calculator = (CALCULATOR > 0);
	g->Zerox = 40 * g->calculator;
	g->Zeroy = 14 * g->calculator;
	g->plane_width = 11 + (g->calculator << 2); // TI89 ? 11 : 15;
	
	/* ----------------------------------------------------- */
	/* 								Load all External Data								 */
	/* ----------------------------------------------------- */
	Load_Config();	// Load the Configuration Settings and High Scores from the "sumocfg" File
	
	/*clrscr();
	printf("%lu, %lu (%lu)\n", (ULONG)Bulk, (BULK_SIZE) + mirror_size + 8, (long)((char*)&g->buf[3][191] - 
		(char*)Bulk));
	printf("%lu, %lu (%lu)\n", (ULONG)Hiddenl, (ULONG)LCD_SIZE, (ULONG)((char*)Hiddenl + LCD_SIZE));
	printf("%lu, %lu (%lu)\n", (ULONG)Hiddend, (ULONG)LCD_SIZE, (ULONG)((char*)Hiddend + LCD_SIZE));
	printf("%lu, %lu (%lu)\n", (ULONG)wr_mirror, (ULONG)mirror_size, (ULONG)((char*)wr_mirror + mirror_size));
	printf("%lu, %lu (%lu)\n", (ULONG)g, sizeof(GlobalVars), (ULONG)((char*)g + sizeof(GlobalVars)));
	printf("%lu, %lu (%lu)\n", (ULONG)clock, sizeof(Clock), (ULONG)((char*)clock + sizeof(Clock)));
	printf("%lu, %lu (%lu)\n", (ULONG)Ring, 2 * sizeof(Ring_Matrix), (ULONG)((char*)Ring + 
		2 * sizeof(Ring_Matrix)));
	printf("%lu, %lu (%lu)\n", (ULONG)g->box0, (ULONG)2 * BOX_SIZE, (ULONG)((char*)g->box0 + 2 * BOX_SIZE));
	printf("%lu, %lu (%lu)\n", (ULONG)g->buf, sizeof(char) * 192 * 4, (ULONG)((char*)g->buf + 
		sizeof(char) * 192 * 4));
	WaitKey();*/
	
	/*g->keys = FALSE;
	while(1) {  // Test the KeyScan function
		while(_rowread(0));
		while(!_rowread(0));
		KeyScan();
		clrscr();
		short a = 0;
		for(a = 8; a--;)
			printf("%d", ((g->keys & (1 << a)) != FALSE));
		printf("\n");
		if (_KeyCheck(UPKEY))
			printf("Up ");
		if (_KeyCheck(LEFTKEY))
			printf("Left ");
		if (_KeyCheck(DOWNKEY))
			printf("Down ");
		if (_KeyCheck(RIGHTKEY))
			printf("Right ");
		if (_KeyCheck(A_KEY))
			printf("A ");
		if (_KeyCheck(B_KEY))
			printf("B ");
	}*/
	
	// Initiate all mirrored ring sprites in the newly allocated block at wr_mirror
	for(a = 0; a < MAX_RING_SPRITES; a++) {
		if (Tiles[a] == NULL) {  // This tile is the mirror of the previous tile
			Tiles[a] = (unsigned short*)wr_mirror;
			// Mirror both tiles (Light/Dark) with one call
			SpriteX8_MIRROR_H_R(16 * 2, (char*)Tiles[a - 1], 2, (char*)Tiles[a]);
			
			wr_mirror += TILE_SIZE;
		}
	}
	
	// Initiate all mirrored wrestler sprites in the newly allocated block at wr_mirror
	// Also calculates the masks for each wrestler sprite
	for(a = 0; a < NO_SPRITES; a++) {
		short size;
		
		if (Wrestler_Light(a) == NULL) {  // This sprite is a mirror
			const short m = (Wrestlers[a].Height ? : a - 1);
			Wrestlers[a].Width  = Wrestlers[m].Width;
			Wrestlers[a].Height = Wrestlers[m].Height;
			const short Height = Wrestlers[a].Height, Width = Wrestler_Width(a);
			size = Width * Height;
			Wrestler_Light(a) = wr_mirror;
			Wrestler_Dark(a)  = wr_mirror + size;
			
			if (Wrestler_Mask(a)) {  // Mirror Light and Dark sprites Vertically
				SpriteX8_MIRROR_V_R(Height, Wrestler_Light(m), Width, wr_mirror);
				SpriteX8_MIRROR_V_R(Height, Wrestler_Dark(m), Width, wr_mirror + size);
				
				if (a >= NO_MASKS)
					Wrestler_Mask(a) = NULL;
				wr_mirror += (size + size);
			} else {  // Mirror Light and Dark sprites Horizontally
				SpriteX8_MIRROR_H_R(Height * 2, Wrestler_Light(m), Width, wr_mirror);
				
				// Shift the mirrored sprite to the left such that there will be no white pixels on the left
				// since there are white pixels on the right of the non-mirrored sprites
				// Ex.  10110000 -> mirrored = 00001101 -> shifted to the left = 11010000
				short wrestler_width  = Wrestlers[a].Width;
				register short shift1 = (wrestler_width & 7);  // & 7 is faster than a mod (%)
				if (shift1 > 0) {
					//register short mask   = 0xFF << shift1;
					register short b = (Height * 2);  // Apply shift to both light and dark sprites
					short shift2     = 8 - shift1;
					short width      = (wrestler_width >> 3);
					short c;
					for(; b--; wr_mirror++) {
						for(c = width; c--; wr_mirror++)
							*wr_mirror = (*wr_mirror << shift2) | (*(wr_mirror + 1) >> shift1);
						*wr_mirror = (*wr_mirror << shift2);
					}
				} else wr_mirror += (size + size);
			}
		} else size = Wrestler_Width(a) * Wrestlers[a].Height;
		
		if (a < NO_MASKS) {
			// Calculate this sprite's mask by taking the inverse of the Light and Dark sprites ORed together
			Wrestler_Mask(a) = wr_mirror;
			register short b = size;
			register unsigned char *light = Wrestler_Light(a);
			register unsigned char *dark = Wrestler_Dark(a);
			for(; b--;)
				*wr_mirror++ = ~(*light++ | *dark++);
			//wr_mirror += size;
		}
	}
	
	g->Characters = NULL;
	Load_Wrestlers();
	
	// Grayscale doublebuffering is messed up on HW1 normally, so we have to check if it is 
	// an HW1 and adjust accordingly
	//short *hwptr = (short *) FL_getHardwareParmBlock();
	//unsigned short HardWareVersion = *hwptr;
	//g->HW1 = (HardWareVersion <= 20 || IsVTI());
	
	if (!GrayOn())  // Turn Grayscale On
		ERROR(MEMORY);
}

#define Default_Char_Size			(sizeof(Character*) * g->NO_WRESTLERS)
#define External_Char_Size		(sizeof(Character) * No_External_Wrestlers)
#define Default_Str_Size			(sizeof(char*) * 5 * g->NO_WRESTLERS)
#define External_Str_Size			(One_String_Size * No_External_Wrestlers)
#define Free_Wrestlers()			SafeFree(g->Characters)

void Load_Wrestlers(void) {
	short No_External_Wrestlers  = Find_All_Files(NULL, FALSE);
	g->NO_WRESTLERS = NO_DEFAULT_WRESTLERS + No_External_Wrestlers;
	unsigned long Character_Size = Default_Char_Size + External_Char_Size;
	unsigned long String_Size = Default_Str_Size + External_Str_Size;
	
	Free_Wrestlers();
	// Allocate memory for Wrestler Character and String buffers
	if ((g->Characters = (Character**)malloc(Character_Size + String_Size)) == NULL)
		ERROR(MEMORY);
	g->strings = (char**)((char*)g->Characters + Character_Size);
	
	//g->strings[5 * (Load_Slot + Counter) + a]
	
	short a;
	for(a = 0; a < No_External_Wrestlers; a++) {
		short index = 5 * (Load_Slot + a);
		g->strings[index + 0] = ((char*)g->strings + Default_Str_Size + a * One_String_Size);
		g->strings[index + 1] = g->strings[index + 0] + 32;
		g->strings[index + 2] = g->strings[index + 1] + 32;
		g->strings[index + 3] = g->strings[index + 2] + 256;
		g->strings[index + 4] = g->strings[index + 3] + 256;
		g->Characters[Load_Slot + a] = (Character*)((char*)g->Characters + Default_Char_Size + 
			a * sizeof(Character));
		g->Characters[Load_Slot + a]->Index = Load_Slot + a;
	}
	memset(g->strings[5 * Load_Slot], 0, External_Str_Size);
	
	// Initiate Default Characters (structures and strings)
	for(a = NO_DEFAULT_WRESTLERS; a--;) {
		g->Characters[a] = &Default_Characters[a];
		register short b;
		for(b = 5; b--;)
			g->strings[5 * a + b] = STRADDR_MASTER(5 * a + b, &strings);
			//Char_String_Addr(5 * a + b);
	}
	
	// Initiate External Characters (structures and strings)
	Find_All_Files(NULL, TRUE);
	
	
	//if ((g->Characters[Load_Slot] = 
	//(Character*)malloc(sizeof(Character) * (No_External_Wrestlers + 1))) == NULL) {
	//	ERROR(MEMORY);
	//}
	//(Character*)((char*)(g->Characters + sizeof(Character*) * NO_DEFAULT_WRESTLERS));
}

/* Uh-Oh */
void __attribute__ ((noreturn)) ERROR(const unsigned char error) {
	static const char * const Error_Msg[4] = {
		EFG("Not enough Memory"/*, "Pas assez de mémoire", "Nicht genug Speicher"*/), 
		EFG("Archive files to free RAM"), 
		
		EFG("Link Error"/*, "Erreur de transmission", "Übertragefehler"*/), 
		EFG("Check Link Connection and Retry"), 
	};
	
	if (Bulk && g->Link == TRUE)
		Exit_Linkplay();
	
	GrayOff();
	PortSet(LCD_MEM, 239, 127);
	
	FontSetSys(F_6x8);
		DrawStr(0, 0, Error_Msg[error], A_NORMAL);
	FontSetSys(F_4x6);
		DrawStr(0, 12, Error_Msg[error+1], A_NORMAL);
	if (error == MEMORY)
		DrawStr(0, 20, EFG("Need about 125,000 bytes of RAM"), A_NORMAL);
		DrawStr(0, LCD_HEIGHT - 6, EFG("Contact phishfud@elknet.net"/*, 
		"Contactez phishfud@elknet.net (En anglais)", "Kontakt: phishfud@elknet.net (auf Englisch)"*/), A_NORMAL);
	while(!(_keytest (RR_ENTER) || _keytest (RR_ESC) || _keytest (RR_F1)));
	while(_keytest (RR_ENTER));
	
	LeadOut();
}

void __attribute__ ((noreturn)) LeadOut(void)  // Thanks to Kevin Kofler for this suggestion
{
	ER_success();  // Before exiting from inside of a TRY block, ER_success(); MUST be called
	LeadOut2();
	
	exit(0);  // Dummy Exit so the compiler won't give Warnings about this being a noreturn function
	// It will Never be called, though, because LeadOut2 calls exit(0); first.
}

/* Frees all possibly allocated memory, cleans up and exits */
void __attribute__ ((noreturn)) LeadOut2(void) {
	GrayOff();
	if (Bulk) {
		Save_Config();  // Save user's preferences in an external file
		while_keypress(TEACHERKEY);
	}
	// Restore default interrupts
	SetIntVec(AUTO_INT_1, interrupt1);
	SetIntVec(AUTO_INT_5, interrupt5);
	OSLinkClose();  // Clean up Link
	
	/*  Free all allocated Memory  */
	if (Bulk) {
		Free_Wrestlers();
		if (g->text)
			free(g->text);
		
		free(Bulk);
	}
	
	// Restore the Home Screen, this is pretty much what #define SAVE_SCREEN does
	if (Home) {
		GKeyFlush();
		
		PortSet(LCD_MEM,239,127);
		LCD_restore(Home);
		ST_helpMsg(EFG("Sumo Wrestling v0.20 -- By Fisch"/*, "Sumo Wrestling v0.20 -- Fisch", 
			"Sumo Wrestling v0.20 -- von Fisch"*/));
		free(Home);
	}
	
	exit(0);  // Exit the Game
}