// C Source File
// Created 12/14/2003; 11:58:15 AM
#include "Structures.h"
register GlobalVars *g asm("a4");

#include "All.h"
#include "Data.h"
#include "faststr.h"
static inline void Initialize_Game(void);
static inline void Compatibility(void);
void LeadOut2(void) __attribute__ ((noreturn));

/* ------------------------------------------------------------------ */
/*																																		*/
/*											 ExciteBike 68k v0.64													*/
/*													- By Fisch -															*/
/*												 Travis Fischer															*/
/*									 French Translation by 240-185										*/
/*				 German Translation by Mathiniste and Kevin Kofler					*/
/*																																		*/
/*											 phishfud@elknet.net													*/
/*																																		*/
/*																																		*/
/*	Start: December 14, 2003																					*/
/*	Beta1: April 16, 2004		(v0.50)																		*/
/*	Done:   																													*/
/*																																		*/
/*	I hope the source is fairly readable and well structured.  If			*/
/*	there is a problem/bug with any aspect of the Game, or you know		*/
/*	a way to improve it in any way, your e-mail is very welcome.  I		*/
/*	have learned so much working on this Game and I hope that the 		*/
/*	source will help others as well.																	*/
/*																																		*/
/* ------------------------------------------------------------------ */

/*	Globals	 */
INT_HANDLER interrupt1;
INT_HANDLER interrupt5;
unsigned char Original_Rate;
Clock  *clock;
char   *Home;
void   *Bulk;
char   *Terrainl;
char   *Terraind;
void   *Hiddenl;
void   *Hiddend;

/* Base taken from TICT Tutorial SP12, Example 3 and modified */
/* Used for Keeping track of Time during the Races */
DEFINE_INT_HANDLER (CLOCK_TIMER) {
	// The clock variables cannot be contained in the global register struct 'g' because 
	// using a global register variable inside of an interrupt handler could very possibly
	// cause a crash.  If that register (a4 in this case) is being used temporarily to 
	// store something other than our global register variable 'g' when the interrupt 
	// handler is called, then kablooey!!! Your calculator will explode... :)
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
			// There is no need for hours or days
		}
	}
}

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
	
	// Main Menu
	static const char Main[7][EFG(13, 13, 21)] = { 
	#ifdef ENGLISH
		{"Selection A"}, {"Selection B"}, {"Multiplayer"}, {"Track Editor"}, 
		{"Options"}, {"Exit"}, {0}
	#elif defined(FRENCH)
		{"Sélection A"}, {"Sélection B"}, {"Multijoueurs"}, {"Éditeur"}, 
		{"Options"}, {"Sortie"}, {0}
	#elif defined(GERMAN)
	  {"Auswahl A"}, {"Auswahl B"}, {"Mehrere Spieler"}, {"Streckeneditor"}, 
		{"Optionen"}, {"Beenden"}, {0}
	#endif
	};
	typedef void (*P2Func)(void);
	static const P2Func Menu_Funcs[] = 
	{&LeadOut, &Selection_A, &Selection_B, &Multiplayer, &Start_Track_Editor, &Options, &LeadOut};
	
	do 
	{
		g->Link = FALSE;
		g->Game_Progress = -1;
		Menu_Funcs[Menu(EFG("- Main Menu -", "- Menu Principal -", "- Hauptmenü -"), (char*)Main, 
			EFG(13, 13, 21), 0, TRUE) + 1]();
	} while (1);  // Loop until Escape is pressed or the user chooses to Exit
	
	ONERR
	
	// If there was an unexpected error, make sure everything is cleaned up 
	// before exiting and having an AMS popup error message
	LeadOut2();
	
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
/*asm("
IsVTI:
	trap #12
	move.w %d0,%d1
	move.w #0x6000,%sr
	move %sr,%d0
	andi #0x4000,%d0
	move.w %d1,%sr
	rts
");*/

/* Initializes and Preps the Game */
static inline void Initialize_Game(void) {
	/*	Disable old interrupts and install our own dummy interrupts	 */
	interrupt1 = GetIntVec(AUTO_INT_1);
	interrupt5 = GetIntVec(AUTO_INT_5);
	SetIntVec(AUTO_INT_1, DUMMY_HANDLER);
	/*	-----------------------------------------------------------	 */
	//Original_Rate = PRG_getStart();
	// Thanks to Lionel Debroux
	while (*(volatile unsigned char*)0x600017 != 0);
	while (*(volatile unsigned char*)0x600017 == 0);
	// Save older timer value
	Original_Rate = *(volatile unsigned char*)0x600017;
	Home    = NULL;
	Bulk    = NULL;
	
	randomize();
	if (SetupCharSet() == FALSE) { // Set up the (Beta) Fast String Drawing Routines by TICT
		ST_helpMsg(EFG("Error Loading Fonts", "Erreur chargement fonte", "Ladefehler: Schriftart"));
		LeadOut();
	}
	if ((Home = malloc(LCD_SIZE)) == NULL) {
		ST_helpMsg(EFG("Error, Not Enough Memory.  Free up RAM", "Pas assez de mémoire.", 
			"Nicht genug Speicher"));
		LeadOut();
	}
	LCD_save(Home);  // Save LCD HomeScreen Contents
	
	if ((Bulk = malloc(BULK_SIZE)) == NULL)
		ERROR(MEMORY);
	// Initiate Pointers within the allocated Bulk block
	Hiddenl  = Bulk;
	Hiddend  = (void*)((char*)Hiddenl + LCD_SIZE);
	g        = (GlobalVars*)((char*)Hiddend + LCD_SIZE);
	Terrainl = (char*)((char*)g + sizeof(GlobalVars));
	Terraind = (char*)((char*)Terrainl + LCD_SIZE);
	clock    = (Clock*)((char*)Terraind + LCD_SIZE);
	
	g->Track          = NULL;
	g->Cheats         = FALSE;
	clock->Reset_Time = TRUE;
	clock->Running    = FALSE;
	SetIntVec(AUTO_INT_5, CLOCK_TIMER);
	Compatibility();
	
	// Grayscale doublebuffering is messed up on HW1 normally, so we have to check if it is 
	// an HW1 and adjust accordingly
	short *hwptr = (short *) FL_getHardwareParmBlock();
	unsigned short HardWareVersion = *hwptr;
	g->HW1 = (HardWareVersion <=20 || IsVTI());
	
	if (!GrayOn())  // Turn Grayscale On
		ERROR(MEMORY);
}

// Initializes some Calculator Constants, depending on what calc type the game is being run on
static inline void Compatibility(void) {
	g->Keys[0] = (RowKey) {RR_ESC};
	g->Keys[1] = (RowKey) {RR_UP};
	g->Keys[2] = (RowKey) {RR_LEFT};
	g->Keys[3] = (RowKey) {RR_DOWN};
	g->Keys[4] = (RowKey) {RR_RIGHT};
	g->Keys[5] = (RowKey) {RR_APPS};
	g->Keys[6] = TI89 ? (RowKey) {RR_2ND} : (TI92PLUS ? (RowKey) {RR_F1} : (RowKey) {RR_HAND}); // Gas
	g->Keys[7] = TI89 ? (RowKey) {RR_SHIFT} : (TI92PLUS ? (RowKey) {RR_F5} : (RowKey) {RR_F1}); // Turbo
	g->Keys[8] = TI89 ? (RowKey) {RR_F5} : (TI92PLUS ? (RowKey) {RR_F4} : (RowKey) {RR_F3});  // Teacher
	g->Keys[9] = (RowKey) g->Keys[6];  // Confirm
	g->Zerox   = 40 * (CALCULATOR > 0);
	g->lcd_height = LCD_HEIGHT;
}

/* Uh-Oh */
void __attribute__ ((noreturn)) ERROR(const unsigned char error) {
	void *plane;
	static const char * const Error_Msg[2] = {
		EFG("Not enough Memory", "Pas assez de mémoire", "Nicht genug Speicher"), 
		EFG("Link Error", "Erreur de transmission", "Übertragefehler"), 
	};
	
	if (Bulk && g->Link == TRUE)
		Exit_Linkplay();
	
	if (GrayCheckRunning() == TRUE) {  // Check if Grayscale is turned on yet
		GrayClearScreen2B_R(Activel, Actived);
		plane = Actived;
	} else {
		clrscr();
		plane = LCD_MEM;
	}
	
	FS_DrawString(0, 0, Error_Msg[error], plane, F_6x8);
	FS_DrawString(0, g->lcd_height - 6, EFG("Contact phishfud@elknet.net", 
	"Contactez phishfud@elknet.net (En anglais)", "Kontakt: phishfud@elknet.net (auf Englisch)"), 
	plane, F_4x6);
	while(!(_keytest (RR_2ND) || _keytest (RR_ESC) || _keytest (RR_F1)));
	
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
	if (Bulk)
		while(_KeyCheck(TEACHERKEY));
	// Restore default interrupts
	*(volatile unsigned char*)0x600017 = Original_Rate;
	SetIntVec(AUTO_INT_1, interrupt1);
	SetIntVec(AUTO_INT_5, interrupt5);
	OSLinkClose();  // Clean up Link
	
	/*	Free all allocated Space	*/
	if (Bulk) {
		SafeFree(g->Track);
		free(Bulk);
	}
	
	// Restore the Home Screen, this is pretty much what #define SAVE_SCREEN does
	if (Home) {
		GKeyFlush();
		
		LCD_restore(Home);
		free(Home);
		ST_helpMsg(EFG("ExciteBike 68k v0.64  --By Fisch", "ExciteBike 68k 0.64 -- Fisch", 
			"ExciteBike 68k 0.64 --von Fisch"));
	}
	
	exit(0);  // Exit the Game
}