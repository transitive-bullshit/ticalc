// C Source File
// Created 8/23/2004; 8:57:22 AM
#include "All.h"

#define CONFIG_SIZE			(5 + sizeof(HSC[5]))
//#define FILE_SIZE				(CONFIG_SIZE + 8)

// Saves the Difficulty and Time Limit Settings
void Save_Config(void) {
	static const unsigned char Extension[6] = { 0, 'c', 'f', 'g', 0, OTH_TAG };
	unsigned short Size = CONFIG_SIZE + 8;
	char *Base;
	HANDLE h;
	
	/* "sumocfg" File Layout:
		unsigned short Size;
		char Game_Speed;				// Defines the amount to slow the game each frame (0-15)
		char Time_Limit;				// Match Time Limit (optional)
		char Smack_Talk;				// PreMatch Smack Talk (On/Off)
		char Difficulty;				// AI Difficulty Level
		char Distortion;				// Wavy screen effect
		HSC high[5];						// Game High Scores
		char Zero1;
		char Type[3];
		char Zero2;
		char Tag;
	*/
	
	UnArchive("sumocfg");  // Make sure that before writing to the file, it is not archived
	
	if ((h = HeapAlloc(Size)) == H_NULL)
		return;
// Note: HeapDeref(H_NULL) == 0xFFFFFFFF.
	if ((Base = HeapDeref(DerefSym(SymAdd(SYMSTR_CONST("sumocfg")))->handle = h)) == (void*)0xFFFFFFFF) {
		HeapFree(h);
    return;
	}
	
	//if (g->Game_Speed < 1)
	//	g->Game_Speed = 9;
	
	// Write data to "sumocfg" file
	*(short*)Base = Size - 2;
	Base += 2;
	memcpy(Base, &g->Game_Speed, CONFIG_SIZE);
	Base += CONFIG_SIZE;
	memcpy(Base, Extension, sizeof(Extension));
	
	Archive("sumocfg");    // Automatically Archive the config file in case of a crash
}

// Attempt to load settings from the external Configuration File "sumocfg"
void Load_Config(void) {
	const char * const custom_names[5] = { "George Lazenby", "Timothy Dalton", "Pierce Brosnan", 
		"Roger Moore", "Sean Connery"};  // 007... Default High Scores
	
	// Set Default Settings
	g->Game_Speed = 10;
	g->Time_Limit = FALSE;
	g->Smack_Talk = TRUE;
	g->Difficulty = FALSE;
	g->Distortion = FALSE;
	short a = 5;
	for(; a--;) {  // Default High Scores
		strcpy(g->high[a].name, custom_names[a]);
		g->high[a].score = 100 * (a + 1);
	}
	
	// Overwrite default settings with those in the external file
	unsigned char *Ptr = Get_Data_Ptr("sumocfg", 0);
	if (Ptr != NULL)
		memcpy(&g->Game_Speed, Ptr, CONFIG_SIZE);
}

// Save the Progress of a Tournament Game
short Save_Tournament(void) {
	static const unsigned char Extension[6] = { 0, 's', 'a', 'v', 0, OTH_TAG };
	unsigned short Size = 13;
	char *Base;
	HANDLE h;
	
	/* "sumtourn" File Layout:
		unsigned short Size;
		short Progress;
		short x;
		char char_index;
		char Zero1;
		char Type[3];
		char Zero2;
		char Tag;
	*/
	
	UnArchive("sumtourn");  // Make sure that before writing to the file, it is not archived
	
	if ((h = HeapAlloc(Size)) == H_NULL)
		return FALSE;
// Note: HeapDeref(H_NULL) == 0xFFFFFFFF.
	if ((Base = HeapDeref(DerefSym(SymAdd(SYMSTR_CONST("sumtourn")))->handle = h)) == (void*)0xFFFFFFFF) {
		HeapFree(h);
    return FALSE;
	}
	
	// Write data to "sumtourn" file
	*(short*)Base = Size - 2;
	memcpy(Base + 2, &g->Progress, sizeof(short) * 2);
	Base += 2 + sizeof(short) * 2;
	*Base++ = g->wrestlers[0].c->Index;
	memcpy(Base, Extension, sizeof(Extension));
	
	Archive("sumtourn");    // Automatically Archive the save file in case of a crash
	
	return TRUE;
}

// Load a saved Tournament from the file "sumtourn"
short Load_Tournament(void) {
	unsigned char *Ptr = Get_Data_Ptr("sumtourn", 0);
	if (Ptr == NULL)
		return FALSE;
	
	memcpy(&g->Progress, Ptr, sizeof(short) * 2);
	Ptr += sizeof(short) * 2;
	g->wrestlers[0].c = g->Characters[*Ptr];
	
	return TRUE;
}

unsigned char *Get_Data_Ptr(const char *Filename, short Offset) {
	unsigned char *Location;
	SYM_ENTRY *SymPtr;
	HANDLE h;
	
	if ((SymPtr = SymFindPtr(SYMSTR(Filename), 0)) == NULL)
		return NULL;  // File not found
	
	h = SymPtr->handle;
	
	if ((Location = (unsigned char*) HeapDeref(h)) == (void*)0xFFFFFFFF)
		return NULL;
	return (Location + 2 + Offset);
}

void Archive(const char *Filename) {
	SYM_ENTRY *SymPtr = NULL;
	HSym hsym = SymFind(SYMSTR(Filename));
	
	SymPtr = DerefSym (hsym);
	
	if (EM_findEmptySlot(*Get_Data_Ptr(Filename, -2)) == NULL)
		return;  // Make sure Garbage Collection will not occur
	if (!SymPtr->flags.bits.archived)
		EM_moveSymToExtMem(SYMSTR(Filename), HS_NULL);
}

void UnArchive(const char *Filename) {
	SYM_ENTRY *SymPtr = NULL;
	HSym hsym = SymFind(SYMSTR(Filename));
	
	SymPtr = DerefSym (hsym);
	
	if (SymPtr->flags.bits.archived)
		EM_moveSymFromExtMem(SYMSTR(Filename), HS_NULL);
}