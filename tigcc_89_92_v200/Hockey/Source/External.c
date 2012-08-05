// C Source File
// Created 10/10/2003; 9:25:14 PM
#include "All.h"

// Saves the Difficulty and Time Limit Settings
void Save_Config(void) {
	//unsigned char Extension[] = { 0, 'c', 'f', 'g', 0, OTH_TAG };
	unsigned short Size = 11 + sizeof(RowKey[8]);
	char *Base;
	HANDLE h;
	if (g->Speed < 1 || g->Speed > 15)
		g->Speed = 7;
	
	/* "hockycfg" File Layout:
		unsigned short Size;
		char Config[3 + sizeof(RowKey[11])];
		char Zero1;
		char Type[3];
		char Zero2;
		char Tag;
	*/
	
	UnArchive(SYMSTR_CONST("hockycfg"));  // Make sure that before writing to the file, it is not archived
	
	if ((h = HeapAlloc(Size)) == H_NULL)
		return;
// Note: HeapDeref(H_NULL) == 0xFFFFFFFF.
	if ((Base = HeapDeref(DerefSym(SymAdd(SYMSTR_CONST("hockycfg")))->handle = h)) == (void*)0xFFFFFFFF) {
		HeapFree(h);
    return;
	}
	
	// Write data to "hockycfg" file
	*(short*)Base = Size - 2;
	Base += 2;
	memcpy(Base, &g->Keys, 3 + sizeof(RowKey[8]));
	Base += 3 + sizeof(RowKey[8]);
	*Base++ = 0;
	strcpy(Base, "cfg");
	*(Base + 4) = OTH_TAG;
	
	Archive(SYMSTR_CONST("hockycfg"));    // Automatically Archive the config file in case of a crash
}

// Tries to Load an external Hockey Configuration File
void Load_Config(void) {
	g->Difficulty = FALSE;  // Default Settings
	g->Time_Limit = 3;
	g->Speed      = 7;
	unsigned char *Ptr = Get_Data_Ptr(SYMSTR_CONST("hockycfg"), 0);
	if (Ptr == NULL || *(short*)(Ptr - 2) > 43)
		return;
	
	memcpy(&g->Keys, Ptr, 3 + sizeof(RowKey[8]));
	if (g->Speed < 1 || g->Speed > 15)
		g->Speed = 7;
}

// Loads all of the Team Data from the External File "hockteam"
void Load_Teams(void) {
	unsigned char *TeamPtr = NULL;
	short loaded = FALSE;
	
	// If there is not an external hockteam File, then just set the default teams
	if ((TeamPtr = Get_Data_Ptr(SYMSTR_CONST("hockteam"), 0)) == NULL) {
		// The Default Teams need to be loaded
		TeamPtr = (unsigned char *)Default_Western;
		g->No_Teams[0] = 16;
		g->No_Teams[1] = 15;
	} else {  // Teams need to be loaded
		g->No_Teams[0] = *TeamPtr++;
		g->No_Teams[1] = *TeamPtr++;
		loaded = TRUE;
	}
	
	if ((Western = malloc(sizeof(Names[g->No_Teams[0]]))) == NULL)
		ERROR(MEMORY);
	memcpy(Western, TeamPtr, sizeof(Names[g->No_Teams[0]]));
	if (loaded)
		TeamPtr += sizeof(Names[g->No_Teams[0]]);
	else TeamPtr = (unsigned char *)Default_Eastern;
	if ((Eastern = malloc(sizeof(Names[g->No_Teams[1]]))) == NULL)
		ERROR(MEMORY);
	memcpy(Eastern, TeamPtr, sizeof(Names[g->No_Teams[1]]));
}

// Saves all of the Team Data into the External File "hockteam"
void Save_Teams(void) {
	short Size = sizeof(Names[g->No_Teams[0]]) + sizeof(Names[g->No_Teams[1]]) + 11;
	char *Base;
	HANDLE h;
	/* "hockteam" File Layout:
		unsigned short Size;
		unsigned char No1;
		unsigned char No2;
		Names Western[g->No_Teams[0]];
		Names Eastern[g->No_Teams[1]];
		char Zero1;
		char Type[4];
		char Zero2;
		char Tag;
	*/
	UnArchive(SYMSTR_CONST("hockteam"));  // Make sure that before writing to the file, it is not archived
	
	if ((h = HeapAlloc(Size)) == H_NULL) {
		Archive(SYMSTR_CONST("hockteam"));
		return;
	} if ((Base = HeapDeref(DerefSym(SymAdd(SYMSTR_CONST("hockteam")))->handle = h)) == (void*)0xFFFFFFFF) {
		HeapFree(h);
    return;
	}
	
	// Write data to "hockteam" file
	*(short*)Base = Size - 2;
	Base += 2;
	*Base++ = g->No_Teams[0];
	*Base++ = g->No_Teams[1];
	memcpy(Base, Western, sizeof(Names[g->No_Teams[0]]));
	Base += sizeof(Names[g->No_Teams[0]]);
	memcpy(Base, Eastern, sizeof(Names[g->No_Teams[1]]));
	Base += sizeof(Names[g->No_Teams[1]]);
	*Base++ = 0;
	strcpy(Base, "Team");
	*(Base + 5) = OTH_TAG;
	
	Archive(SYMSTR_CONST("hockteam"));    // Automatically Archive the team file in case of a crash
}

// Used by Load_Teams(); Returns a Pointer to the Data of a File
unsigned char *Get_Data_Ptr(SYM_STR Filename, short Offset) {
	unsigned char *Location;
	SYM_ENTRY *SymPtr;
	HANDLE h;
	
	if ((SymPtr = SymFindPtr(Filename, 0)) == NULL)
		return NULL;  // File not found
	
	h = SymPtr->handle;
	
	if ((Location = (unsigned char*) HeapDeref(h)) == NULL)
		return NULL;
	return (Location + 2 + Offset);
}

void Archive(SYM_STR Filename) {
	SYM_ENTRY *SymPtr = NULL;
	HSym hsym = SymFind(Filename);
	
	SymPtr = DerefSym (hsym);
	
	if (EM_findEmptySlot(*Get_Data_Ptr(Filename, -2)) == NULL)
		return;  // Make sure Garbage Collection will not occur
	if (!SymPtr->flags.bits.archived)
		EM_moveSymToExtMem(Filename, HS_NULL);
}

void UnArchive(SYM_STR Filename) {
	SYM_ENTRY *SymPtr = NULL;
	HSym hsym = SymFind(Filename);
	
	SymPtr = DerefSym (hsym);
	
	if (SymPtr->flags.bits.archived)
		EM_moveSymFromExtMem(Filename, HS_NULL);
}

#ifdef COMPRESSION

/* Decompresses the RLE Vertically Compressed Titlescreen */
void RLE_Decompress(unsigned char *src, unsigned char *dest, short size)
{
	int i = 0, j;
	int pos = 0;
	int dcmpNum = 0;
	
	while (dcmpNum < size)
	{
		//decompress a run
		if (src[i] == 0x91)
		{
			i++;
			for(j = 0; j < src[i + 1]; j++)
			{
				dest[pos] = src[i];
				pos += 20;
				if (pos > 20 * 100)
				{
					pos -= 20 * 100;
					pos++;
				}
			}
			i++;
			dcmpNum += src[i];
		}
		else
		{
			dest[pos] = src[i];
			pos += 20;
			if (pos > 20 * 100)
			{
				pos -= 20 * 100;
				pos++;
			}
			dcmpNum++;
		}
		i++;
	}
}
#endif

/*void Save_Tournament(void) {
	char *Base, FileName[8] = "hockys";
	HANDLE h;
	
	struct {
		unsigned char Size;
		char Score[2];
		char Teams[2][2];
		char T_Board[8][3];
		char Round;
		char Difficulty;
		char Seconds;
		char Minutes;
		char Zero1;
		char Type[3];
		char Zero2;
		char Tag;
	} Save_File;
}*/