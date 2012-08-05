// C Source File
// Created 1/28/2004; 8:06:41 PM
#include "All.h"
#define Max_Terrain		20
static inline void Save_Track(unsigned char*);
static inline short Find_All_Files(char (*)[9], const unsigned short);

void Start_Track_Editor(void) {
	#ifdef ENGLISH
	static const char edit_menu[][13] = { {"New Track"}, {"Edit Track"}, {"Delete Track"}, {"Back"}, {0} };
	#elif defined(FRENCH)
	static const char edit_menu[][13] = { {"Nouveau"}, {"Éditer"}, {"Supprimer"}, {"Retour"}, {0} };
	#elif defined(GERMAN)
	static const char edit_menu[][13] = { {"Neue Strecke"}, {"Anzeigen"}, {"Löschen"}, {"Zurück"}, {0} };
	#endif
	
	short Choice;
	
	do
	{
		Choice = Menu(EFG("- Track Editor -", "- Éditeur de circuits -", "- Streckeneditor -"), (char*)edit_menu, 
			13, 0, TRUE);
		
		if (Choice == 0) {
			if (Get_Input(EFG("New Track Name:", "Nouveau nom du circuit :", "Neuer Name der Strecke:"), 
			g->Track_Name, 8) != QUIT)
				Run_Editor(FALSE);
			
			return;
		} else if (Choice == 1 || Choice == 2) {
			if (Select_External_Track(Choice == 1 ? EFG("- Edit Track -", "- Éditer circuit -", 
				"- Strecke bearbeiten -") : EFG("- Delete Track -", "- Supprimer circuit -", 
				"- Strecke löschen -")) == QUIT) {
				continue;
			} else if (Choice == 1) {  // Edit Track
				Run_Editor(TRUE);
				return;
			} else if (Choice == 2) {  // Delete Track
				UnArchive(g->Track_Name);
				#ifdef ENGLISH
				if (SymDel(SYMSTR(g->Track_Name)) == TRUE) {  // Track successfully deleted
					Message((const char *const[]){ "- Success -", "", "The Track", g->Track_Name, "Has been Deleted", 
						NULL }, A_DISSOLVE | A_WAITKEY);
				} else {
					Message((const char *const[]){ "- Error -", "", "The Track", g->Track_Name, "Cannot be Deleted", 
						NULL }, A_DISSOLVE | A_WAITKEY);
				}
				#elif defined(FRENCH)
				if (SymDel(SYMSTR(g->Track_Name)) == TRUE) {  // Track successfully deleted
					Message((const char *const[]){ "- Réussite -", "", "Le circuit", g->Track_Name, "a été supprimé", 
						NULL }, A_DISSOLVE | A_WAITKEY);
				} else {
					Message((const char *const[]){ "- Erreur -", "", "Le circuit", g->Track_Name, 
						"ne peut être supprimé", NULL }, A_DISSOLVE | A_WAITKEY);
				}
				#elif defined(GERMAN)
				if (SymDel(SYMSTR(g->Track_Name)) == TRUE) {  // Track successfully deleted
					Message((const char *const[]){ "- Erfolg -", "", "Die Strecke", g->Track_Name, "wurde gelöscht", 
						NULL }, A_DISSOLVE | A_WAITKEY);
				} else {
					Message((const char *const[]){ "- Fehler -", "", "Die Strecke", g->Track_Name, 
						"kann nicht gelöscht werden", NULL }, A_DISSOLVE | A_WAITKEY);
				}
				#endif
			}
		} else return;
	} while(1);
}

// Returns the bytewidth of a given Terrain type
#define track_width(No) ((widths[(No)] >> 3) + (((No) == 15 || (No) == 16) * 54) + (((No) == 17) * 22))

void Run_Editor(const char Edit_Track) {
	#ifdef ENGLISH
	static const char edit_menu[][7] = { {"Yes"}, {"No"}, {"Cancel"}, {0} };
	#elif defined(FRENCH)
	static const char edit_menu[][7] = { {"Oui"}, {"Non"}, {"Annuler"}, {0} };
	#elif defined(GERMAN)
	static const char edit_menu[][10] = { {"Ja"}, {"Nein"}, {"Abbrechen"}, {0} };
	#endif
	short pos = 0, a, b, Cur_Piece, Fading = TRUE, On = FALSE, wait = 120;
	unsigned char Track[4000], keys[5] = { 0 } /* Left, Right, Gas, Backspace, Turbo */;
	unsigned char Old_Cheats = g->Cheats;
	Tile tile;
	memset(Track, 0, 4000);
	static const unsigned char widths[Max_Terrain + 1] =  // Width for each type of Terrain
	{ 8, 24, 40, 72, 72, 40, 48, 48, 16, 8, 8, 24, 24, 16, 16, 16, 16, 16, 208, 216, 96 };
	//A,  B,  C,  D,  E,  F,  G,  H,  I, J, K,  L,  M,  N,  O,  P,  Q,  R,   S,   T,  /
	
	/* NOTE:
	For a brief description of each type of terrain, see enum Tile_Detections in the 
	file Definitions.h -- */
	
	if (Edit_Track == TRUE) {
		a = 0;
		
		do 
		{
			Track[a] = g->Track[a];
		} while(g->Track[a++] < Fi11);
	}
	SafeFree(g->Track);
	
	memset(clock, 0, sizeof(Clock));
	g->camera_x  = 50;
	g->softx     = -1;
	g->Width     = 4000;
	g->Track     = Track;
	g->Cheats    = FALSE;
	
	do 
	{
		GrayClearScreen2B_R(Terrainl, Terraind);
		for(a = 0; a < LCD_WIDTH + (TI89 << 3); a+=8) {
			b = g->camera_x - (LCD_WIDTH >> 3) + (a >> 3);
			
			if (On == FALSE && (a > 24 && a < 
			(track_width(Convert_Track(g->camera_x - ((LCD_WIDTH - 32) >> 3), widths)) << 3) + 32)) {
				if (Track[g->camera_x - ((LCD_WIDTH - 32) >> 3)] == Gra1)
					continue;
				tile = Tiles[Gra1];
			} else tile = Tiles[Track[b]];
			
			short cut = (TI89 && tile.Height == 96);
			CGraySprite8_OR_R(a, g->lcd_height - tile.Height + cut, tile.Height + cut, 
				tile.PtrLight + cut, tile.PtrDark + cut);
			
			if (b & 1 && tile.Height <= 56)
				CGraySprite8_OR_R(a, g->lcd_height - 64, 8, Hedge, Hedge + 8);
		}
		if (keys[4] != 2) keys[4] = On;
		Draw_Editor(pos, &Fading);
		
		// Delay in Terrain Blinking
		if (keys[4] != 2) {
			for(; wait--;) {
				pokeIO(0x600005,0b11111); /* low power mode, wake up CPU only on AI 1 
		     	                          (grayscale), 2 (Keypress), 3 (AMS clock), 4 
		      	                         (Link), 5 (AMS timer base, needed for APD) 
		        	                       and 6 (ON, always wakes up the CPU)
		         	                      -- Saves Battery Power -- */
		    if (_rowread(0)) {
		    	wait -= 5;
		    	break;
		    }
		  }
		  if (wait <= 0) {  // Make the Terrain Blink
				wait = 120;
				On = !On;
			}
			
			if (_KeyCheck(LEFTKEY)) {
				if (keys[0] == FALSE || ++keys[0] > 5) {
					keys[0] = TRUE;
					if (--pos < 0) pos = Max_Terrain;
				}
			} else keys[0] = FALSE;
			if (_KeyCheck(RIGHTKEY)) {
				if (keys[1] == FALSE || ++keys[1] > 5) {
					keys[1] = TRUE;
					if (++pos > Max_Terrain) pos = 0;
				}
			} else keys[1] = FALSE;
			
			if (_KeyCheck(ESCKEY)) {
				b = FALSE;
				for(a = 0; a < 4000; a++) {  // Search for a Finish Line
					if (Track[a] == Fi11) {
						b = TRUE;
						break;
					}
				}
				
				g->Track = NULL;
				a = Menu(EFG("Save Track?", "Sauvegarder ?", "Strecke speichern?"), (char*)edit_menu, EFG(7,7,10), 0, TRUE);
				g->Track = Track;
				if (a == 0) {   // Exit and save the Track
					if (b == TRUE)
						break;
					// Display Message telling the User to place a Finish Line before saving
					#ifdef ENGLISH
					Message((const char *const[]){ "- Save Error -", "", "Must Place Finish Line!", NULL }, 
						A_WAITKEY);
					#elif defined(FRENCH)
					Message((const char *const[]){ "- Erreur -", "", "Où est l'arrivée ?", NULL }, A_WAITKEY);
					#elif defined(GERMAN)
					Message((const char *const[]){ "- Speicherfehler -", "", "Kein Ziel platziert!", NULL }, A_WAITKEY);
					#endif
				} else if (a != 2) {  // Exit without saving the Track
					g->Cheats = Old_Cheats;
					g->Track  = NULL;
					return;
				}
				
				// Return to the Editor
				Fading = TRUE;
			}
			
			if (_KeyCheck(GASKEY)) {  // Choose a piece to set down
				if (keys[2] == FALSE) {
					keys[2] = TRUE;
					Cur_Piece = 0;
					for(b = 0; b < pos; b++)
						Cur_Piece += (widths[b] >> 3);
					
					for(a = 0; a < track_width(pos); a++) {
						for(b = track_width(Convert_Track(g->camera_x - ((LCD_WIDTH - 32) >> 3) + a, widths)); b--;)
							Track[g->camera_x - ((LCD_WIDTH - 32) >> 3) + a + b] = Gra1;
						
						if (pos == 15 || pos == 16)
							Track[g->camera_x - ((LCD_WIDTH - 32) >> 3) + a] = Cur_Piece + (a != 0 && a != 55);
						else if (pos == 17)
							Track[g->camera_x - ((LCD_WIDTH - 32) >> 3) + a] = Cur_Piece + (a != 0 && a != 23);
						else Track[g->camera_x - ((LCD_WIDTH - 32) >> 3) + a] = Cur_Piece + a;
					}
					On = TRUE;
					wait = 120;
				}
			} else keys[2] = FALSE;
			
			if (_keytest (RR_BCKSPC)) {
				if (g->camera_x > 50 && (keys[3] == FALSE || ++keys[3] > 5)) {
					keys[3] = TRUE;
					g->camera_x -= track_width(Convert_Track(g->camera_x - ((LCD_WIDTH - 24) >> 3), widths));
					continue;
				}
			} else keys[3] = FALSE;
		}
		
		if (_KeyCheck(TEACHERKEY)) {
			g->Track = NULL;
			LeadOut();
		}
		
		if (keys[4] == 2 || (_KeyCheck(TURBOKEY) && keys[4] == TRUE)) {
			
			do 
			{
				// Retrieve the currently selected Terrain type
				Cur_Piece = Convert_Track(g->camera_x - ((LCD_WIDTH - 32) >> 3), widths);
				
				// Scroll past the currently selected Terrain
				for(a = 0; a < (track_width(Cur_Piece) << 3); a+=2) {
					Shift_Left(2);
					Draw_Editor(pos, &Fading);
				}
				
			} while(_KeyCheck(TURBOKEY) && Cur_Piece != Max_Terrain);
			
			if (Cur_Piece == Max_Terrain)  // Finish Line (End of Track)
				break;
			keys[4] = FALSE;
			On = FALSE;
			wait = 120;
		}
		if (_KeyCheck(TURBOKEY)) {
			keys[4] = 2;
			On = TRUE;
		}
	} while (1);
	while(_KeyCheck(ESCKEY));
	
	g->Cheats = Old_Cheats;
	g->Track  = NULL;
	Save_Track(Track);
}

short Convert_Track(short x, const unsigned char *widths) {
	short Cur = g->Track[x], b = -1;
	do 
	{
		Cur -= (widths[++b] >> 3);
	} while (Cur >= 0);
	
	return b;
}

void Draw_Editor(short pos, short *Fading) {
	short a = Max_Terrain;
	unsigned char No[2] = {0, 0};
	
	CopyGrayScreens(Terrainl, Terraind, Hiddenl, Hiddend);
	GrayClipSprite32_MASK_R(12, g->lcd_height - 24, 24, BikeGfx[Regular1], BikeGfx[Regular1] + 24, 
		BikeGfx[Regular1] + 48, BikeGfx[Regular1] + 48, Hiddenl, Hiddend);
	for(; a--;) {
		No[0] = 'A' + a;
		FS_DrawGrayString(7 * a, 0, No, A_SHADOWED, F_6x8);
	}
	
	FS_DrawGrayString(7 * Max_Terrain, 0, "/", A_SHADOWED, F_6x8);
	GrayInvertRect2B(7 * pos, 0, 7 * pos + 6, 7, Hiddenl, Hiddend);
	/*if (Fading == NULL) {
		FS_DrawGrayString(0, 18, "Must Place Finish Line!", A_SHADOWED|A_CENTERED, F_6x8);
		Toggle_Screens_Fast();
	} else */
	if (*Fading == TRUE) {
		*Fading = FALSE;
		Dissolve();
	} else Toggle_Screens_Fast();
}

static inline void Save_Track(unsigned char *Track) {
	g->Width = 0;
	
	while(Track[++g->Width] < Fi11);
	// Ask User for the Number of Laps in their custom level
	if ((Track[g->Width + 1] = Get_Number(EFG("Number of Laps?", "Nombre de tours ?", "Anzahl Runden?"), 
		2, 1, 9)) == FALSE)
		return;
	//Track[g->Width + 2] = 0;
	//Track[g->Width + 3] = 0;
	
	#ifdef ENGLISH
	if (Save_File(g->Track_Name, Track, g->Width + 2) == FALSE)
		Message((const char *const[]){ "- Error -", "", "Track save has Failed", NULL }, 
			A_DISSOLVE | A_WAITKEY);
	else Message((const char *const[]){ "- Success -", "", "Track has been Saved", NULL }, 
			A_DISSOLVE | A_WAITKEY);
	#elif defined(FRENCH)
	if (Save_File(g->Track_Name, Track, g->Width + 2) == FALSE)
		Message((const char *const[]){ "- Erreur -", "", "La sauvegarde a échoué", NULL }, 
			A_DISSOLVE | A_WAITKEY);
	else Message((const char *const[]){ "- Réussite -", "", "La sauvegarde a réussi", NULL }, 
			A_DISSOLVE | A_WAITKEY);
	#elif defined(GERMAN)
	if (Save_File(g->Track_Name, Track, g->Width + 2) == FALSE)
		Message((const char *const[]){ "- Fehler -", "", "Speichern gescheitert", NULL }, 
			A_DISSOLVE | A_WAITKEY);
	else Message((const char *const[]){ "- Erfolg -", "", "Speichern gelungen", NULL }, 
			A_DISSOLVE | A_WAITKEY);
	#endif
}

// Decompress, allocate, and Load a Track
short Load_Track(unsigned char *Track) {
	if (Track == NULL)
		return QUIT;
	
	SafeFree(g->Track);
	char Best_Time1 = *Track++, Best_Time2 = *Track++;
	if ((g->Track = malloc(*(unsigned short*)Track + 2)) == NULL)
		return QUIT;
	
	// Decompress the external track's data into the newly allocated Track buffer
	DecompressRLE(g->Track, (Track + 2), *(unsigned short*)Track);
	
	// Determine the Track's Length by searching for the End of the Finish Line
	//g->Width = 0;
	//g->Width = *(unsigned short*)Track - 2 - 2 * (g->Game_Progress == QUIT);
	g->Width = *(unsigned short*)Track - 2;
	g->Track[g->Width + 2] = Best_Time1;
	g->Track[g->Width + 3] = Best_Time2;
	//while(g->Track[++g->Width] < Fi11);
	return 0;
}

short Select_External_Track(const char *Title) {
	char tracks[99][9];
	short Choice;
	
	if (Find_All_Files(tracks, Track_Magic) > 0) {
		if ((Choice = Menu(Title, (char*)tracks, 9, 0, TRUE)) != QUIT) {
			if (Load_Track(Get_Data_Ptr(tracks[Choice], 2)) != QUIT) {
				strcpy(g->Track_Name, tracks[Choice]);
				return 0;  // Track Successfully Decompressed and Loaded
			} else {
				#ifdef ENGLISH
				Message((const char *const[]){ "- Error -", "", "Loading Track has", "Failed", NULL }, 
					A_DISSOLVE | A_WAITKEY);
				#elif defined(FRENCH)
				Message((const char *const[]){ "- Erreur -", "", "Le chargement du", "circuit a échoué", NULL }, 
					A_DISSOLVE | A_WAITKEY);
				#elif defined(GERMAN)
				Message((const char *const[]){ "- Fehler -", "", "Laden der Strecke", "gescheitert", NULL }, 
					A_DISSOLVE | A_WAITKEY);
				#endif
			}
		}
	} else {
		#ifdef ENGLISH
		Message((const char *const[]){ "- Error -", "", "No External Tracks", "Located on Calc", NULL }, 
			A_DISSOLVE | A_WAITKEY);
		#elif defined(FRENCH)
		Message((const char *const[]){ "- Erreur -", "", "Aucun circuit externe", "n'a été trouvé", NULL }, 
			A_DISSOLVE | A_WAITKEY);
		#elif defined(GERMAN)
		Message((const char *const[]){ "- Fehler -", "", "Keine externe Strecke", "auf dem Rechner", NULL }, 
			A_DISSOLVE | A_WAITKEY);
		#endif
	}
	
	return QUIT;
}

static inline short Find_All_Files(char (*File_Names)[9], const unsigned short Magic_NO) {
	short Counter = 0;
	SYM_ENTRY *SymPtr = SymFindFirst (NULL, FO_RECURSE);
	
	while(SymPtr != NULL) {
		if (*(unsigned short*)(HeapDeref(SymPtr->handle) + 2) == Magic_NO)
			strcpy(File_Names[Counter++], SymPtr->name);
		SymPtr = SymFindNext();
	}
	
	File_Names[Counter][0] = 0;
	return Counter;
}

short Best_Times(short i) {
	unsigned short *file = NULL, Time;
	
	/*if (g->Game_Progress == QUIT)  // Track is loaded from an external file
		Time = (g->Track[g->Width + 2] << 8) + (g->Track[g->Width + 3]);
	else {  // One of the Default Tracks
		if ((file = (short*)Get_Data_Ptr(exctimes, g->Game_Progress * 2)) != NULL)
			Time = *file;
		else Time = Default_Times[g->Game_Progress];
	}*/
	
	if (g->Game_Progress == QUIT || (file = (short*)Get_Data_Ptr(exctimes, g->Game_Progress * 2)) == NULL)
		Time = (short)((g->Track[g->Width + 2] << 8) + (g->Track[g->Width + 3]));
	else Time = *file;
	
	if (i == 0)  // Minutes
		Time = (Time & 0x07FF) / 60;
	if (i == 1)  // Seconds
		Time = (Time & 0x07FF) % 60;
	if (i == 2)  // Mseconds50
		Time = (Time & 0xF800) >> 9;
	
	return Time;
}

short New_Track_Record(void) {
	unsigned short Time = (60 * clock->Minutes + clock->Seconds) | (clock->Mseconds50 << 11);
	char *Base;
	
	if (g->Game_Progress == QUIT) { // Track is loaded from an external file
		UnArchive(g->Track_Name);
		
		// Locate File Base
		if ((Base = Get_Data_Ptr(g->Track_Name, 2)) == NULL)
			return FALSE;
		
		/*short a;
		for(a = -5; a < 5; a++) {
			PortSet(Hiddend, 239, 127);
			printf_xy(0, 7*(a + 5), "%d %d", *(char*)(Base + (*(unsigned short*)Base - 6 - 
				((*(short*)(Base + 4)) & 1)) + a), ((*(short*)(Base + 4)) & 1));
			PortSet(Hiddenl, 239, 127);
			printf_xy(0, 7*(a + 5), "%d %d", *(char*)(Base + (*(unsigned short*)Base - 6 - 
				((*(short*)(Base + 4)) & 1)) + a), ((*(short*)(Base + 4)) & 1));
		}
		CopyGrayScreens(Hiddenl, Hiddend, Activel, Actived);
		
		while(!_keytest(RR_CLEAR));
		while(_keytest(RR_CLEAR));*/
		//Base += *(unsigned short*)Base - 6 - ((*(short*)(Base + 4)) & 1);
		
		// Write record to the file
		*(Base + 0) = (char)(Time >> 8);
		*(Base + 1) = (char)Time;
		
		Archive(g->Track_Name);
	} else {  // One of the Default Tracks
		const unsigned char Tag[6] = { 0, 'r', 'e', 'c', 0, OTH_TAG };
		unsigned short Size = sizeof(Default_Times) + sizeof(Tag) + 2;
		HANDLE h;
		
		UnArchive(exctimes);  // Make sure that before writing to the file, it is not archived
		
		if ((Base = Get_Data_Ptr(exctimes, 0)) == NULL) {
			if ((h = HeapAlloc(Size)) == H_NULL)
				return FALSE;
			if ((Base = HeapDeref(DerefSym(SymAdd(SYMSTR(exctimes)))->handle = h)) == NULL) {
				HeapFree(h);
		    return FALSE;
			}
			*(short*)Base = Size - 2;
			Base += 2;
			memcpy(Base, &Default_Times, sizeof(Default_Times));
			memcpy(Base + sizeof(Default_Times), &Tag, sizeof(Tag));
		}
		// Write the new record time to the file
		*(short*)(Base + 2 * g->Game_Progress) = Time;
		
		Archive(exctimes);    // Automatically Archive the file in case of a crash
	}
	
	return TRUE;
}