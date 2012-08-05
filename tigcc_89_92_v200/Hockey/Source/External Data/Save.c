// C Source File
// Created 11/11/2003; 6:45:32 PM
#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization
#define MIN_AMS 100           // Compile for AMS 1.00 or higher

#include <tigcclib.h>         // Include All Header Files
#include "Data.h"							// Include the Data/GFX for Hockey
#include "Teams.h"						// Include the 32 Teams
#include "Font.h"							// Include the 8x8 Font
void Error(FILE*);

void Archive(const char *Filename) {
	SYM_ENTRY *SymPtr = NULL;
	HSym hsym = SymFind(SYMSTR(Filename));
	
	SymPtr = DerefSym (hsym);
	
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

// Main Function
void _main(void)
{
	ST_helpMsg("Saving external data, please wait...");
	UnArchive("hockgfx");
	FILE *fp = fopen("hockgfx", "wb");
	if (!fp)
		Error(NULL);
	
	/* Write all Graphics Data into the 'hockedat' File */
	if (fwrite(Hand, sizeof(Hand), 1, fp) != 1)
		Error(fp);
	if (fwrite(PlrGfx, sizeof(PlrGfx), 1, fp) != 1)
		Error(fp);
	if (fwrite(SkinnyGfx, sizeof(SkinnyGfx), 1, fp) != 1)
		Error(fp);
	if (fwrite(LevelData, sizeof(LevelData), 1, fp) != 1)
		Error(fp);
	if (fwrite(Blocks, sizeof(Blocks), 1, fp) != 1)
		Error(fp);
	if (fwrite(Arrows, sizeof(Arrows), 1, fp) != 1)
		Error(fp);
	if (fwrite(Easternl, sizeof(Easternl), 1, fp) != 1)
		Error(fp);
	if (fwrite(Easternd, sizeof(Easternd), 1, fp) != 1)
		Error(fp);
	if (fwrite(Westernl, sizeof(Westernl), 1, fp) != 1)
		Error(fp);
	if (fwrite(Westernd, sizeof(Westernd), 1, fp) != 1)
		Error(fp);
	if (fwrite(C_Mask, sizeof(C_Mask), 1, fp) != 1)
		Error(fp);
	if (fwrite(Flower, sizeof(Flower), 1, fp) != 1)
		Error(fp);
	if (fwrite(LinkPlay, sizeof(LinkPlay), 1, fp) != 1)
		Error(fp);
	if (fwrite(RLE_NHLd, sizeof(RLE_NHLd), 1, fp) != 1)
		Error(fp);
	if (fwrite(RLE_NHLl, sizeof(RLE_NHLl), 1, fp) != 1)
		Error(fp);
	if (fwrite(Font, sizeof(Font), 1, fp) != 1)
		Error(fp);
	if (fwrite(Starting_Pos, sizeof(Starting_Pos), 1, fp) != 1)
		Error(fp);
	if (fwrite(Western, sizeof(Western), 1, fp) != 1)
		Error(fp);
	if (fwrite(Eastern, sizeof(Eastern), 1, fp) != 1)
		Error(fp);
	
	/*
	clrscr();
	printf("
	%lu %lu %lu\n
	%lu %lu %lu\n
	%lu %lu %lu\n
	%lu %lu %lu\n
	%lu %lu %lu\n
	%lu %lu %lu\n
	%lu
	", 
	sizeof(Hand), sizeof(PlrGfx), sizeof(SkinnyGfx), sizeof(LevelData), sizeof(Blocks), sizeof(Arrows), 
	sizeof(Easternl), sizeof(Easternd), sizeof(Westernl), sizeof(Westernd), sizeof(C_Mask), sizeof(RLE_NHLd), 
	sizeof(RLE_NHLl), sizeof(Flower), sizeof(LinkPlay), sizeof(Font), sizeof(Starting_Pos), sizeof(Western), 
	sizeof(Eastern));
	ngetchx();*/
	
	// Put the extension "gfx" onto the Graphics File and close it
	fputc(0, fp);
	fputs("gfx", fp);
	fputc(0, fp);
	fputc(OTH_TAG, fp);
	fclose(fp);
	Archive("hockgfx");  // Archive the Configuration File
	ST_helpMsg("External Data File 'hockgfx' created");
}

void Error(FILE *fp) {
	if (fp != NULL)
		fclose(fp);
	ST_helpMsg("Error creating 'hockgfx' File");
	exit(1);
}