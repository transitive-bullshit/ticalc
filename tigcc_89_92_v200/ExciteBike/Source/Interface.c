// C Source File
// Created 1/29/2004; 6:24:26 PM
#include "All.h"
static inline void ScrollText(const char *const[]);
static inline void Info(void);
static inline void Track_Records(void);
static inline void Cheats(void);

/*	Returns the option chosen by the user	 */
short Menu(const char *title, const char Options[], short Max_Length, short pos, short Fading) {
	short width = 0, No_Options = 0, Main_Pos = 0;
	short a, Changing = FALSE;
	
	while(_KeyCheck(ESCKEY));
	do 
	{
		if (DrawStrWidth(Options + (Max_Length * No_Options), F_6x8) > width)
			width = DrawStrWidth(Options + (Max_Length * No_Options), F_6x8);
	} while(*(Options + (Max_Length * (++No_Options))) != 0);
	
	do {
		
		memset(Hiddenl, 0xFF, LCD_SIZE + LCD_SIZE);
		GraySpriteX8_AND(g->Zerox + 8, 0, 16, ExciteBike_Title[1], ExciteBike_Title[0], 18, 
			Hiddenl, Hiddend);
		
		Display_Author();
		FS_DrawGrayString(0, 22, title, A_SHADOWED | A_XOR | A_CENTERED, F_6x8);
		for(a = (No_Options > 5 ? 6 : No_Options); a--;)
			FS_DrawGrayString(g->Zerox + 22, 34 + 10 * a, Options + (Max_Length * (Main_Pos + a)), 
			A_XOR, F_6x8);
		
		register short Offset = 2 + (g->Zerox >> 3) + 30 * (10 * pos + 0 + 36);
		*((char*)Hiddend + Offset +  0) &= 0x1F;
		*((char*)Hiddend + Offset + 30) &= 0x1F;
		*((char*)Hiddenl + Offset + 30) &= 0xBF;
		*((char*)Hiddend + Offset + 60) &= 0x1F;
		
		// Highlight the selected Option
		GrayInvertRect2B(g->Zerox + 21, 33 + 10 * pos, g->Zerox + 23 + width, 41 + 10 * pos, Hiddenl, Hiddend);
		
		if (!strcmp(title, "- Choose Mode -")) {
			a = Zeroy + 75;
			if (pos == 0) {
				FS_DrawGrayString(0, a, EFG("(The fastest time at the end of the race wins)", 
					"(Le plus rapide à la course gagne)", "(die beste Zeit am Ende des Rennens gewinnt)"), A_CENTERED | A_XOR, F_4x6);
				FS_DrawGrayString(0, a + 7, EFG("Hint: Perform flips to lower your time", 
					"Astuce: pirouettes pour meil. tps", "Tipp: Loopings für eine bessere Zeit"), 
					A_CENTERED | A_XOR, F_4x6);
			}
			if (pos == 1) {
				FS_DrawGrayString(0, a, EFG("(The first person to cross the finish line wins)", 
					"(Celui qui arrive en premier gagne)", "(derjenige, der als Erster ankommt, gewinnt)"), 
					A_CENTERED | A_XOR, F_4x6);
			}
		}
		
		if (Fading) {  // Make a new menu screen fade in
			Dissolve();
			Fading = FALSE;
		} else {
			Toggle_Screens();
		}
		
		// Algorithm allows for any size menues to be scrolled easily
		if (_keytest(RR_UP)) {
		 	if (Changing == FALSE) {
				Changing = TRUE;
				if (--pos < 0) {
					if (--Main_Pos < 0) {
						pos = (No_Options > 5 ? 5 : No_Options - 1);
						Main_Pos = (No_Options > 5) * (No_Options - 6);
					} else pos = 0;
				}
			}
		} else if (_keytest(RR_DOWN)) {
			if (Changing == FALSE) {
				Changing = TRUE;
				if (++pos >= (No_Options > 5 ? 6 : No_Options)) {
					if (++Main_Pos >= (No_Options > 5) * (No_Options - 5))
						pos = Main_Pos = 0;
					else pos = 5;
				}
			}
		} else Changing = FALSE;
		
		if (_KeyCheck(ESCKEY) || _KeyCheck(TEACHERKEY)) {
			Exit_Linkplay();
			while(_KeyCheck(ESCKEY));
			
			return QUIT;
		}
	} while (!_KeyCheck(CONFIRMKEY) && !(_keytest(RR_ENTER) || (CALCULATOR && _keytest(RR_ENTER1)) || 
	(CALCULATOR && _keytest(RR_ENTER2))));
	
	while(_rowread(0));
	return Main_Pos + pos;  // Return the option chosen by the user
}

static inline void Info(void) {
	#ifdef ENGLISH
	
	ScrollText((const char * const[]) {
	 "ExciteBike 68k", "v0.64", "--", "", "Programmed By", "- Fisch -", "Travis Fischer", "", "", 
	"Released In", "May 2004", "", "", 
	"- Keys -", "", 
	TI89 ? "2nd Accelerates" : (TI92PLUS ? "F1 Accelerates" : "Hand Accelerates"), 
	"", 
	TI89 ? "Shift Turbo Speed" : (TI92PLUS ? "F2 Turbo Speed" : "Q Turbo Speed"), 
	"", 
	"APPS Pauses", 
	"", 
	TI89 ? "F5 Teacher Key" : (TI92PLUS ? "F4 Teacher Key" : "F3 Teacher Key"), 
	"", 
	"!  Hold Down and Left   !", 
	"! in the air to perform !", 
	"!      a back flip      !", 
	"", 
	"!   Hold Up and Right   !", 
	"! in the air to perform !", 
	"!     a front flip      !", 
	"Each flip will give a", 
	"three second time bonus", 
	"--", "", "", 
	"- Track Editor -", 
	"", 
	"Press Accelerate to", 
	"place a piece of track", 
	"", 
	"Press Turbo to accept", 
	"the track selection and", 
	"move on to the next piece", 
	"of track", 
	"", 
	"Press Left and Right to", 
	"switch between terrain", 
	"types", 
	"", 
	"Press Backspace to move", 
	"backwards in the Track", 
	"This will not erase", 
	"the terrain", 
	"--", "", "", 
	"Thanks To:", 
	"", 
	"TICT,", "The TIGCC Team,", "Nintendo,", " for translating", "the French version,", 
	"Mathiniste & Kevin Kofler", "for translating", "the German version,"
	"", 
	"And Cisco, My Cat", 
	"--", 
	"", "", "Beta Testers:", 
	"", 
	"Clawsonia,", 
	"Angelboy,", 
	"Jordan Krage,", 
	"Kevin Kofler,", 
	"Geoffrey Anneheim,", 
	"AS,", 
	"240-185", //,
	"--", 
	"", "", "phishfud@elknet.net", NULL});
	
	#elif defined(FRENCH) // French Information Screen
	
	ScrollText((const char * const[]) {
	 "ExciteBike 68k", "0.64 - Français", "--", "", "Programmé par", "- Fisch -", "Travis Fischer", "", 
	"Traduit par","240-185","", "Terminé en", "Mai 2004", "", "", 
	"- Touches -", "", 
	TI89 ? "2nd : Accélérer" : (TI92PLUS ? "F1 accélérer" : "Main (Hand) accélérer"), 
	"", 
	TI89 ? "Shift : Turbo" : (TI92PLUS ? "F2 : Turbo" : "Q : Turbo"), 
	"", 
	"APPS suspend le jeu", 
	"", 
	TI89 ? "F5 : Touche prof" : (TI92PLUS ? "F4 Touche prof" : "F3 Touche prof"), 
	"", 
	"!Utiliser Bas  et Gauche!", 
	"!en l'air pour  exécuter!", 
	"! une pirouette arrière !", 
	"", 
	"!Utiliser Haut  et Droit!", 
	"!en l'air pour  exécuter!", 
	"!  une pirouette avant  !", 
	"",
	"Chaque pirouette vous", 
	"donne droit à trois",
	"secondes de bonus", 
	"--", "", "", 
	"- Éditeur de circuits -", 
	"", 
	"Appuyez sur Accélérer", 
	"pour placer un morceau",
	"de terrain", 
	"", 
	"Appuyez sur Turbo pour", 
	"poser le morceau et", 
	"continuer à la portion", 
	"suivante du circuit", 
	"", 
	"Appuyez sur Gauche et", 
	"droite pour changer de", 
	"revêtement de circuit", 
	"", 
	"Appuyez sur <- pour", 
	"revenir en arrière.", 
	"Ceci n'affectera pas", 
	"le circuit.", 
	"--", "", "", 
	"Remerciements à :", 
	"", 
	"TICT,", "La TIGCC Team,", "Nintendo,", "240-185,", "Mathiniste & Kevin Kofler", 
	"", 
	"et Cisco, mon chat", 
	"--", 
	"", "", "Bêta-Testeurs", 
	"", 
	"Clawsonia,", 
	"Angelboy,", 
	"Jordan Krage,", 
	"Kevin Kofler,", 
	"Geoffrey Anneheim,", 
	"AS,", 
	"240-185", //,
	"--", 
	"", "", "phishfud@elknet.net", NULL});
	
	#elif defined(GERMAN)
	
	ScrollText((const char * const[]) {
	 "ExciteBike 68k", "0.64 - deutsch", "--", "", "programmiert von", "- Fisch -", "Travis Fischer", "", 
	"von Mathiniste und","Kevin Kofler übersetzt","", "beendet im", "Mai 2004", "", "", 
	"- Tasten -", "", 
	TI89 ? "2nd: Gas geben" : (TI92PLUS ? "F1: Gas geben" : "Hand: Gas geben"), 
	"", 
	TI89 ? "Shift: Turbo" : (TI92PLUS ? "F2: Turbo" : "Q: Turbo"), 
	"", 
	"APPS: eine Pause machen", 
	"", 
	TI89 ? "F5: Lehrertaste" : (TI92PLUS ? "F4: Lehrertaste" : "F3: Lehrertaste"), 
	"", 
	"!Benutzen Sie unten und !", 
	"! links in der Luft, um !", 
	"! eine Rückwärtswendung !", 
	"!      auszuführen      !", 
	"", 
	"! Benutzen Sie oben und !", 
	"!rechts in der Luft, um !", 
	"! eine Vorwärtswendung  !",
	"!      auszuführen      !", 
	"",
	"Jedes Looping gibt Ihnen", 
	"drei Sekunden Zeitgewinn",
	"--", "", "", 
	"- Streckeneditor -", 
	"", 
	"Drücken Sie Gas geben, um", 
	"ein Stück Strecke zu", 
	"platzieren", 
	"", 
	"Drücken Sie Turbo, um das", 
	"Stück festzusetzen und ", 
	"zum nächsten Abschnitt", 
  "der Strecke", 
  "weiterzufahren", 
	"", 
	"Drücken Sie auf Links und", 
	"Rechts, um den Belag der", 
	"Strecke zu verändern", 
	"", 
	"Drücken Sie auf <-,", 
	"um zurückzukehren.", 
	"Das wird die Strecke", 
	"nicht verändern.", 
	"--", "", "", 
	"Dank an:", 
	"", 
	"TICT,", "Das TIGCC-Team,", "Nintendo,", "240-185,", "Mathiniste & Kevin Kofler", "", 
	"und Cisco, meine Katze", 
	"--", 
	"", "", "Betatester", 
	"", 
	"Clawsonia,", 
	"Angelboy,", 
	"Jordan Krage,", 
	"Kevin Kofler,", 
	"Geoffrey Anneheim,", 
	"AS,", 
	"240-185", //,
	"--", 
	"", "", "phishfud@elknet.net", NULL});
	
	#endif
}

// Scrolls Text from the bottom to the top of the screen
static inline void ScrollText(const char *const str[]) {
	short pos = 0, nul = FALSE, y = 8, time = 26, Wait;
	const short offset = (14 * 30) * (CALCULATOR > 0);
	
	memset(Hiddenl, 0xFF, LCD_SIZE * 2);  // Fill the virtual planes Black
	static unsigned long Flower[48] =  // (31x16)
{ 0x3C000,0xC7000,0x37EC00,0x29B400,0x7E817E0,0x3973DD18,0x438FF284,0x82981B82,0x63F9DF86,0x3F5E73FE,
	0x1F63C6BC,0x9CCB310,0x8793C10,0x8078010,0x81FF0F0,0xFFC3FE0, // Light
	0x3C000,0xFF000,0x381C00,0x300C00,0x7F7EFE0,0x3E3FF8F8,0x7C7C3C7C,0xFC7C3C7E,0x7DFE3FFE,0x1E3FFC78,
	0x61FF860,0xE03C0F0,0xF87C3F0,0xFFFFFF0,0xFFC7FF0,0xFF00F00,  // Dark
	0xFFFC3FFF,0xFFF00FFF,0xFFC003FF,0xFFC003FF,0xF800001F,0xC0000007,0x80000003,0x1,0x80000001,  // Mask
	0xC0000001,0xE0000003,0xF000000F,0xF000000F,0xF000000F,0xF000000F,0xF003C01F
};
	
	do {
		
		if (++y > 8) {
			y = 0;
			memset(Hiddenl+3000+offset, 0xFF, 270);  // Fill in another row of 9 black lines for new text
			memset(Hiddend+3000+offset, 0xFF, 270);
			
			if (nul == FALSE) {
				if (str[pos] == NULL)
					nul = TRUE;
				else FS_DrawGrayString(0, 100 + 14 * (CALCULATOR > 0), (char*)str[pos++], A_XOR|A_CENTERED, F_6x8);
			} else if (nul++ > 10) break;
		}
		if (_keytest (RR_PLUS) && time > 1)  // Speed up or slow down text
			time--;
		if (_keytest (RR_MINUS) && time < 100)
			time++;
		
		// Scroll up and Copy virtual screens to Hidden gray planes
		ScrollUp240_R((short*) (Hiddenl + offset), 109);
		ScrollUp240_R((short*) (Hiddend + offset), 109);
		Toggle_Screens();
		if (CALCULATOR > 0) {
			memset(Actived+3000+offset, 0xFF, 270);
			memset(Activel+3000+offset, 0xFF, 270);  // Fill in another row of 9 black lines for new text
		}
		
		// Draw the perty flowers
		GrayClipSprite32_MASK_R(g->Zerox, Zeroy, 16, Flower, Flower + 16, Flower + 32, Flower + 32, 
			Activel, Actived);
		GrayClipSprite32_MASK_R(g->Zerox + 128, Zeroy, 16, Flower, Flower + 16, Flower + 32, Flower + 32, 
			Activel, Actived);
		
		// Slow Down the Scrolling
		for(Wait = time; Wait--;)
			pokeIO(0x600005,0b11111);
	} while (!_KeyCheck(ESCKEY) && !_KeyCheck(TEACHERKEY));
	
	if (_KeyCheck(TEACHERKEY))
		LeadOut();
}

void Options(void) {
	#ifdef ENGLISH
	const char options[6][14] = { {"View Records"}, {"Clear Records"}, {"Information"}, {"Cheats"}, 
	{"Back"}, {0} };
	#elif defined(FRENCH)
	const char options[6][14] = { {"Voir records"}, {"Effacer..."}, {"Informations"}, {"Tricher !"}, 
	{"Retour"}, {0} };
	#elif defined(GERMAN)
	const char options[6][18] = { {"Rekorde anschauen"}, {"Löschen..."}, {"Informationen"}, {"Cheats"}, 
	{"Retour"}, {0} };
	#endif
	static const char Affirm_menu[][5] = { {EFG("Yes", "Oui", "Ja")}, {EFG("No", "Non", "Nein")}, {0} };
	const char Choice = Menu(EFG("- Options -", "- Options -", "- Optionen -"), (char*)options,
	EFG(14, 14, 18), 0, TRUE);
	
	if (Choice == 0)
		Track_Records();
	if (Choice == 2)
		Info();
	if (Choice == 3)
		Cheats();
	if (Choice == 1 && Menu(EFG("Reset Default Records?", "Effacer les records ?", "Die Rekorde löschen?"), 
	(char*)Affirm_menu, 5, 0, TRUE) == 0) {
		UnArchive(exctimes);
		#ifdef ENGLISH
		if (SymDel(SYMSTR(exctimes)) == TRUE) {
			Message((const char *const[]){ "- Success -", "", "Default Track Records", "Have been Reset", 
				NULL }, A_DISSOLVE | A_WAITKEY);
		} else {
			Message((const char *const[]){ "- Error -", "", "Default Track Records", "Cannot be Reset", 
				NULL }, A_DISSOLVE | A_WAITKEY);
		}
		#elif defined(FRENCH)
		if (SymDel(SYMSTR(exctimes)) == TRUE) {
			Message((const char *const[]){ "- Réussite -", "", "Les records par défaut", "ont été remis", 
				NULL }, A_DISSOLVE | A_WAITKEY);
		} else {
			Message((const char *const[]){ "- Erreur -", "", "Les records par défaut", "ne peuvent être remis", 
				NULL }, A_DISSOLVE | A_WAITKEY);
		}
		#elif defined(GERMAN)
		if (SymDel(SYMSTR(exctimes)) == TRUE) {
			Message((const char *const[]){ "- Erfolg -", "", "Die Rekorde wurden", "zurückgesetzt",
  			NULL }, A_DISSOLVE | A_WAITKEY);
		} else {
			Message((const char *const[]){ "- Fehler -", "", "Die Rekorde konnten", 
				"nicht zurückgesetzt", "werden", NULL }, A_DISSOLVE | A_WAITKEY);
		}
		#endif
	}
}

// Allow the user to browse through each Track's best time
static inline void Track_Records(void) {
	char buf[30], Clear;
	
	while(Choose_Track() != QUIT) {
		if (g->Game_Progress > QUIT) {
			sprintf(g->Track_Name, EFG("Track %d", "Circuit %d", "Strecke %d"), g->Game_Progress + 1);
			
			clock->Minutes = (Default_Times[g->Game_Progress] & 0x07FF) / 60;
			clock->Seconds = (Default_Times[g->Game_Progress] & 0x07FF) % 60;
			clock->Mseconds50 = (Default_Times[g->Game_Progress] & 0xF800) >> 9;
		} else clock->Minutes = clock->Seconds = clock->Mseconds50 = 0;
		
		Clear = FALSE;
		do 
		{
			
			if (Clear == FALSE)
				sprintf(buf, EFG("Best Time %d:%02d:%02d", "Meil. Tps %d:%02d:%02d", "Beste Zeit %d:%02d:%02d"), 
				Best_Minutes, Best_Seconds, Best_Mseconds50);
			else sprintf(buf, EFG("Best Time %d:%02d:%02d", "Meil. Tps %d:%02d:%02d", "Beste Zeit %d:%02d:%02d"),
			clock->Minutes, clock->Seconds, clock->Mseconds50);
			
			Message((const char *const[]){ g->Track_Name, "", "--", buf, "--", NULL }, A_DISSOLVE * (!Clear));
			if (Clear == FALSE) {
				FS_DrawGrayString(0, g->lcd_height - 6, EFG("Clear resets record", "Clear : Effacer", 
				 "Clear: Löschen"),A_SHADOWED|A_XOR, F_4x6);
				Toggle_Screens();
			}
			
			Clear = FALSE;
			
			while(_rowread(0)) {  // while the user is holding a key down (except ON)
				if (_KeyCheck(TEACHERKEY))
					LeadOut();
				if (_keytest(RR_CLEAR) && Clear == FALSE) {  // Erase (reset) this track's current best time
					if (New_Track_Record() == FALSE) {
						#ifdef ENGLISH
						Message((const char *const[]){ "- Error -", "", "Record Cannot be", "Cleared", NULL }, 
							A_DISSOLVE | A_WAITKEY);
						#elif defined(FRENCH)
						Message((const char *const[]){ "- Erreur -", "", "Le record ne peut", "être effacé", NULL }, 
							A_DISSOLVE | A_WAITKEY);
						#elif defined(GERMAN)
						Message((const char *const[]){ "- Fehler -", "", "Der Rekord kann nicht", "gelöscht werden", 
							NULL }, A_DISSOLVE | A_WAITKEY);
						#endif
					}
					
					Clear = TRUE;
					break;
				}
			}
			if (Clear == TRUE)
				continue;
			while(!_rowread(0))   // while no keys are pressed
				pokeIO(0x600005,0b11111);
			while(_rowread(0)) {  // while the user is holding a key down (except ON)
				if (_KeyCheck(TEACHERKEY))
					LeadOut();
				if (_KeyCheck(ESCKEY))
					return;
				
				if (_keytest(RR_CLEAR)) {  // Erase (reset) this track's current best time
					if (New_Track_Record() == FALSE) {
						#ifdef ENGLISH
						Message((const char *const[]){ "- Error -", "", "Record Cannot be", "Cleared", NULL }, 
							A_DISSOLVE | A_WAITKEY);
						#elif defined(FRENCH)
						Message((const char *const[]){ "- Erreur -", "", "Le record ne peut", "être effacé", NULL }, 
							A_DISSOLVE | A_WAITKEY);
						#elif defined(GERMAN)
						Message((const char *const[]){ "- Fehler -", "", "Der Rekord kann nicht", "gelöscht werden", 
							NULL }, A_DISSOLVE | A_WAITKEY);
						#endif
					}
					
					Clear = TRUE;
					break;
				}
			}
		} while(Clear == TRUE);
	}
}

// Unlock new cheats by getting a time less than 50 seconds on Track 5
static inline void Cheats(void) {
	unsigned char Old_Cheats = g->Cheats, a = 0;
	char buf[35];
	
	if (Get_Input(EFG("Enter Cheat:", "Entrer code :", "Code eingeben:"), buf, 8) == QUIT)
		return;
	do
	{
		buf[a] = toupper(buf[a]);
	} while(buf[++a] != 0);
	
	// Most cheats are from the original NES GameShark.
	// I found these cheats on the web, so if they are 
	// incomplete and/or false, please tell me.
	for(a = 0; a < 7; a++) {
		if (!strcmp(buf, cheats[a][1])) {
			g->Cheats ^= (1 << a);
			strcpy(buf, cheats[a][0]);
		}
	}
	
	if (!strcmp(buf, cheats[7][1])) {  // Unlock All Cheats
		if (g->Cheats == 0xFF)
			g->Cheats = !g->Cheats;
		else g->Cheats = 0xFF;
		strcpy(buf, cheats[7][0]);
	}
	
	#ifdef ENGLISH
	if (g->Cheats == Old_Cheats)
		strcpy(buf, "Sorry, try again");
	else strcat(buf, g->Cheats > Old_Cheats ? " Enabled" : " Disabled");
	Message((const char *const[]){ "", "--", buf, "--", NULL }, A_DISSOLVE | A_WAITKEY);
	#else
	if (g->Cheats == Old_Cheats) {
		strcpy(buf, EFG("Sorry, try again", "Désolé, essayez encore", "Nein, nochmal versuchen"));
		Message((const char *const[]){ "", "--", buf, "--", NULL }, A_DISSOLVE | A_WAITKEY);
	} else {
		Message((const char *const[]){ "", "--", buf, g->Cheats > Old_Cheats ? EFG("Enabled", "Activés", 
		"Ein") : EFG("Disabled", "Desactivés", "Aus"), "--", NULL }, A_DISSOLVE | A_WAITKEY);
	}
	#endif
}