// Header File
// Created 5/22/2004; 12:29:19 PM

/* Global Variables, seen by all C Source Files */
extern volatile short count;
extern WRESTLER Wrestlers[NO_SPRITES];
extern Clock  *clock;
extern unsigned char Ring_Matrix[8][15];
extern unsigned short *Tiles[];
extern unsigned char (*Ring)[8][15];
extern void *Hiddenl;
extern void *Hiddend;

// Misc Graphics which do not need masks or mirrors calculated and are not wrestler sprites
extern unsigned char vs[2][6 * 30];
extern unsigned long LinkPlay[3][22];
extern unsigned char salt[16];
extern unsigned char _EarthQuake_[5];
extern unsigned char blood[4][16];
extern unsigned char Arrow[3][6 * 2];
extern unsigned char salt_x[2][9];
extern unsigned char salt_y[2][8];
extern unsigned char tear[3];
extern const Finish_Text finish_text[8];
extern unsigned short maskstar[2][16];
// Tournament Mode Graphics
extern unsigned short Walkway[2][16 * 2];
extern unsigned short T_Parallax_Tile[16 * 2];
extern unsigned short sign[2][16 * 2];

extern INT_HANDLER interrupt1;
extern INT_HANDLER interrupt5;
// For synchronizing the scrolling Text display
DEFINE_INT_HANDLER (INT_1_COUNTER) {
	count++;
}
/* Base taken from TICT Tutorial SP12, Example 3 and modified */
/* Used for Keeping track of Time during the Matches */
DEFINE_INT_HANDLER (CLOCK_TIMER) {
	// The clock variables cannot be contained in the global register struct 'g' because 
	// using a global register variable inside of an interrupt handler could very possibly
	// cause a crash.  If that register (a4 in this case) is being used temporarily to 
	// store something other than our global register variable 'g' when the interrupt 
	// handler is called, then kablooey!!! Your calculator will explode... :)
	
	//------------------------------------------
	// count interrupts only if running is not 0
	//------------------------------------------
	if (clock->Running == TRUE && ++clock->Mseconds50 >= 19) {
		clock->Mseconds50 -= 19;
		clock->Seconds += 1 - 2 * (clock->Time_Limit > 0);
		if (clock->Seconds < 0)
			clock->Seconds = 0;
	}
}