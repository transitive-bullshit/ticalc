// Header File
// Created 11/11/2003; 9:00:45 PM

/* Global Pointers to Data in the 'hockgfx' File */
unsigned short *Hand;
unsigned short (*PlrGfx)[32];
unsigned short (*SkinnyGfx)[36];
unsigned char  (*LevelData)[Width];
unsigned short (*Blocks)[32];
unsigned short (*Arrows)[32];
unsigned char *Easternl;
unsigned char *Easternd;
unsigned char *Westernl;
unsigned char *Westernd;
unsigned char *C_Mask;
unsigned char *RLE_NHLd;
unsigned char *RLE_NHLl;
unsigned long *Flower;
unsigned long *LinkPlay;
unsigned char (*Font)[24];
unsigned char (*Starting_Pos)[2];
Names *Default_Western;
Names *Default_Eastern;

static inline void Load_Gfx(void) {
	SYM_ENTRY *SymPtr;
	if ((SymPtr = SymFindPtr(SYMSTR_CONST("hockgfx"), 0)) == NULL)
		ERROR(EXTERNAL_FILE_HOCKGFX);  // File not found
	Hand = (unsigned short*)HLock(SymPtr->handle);
	if (Hand == NULL)
		ERROR(EXTERNAL_FILE_HOCKGFX);
	Hand     += 1;
	//Hand      = (unsigned short*)Get_Data_Ptr("hockgfx", 0);
	PlrGfx    = (unsigned short (*)[32])((char*)Hand + 64);
	SkinnyGfx = (unsigned short (*)[36])((char*)PlrGfx + (22 * 32 * 2));
	LevelData = (unsigned char (*)[Width])((char*)SkinnyGfx + (10 * 36 * 2));
	Blocks    = (unsigned short (*)[32])((char*)LevelData + (Height * Width));
	Arrows    = (unsigned short (*)[32])((char*)Blocks + (88 * 32 * 2));
	Easternl  = (unsigned char *)((char*)Arrows + (4 * 32 * 2));
	Easternd  = (unsigned char *)((char*)Easternl + 228);
	Westernl  = (unsigned char *)((char*)Easternd + 228);
	Westernd  = (unsigned char *)((char*)Westernl + 228);
	C_Mask    = (unsigned char *)((char*)Westernd + 228);
	Flower    = (unsigned long *)((char*)C_Mask   + 228);
	LinkPlay  = (unsigned long *)((char*)Flower   + 192);
	RLE_NHLd  = (unsigned char *)((char*)LinkPlay + (21 * 3 * 4));
	//RLE_NHLd  = (unsigned char *)((char*)C_Mask   + 228);
	RLE_NHLl  = (unsigned char *)((char*)RLE_NHLd + 1517);
	Font      = (unsigned char (*)[24])((char*)RLE_NHLl + 1500);
	Starting_Pos = (unsigned char (*)[2])((char*)Font + (39 * 24));
	Default_Western = (Names *)((char*)Starting_Pos + (9 * 2));
	Default_Eastern = (Names *)((char*)Default_Western + 1360);
}

static inline void SafeUnlock(SYM_STR Filename) {
	SYM_ENTRY *SymPtr;
	HANDLE h;
	
	if ((SymPtr = SymFindPtr(Filename, 0)) == NULL)
		return;  // File not found
	if (SymPtr->flags.bits.locked == FALSE)
		return;  // File is not Locked
	if (!(h = SymPtr->handle))
		return;  // The File's Handle is unusable?
	
	HeapUnlock(h);
}