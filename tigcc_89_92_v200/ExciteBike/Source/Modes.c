// C Source File
// Created 1/31/2004; 1:59:54 PM
#include "All.h"

void Selection_A(void) {
	Main_Game(1);
}

void Selection_B(void) {
	Main_Game(Get_Number(EFG("Number of Bikes?", "Nombre de motos ?", "Anzahl Motorräder?"), 4, 2, 4));
}

void Main_Game(short No_Bikes) {
	if (No_Bikes <= 0 || Choose_Track() == QUIT)
		return;
	
	do 
	{
		
		Initialize_Race(No_Bikes);
		MAIN_LOOP();
		
	} while(g->Game_Progress != QUIT && Load_Track(Tracks[g->Game_Progress]) != QUIT);
}

void Multiplayer(void) {
	#ifdef ENGLISH
	static const char mode_menu[][16] = { {"Fastest Time"}, {"First to Finish"}, {"Cancel Race"}, {0} };
	#elif defined(FRENCH)
	static const char mode_menu[][18] = { {"Temps min."}, {"Premier arrivé"}, {"Annuler la course"}, {0} };
	#elif defined(GERMAN)
	static const char mode_menu[][22] = { {"Schnellste Zeit"}, {"Als Erster angekommen"}, 
	{"Das Rennen stornieren"}, {0} };
	#endif
	char Receive = 0;
	
	if (Establish_Connection() == TRUE) {
		
		if (g->Calc == Host) {
			if (Choose_Track() == QUIT || 
			(g->First = Menu(EFG("- Choose Mode -", "- Choisir Mode -", "- Modus auswählen -"), 
			(char*)mode_menu, EFG(16, 18, 22), 0, TRUE)) & ~0x1) {
				Exit_Linkplay();
				return;
			}
			g->First++;
			
			if (OSReadLinkBlock(&Receive, 1) == 1 && Receive == QUIT) {
				Notify_Linkplay_Exit();
				return;
			}
			
			const char Send = LINK_NO;
			OSWriteLinkBlock(&Send, 1);
			OSWriteLinkBlock((char*)&g->Width, 2);
			OSWriteLinkBlock((char*)&g->First, 1);
			if (LIO_SendData((char*)g->Track, g->Width + 4))  // Send Track Data
				ERROR(LINK_ERROR);
		} else {
			#ifdef ENGLISH
			Message((const char *const[]){"Waiting for the Host", "to setup the Race", "--", NULL}, A_DISSOLVE);
			#elif defined(FRENCH)
			Message((const char *const[]){"En attente de l'hôte", "pour configurer la course", "--", NULL}, 
				A_DISSOLVE);
			#elif defined(GERMAN)
			Message((const char *const[]){"Warten, bis der Host", "den Wettlauf konfiguriert", "--", NULL},
				A_DISSOLVE);
			#endif
			SafeFree(g->Track);
			
			while(Receive != LINK_NO) {  // Wait until a signal is received from the Joining Calc
				OSReadLinkBlock(&Receive, 1);
				if (Receive == QUIT) {
					Notify_Linkplay_Exit();
					return;
				}
				
				// Check if the Host wants to exit the game and send a Signal to the Other calc if so
				if (_KeyCheck(ESCKEY) || _KeyCheck(TEACHERKEY)) {
					Exit_Linkplay();
					return;
				}
			}
			
			if (LIO_RecvData((char*)&g->Width, 2, 20))  // Receive Track Width
				ERROR(LINK_ERROR);
			if (LIO_RecvData((char*)&g->First, 1, 20))  // Receive Game Mode
				ERROR(LINK_ERROR);
			if ((g->Track = malloc(g->Width + 4)) == NULL)
				ERROR(MEMORY);
			
			if (LIO_RecvData((char*)g->Track, g->Width + 4, 30))  // Receive Track Data
				ERROR(LINK_ERROR);
		}
		
		g->Game_Progress = QUIT;
		Initialize_Race(2);
		MAIN_LOOP();
	}
}

short Choose_Track(void) {
	static const char tracks[7][11] = { 
	#ifdef ENGLISH
		{"Track 1"}, {"Track 2"}, {"Track 3"}, {"Track 4"}, {"Track 5"}, 
		{"Load Track"}, {0}
	#elif defined(FRENCH)
		{"Circuit 1"}, {"Circuit 2"}, {"Circuit 3"}, {"Circuit 4"}, {"Circuit 5"}, 
		{"Charger..."}, {0}
	#elif defined(GERMAN)
		{"Strecke 1"}, {"Strecke 2"}, {"Strecke 3"}, {"Strecke 4"}, {"Strecke 5"}, 
		{"Laden..."}, {0}
	#endif
	};
	short Choice = Menu(EFG("- Select Track -", "- Choix du circuit -", "- Strecke auswählen -"), 
	(char*)tracks, 11, 0, TRUE);
	
	if (Choice == 5) {        // Load a Custom External Track
		g->Game_Progress = QUIT;
		return Select_External_Track(EFG("- Load Track -", "- Charger circuit -", "- Strecke laden -"));
	} else if (Choice >= 0) { // Load a Built-In Track
		g->Game_Progress = Choice;
		return Load_Track(Tracks[Choice]);
	} else return QUIT;
}

// Race Summary Screen informing the user how he/she did
void Post_Race(short Recursive) {
	if (Recursive) {
		clock->Running = FALSE; clock->Minutes = g->OldMinutes; clock->Seconds = g->OldSeconds;
		clock->Mseconds50 = g->OldMseconds50;
	}
	//short Best_Minutes2 = 0, Best_Seconds2 = 4, Best_Mseconds502 = 20;
	//clock->Minutes = 0, clock->Seconds = 12, clock->Mseconds50 = Get_Number("Msecs?", 5, 0, 19);
	short Best_Time = 60 * Best_Minutes + Best_Seconds;
	short Time      = 60 * clock->Minutes + clock->Seconds;
	short Rank      = (Time - Best_Time) + (Best_Mseconds50 < (clock->Mseconds50 << 2));
	short a, y = 0, add = 0;
	char buf[30];
	
	if (Best_Time == FALSE)  // Track is new, its best time is 0:00:00
		Rank = -1;
	if (g->First)
		Rank = !(g->First - 1);
	
	memset(Hiddenl, 0xFF, LCD_SIZE + LCD_SIZE);
	GraySpriteX8_AND(g->Zerox + 68, Zeroy + 24, 16, Podium, Podium, 3, Hiddenl, Hiddend);
	GraySpriteX8_AND(g->Zerox, Zeroy + 7, 8, Star[0], Star[1], 1, Hiddenl, Hiddend);
	GraySpriteX8_AND(g->Zerox + 8, Zeroy + 7, 8, Star[0], Star[1], 1, Hiddenl, Hiddend);
	GraySpriteX8_AND(g->Zerox + 153, Zeroy + 7, 8, Star[0], Star[1], 1, Hiddenl, Hiddend);
	GraySpriteX8_AND(g->Zerox + 145, Zeroy + 7, 8, Star[0], Star[1], 1, Hiddenl, Hiddend);
	GraySpriteX8_AND(g->Zerox + 20, Zeroy, 15, Trophy[0], Trophy[1], 2, Hiddenl, Hiddend);
	GraySpriteX8_AND(g->Zerox + 124, Zeroy, 15, Trophy[0], Trophy[1], 2, Hiddenl, Hiddend);
	for(a = 0; a < 160; a+=8) {
		GraySpriteX8_AND(g->Zerox + a, Zeroy + 44, 8, Dots, Dots, 1, Hiddenl, Hiddend);
		GraySpriteX8_AND(g->Zerox + a, Zeroy + 44 + 48, 8, Dots, Dots, 1, Hiddenl, Hiddend);
		if (a < 48) {
			GraySpriteX8_AND(g->Zerox, Zeroy + 44 + a, 8, Dots, Dots, 1, Hiddenl, Hiddend);
			GraySpriteX8_AND(g->Zerox + 152, Zeroy + 44 + a, 8, Dots, Dots, 1, Hiddenl, Hiddend);
		}
	}
	
	sprintf(buf, "%s %d:%02d:%02d", g->Link ? EFG("Player 2 ", "Joueur 2", "Spieler 2 ") : EFG("Best Time", 
	"Meil. Tps", "Beste Zeit"), Best_Minutes, Best_Seconds, Best_Mseconds50);
	FS_DrawGrayString(g->Zerox + 28, Zeroy + 44 + 10, buf, A_XOR, F_6x8);
	sprintf(buf, EFG("Your Time %d:%02d:%02d", "Temps %d:%02d:%02d", "Deine Zeit %d:%02d:%02d"), 
		clock->Minutes, clock->Seconds, (clock->Mseconds50 << 2));
	FS_DrawGrayString(g->Zerox + 28, Zeroy + 44 + 20, buf, A_XOR, F_6x8);
	
	sprintf(buf, "Game Over");
	if (Rank <= 0) {  // New Best Time!!!
		Rank = 0;
		if (Recursive == FALSE && g->Link == FALSE)
			sprintf(buf, EFG("It's a New Record!", "Record battu !", "Ein neuer Rekord!"));
	} else if (g->Link == TRUE || (Rank /= 3) <= 0)
		Rank = 1;
	else if (Rank > 128)
		Rank = 128;
	if (g->Game_Progress != QUIT && (Rank == 1 || Rank == 2 || Recursive))
		sprintf(buf, EFG("Try the Next Track", "Essayez le prochain...", "Probier die nächste..."));
	
	FS_DrawGrayString(0, Zeroy + 44 + 40, buf, A_XOR|A_CENTERED|A_SHADOWED, F_6x8);
	sprintf(buf, EFG("Ranking   #%d", "Class.   #%d", "Platz      #%d"), Rank + 1);
	FS_DrawGrayString(g->Zerox + 28, Zeroy + 44 + 30, buf, A_XOR, F_6x8);
	for(a = 3 - (g->Link == TRUE); a--;) {
		if (Rank != (a + (a == 0) - (a == 1)))
			GraySpriteX8_AND(g->Zerox + 68 + (a << 3), Zeroy + 16 - ((a == 1) << 3), 16, 
			Celebrating[a == 1 ? 0 : (2 + (a == 2))], Celebrating[a == 1 ? 0 : (2 + (a == 2))] + 16, 1, 
			Hiddenl, Hiddend);
	}
	if (Recursive == 1)
		return;
	
	if (Recursive == 2) {
		Toggle_Screens();
		while(_rowread(0));
	} else {
		Dissolve();
		if (Rank == 0) {
			g->OldMinutes = clock->Minutes;
			g->OldSeconds = clock->Seconds;
			g->OldMseconds50 = clock->Mseconds50;
			clock->Running = TRUE;
		}
	}
	
	do 
	{
		y += add;
		Toggle_Screens();
		if (Rank < 3) {
			GraySpriteX8_AND(g->Zerox + 68 + ((Rank != 1) << 3) + ((Rank == 2) << 3), Zeroy + 16 - 
			((Rank == 0) << 3) - y, 16, Celebrating[(y <= 0)], Celebrating[(y <= 0)] + 16, 1, 
			Activel, Actived);
			
			if (Rank == 0 && Recursive == FALSE && 60 * clock->Minutes + clock->Seconds > 60 * g->OldMinutes + 
			g->OldSeconds + 5) {
				Post_Race(1);
				Recursive = TRUE;
			}
		}
		
		for(a = 18; a--;) {
			pokeIO(0x600005,0b11111); /* low power mode, wake up CPU only on AI 1 
		    	                         (grayscale), 2 (Keypress), 3 (AMS clock), 4 
		     	                         (Link), 5 (AMS timer base, needed for APD) 
		       	                       and 6 (ON, always wakes up the CPU)
		        	                      -- Saves Battery Power -- */
			if (_rowread(0))
				break;
		}
		
		if (y >= 8)
			add = -1;
		else if (y <= 0)
			add = !add;
		
		if (_KeyCheck(TEACHERKEY)) {
			Recursive = QUIT;
			break;
		}
	} while ((_KeyCheck(CONFIRMKEY) || _KeyCheck(ESCKEY) || (_keytest(RR_ENTER) || (CALCULATOR &&
	 _keytest(RR_ENTER1)) || (CALCULATOR && _keytest(RR_ENTER2)))) == FALSE);
	if (Recursive == 2)
		return;
	if (Rank == 0 && Recursive == FALSE)
		Post_Race(2);
	
	if (Recursive >= 0 && Time < 50 && g->Game_Progress == 4 && g->Cheats == FALSE) {
		a = random(8);
		
		#ifdef ENGLISH
		Message((const char *const[]){ "New Cheat Unlocked", "", cheats[a][0], "--", cheats[a][1], NULL }, 
			A_DISSOLVE | A_WAITKEY);
		#elif defined(FRENCH)
		Message((const char *const[]){ "Nouveau Cheat code", "débloqué :", cheats[a][0], "--", cheats[a][1], 
			NULL }, A_DISSOLVE | A_WAITKEY);
		#elif defined(GERMAN)
		Message((const char *const[]){ "Neuer Cheatcode", "freigegeben:", cheats[a][0], "--", cheats[a][1], 
			NULL }, A_DISSOLVE | A_WAITKEY);
		#endif
	}
	
	// Top three place finishers continue
	if (Rank < 3) {
		// Attempt to save a new track record
		if (Rank <= 0 && g->Calc == Host && New_Track_Record() == FALSE && Recursive >= 0) {
			#ifdef ENGLISH
			Message((const char *const[]){ "- Error -", "", "New Record Cannot", "be Saved", NULL }, 
				A_DISSOLVE | A_WAITKEY);
			#elif defined(FRENCH)
			Message((const char *const[]){ "- Erreur -", "", "Le nouveau record", "ne peut être sauvé.", NULL }, 
				A_DISSOLVE | A_WAITKEY);
			#elif defined(GERMAN)
			Message((const char *const[]){ "- Fehler -", "", "Der neue Rekord kann", "nicht gespeichert werden", 
				NULL }, A_DISSOLVE | A_WAITKEY);
			#endif
		}
		
		if (g->Game_Progress != QUIT && ++g->Game_Progress >= 5)
			g->Game_Progress = 0;
	} else g->Game_Progress = QUIT;
	
	if (Recursive == QUIT)
		LeadOut();
}

/*unsigned char Best_Time_Image[] = {
0xFB,0xFF,0xBE,0xF3,0xFF,0x9E,0xE3,0xFF,0x8E,0xC7,0xFF,0xC6,0x8F,0xFF,0xE2,0x1F,
0xFF,0xF0,0xFF,0xFF,0xFE,0xFF,0xFF,0xFE,0xFF,0xFF,0xFE,0xFF,0xFF,0xFE,0xFF,0xFF,
0xFE,0xFF,0xFF,0xFE,0xFF,0xFF,0xFE,0xFF,0xFF,0xFE,0xFF,0xFF,0xFE,0xFF,0xFF,0xFE,
0x1F,0xFF,0xF0,0x8F,0xFF,0xE2,0xC7,0xFF,0xC6,0xE3,0xFF,0x8E,0xF0,0x00,0x1E,0xF8,
0x00,0x3E,0xFC,0x00,0x7E
}*/