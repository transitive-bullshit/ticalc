// C Source File
// Created 7/19/2004; 11:19:43 AM
#include "All.h"
//static inline void Transfer_String(const short);
void Transfer(char*, const char*, const unsigned long);
void Receive_Initial_Data(char*, const unsigned short);
void Send_Initial_Data(char*, const unsigned short);

#define HOST_NO								82  // random
#define JOIN_NO								91  // random
#define LINK_NO								110 // random

/* Establishes a Connection between two calcs.  The Calc to Join the Game first will be the Host */
short Establish_Connection(void) {
	char Send, Receive = 0, Signal = FALSE, join_no = JOIN_NO;
	Message((const char *const[]){EFG("Connecting"), NULL}, 0);
	//"Attempting to"/*, "Essai de", "Versuch einer"*/), 
	//	EFG("Connect"/*, "connexion", "Verbindung"*/), NULL}, 0);
	g->Link = TRUE;
	
	// Clear the Link buffer until we empty it or receive the Host Calc's LINK_NO Signal
	while(OSReadLinkBlock(&Receive, 1) == 1) {
		if (Receive == HOST_NO)
			Signal = TRUE;
		else if (Receive == QUIT)
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
		Message((const char *const[]){"Waiting for other", "player to join", "", "", "", "Press ESC", 
			"to cancel", "--", NULL}, 0);
		#elif defined(FRENCH)
		Message((const char *const[]){"Essai de", "connexion", NULL}, 0);
		#elif defined(GERMAN)
		Message((const char *const[]){"Warten auf anderen", "Spieler", "", "", "", "Abbruch mit ESC", NULL}, 0);
		#endif
		GraySpriteX8_MASK(g->Zerox + 69, g->Zeroy + 31, 22, (char*)LinkPlay[0], (char*)LinkPlay[1], 
			(char*)LinkPlay[2], (char*)LinkPlay[2], 4, Activel, Actived);
		
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
			}
			
			// Check if the Host wants to exit the game and send a Signal to the Other calc if so
			KeyScan();
			if (_KeyCheck(ESCKEY | TEACHERKEY)) {
				Exit_Linkplay();
				return QUIT;
			}
		}
	}
	// ----------------------------------- //
	// Connection successfully established //
	// ----------------------------------- //
	
	if (Choose_Wrestler(EFG("Choose your wrestler"), g->Calc, FALSE, TRUE) == QUIT || Align_Calcs() == QUIT) {
		Exit_Linkplay();
		return QUIT;
	}
	
	g->wrestlers[!g->Calc].c = g->Characters[Load_Slot];
	//g->Random[2*g->Calc] = random(3);
	//g->Random[2*g->Calc + 1] = random(3);
	
	#define Data_Size				(24)//(sizeof(Character) - 1 + 1 + 1 + 5 + 1 + 1)
	
	char *data = NULL;
	unsigned short size = Data_Size, other_size, a;
	for(a = 5; a--;)
		size += strlen(Char_String(g->wrestlers[g->Calc].c, a));
	
	Transfer((char*)&other_size, (char*)&size, sizeof(short));
	if (other_size > size)
		size = other_size;
	
	if ((data = malloc(size)) == NULL)
		ERROR(MEMORY);
	
															/* ---------------------------- */
															/* --- Linkplay Information --- */
															/* ---------------------------- */
	// All data needed to be transferred at the beginning of a linkplay match is pooled together 
	// and sent/received all at once.  This increases the chances of a stable connection because 
	// TI's link handling is tempermental when attempting to switch off transmitting and receiving 
	// several packets of information.  Also, when working with the linkport, attempting to send 
	// info simultaneously from both calcs will simply not work.  This is, however, the most 
	// common mistake I have seen people make when trying to set up a linkplay format.
	// *A good rule of thumb is that whenever one calc is sending, the other should be waiting to 
	// receive, and vice-versa.*  Take a look at the Transfer() function for a good example.
	if (g->Calc == Host) {
		Send_Initial_Data(data, size);
		Receive_Initial_Data(data, size);
	} else {
		Receive_Initial_Data(data, size);
		Send_Initial_Data(data, size);
	}
	
	free(data);
	
	//Transfer((char*)g->Characters[Load_Slot] + 1, (char*)g->wrestlers[g->Calc].c + 1, sizeof(Character) - 1);
	//Transfer(&g->Random[!g->Calc], &g->Random[g->Calc], sizeof(char));
	
	// Exchange Name, Origin, and 3 Smack Talk sayings
	//for(a = 5; a--;)
	//	Transfer_String(a);
	
	/*memset(Activel, 0, LCD_SIZE);
	memset(Actived, 0, LCD_SIZE);
	PortSet(Actived, 239, 127);
	printf("%d) %d %d\n%s %s\n%s %s\n%s %s\n%s %s\n%s %s\n%s %s\n", g->Calc, g->Random[0], g->Random[1], 
	g->strings[0], g->strings[1], 
	Char_String(g->wrestlers[0].c, 0), Char_String(g->wrestlers[1].c, 0), 
	Char_String(g->wrestlers[0].c, 1), Char_String(g->wrestlers[1].c, 1), 
	Char_String(g->wrestlers[0].c, 2), Char_String(g->wrestlers[1].c, 2), 
	Char_String(g->wrestlers[0].c, 3), Char_String(g->wrestlers[1].c, 3), 
	Char_String(g->wrestlers[0].c, 4), Char_String(g->wrestlers[1].c, 4)
	);
	WaitKey();
	Align_Calcs();*/
	
	if (Pre_Match() == QUIT) // Display Pre_Match screen, stats, and smack-talk
		return QUIT;
	
	return Align_Calcs();
}

void Receive_Initial_Data(char *data, const unsigned short size) {
	if (LIO_RecvData(data, size, 20))
		ERROR(LINK_ERROR);
	
	// Try removing ptr and just using the data variable
	memcpy((char*)g->Characters[Load_Slot] + 1, data, sizeof(Character) - 1);
	char *ptr = (data + sizeof(Character) - 1);
	//g->Random[2 * (!g->Calc)] = *ptr++;
	//g->Random[2 * (!g->Calc) + 1] = *ptr++;
	if (g->Calc == Host)
		ptr += 5;
	else {
		g->Distortion = *ptr++;
		g->Game_Speed = *ptr++;
		g->Time_Limit = *ptr++;
		g->Smack_Talk = *ptr++;
		srand(*ptr++);
	}
	
	short a;
	for(a = 5; a--;) {
		char *str = g->strings[5 * Load_Slot + a];
		while((*str++ = *ptr++) != 0);
	}
}

void Send_Initial_Data(char *data, const unsigned short size) {
	memcpy(data, (char*)g->wrestlers[g->Calc].c + 1, sizeof(Character) - 1);
	char *ptr = (data + sizeof(Character) - 1);
	//*ptr++ = g->Random[2 * g->Calc];
	//*ptr++ = g->Random[2 * g->Calc + 1];
	*ptr++ = g->Distortion;
	*ptr++ = g->Game_Speed;
	*ptr++ = g->Time_Limit;
	*ptr++ = g->Smack_Talk;
	// Synchronize calc's random no generator seeds
	char seed = (g->Calc == Host ? random(127) : 0);
	*ptr++ = seed;
	if (g->Calc == Host)
		srand(seed);
	
	short a;
	for(a = 5; a--;) {
		strcpy(ptr, Char_String(g->wrestlers[g->Calc].c, a));
		ptr += strlen(Char_String(g->wrestlers[g->Calc].c, a)) + 1;
	}
	
	// Data_Size may be > 128 so the lower-level OSWriteLinkBlock cannot be used (would produce an error)
	if (LIO_SendData(data, size))
		ERROR(LINK_ERROR);
}

// Main link routine in-game, to exchange/update information (every frame) during a match
void Transfer_Data(void) {
	const short host = (g->Calc == Host);
	char *Move       = host ? &g->Move1 : &g->Move0;
	const char send[2] = { g->Status, host ? g->Move0 : g->Move1 };
	char rec[2];
	
	Transfer(rec, send, sizeof(send));
	*Move = rec[1];
	
	// Make sure both Calcs have the same Game Status (Normal, Quitting, Match_Over, etc)
	if (rec[0] != g->Status && rec[0] != M_Start && Normal_Match_Process) {
		if (g->Status == M_Start && rec[0] == M_Normal)
			End_Introductions();
		else if (rec[0] == Fast_Quit)
			g->Status = QUIT_AUTO;
		else {
			if (rec[0] == Game_Paused || rec[0] == QUIT)
				g->Old_Status = g->Status;
			
			if (rec[0] != M_Normal)
				g->Status = rec[0];
		}
	}
}

void Transfer(char *Dest, const char *Src, const unsigned long Size) {
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

/*static inline void Transfer_String(const short No) {
	char *Src = Char_String(g->wrestlers[g->Calc].c, No);
	unsigned char Size;
	
	if (g->Calc == Host) {
		Size = strlen(Src) + 1;
		OSWriteLinkBlock(&Size, sizeof(char));
		OSWriteLinkBlock(Src, Size);
		
		if (LIO_RecvData(&Size, sizeof(char), 20))  // Receive sizeof string
			ERROR(LINK_ERROR);
		if (LIO_RecvData(g->strings[5 * Load_Slot + No], Size, 20))  // Receive actual string data
			ERROR(LINK_ERROR);
	} else {
		if (LIO_RecvData(&Size, sizeof(char), 20))  // Receive sizeof string
			ERROR(LINK_ERROR);
		if (LIO_RecvData(g->strings[5 * Load_Slot + No], Size, 20))  // Receive actual string data
			ERROR(LINK_ERROR);
		
		Size = strlen(Src) + 1;
		OSWriteLinkBlock(&Size, sizeof(char));
		OSWriteLinkBlock(Src, Size);
	}
}*/

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
	Message((const char *const[]){"", "Other calc has", "exited the game", "or", "encountered", "an error", 
		NULL}, A_DISSOLVE|A_WAITKEY);
	#elif defined(FRENCH)
	Message((const char *const[]){"", "L'autre", "calculatrice", "a quitté le jeu", "ou a rencontré", 
		"une erreur", NULL}, A_DISSOLVE|A_WAITKEY);
	#elif defined(GERMAN)
	Message((const char *const[]){"", "Der andere Rechner", "hat das Spiel", "verlassen", "oder hat einen", 
		"Fehler verursacht", NULL}, A_DISSOLVE|A_WAITKEY);
	#endif
}

// Make sure Calcs are aligned before starting a game
short Align_Calcs(void) {
	char Send = LINK_NO, Receive = 0;
	Message((const char *const[]){"", EFG("Waiting for"/*, "En attente...", "Warten auf 2."*/), 
		EFG("other calc"/*, ,  "Rechner"*/), NULL}, 0);
	
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
			KeyScan();
			if (_KeyCheck(ESCKEY | TEACHERKEY)) {
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
			KeyScan();
			if (_KeyCheck(ESCKEY | TEACHERKEY)) {
				Exit_Linkplay();
				return QUIT;
			}
		}
		
		if (OSReadLinkBlock(&Receive, 1) == 1 && (Receive == QUIT))
			return QUIT;
		OSWriteLinkBlock(&Send, 1);
	}
	
	return TRUE;
}