// C Source File
// Created 2/2/2004; 6:48:42 PM
#include "All.h"

short Save_File(const char *FileName, unsigned char *Data, unsigned long Size) {
	const unsigned char Tag[6] = { 0, 'l', 'v', 'l', 0, OTH_TAG };
	const unsigned short Uncompressed_Size = Size;
	char *Base;
	HANDLE h;
	Size = CompressRLE(Data, Size);  // RLE Compress the File
	struct {
		unsigned short size;
		unsigned short Magic_No;
		unsigned short Best_Time;
		unsigned short Track_Size;
		unsigned char Data[Size + (Size & 1)];
		unsigned char Tag[6];
	} Save_File;
	
	Save_File.Magic_No = Track_Magic;
	Save_File.Best_Time = 0;
	Save_File.Track_Size = Uncompressed_Size;
	memcpy(&Save_File.Data, Data, Size);
	memcpy(&Save_File.Tag, Tag, 6);
	Size = sizeof(Save_File);
	Save_File.size = Size - 2;
	
	UnArchive(FileName);  // Make sure that before writing to the file, it is not archived
	
	if ((h = HeapAlloc(Size)) == H_NULL)
		return FALSE;
	if ((Base = HeapDeref(DerefSym(SymAdd(SYMSTR(FileName)))->handle = h)) == NULL) {
		HeapFree(h);
    return FALSE;
	}
	memcpy(Base, &Save_File, Size);
	
	Archive(FileName);    // Automatically Archive the file in case of a crash
	return TRUE;
}

unsigned char *Get_Data_Ptr(const char *Filename, short Offset) {
	unsigned char *Location;
	SYM_ENTRY *SymPtr;
	
	if ((SymPtr = SymFindPtr(SYMSTR(Filename), 0)) == NULL)
		return NULL;  // File not found
	
	if ((Location = (unsigned char*) HeapDeref(SymPtr->handle)) == NULL)
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