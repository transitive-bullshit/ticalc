// C Source File
// Created 9/7/2003; 6:41:15 PM
#include "Structures.h"
register Game *g asm("a4");
#include "All.h"
#ifdef EXTERNAL_GFX
#include "HockeyGfx.h"
#else
#include "Data_Gfx.h"
#include "Teams.h"
#endif
#include "faststr.h"
static inline void Initialize_Game(void);

/* ------------------------------------------------------------------ */
/*																																		*/
/*												Hockey 68k v2.75														*/
/*													- By Fisch -															*/
/*																																		*/
/*											 phishfud@elknet.net													*/
/*																																		*/
/*																																		*/
/*	Started: September 7, 2003																				*/
/*	Beta1:	 October 30, 2003																					*/
/*	Public:  December 02, 2003																				*/
/*	Ended:   																													*/
/*																																		*/
/*	I hope the source is fairly readable and well structured.  If			*/
/*	there is a problem/bug with any aspect of the Game, or you know		*/
/*	a way to improve it in any way, your e-mail is very welcome.  I		*/
/*	have learned So much working on this Game and I hope that the 		*/
/*	source will help others as well.																	*/
/*																																		*/
/* ------------------------------------------------------------------ */

/*#ifdef USE_TI92PLUS  // Just a reminder to myself
#warning "compiling for Ti-92+/v200"
#else
#warning "compiling for Ti-89"
#endif*/

/*	Globals	 */
INT_HANDLER interrupt1;
INT_HANDLER interrupt5;
Team   *Team1;
Team   *Team2;
Clock  *clock;
char   *Home;
void   *Bulk;
#ifdef COMPRESSION
unsigned char *NHLl;
unsigned char *NHLd;
#endif
Names *Western;
Names *Eastern;
void  *Hiddenl;
void  *Hiddend;

/* Base taken from TICT Tutorial SP12, Example 3 and modified */
/* Used for Keeping track of Time during the Games */
DEFINE_INT_HANDLER (CLOCK_TIMER) {
	if (clock->Reset_Time) {
		clock->Mseconds50 = clock->Seconds = clock->Minutes = clock->Reset_Time = 0;
	}
	//------------------------------------------
	// count interrupts only if running is not 0
	//------------------------------------------
	else if (clock->Running == TRUE && ++clock->Mseconds50 >= 19) {
		clock->Mseconds50 -= 19;
		if (++clock->Seconds >= 60) {
			clock->Seconds -= 60;
			++clock->Minutes;
			// There is no need for hours or days because the hockey matches cannot last that long
		}
	}
}

/*void Gallery(void) {
	GrayClearScreen2B(Activel, Actived);
	short a = 0, x = 0, y = 0;
	
	do {
		
		GraySprite16_OR(x, y, 16, PlrGfx[a], PlrGfx[a] + 16, Activel, Actived);
		
		if ((x += 16) >= LCD_WIDTH - 15) {
			x = 0;
			if ((y += 16) >= LCD_HEIGHT - 15) {
				y = 0;
				WaitKey();
				GrayClearScreen2B(Activel, Actived);
			}
		}
	} while(++a < 20);
	
	a = 0;
	do {
		
		GraySprite16_OR(x, y, 16, Blocks[a], Blocks[a] + 16, Activel, Actived);
		
		if ((x += 16) >= LCD_WIDTH - 15) {
			x = 0;
			if ((y += 16) >= LCD_HEIGHT - 15) {
				y = 0;
				WaitKey();
				GrayClearScreen2B(Activel, Actived);
			}
		}
	} while(++a < H9);
	
	WaitKey();
}*/

// Main Function
void _main(void)
{
	
	Initialize_Game();
	
	//Gallery();
	TRY
	
	#if !defined(DEBUG)
	Show_Title();
	#endif
	
	Main_Menu();
	
	//FINALLY // (caused random Address Error crashes upon program exit in v0.62)
	ONERR
	// <error handler>
	LeadOut2();
	
	//ENDFINAL
	ENDTRY
}

//=============================================================================

// VTI detection routine (thanx go to Julien Muchembled)

//=============================================================================
short IsVTI(void);
asm("
IsVTI:
    trap   #12         /* enter supervisor mode. returns old (%sr) in %d0.w   */
    move.w #0x3000,%sr /* set a non-existing flag in %sr (but keep s-flag !!) */
    swap   %d0         /* save %d0.w content in upper part of %d0             */
    move.w %sr,%d0     /* get %sr content and check for non-existing flag     */
    btst   #12,%d0     /* this non-existing flag can only be set on the VTI   */
    bne    __VTI           
    swap   %d0         /* restore old %sr content and return 0                */
    move.w %d0,%sr
    moveq  #0,%d0
    rts
__VTI:
    swap   %d0         /* restore old %sr content and return 1                */
    move.w %d0,%sr
    moveq  #1,%d0
    rts
");


/* Initializes and Preps the Game */
static inline void Initialize_Game(void) {
	/*	Disable old interrupts and install our own dummy interrupts	 */
	interrupt1 = GetIntVec(AUTO_INT_1);
	interrupt5 = GetIntVec(AUTO_INT_5);
	SetIntVec(AUTO_INT_1, DUMMY_HANDLER);
	/*	-----------------------------------------------------------	 */
	short a;
	Bulk = NULL;
	Home = NULL;
	Western = NULL;
	Eastern = NULL;
	
	if ((Home = malloc(LCD_SIZE)) == NULL) {
		ST_helpMsg("Error, Not Enough Memory.  Free up RAM");
		LeadOut();
	}
	LCD_save(Home);  // Save LCD HomeScreen Contents
	
	/* Allocate all memory in one block, and then initiate the pointers afterward */
	/* -- Suggested by Lionel Debroux -- */
	#ifdef COMPRESSION
	if ((Bulk = malloc(Bulk_Size + 4002)) == NULL)
		ERROR(MEMORY);
	#else
	if ((Bulk = malloc(Bulk_Size)) == NULL)
		ERROR(MEMORY);
	#endif
	
	// Setup Pointers, within the allocated Bulk block
	Team1   = (Team*)Bulk;
	Team2   = (Team*)((char*)Team1 + sizeof(Team));
	Hiddenl = (void*)((char*)Team2 + sizeof(Team));
	Hiddend = (void*)((char*)Hiddenl + HIDDEN_SIZE);
	g       = (Game*)((char*)Hiddend + HIDDEN_SIZE);
	clock   = (Clock*)((char*)g + sizeof(Game));
	#ifdef COMPRESSION
	NHLl    = (unsigned char*)((char*)clock + sizeof(Clock));
	NHLd    = (unsigned char*)((char*)NHLl + 2001);
	#endif
	/* Initiate all other values */
	for(a = 3; a--;) {
		Team1->plrs[a] = NULL;
		Team2->plrs[a] = NULL;
	}
	g->Teams_Changed = FALSE;
	g->Friction      = TRUE;
	g->Distortion    = FALSE;
	Compatibility();
	Allocate_Players(Team1);
	Allocate_Players(Team2);
	randomize();
	
	// Take care of the Game's Clock values
	clock->Running    = FALSE;
	clock->Reset_Time = TRUE;
	SetIntVec(AUTO_INT_5, CLOCK_TIMER);  // Turn the Clock Interrupt On
	// The Clock Interrupt uses values in the clock struct, which has to be allocated before 
	// the clock can be turned on
	
	
	/*g->keys = FALSE;
	while(1) {  // Test the KeyScan function
		while(_rowread(0));
		while(!_rowread(0));
		KeyScan();
		clrscr();
		register short a = 7;
		for(; a--;)
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
		if (_KeyCheck(SHOTKEY))
			printf("Shot ");
		if (_KeyCheck(PASSKEY))
			printf("Pass ");
		if (_KeyCheck(CHANGEKEY))
			printf("Change ");
	}*/
	
	
	
	
	/* ----------------------------------------------------- */
	/* 								Load all External Data								 */
	/* -----------------------------------------------------
	   Hockey exceeded the 64kb Program Size Limit, so I had 
	   to put some of the Data into external Files.  For 
	   more information, look in the SaveGfx Project in the 
	   External Data Folder or in the External.c File.
	   ----------------------------------------------------- */
	#ifdef EXTERNAL_GFX
	Load_Gfx();			// Load Graphics from the .......................... "hockgfx" File
	#endif
	Load_Config();	// Load the User's Configurational Settings from the "hockycfg" File
	Load_Teams();		// Load the User Customizable Teams from the ....... "hockteam" File
	
	#ifdef COMPRESSION
	RLE_Decompress(RLE_NHLl, NHLl, 1999);  // Buggy...
	RLE_Decompress(RLE_NHLd, NHLd, 1999);
	*((char*)NHLl + 1) = 0xE7;
	*((char*)NHLd + 1) = 0x18;
	#endif
	
	if (SetupCharSet() == FALSE)  // Set up the (Beta) Fast String Drawing Routines by TICT
		ERROR(FONT_LOADING);
	
	// Timing is different on HWq, so we have to check if it is an HW1 and adjust
	short *hwptr = (short *) FL_getHardwareParmBlock();
	unsigned short HardWareVersion = *hwptr;
	g->HW1 = (HardWareVersion <= 20 || IsVTI());
	
	if (!GrayOn())  // Turn Grayscale on
		ERROR(MEMORY);
}

void Allocate_Players(Team *team) {
	short Loop = 3;
	
	for(; Loop--;) {
		if ((team->plrs[Loop] = malloc(sizeof(Plr))) == NULL)
			ERROR(MEMORY);
	}
}

// Initializes some Calculator Constants, depending on what calc type the game is being run on
void Compatibility(void) {
	/*g->Keys[0] = (RowKey) {RR_ESC};
	g->Keys[1] = (RowKey) {RR_UP};
	g->Keys[2] = (RowKey) {RR_LEFT};
	g->Keys[3] = (RowKey) {RR_DOWN};
	g->Keys[4] = (RowKey) {RR_RIGHT};
	g->Keys[5] = (RowKey) {RR_APPS};
	g->Keys[6] = TI89 ? (RowKey) {RR_2ND} : (TI92PLUS ? (RowKey) {RR_F1} : (RowKey) {RR_HAND});
	g->Keys[7] = TI89 ? (RowKey) {RR_SHIFT} : (TI92PLUS ? (RowKey) {RR_F5} : (RowKey) {RR_F1});
	g->Keys[8] = TI89 ? (RowKey) {RR_DIAMOND} : (TI92PLUS ? (RowKey) {RR_F2} : (RowKey) {RR_Q});
	g->Keys[9] = TI89 ? (RowKey) {RR_F5} : (TI92PLUS ? (RowKey) {RR_F4} : (RowKey) {RR_F3});
	g->Keys[10] = (RowKey) g->Keys[6];*/
	
	g->Keys[0] = (RowKey) {RR_UP};
	g->Keys[1] = (RowKey) {RR_LEFT};
	g->Keys[2] = (RowKey) {RR_DOWN};
	g->Keys[3] = (RowKey) {RR_RIGHT};
	g->Keys[4] = TI89 ? (RowKey) {RR_2ND} : (TI92PLUS ? (RowKey) {RR_F1} : (RowKey) {RR_HAND});
	g->Keys[5] = TI89 ? (RowKey) {RR_SHIFT} : (TI92PLUS ? (RowKey) {RR_F5} : (RowKey) {RR_F1});
	g->Keys[6] = TI89 ? (RowKey) {RR_DIAMOND} : (TI92PLUS ? (RowKey) {RR_F2} : (RowKey) {RR_Q});
	g->Keys[7] = (RowKey) {RR_APPS};
	
	static const short plane_89[9] = { 11, 7, 94, 60, 58, 100, 40, 41, 20 };
	static const short plane_92[9] = { 16, 9, 122, 90, 88, 150, 60, 52, 26 };
	memcpy(&g->SPRWIDTH, (CALCULATOR ? plane_92 : plane_89), sizeof(plane_89));
}

/* Uh-Oh */
void ERROR(const unsigned char error) {
	static const char * const Error_Msg[8] = {
		"Not enough Memory", 
		"Needs about 85,000 bytes of RAM to run", 
		"Link Error", 
		"Check Link Connection", 
		"Error loading Gfx", 
		"Make sure hockgfx file is on calc", 
		"Error Loading Fonts", 
		"Contact phishfud@elknet.net"
	};
	
	// The 80,000 Byte number was found by using Command Post's Low Ram Simulation, by Greg Dietsche
	// It's old...
	if (GrayCheckRunning() == TRUE) {  // Check if Grayscale is turned on yet
		GrayClearScreen2B_R(Activel, Actived);
	} else {
		clrscr();
		PortSet(LCD_MEM,239,127);
	}
	
	/* ----------------------------------------------------------------------------------
		 x << 1 (shifting x to the left 1 bit), is the same as x * 2 (multiplying x by 2)
		 Bit shifting is very important and pretty easy to use once you get the hang of it.
		 Bit shifting is faster than regular multiplication most of the time
	 	 x << n is the same as multiplying x * 2^n
	 	 Likewise, x >> n is the same as dividing x / 2^n
	   ---------------------------------------------------------------------------------- */
	FontSetSys(F_6x8);  // Medium Font
		DrawStr(0, 0, (char*) Error_Msg[error * 2], A_REPLACE);
	FontSetSys(F_4x6);  // Smallest Font
		DrawStr(0, LCD_HEIGHT - 6, (char*) Error_Msg[(error * 2) + 1], A_REPLACE);
	OSLinkOpen();
	while(!(_keytest(RR_ENTER) || (CALCULATOR && _keytest(RR_ENTER1)) || (CALCULATOR && _keytest(RR_ENTER2))
	|| _keytest(RR_ESC)))
	PortRestore();
	
	if (error == LINK_ERROR && !_keytest(RR_ESC))
		Main_Menu();
	
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
	short a;
	
	Save_Config();
	if (Bulk && g->Teams_Changed == TRUE)  // Only Save the Teams if the User has made Changes
		Save_Teams();
	#ifdef EXTERNAL_GFX
	SafeUnlock(SYMSTR_CONST("hockgfx"));
	#endif
	OSLinkClose();  // Clean up Link
	if (Western)	free(Western);
	if (Eastern)	free(Eastern);
	if (Bulk) {
		while_keypress(TEACHERKEY);
	}
	
	GrayOff();
	SetIntVec(AUTO_INT_1, interrupt1);  // Restore default interrupts
	SetIntVec(AUTO_INT_5, interrupt5);
	
	/*	Free all allocated Space	*/
	if (Bulk) {
		for(a = 3; a--;) {  // Each player on each team has to be freed separately
			if (Team1->plrs[a])
				free(Team1->plrs[a]);
			if (Team2->plrs[a])
				free(Team2->plrs[a]);
		}
		free(Bulk);  // Free the main bulk of Allocated memory
	}
	
	// Restore the Home Screen, this is pretty much what #define SAVE_SCREEN does
	if (Home) {
		GKeyFlush();
		
		LCD_restore(Home);
		ST_helpMsg("Hockey 68k v2.75  --By Fisch");
		free(Home);
	}
	
	exit(0);  // Exit the Game
}