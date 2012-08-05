// C Source File
// Created 10/16/2004; 3:47:05 PM
#include "All.h"
static inline void Run_Editor(const char Edit);
static inline short Save_Wrestler(short);

void Wrestler_Editor(void) {
	#ifdef ENGLISH
	static const char *const edit_menu[] = 
		{ "New Wrestler", "Edit Wrestler", "Delete Wrestler", "- Back", NULL };
	#elif defined(FRENCH)
	
	#elif defined(GERMAN)
	
	#endif
	
	short Choice;
	
	do
	{
		Choice = Menu(EFG("- Sumo Editor -"), edit_menu, 0, TRUE, 0);
		
		if (Choice == 1 || Choice == 2) {
			g->wrestlers[1].c = NULL;
			if (Choose_Wrestler(Choice == 1 ? EFG("Edit Wrestler") : EFG("Delete Wrestler"), 0, TRUE, TRUE) == QUIT)
				continue;
		}
		if (Choice == 2) {  // Delete Wrestler
			UnArchive(g->Sumo_Name);
			if (SymDel(SYMSTR(g->Sumo_Name)) == FALSE) { // Error deleting wrestler
				#ifdef ENGLISH
				Message((const char *const[]){ "- Error -", "", g->Sumo_Name, "Cannot be Deleted", NULL }, 
					A_DISSOLVE | A_WAITKEY);
				#elif defined(FRENCH)
				
				#elif defined(GERMAN)
				// 60,379
				#endif
			} else {
				Load_Wrestlers();
				g->wrestlers[0].c = NULL;
				#ifdef ENGLISH
				Message((const char *const[]){ "- Success -", "", g->Sumo_Name, "Has been Deleted", NULL }, 
					A_DISSOLVE | A_WAITKEY);
				#elif defined(FRENCH)
				
				#elif defined(GERMAN)
				
				#endif
			}
		} else if (Choice == FALSE || Choice == 1) {  // Create/Edit Wrestler
			Run_Editor(Choice);
			return;
		} else return;
		
		
		/*if (Choice == 0) {
			//if (Get_Input("New Wrestler Name:", g->Sumo_Name, 8) != QUIT)
			Run_Editor(FALSE);
			
			return;
		} else if (Choice == 1 || Choice == 2) {
			
			if (Choose_Wrestler(Choice == 1 ? "Edit Wrestler" : "Delete Wrestler", 0, TRUE, TRUE) == QUIT) {
				continue;
			} else if (Choice == 1) {  // Edit Wrestler
				Run_Editor(TRUE);
				return;
			} else if (Choice == 2) {  // Delete Wrestler
				UnArchive(g->Sumo_Name);
				if (SymDel(SYMSTR(g->Sumo_Name)) == FALSE)  // Error deleting wrestler
					Message((const char *const[]){ "- Error -", "", "The Wrestler", g->Sumo_Name, 
						"Cannot be Deleted", NULL }, A_DISSOLVE | A_WAITKEY);
			}
		} else return;*/
	} while(1);
}

static inline void Run_Editor(const char Edit) {
	unsigned short points = 80;//MAX_STAT * 5 / 2;
	short pos = 0, Fading = TRUE;
	short Exit = FALSE, a;
	
	if (Edit) {
		points = g->Remaining_Pts;
		memcpy((char*)g->Characters[Load_Slot] + 1, (char*)g->wrestlers[0].c + 1, sizeof(Character) - 1);
		memcpy(g->strings[5 * Load_Slot], g->strings[5 * g->wrestlers[0].c->Index], One_String_Size);
	} else {
		if (*g->strings[5 * Load_Slot + C_Name] == FALSE)
			strcpy(g->strings[5 * Load_Slot + C_Name], EFG("New Wrestler"));
		if (*g->strings[5 * Load_Slot + C_Origin] == FALSE)
			strcpy(g->strings[5 * Load_Slot + C_Origin], EFG("unknown"));
		
		memset((char*)g->Characters[Load_Slot] + 1, 0, sizeof(Character) - 1);
		memset(g->strings[5 * Load_Slot + C_Smack], 0, 256 * 3);
	}
	
	g->wrestlers[0].c = g->Characters[Load_Slot];
	g->wrestlers[1].c = NULL;
	Draw_Pre_Match(FALSE, pos);
	New_Text(EFG("Up/Down selects trait\x0A""Enter modifies it"), PAUSE_DELAY);
	
	do {
		
		Run_Text();
		if (Fading) {
			Dissolve();
			Fading = FALSE;
		} else Toggle_Screens();
		
		KeyScan();
		if (_KeyCheck(TEACHERKEY))
			LeadOut();
		
		if (_KeyCheck(ENTERKEY | A_KEY)) {
			if (pos == 0 || pos == 3) {
				Get_Input(pos ? EFG("Name:") : EFG("Origin:"), g->strings[5 * Load_Slot + !pos], 52, NULL);
				Fading = TRUE;
			}
			if (pos == 1) {
				short Age;
				Get_Input(EFG("Age:"), NULL, 3, &Age);
				g->Characters[Load_Slot]->Age = Age;
				Fading = TRUE;
			}
			if (pos == 2) {
				Get_Input(EFG("Weight:"), NULL, 4, &g->Characters[Load_Slot]->Weight);
				Fading = TRUE;
			}
			if (pos == 4) { // Smack Talk Strings
				while_keypress(ENTERKEY);
				// Temporarily reinstall AMS default interrupt settings
				GrayOff();
				SetIntVec(AUTO_INT_1, interrupt1);
				SetIntVec(AUTO_INT_5, interrupt5);
				
				HANDLE dlg = H_NULL;
				if ((dlg = DialogNewSimple(150, 60)) == H_NULL)
					ERROR(MEMORY);
				
				char smack_talk[3 * 256];  // 256 is the Maximum length of a user-defined string
				memcpy(smack_talk, g->strings[5 * Load_Slot + 2], 256 * 3);
				/*strcpy(smack_talk, g->strings[5 * Load_Slot + 2]);
				strcpy(smack_talk + 256, g->strings[5 * Load_Slot + 3]);
				strcpy(smack_talk + 512, g->strings[5 * Load_Slot + 4]);*/
				
				DialogAddTitle(dlg,EFG("- Smack Talk -"), BT_OK, BT_NONE);
				DialogAddRequest(dlg, 2, 15, "1)", 0, 256, 22);
				DialogAddRequest(dlg, 2, 26, "2)", 256, 256, 22);
				DialogAddRequest(dlg, 2, 37, "3)", 512, 256, 22);
				if (DialogDo(dlg, CENTER, CENTER, smack_talk, NULL) == KEY_ENTER)
					memcpy(g->strings[5 * Load_Slot + 2], smack_talk, 256 * 3);
				
				// CleanUp Dialog Box and Restore Game Settings
				HeapFree(dlg);
				SetIntVec(AUTO_INT_1, INT_1_COUNTER);
				SetIntVec(AUTO_INT_5, CLOCK_TIMER);
				while_keypress(ENTERKEY | ESCKEY);
				if (!GrayOn())  // Turn Grayscale Back On
					ERROR(MEMORY);
			}
			if (pos == 9)
				Exit = TRUE;
		}
		
		if (_KeyCheck(ESCKEY))
			Exit = TRUE;
		
		if (g->text == NULL) {
			char buf[20];
			sprintf(buf, EFG("%d Pts Remaining"), points);
			//short old = (*g->text != 'U');
			New_Text(buf, FALSE);
			//if (old) g->text_i = 15;
		}
		
		if (Exit) {
			#ifdef ENGLISH
			short a = Menu("- Save Changes -", (const char *const[]){ "Yes", "No", "Cancel", NULL }, 0, TRUE, 0);
			#elif defined(FRENCH)
			
			#elif defined(GERMAN)
			
			#endif
			/*if (a == 0 && Save_Wrestler(points) == FALSE) {
				Message((const char *const[]){ "- Error -", "", "Wrester not saved", NULL }, A_DISSOLVE | A_WAITKEY);
			} else if (a != 2)
				return;*/
			
			if (a == 0) {
				if (Save_Wrestler(points) != FALSE) {
					Load_Wrestlers();
					g->wrestlers[0].c = g->Characters[g->NO_WRESTLERS - 1];
					return;
				}
				Message((const char *const[]){ EFG("- Error -"), "", EFG("Save Failed"), NULL }, 
					A_DISSOLVE | A_WAITKEY);
			} else if (a != 2) {
				g->wrestlers[0].c = NULL;
				return;
			}
			
			Exit = FALSE;
			Fading = TRUE;
		} else if (_KeyCheck(UPKEY)) {
			if (--pos < 0) pos = 9;
		} else if (_KeyCheck(DOWNKEY)) {
			if (++pos > 9) pos = 0;
		} else if (pos >= 5 && pos <= 8) {
			char *Ptr = ((char*)(&g->Characters[Load_Slot]->Strength) + (pos - 5));
			
			short calc = g->calculator;
			if (_keytest(5-2*calc, !calc))
				points = 128;
			else if (points > 0 && *Ptr < MAX_STAT  && _KeyCheck(ENTERKEY | A_KEY | RIGHTKEY)) {
				(*Ptr)++; points--;
			} else if (*Ptr > 0 && _KeyCheck(LEFTKEY)) {
				(*Ptr)--; points++;
			} else continue;
			
			short y0 = g->Zeroy + 47 + 7 * (pos - 5);
			GrayDrawRect2B(g->Zerox + 26, y0, g->Zerox + 56, y0 + 2, COLOR_LIGHTGRAY, RECT_FILLED, Hiddenl, Hiddend);
			if (*Ptr > 0)
				GrayDrawRect2B(g->Zerox + 26, y0, g->Zerox + 25 + (*Ptr), y0 + 2, COLOR_BLACK, RECT_FILLED, 
					Hiddenl, Hiddend);
			
			for(a = 10; a--;)
				pokeIO(0x600005,0b11101);
			
			char buf[20];
			sprintf(buf, EFG("%d Pts Remaining"), points);
			New_Text(buf, FALSE);
			continue;
		} else if (Fading == FALSE) continue;
		
		Draw_Pre_Match(FALSE, pos);  // Redraw Editor Screen
		for(a = 15; a--;)
			pokeIO(0x600005,0b11101);
	} while(1);
}

// FileName is derived from wrestler's name by modifying it to create a Valid SymName
void ValidateSumoFileName(char *FileName, short Index) {
	char *Name = Char_String(g->Characters[Index], C_Name);  // Src string (wrestler's name)
	unsigned char Val, a = 0;
	
	do // Ensure FileName is Valid
	{
		Val = tolower(*Name++);
		if ((Val >= 'a' && Val <= 'z') || (Val >= '0' && Val <= '9'))
			FileName[a++] = Val;  // FileName Dest buffer
	} while (Val != 0 && a < 8);
	FileName[a] = 0;
}

/* ------------------------------------------------------------------------------- */
/* --- Saves a Wrestler (stored in the Load_Slot position) to an external file --- */
/* ------------------------------------------------------------------------------- */
static inline short Save_Wrestler(short points) {
	const unsigned char Tag[7] = { 0, 's', 'u', 'm', 'o', 0, OTH_TAG };
	unsigned short str_size = 5, a;
	char *Base;
	HANDLE h;
	
	for(a = 5; a--;)
		str_size += strlen(Char_String(g->Characters[Load_Slot], a));
	
	/* External Wrestler File Layout:
		unsigned short Size;
		unsigned short Magic_No;
		Character c;
		char Remaining_Pts;
		char stringdata[str_size];//Size + (Size & 1)
		unsigned char Tag[7];
	*/
	unsigned short Size = 5 + sizeof(Character) + str_size + sizeof(Tag);
	
	char FileName[9];
	//sprintf(FileName, "%.8s", Char_String(g->Characters[Load_Slot], C_Name));
	
	ValidateSumoFileName(FileName, Load_Slot);  // Ensure FileName is Valid
	
	/*memset(Activel, 0, LCD_SIZE);  // Check for Valid FileName
	memset(Actived, 0, LCD_SIZE);
	PortSet(Actived, 239, 127);
	printf("%s\n%s", Char_String(g->Characters[Load_Slot], C_Name), FileName);
	WaitKey();*/
	
	UnArchive(FileName);  // Make sure that before writing to the file, it is not archived
	
	if ((h = HeapAlloc(Size)) == H_NULL)
		return FALSE;
	if ((Base = HeapDeref(DerefSym(SymAdd(SYMSTR(FileName)))->handle = h)) == NULL) {
		HeapFree(h);
    return FALSE;
	}
	
	// Write data to file
	*(short*)Base = Size - 2;
	Base += 2;
	*(short*)Base = Sumo_Magic;
	Base += 2;
	memcpy(Base, g->Characters[Load_Slot], sizeof(Character));
	Base += sizeof(Character);
	*Base++ = points;
	for(a = 0; a < 5; a++) {
		strcpy(Base, Char_String(g->Characters[Load_Slot], a));
		Base += strlen(Char_String(g->Characters[Load_Slot], a)) + 1;
	}
	memcpy(Base, Tag, sizeof(Tag));
	
	Archive(FileName);    // Automatically Archive the file in case of a crash
	return TRUE;
}

/* External wrestler File Layout:
	unsigned short Size;
	unsigned short Magic_No;
	Character c;
	char Remaining_Pts;
	char stringdata[str_size];
	char Zero1;
	char Type[4];  ("sumo")
	char Zero2;
	char Tag;
*/

short Find_All_Files(char (*File_Names)[9], const char Copy_Info) {
	SYM_ENTRY *SymPtr = SymFindFirst (NULL, FO_RECURSE);
	short Counter = 1;
	
	// Loop through all files in the VAT, searching for the specific files characterized by Sumo_Magic
	while(SymPtr != NULL) {
		unsigned char *Base = (HeapDeref(SymPtr->handle) + 2);
		if (*(unsigned short*)Base == Sumo_Magic) {
			if ((char*)File_Names != NULL) {
				strcpy(File_Names[Counter], SymPtr->name);
			} else if (Copy_Info) {
				/*short a = 0;
				do {
					memset(Activel, 0, LCD_SIZE);
					memset(Actived, 0, LCD_SIZE);
					PortSet(Actived, 239, 127);
					clrscr(); ClrScr();
					if (a & 1)
						printf("%d) %d", a, *Base);
					else printf("%d) %d %d", a, *Base, *(short*)Base);
					
					a++; Base++;
					WaitKey();
				} while(1);*/
				
				Base += 3;
				memcpy((char*)g->Characters[Load_Slot + Counter] + 1, Base, sizeof(Character) - 1);
				Base += sizeof(Character) - 1;
				g->Remaining_Pts = *Base++;
				
				short a;
				for(a = 0; a < 5; a++) {
					//short len = 0;
					//while(*(Base + len++) != 0);
					
					char *str = g->strings[5 * (Load_Slot + Counter) + a];
					strcpy(str, Base);
					Base += strlen(str) + 1;
				}
			}
			
			Counter++;
		}
		
		SymPtr = SymFindNext();
	}
	
	return Counter;
}

/*static inline short Find_All_Files(char (*File_Names)[9], const unsigned short Magic_NO) {
	short Counter = 0;
	SYM_ENTRY *SymPtr = SymFindFirst (NULL, FO_RECURSE);
	
	while(SymPtr != NULL) {
		if (*(unsigned short*)(HeapDeref(SymPtr->handle) + 2) == Magic_NO)
			strcpy(File_Names[Counter++], SymPtr->name);
		SymPtr = SymFindNext();
	}
	
	File_Names[Counter][0] = 0;
	return Counter;
}*/

/*void strcpy_truncate(char *dest, const char *src, short max_width) {
	short a = 0;
	unsigned char val;
	do {
		val = *(src + a);
		*dest++ = val;
		*dest = '\0';
		
		if (DrawStrWidth(dest, F_4x6) > 
		
		++a >= max_width - 1) {
			break;
		}
	} while(val != '\0');
}*/