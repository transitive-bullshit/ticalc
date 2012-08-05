// Header File
// Created 12/14/2003; 12:03:17 PM
/* Global Variables, seen by all C Source Files */

register GlobalVars *g asm("a4");
extern unsigned char Original_Rate;
extern Clock  *clock;
extern char   *Terrainl;
extern char   *Terraind;
extern void   *Hiddenl;
extern void   *Hiddend;

// Terrain.h
extern const Tile Tiles[129];
// Data.h
extern unsigned char Track1[];
extern unsigned char Track2[];
extern unsigned char Sin_Table128[5];
extern unsigned char *Tracks[5];
extern const char *exctimes;
extern unsigned short Default_Times[5];
extern const char *const cheats[8][2];
// Graphics.h
extern unsigned char Hedge[8*2];
extern unsigned char Shadow[3];
extern unsigned short Rider[3][16 * 3];
extern unsigned char Starting_Blocks[2][10 * 2];
extern unsigned char Sparks[2][8];
extern unsigned long BikeGfx[23][24*3];
extern unsigned char ExciteBike_Title[2][18 * 16];
extern unsigned char Trophy[2][2 * 15];
extern unsigned char Star[2][8];
extern unsigned char Podium[3 * 16];
extern unsigned char Dots[8];
extern unsigned char Celebrating[4][2 * 16];
extern unsigned char Mask[10];
extern unsigned short Beep_Mask[16];
extern unsigned short Beep[4][16 * 2];

extern INT_HANDLER interrupt1;
extern INT_HANDLER interrupt5;