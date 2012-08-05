// C Source File
// Created 2/16/2004; 6:53:09 AM
#include "All.h"

#define HOST_NO								103 // random
#define JOIN_NO								71  // random

/* Establishes a Connection between two calcs.  The Calc to Join the Game first will be the Host */
short Establish_Connection(void) {
	char Send, Receive = 0, Signal = FALSE, join_no = JOIN_NO;
	#ifdef ENGLISH
	Message((const char *const[]){"Attempting to", "Connect", NULL}, 0);
	#elif defined(FRENCH)
	Message((const char *const[]){"Essai de", "connexion", NULL}, 0);
	#elif defined(GERMAN)
	Message((const char *const[]){"Versuch einer", "Verbindung", NULL}, 0);
	#endif
	g->Link = TRUE;
	
	// Clear the Link buffer until we empty it or receive the Host Calc's LINK_NO Signal
	while(OSReadLinkBlock(&Receive, 1) == 1) {
		if (Receive == HOST_NO)
			Signal = TRUE;
		else if (Receive == LINK_NO) { // Backwards compatibility for Linking
			// Two different versions of Excitebike are attempting to connect
			Signal  = TRUE;
			join_no = LINK_NO;
		} else if (Receive == QUIT)
			Signal = QUIT;
	}
	
	if (Signal == TRUE) { // This calc is 2nd to join the game
		Send = join_no;
		OSWriteLinkBlock(&Send, 1);
		g->Calc = Join;
	} else {
		Send = HOST_NO;
		OSWriteLinkBlock(&Send, 1);
		// Tell the Host plr to wait and press Escape to cancel
		#ifdef ENGLISH
		Message((const char *const[]){"Waiting for other", "player to join", "", "Press ESC to cancel", 
		"--", NULL}, 0);
		#elif defined(FRENCH)
		Message((const char *const[]){"Essai de", "connexion", NULL}, 0);
		#elif defined(GERMAN)
		Message((const char *const[]){"Warten auf anderen", "Spieler", "", "Abbruch mit ESC", NULL}, 0);
		#endif
		
		g->Calc = Host;
		while(Receive != JOIN_NO) {  // Wait until a signal is received from the Joining Calc
			OSReadLinkBlock(&Receive, 1);
			if (Receive == HOST_NO) {
			// This calc entered Multiplayer before the other calc had even started the game.  Thus, this 
			// calc's original HOST_NO message to the other calc was disregarded by the other calc's AMS.
				OSLinkOpen();
				Send = JOIN_NO;
				OSWriteLinkBlock(&Send, 1);
				g->Calc = Join;
				break;
			} else if (Receive == LINK_NO) {  // Two different versions of Excitebike are attempting to connect
				OSLinkOpen();
				Send = LINK_NO;
				OSWriteLinkBlock(&Send, 1);
				g->Calc = Join;
				break;
			}
			
			// Check if the Host wants to exit the game and send a Signal to the Other calc if so
			if (_KeyCheck(ESCKEY) || _KeyCheck(TEACHERKEY)) {
				Exit_Linkplay();
				return QUIT;
			}
		}
	}
	Send = (CALCULATOR > 0);
	Transfer(&Receive, &Send, 1);
	
	g->LinkplayOffset = 0;
	if (Send != Receive)
		g->LinkplayOffset = (Send > 0 ? 28 : -28);
	
	return TRUE;  // Connection successfully established
}

/* Establishes a Connection between two calcs.  The Calc to Join the Game first will be the Host */
/*short Establish_Connection(void) {
	char Send = LINK_NO, Receive = 0, Signal = FALSE;
	#ifdef ENGLISH
	Message((const char *const[]){"Attempting to", "Connect", NULL}, 0);
	#elif defined(FRENCH)
	Message((const char *const[]){"Essai de", "connexion", NULL}, 0);
	#elif defined(GERMAN)
	Message((const char *const[]){"Versuch", "sich einzuloggen", NULL}, 0);
	#endif
	g->Link = TRUE;
	
	// Clear the Link buffer until we empty it or receive the Host Calc's LINK_NO Signal
	while(OSReadLinkBlock(&Receive, 1) == 1) {
		if (Receive == LINK_NO)
			Signal = TRUE;
		else if (Receive == QUIT)
			Signal = QUIT;
	}
	
	OSWriteLinkBlock(&Send, 1);
	Send = (CALCULATOR > 0);
	if (Signal == TRUE) { // This calc is 2nd to join the game
		g->Calc = Join;
		Transfer(&Receive, &Send, 1);
	} else {
		// Tell the Host plr to wait and press Escape to cancel
		#ifdef ENGLISH
		Message((const char *const[]){"Waiting for other", "player to join", "", "Press ESC to cancel", 
		"--", NULL}, 0);
		#elif defined(FRENCH)
		Message((const char *const[]){"Essai de", "connexion", NULL}, 0);
		#elif defined(GERMAN)
		Message((const char *const[]){"Warten auf anderen", "Spielern ", "Drücken Sie auf ESC", "zum Verlassen",
		NULL}, 0);
		#endif
		
		while(Receive != LINK_NO) {  // Wait until a signal is received from the Joining Calc
			OSReadLinkBlock(&Receive, 1);
			// Check if the Host wants to exit the game and send a Signal to the Other calc if so
			if (_KeyCheck(ESCKEY) || _KeyCheck(TEACHERKEY)) {
				Exit_Linkplay();
				return QUIT;
			}
		}
		g->Calc = Host;
		Transfer(&Receive, &Send, 1);
	}
	
	g->LinkplayOffset = 0;
	if (Send != Receive)
		g->LinkplayOffset = (Send > 0 ? 28 : -28);
	OSLinkOpen();
	
	return TRUE;  // Connection successfully established
}*/

void Transfer_Data(void) {
	char Other_CalcStatus;
	
	Transfer(&Other_CalcStatus, &g->Status, sizeof(char));  // Synchronize Calc status'
	if (g->Status != Game_Paused)
		g->Old_Status = g->Status;
	if (g->Status == End_Wheelie) {
		if (Other_CalcStatus == End_Wheelie)
			g->Status = Race_Finished;
		else if (g->First == 1) g->First = 2;
	}
	
	// Make sure both Calcs have the same Game Status (Normal, Quitting, Race_Over, etc)
	if (Other_CalcStatus != Normal && Other_CalcStatus != g->Status && Other_CalcStatus != End_Wheelie) {
		if (Other_CalcStatus == Fast_Quit)
			g->Status = QUIT;
		else g->Status = Other_CalcStatus;
	}
	
	register short a = g->No_Bikes;
	for(; a--;) {  // Transfer each Bike every Frame (only the first 7 bytes of each bike are important)
		if (g->Calc == g->bikes[a].Control || (Is_Computer(g->bikes[a].Control) && g->Calc == Host))
			OSWriteLinkBlock((char*)&g->bikes[a], 7);
		else {
			if (LIO_RecvData((char*)&g->bikes[a], 7, 20))
				ERROR(LINK_ERROR);
			
			// Adjustment for screen size differences between the Ti-89/Titanium and the Ti-92+/V200
			g->bikes[a].y += g->LinkplayOffset;
		}
	}
}

void Transfer(char *Dest, char *Src, unsigned long Size) {
	if (g->Calc == Host) {
		OSWriteLinkBlock(Src, Size);
		if (LIO_RecvData(Dest, Size, 20))
			ERROR(LINK_ERROR);
	} else {
		if (LIO_RecvData(Dest, Size, 20))
			ERROR(LINK_ERROR);
		OSWriteLinkBlock(Src, Size);
	}
}

// Notify Other calc that the user has exited from the game
void Exit_Linkplay(void) {
	if (g->Link) {
		const char Signal = QUIT;
		OSWriteLinkBlock(&Signal, 1);
	}
	
	if (_KeyCheck(TEACHERKEY))
		LeadOut();
}

// Notify the user that the other calc has exited the game
void Notify_Linkplay_Exit(void) {
	OSLinkOpen();
	#ifdef ENGLISH
	Message((const char *const[]){"Other calc has", "exited the game", "or", "encountered an error", NULL}, 
		A_DISSOLVE|A_WAITKEY);
	#elif defined(FRENCH)
	Message((const char *const[]){"L'autre calculatrice", "a quitté le jeu", "ou a", "rencontré une erreur", 
		NULL}, A_DISSOLVE|A_WAITKEY);
	#elif defined(GERMAN)
	Message((const char *const[]){"Der andere Rechner", "hat das Spiel verlassen", "oder hat einen", 
		"Fehler verursacht", NULL}, A_DISSOLVE|A_WAITKEY);
	#endif
}

// Make sure Calcs are aligned before starting a game
short Align_Calcs(void) {
	char Send = LINK_NO, Receive = 0;
	Message((const char *const[]){EFG("Waiting for other calc", "En attente...", "Warten auf 2. Rechner"), 
		NULL}, 0);
	
	if (g->Link && g->Calc == Host) {
		OSWriteLinkBlock(&Send, 1);
		// Wait until signal from the Joining calc
		while(Receive != LINK_NO) {
			// Read the Link buffer and check if the other calc has exited the game
			if (OSReadLinkBlock(&Receive, 1) == 1 && Receive == QUIT) {
				Notify_Linkplay_Exit();
				return QUIT;
			}
			// Break out of Loop and notify other calc
			if (_KeyCheck(ESCKEY) || _KeyCheck(TEACHERKEY)) {
				Exit_Linkplay();
				return QUIT;
			}
		}
	} else if (g->Calc == Join) {
		// Wait until signal from the Host
		while(Receive != LINK_NO) {
			if (OSReadLinkBlock(&Receive, 1) == 1 && Receive == QUIT) {
				Notify_Linkplay_Exit();
				return QUIT;
			}
			// Break out of Loop and notify other calc
			if (_KeyCheck(ESCKEY) || _KeyCheck(TEACHERKEY)) {
				Exit_Linkplay();
				return QUIT;
			}
		}
		
		if (OSReadLinkBlock(&Receive, 1) == 1 && (Receive == QUIT))
			return QUIT;
		OSWriteLinkBlock(&Send, 1);
	}
	
	return 0;
}