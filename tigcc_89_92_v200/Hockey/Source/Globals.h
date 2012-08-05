// C Header File
// Created 9/7/2003; 12:11:12 PM
/* Global Variables, seen by all C Source Files */
extern Team   *Team1;
extern Team   *Team2;
extern Clock  *clock;
extern char   *Home;
extern void   *Bulk;
extern void   *Hiddenl;
extern void   *Hiddend;

extern volatile unsigned short count;

extern INT_HANDLER interrupt1;
extern INT_HANDLER interrupt5;

#ifdef EXTERNAL_GFX
/*****************************************************
* Graphics/Data loaded from External 'hockgfx' File 
*****************************************************/
extern unsigned short *Hand;
extern unsigned short (*PlrGfx)[32];
extern unsigned short (*SkinnyGfx)[36];
extern unsigned char  (*LevelData)[Width];
extern unsigned short (*Blocks)[32];
extern unsigned short (*Arrows)[32];
extern unsigned char *Easternl;
extern unsigned char *Easternd;
extern unsigned char *Westernl;
extern unsigned char *Westernd;
extern unsigned char *C_Mask;
extern unsigned char *RLE_NHLd;
extern unsigned char *RLE_NHLl;
extern unsigned long *Flower;
extern unsigned long *LinkPlay;
extern unsigned char (*Font)[24];
extern unsigned char (*Starting_Pos)[2];
extern Names *Default_Western;
extern Names *Default_Eastern;
extern unsigned char *NHLl;
extern unsigned char *NHLd;
#else
/*****************************************************
* Graphics/Data stored in main program 
*****************************************************/
extern unsigned char Font[39][24];
extern unsigned short Hand[32];
extern unsigned short PlrGfx[][32];
extern unsigned short SkinnyGfx[][36];
extern unsigned char LevelData[Height][Width];
extern unsigned short Blocks[][32];
extern unsigned short Arrows[4][32];
extern unsigned char Easternl[];
extern unsigned char Easternd[];
extern unsigned char Westernl[];
extern unsigned char Westernd[];
extern unsigned char C_Mask[];
extern unsigned long Flower[48];
extern unsigned long LinkPlay[21 * 3];
#ifdef COMPRESSION
extern unsigned char RLE_NHLl[];
extern unsigned char RLE_NHLd[];
extern unsigned char *NHLl;
extern unsigned char *NHLd;
#else
extern unsigned char NHLl[];
extern unsigned char NHLd[];
#endif
extern unsigned long Flower[48];
extern unsigned long LinkPlay[21 * 3];
extern unsigned char Starting_Pos[9][2];
extern Names Default_Western[16];
extern Names Default_Eastern[15];
#endif

extern Names *Western;
extern Names *Eastern;

/* DLL IMPORTS */
/*
#define Font ((unsigned char (*)[24])(_DLL_reference(unsigned char, 0)))
#define PlrGfx ((unsigned short (*)[48])(_DLL_reference(unsigned short, 1)))
#define PlrGfx2 ((unsigned short (*)[48])(_DLL_reference(unsigned short, 2)))
#define LevelData ((unsigned char (*)[Width])(_DLL_reference(unsigned char, 3)))
#define Blocks ((unsigned short (*)[32])(_DLL_reference(unsigned short, 4)))
#define Arrows ((unsigned short (*)[32])(_DLL_reference(unsigned short, 5)))
#define Easternl ((unsigned char(*))(_DLL_reference(unsigned char, 6)))
#define Easternd ((unsigned char(*))(_DLL_reference(unsigned char, 7)))
#define Westernl ((unsigned char(*))(_DLL_reference(unsigned char, 8)))
#define Westernd ((unsigned char(*))(_DLL_reference(unsigned char, 9)))
#define C_Mask ((unsigned char(*))(_DLL_reference(unsigned char, 10)))
#define NHLl ((unsigned char(*))(_DLL_reference(unsigned char, 11)))
#define NHLd ((unsigned char(*))(_DLL_reference(unsigned char, 12)))
#define Western ((Names (*))(_DLL_reference(Names, 13)))
#define Eastern ((Names (*))(_DLL_reference(Names, 14)))
#define Starting_Pos ((unsigned char (*)[2])(_DLL_reference(unsigned char, 15)))*/