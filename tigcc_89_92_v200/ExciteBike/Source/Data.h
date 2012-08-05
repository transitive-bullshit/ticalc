// Header File
// Created 12/14/2003; 11:58:57 AM
#include "Terrain.h"       // Include all of the Terrain Graphics
#include "Graphics.h"  // Include all of the Bike Graphics

#define Gray					Gra1, Gra1, Gra1, Gra1
#define Small_Bump		SL00, SL01, SL02
#define Medium_Bump		SL10, SL11, SL12, SL13, SL14
#define Large_Bump		SL20, SL21, SL22, SL23, SL24, SL25, SL26, SL27, SL28
#define Long_Bump			SL30, SL31, SL32, SL33, SL34, SL35, SL36, SL37, SL38
#define Tall_Bump			SL40, SL41, SL42, SL43, SL44
#define Slanted_Right	SL50, SL51, SL52, SL53, SL54, SL55
#define Slanted_Left	SL60, SL61, SL62, SL63, SL64, SL65
#define Small_Jump		Jum1,	Jum2
#define BarricadeTop	Blo1, Gra1
#define BarricadeLow	Blo2, Gra1
#define Gravel2and4		Rok1,	Rok2, Rok3
#define Gravel1and3		Rok4,	Rok5, Rok6
#define TurboTop			Tur1,	Tur2
#define TurboLow			Tur3,	Tur4
#define Dry_PitLow		Dry0, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, \
Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, \
Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, \
Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, \
Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry1, Dry0  // 56
#define Dry_PitHigh		Dry2, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, \
Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, \
Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, \
Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, \
Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry3, Dry2  // 56
#define Dry_PitFull		Dry4, Dry5, Dry5, Dry5, Dry5, Dry5, Dry5, Dry5, Dry5, Dry5, Dry5, Dry5, \
Dry5, Dry5, Dry5, Dry5, Dry5, Dry5, Dry5, Dry5, Dry5, Dry5, Dry5, Dry4  // 24
#define Monster_Jump	MO00, MO01, MO02, MO03, MO04, MO05, MO06, MO07, MO08, MO09, MO10, MO11, \
MO12, MO13, MO14, MO15, MO16, MO17, MO18, MO19, MO20, MO21, MO22, MO23, MO24, MO25
#define Monster_Ramp	MR00, MR01, MR02, MR03, MR04, MR05, MR06, MR07, MR08, MR09, MR10, MR11, \
MR12, MR13, MR14, MR15, MR16, MR17, MR18, MR19, MR20, MR21, MR22, MR23, MR24, MR25, MR26
#define Finish_Line		Fin0, Fin1,	Fin2, Fin3, Fin4, Fin5, Fin6, Fin7, Fin8, Fin9, Fi10, Fi11

// Original Track 1 from NES ExciteBike
/*unsigned char Track1[] = {
// Each NES Screen is 32 * 8 Pixels (256)
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,  // = One Screen (8 Gray's * 4 = 32)
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gray,Gray,Gravel1and3,Gray,Gray,
Large_Bump,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Medium_Bump,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,TurboLow,Gray,Gray,Gra1,Gra1,
Medium_Bump,Gra1,Gra1,Medium_Bump,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gray,Gray,Gray,Gray,Gray,Gra1,Gra1,Gra1,
Medium_Bump,Gra1,Gra1,
Medium_Bump,Gra1,Gra1,
Medium_Bump,Gra1,Gra1,
Gray,Gray,Gray,TurboTop,
Gray,Gray,Gray,Gray,
Gravel2and4,
Gray,Gray,Gray,Gray,
Slanted_Right,Gray,Gra1,Slanted_Left,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gray,Gra1,Gra1,Gra1,Small_Jump,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gra1,
TurboTop,Gra1,Gra1,
Small_Jump,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gra1,Gra1,
Small_Jump,Gra1,Gra1,
Small_Bump,Gra1,Gra1,
Small_Bump,Gra1,Gra1,
Small_Bump,Gra1,Gra1,
Small_Bump,
Gray,Gray,Gray,Gray,Gray,
Slanted_Right,Gra1,Gra1,
Dry_PitFull,Gray,Gray,Gray,Gray,Gray,
Long_Bump,Gra1,Gra1,
Long_Bump,Gra1,Gra1,Gray,
Long_Bump,Gra1,Gra1,
Long_Bump,
Gray,Gray,Gray,Gray,Gray,Gray,Gra1,Gra1,Gra1,
Gravel2and4,Gray,Gray,Gra1,Gra1,Gra1,
Gravel1and3,Gray,Gray,Gra1,Gra1,Gra1,
Gravel2and4,Gray,Gray,Gray,Gra1,Gra1,
Finish_Line,
2,				// Number of Laps
0, 0, 0,	// Default Best Time
};

// Original Track 2 from NES ExciteBike
unsigned char Track2[] = {
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gray,Gray,Gray,Gray,Gra1,Gra1,
Monster_Jump,
Gray,Gray,Gra1,Gra1,
Medium_Bump,
Gray,Gray,Gray,Gra1,Gra1,
Gravel2and4,
Gray,Gray,Gra1,Gra1,Gra1,
Monster_Jump,
Gray,Gray,Gray,Gra1,Gra1,Gra1,
TurboTop,Gra1,Gra1,
TurboLow,Gray,Gray,Gray,Gra1,Gra1,Gra1,
Medium_Bump,Gray,Gray,Gra1,Gra1,Gra1,
Medium_Bump,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gra1,Gra1,
Small_Bump,Gra1,Gra1,
Tall_Bump,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gra1,Gra1,
Small_Jump,Gra1,Gra1,
Small_Bump,Gra1,Gra1,
Dry_PitFull,Gra1,Gra1,
Small_Jump,Gra1,Gra1,
Dry_PitFull,Gra1,Gra1,
Gray,Gray,Gray,Gray,Gray,Gra1,
TurboLow,Gray,Gray,Gra1,Gra1,
Long_Bump,Gra1,Gra1,
Long_Bump,Gra1,Gra1,
Long_Bump,Gra1,Gra1,
Long_Bump,Gra1,Gra1,Gra1,
Gray,Gray,Gray,Gray,Gray,
Dry_PitHigh,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gravel2and4,Gray,Gray,Gray,Gra1,Gra1,
Gravel1and3,Gray,Gray,Gra1,
Gravel2and4,
Gray,Gray,Gray,Gray,Gray,Gray,
Gra1,Gra1,Gra1,
Finish_Line,
2,				// Number of Laps
0, 0, 0,	// Default Best Time
};

// Original Track 3 from NES ExciteBike
unsigned char Track3[] = {
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gray,Gray,Gray,Gray,Gra1,Gra1,Gra1,
BarricadeTop,Gray,Gra1,Gra1,Gra1,
BarricadeLow,Gray,Gray,Gray,
BarricadeTop,Gray,Gray,Gra1,Gra1,
BarricadeLow,Gray,Gray,Gray,Gra1,Gra1,
BarricadeTop,Gray,Gray,Gray,Gra1,Gra1,Gra1,
BarricadeLow,Gray,Gray,Gray,Gray,
Medium_Bump,Gray,
Medium_Bump,
Gray,Gray,Gray,Gray,Gra1,Gra1,Gra1,
Slanted_Right,Gray,Gray,Gra1,Gra1,
Slanted_Left,Gray,Gra1,Gra1,Gra1,
TurboTop,Gra1,Gra1,
TurboLow,Gray,Gra1,Gra1,Gra1,
Slanted_Right,Gra1,Gra1,
Tall_Bump,Gra1,Gra1,
Slanted_Left,
Gray,Gray,Gray,Gray,Gray,Gra1,Gra1,
Dry_PitHigh,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gra1,Gra1,
Small_Jump,Gray,Gray,Gra1,
Medium_Bump,Gra1,Gra1,
Dry_PitFull,
Gray,Gray,Gray,Gra1,Gra1,Gra1,
Small_Jump,Gray,Gray,Gra1,Gra1,Gra1,
Large_Bump,Gray,Gray,Gray,Gra1,Gra1,
BarricadeTop,Gray,Gray,Gray,Gra1,Gra1,
Large_Bump,Gray,Gra1,Gra1,
Slanted_Left,Gray,Gra1,Gra1,
Slanted_Right,Gray,Gra1,
Large_Bump,Gray,Gray,Gray,Gra1,Gra1,Gra1,
TurboTop,Gray,Gray,Gra1,Gra1,Gra1,
Dry_PitHigh,
Gray,Gray,Gray,Gra1,
Dry_PitLow,
Gray,Gray,Gray,Gray,Gra1,
BarricadeLow,Gray,Gray,
Gravel1and3,Gray,Gra1,
Gravel2and4,Gray,Gra1,
BarricadeTop,
Gray,Gray,Gray,Gray,Gray,Gra1,Gra1,Gra1,
Finish_Line,
2,				// Number of Laps
0, 0, 0,	// Default Best Time
};

// Original Track 4 from NES ExciteBike
unsigned char Track4[] = {
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gray,Gray,Gray,Gray,
Monster_Ramp,
Gray,Gray,Gray,Gra1,Gra1,Gra1,
Gravel1and3,Gray,Gray,
Gravel2and4,Gray,Gray,Gra1,
Monster_Ramp,Gray,
Medium_Bump,Gray,Gray,Gray,Gra1,
BarricadeTop,Gray,Gray,Gra1,
BarricadeLow,Gray,Gray,Gra1,Gra1,Gra1,
TurboLow,Gray,Gray,Gray,Gray,Gray,Gray,
Medium_Bump,Gra1,Gra1,
Dry4,Dry5,Dry5,Dry5,Dry5,Dry5,Dry5,Dry5,
Dry5,Dry5,Dry5,Dry4, // Custom
Gray,Gray,Gray,Gray,Gray,Gray,Gra1,
Small_Jump,Gray,Gray,Gray,Gra1,
Small_Jump,Gray,
Tall_Bump,Gray,
Small_Bump,Gra1,Gra1,
Small_Bump,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,
BarricadeTop,Gray,Gray,Gra1,
BarricadeLow,Gray,Gray,
Small_Jump,Gray,
Tall_Bump,Gra1,Gra1,
Tall_Bump,Gra1,Gra1,
Tall_Bump,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gra1,TurboTop,
Gray,Gray,Gray,
Gravel1and3,Gray,
// Custom, shortened Dry Patches (Not available in Editor Mode)
Dry2,Dry3,Dry3,Dry3,Dry3,Dry3,Dry3,Dry3,
Dry3,Dry3,Dry3,Dry3,Dry3,Dry3,Dry3,Dry3,
Dry3,Dry3,Dry3,Dry3,Dry3,Dry3,Dry3,Dry3,
Dry3,Dry2,Gray,Gra1,Gra1,
Dry0,Dry1,Dry1,Dry1,Dry1,Dry1,Dry1,Dry1,
Dry1,Dry1,Dry1,Dry1,Dry1,Dry1,Dry1,Dry1,
Dry1,Dry1,Dry1,Dry1,Dry1,Dry1,Dry1,Dry1,
Dry1,Dry0,Gra1,Gra1,Gra1,
BarricadeTop,Gray,Gray,
Large_Bump,Gra1,Gra1,
Large_Bump,Gra1,Gra1,
Large_Bump,Gra1,Gra1,
Large_Bump,Gra1,Gra1,
Large_Bump,Gra1,Gra1,
Dry4,Dry5,Dry5,Dry5,Dry5,Dry5,Dry5,Dry5,
Dry5,Dry5,Dry5,Dry4, // Custom again
Gray,Gray,Gray,Gray,Gra1,
TurboLow,Gray,Gray,Gray,Gra1,Gra1,
Gravel2and4,
Gray,Gray,Gray,Gra1,Gra1,Gra1,
Gravel1and3,Gray,Gray,Gray,Gra1,Gra1,
Gravel2and4,Gray,Gra1,
Gravel1and3,Gray,Gray,
Gravel2and4,Gray,Gray,Gray,
Gravel2and4,Gray,
Gravel1and3,Gray,Gray,Gra1,
Small_Jump,Gray,Gra1,
Finish_Line,
2,				// Number of Laps
0, 0, 0,	// Default Best Time
};

// Original Track 5 from NES ExciteBike
unsigned char Track5[] = {
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gray,Gray,Gray,Gray,Gray,Gra1,
TurboTop,Gray,Gray,Gray,Gra1,Gra1,Gra1,
Gravel2and4,Gray,Gray,
Small_Jump,Gra1,Gra1,
// Custom Dry Pit
Dry4,Dry5,Dry5,Dry5,Dry5,Dry5,Dry5,Dry5,
Dry5,Dry5,Dry5,Dry5,Dry5,Dry5,Dry5,Dry4,
Gra1,Gra1,Slanted_Left,Gray,Gra1,Gra1,
Monster_Jump,Gra1,Gra1,
Small_Bump,Gra1,Gra1,Gra1,
Small_Bump,Gra1,Gra1,Gra1,
Small_Bump,Gray,Gray,Gray,Gray,Gray,
BarricadeLow,Gray,Gray,
TurboLow,Gray,Gray,Gray,Gray,Gra1,
Gravel1and3,
Gray,Gray,Gray,Gray,Gra1,Gra1,Gra1,
Gravel2and4,Gray,Gray,Gray,Gray,Gray,
Small_Jump,
Gray,Gray,Gray,Gray,Gra1,Gra1,Gra1,
Large_Bump,Gray,
Large_Bump,Gray,Gra1,
Large_Bump,Gray,
Slanted_Right,Gra1,Gra1,
// Custom Dry Pit
Dry4,Dry5,Dry5,Dry5,Dry5,Dry5,Dry5,Dry5,
Dry5,Dry5,Dry5,Dry4,Gray,Gra1,
Gravel2and4,Gra1,Gra1,Gra1,
Monster_Ramp,Gray,Gra1,
Medium_Bump,Gray,Gra1,Gra1,Gra1,
Small_Jump,
Gray,Gray,Gray,Gray,Gray,Gray,Gra1,Gra1,
Small_Jump,Gra1,Gra1,
// Custom Dry Pit
Dry4,Dry5,Dry5,Dry5,Dry5,Dry5,Dry4,
Gra1,Gra1,
Tall_Bump,Gray,Gra1,Gra1,
Long_Bump,Gra1,Gra1,
Long_Bump,Gra1,Gra1,
Small_Bump,Gra1,Gra1,
Long_Bump,Gra1,Gra1,
Long_Bump,Gra1,Gra1,
Long_Bump,Gra1,Gra1,
Long_Bump,Gra1,Gra1,
Large_Bump,Gra1,
Gray,Gray,Gray,Gray,Gray,Gray,Gray,Gray,
Gravel2and4,Gray,Gray,
Gravel1and3,Gray,
BarricadeLow,Gray,Gray,Gra1,Gra1,
BarricadeTop,Gray,Gra1,Gra1,Gra1,
Gravel1and3,Gray,Gra1,Gra1,
Gravel2and4,Gray,Gray,
Gravel1and3,Gray,Gra1,Gra1,
BarricadeLow,Gray,Gra1,
Finish_Line,
2,				// Number of Laps
0, 0, 0,	// Default Best Time
};*/

unsigned char Compressed_Track1[] = {
  0x00, 0x4C, // Default Track Record
  0x02, 0xCA, // Length of the Uncompressed Track 1 (unsigned short 714)
  0x91, 0x00, // Start of Track Data
  0x6C, 0x33, 0x34, 0x35, 0x91, 0x00, 0x08, 0x92, 0x09, 0x09, 0x91, 0x00, 0x20, 0x92, 0x04, 0x05, 
  0x91, 0x00, 0x24, 0x38, 0x39, 0x91, 0x00, 0x0A, 0x92, 0x04, 0x05, 0x00, 0x00, 0x92, 0x04, 0x05, 
  0x91, 0x00, 0x3B, 0x92, 0x04, 0x05, 0x00, 0x00, 0x92, 0x04, 0x05, 0x00, 0x00, 0x92, 0x04, 0x05, 
  0x91, 0x00, 0x0E, 0x36, 0x37, 0x91, 0x00, 0x10, 0x30, 0x31, 0x32, 0x91, 0x00, 0x10, 0x92, 0x20, 
  0x06, 0x91, 0x00, 0x05, 0x92, 0x26, 0x06, 0x91, 0x00, 0x2B, 0x2C, 0x2D, 0x91, 0x00, 0x1D, 0x36, 
  0x37, 0x00, 0x00, 0x2C, 0x2D, 0x91, 0x00, 0x26, 0x2C, 0x2D, 0x00, 0x92, 0x00, 0x04, 0x00, 0x92, 
  0x00, 0x04, 0x00, 0x92, 0x00, 0x04, 0x00, 0x92, 0x00, 0x04, 0x91, 0x00, 0x14, 0x92, 0x20, 0x06, 
  0x00, 0x00, 0x3E, 0x91, 0x3F, 0x16, 0x3E, 0x91, 0x00, 0x14, 0x92, 0x12, 0x09, 0x00, 0x00, 0x92, 
  0x12, 0x09, 0x91, 0x00, 0x06, 0x92, 0x12, 0x09, 0x00, 0x00, 0x92, 0x12, 0x09, 0x91, 0x00, 0x1B, 
  0x30, 0x31, 0x32, 0x91, 0x00, 0x0B, 0x33, 0x34, 0x35, 0x91, 0x00, 0x0B, 0x30, 0x31, 0x32, 0x91, 
  0x00, 0x0E, 0x92, 0x75, 0x0C, 
  0x02, // Number of Laps
};

unsigned char Compressed_Track2[] = {
  0x00, 0x4C, 
  0x02, 0xA7, // Length of the Uncompressed Track 2 (unsigned short 679)
  0x91, 0x00, 0x76, 0x92, 
  0x40, 0x1A, 0x91, 0x00, 0x0A, 0x92, 0x04, 0x05, 0x91, 0x00, 0x0E, 0x30, 0x31, 0x32, 0x91, 0x00, 
  0x0B, 0x92, 0x40, 0x1A, 0x91, 0x00, 0x0F, 0x36, 0x37, 0x00, 0x00, 0x38, 0x39, 0x91, 0x00, 0x0F, 
  0x92, 0x04, 0x05, 0x91, 0x00, 0x0B, 0x92, 0x04, 0x05, 0x91, 0x00, 0x26, 0x01, 0x02, 0x03, 0x00, 
  0x00, 0x92, 0x1B, 0x05, 0x91, 0x00, 0x26, 0x2C, 0x2D, 0x00, 0x92, 0x00, 0x04, 0x00, 0x00, 0x3E, 
  0x91, 0x3F, 0x16, 0x3E, 0x00, 0x00, 0x2C, 0x2D, 0x00, 0x00, 0x3E, 0x91, 0x3F, 0x16, 0x3E, 0x91, 
  0x00, 0x17, 0x38, 0x39, 0x91, 0x00, 0x0A, 0x92, 0x12, 0x09, 0x00, 0x00, 0x92, 0x12, 0x09, 0x00, 
  0x00, 0x92, 0x12, 0x09, 0x00, 0x00, 0x92, 0x12, 0x09, 0x91, 0x00, 0x17, 0x3C, 0x91, 0x3D, 0x36, 
  0x3C, 0x91, 0x00, 0x20, 0x30, 0x31, 0x32, 0x91, 0x00, 0x0E, 0x33, 0x34, 0x35, 0x91, 0x00, 0x09, 
  0x30, 0x31, 0x32, 0x91, 0x00, 0x1B, 0x92, 0x75, 0x0C, 
  0x02, // Number of Laps
};

unsigned char Compressed_Track3[] = {
  0x00, 0x50, 
  0x03, 0x23, // Length of the Uncompressed Track 3 (unsigned short 803)
  0x91, 0x00, 0x77, 0x2E, 0x91, 0x00, 0x08, 0x2F, 0x91, 0x00, 0x0D, 0x2E, 0x91, 0x00, 0x0B, 0x2F, 
  0x91, 0x00, 0x0F, 0x2E, 0x91, 0x00, 0x10, 0x2F, 0x91, 0x00, 0x11, 0x92, 0x04, 0x05, 0x91, 0x00, 
  0x04, 0x92, 0x04, 0x05, 0x91, 0x00, 0x13, 0x92, 0x20, 0x06, 0x91, 0x00, 0x0A, 0x92, 0x26, 0x06, 
  0x91, 0x00, 0x07, 0x36, 0x37, 0x00, 0x00, 0x38, 0x39, 0x91, 0x00, 0x07, 0x92, 0x20, 0x06, 0x00, 
  0x00, 0x92, 0x1B, 0x05, 0x00, 0x00, 0x92, 0x26, 0x06, 0x91, 0x00, 0x16, 0x3C, 0x91, 0x3D, 0x36, 
  0x3C, 0x91, 0x00, 0x22, 0x2C, 0x2D, 0x91, 0x00, 0x09, 0x92, 0x04, 0x05, 0x00, 0x00, 0x3E, 0x91, 
  0x3F, 0x16, 0x3E, 0x91, 0x00, 0x0F, 0x2C, 0x2D, 0x91, 0x00, 0x0B, 0x92, 0x09, 0x09, 0x91, 0x00, 
  0x0E, 0x2E, 0x91, 0x00, 0x0F, 0x92, 0x09, 0x09, 0x91, 0x00, 0x06, 0x92, 0x26, 0x06, 0x91, 0x00, 
  0x06, 0x92, 0x20, 0x06, 0x91, 0x00, 0x05, 0x92, 0x09, 0x09, 0x91, 0x00, 0x0F, 0x36, 0x37, 0x91, 
  0x00, 0x0B, 0x3C, 0x91, 0x3D, 0x36, 0x3C, 0x91, 0x00, 0x0D, 0x3A, 0x91, 0x3B, 0x36, 0x3A, 0x91, 
  0x00, 0x11, 0x2F, 0x91, 0x00, 0x09, 0x33, 0x34, 0x35, 0x91, 0x00, 0x05, 0x30, 0x31, 0x32, 0x91, 
  0x00, 0x05, 0x2E, 0x91, 0x00, 0x18, 0x92, 0x75, 0x0C, 
  0x02, // Number of Laps
};

unsigned char Compressed_Track4[] = {
  0x00, 0x46, 
  0x03, 0x25, // Length of the Uncompressed Track 4 (unsigned short 805)
  0x91, 0x00, 0x94, 0x92, 0x5A, 0x1B, 0x91, 0x00, 0x0F, 0x33, 0x34, 0x35, 0x91, 0x00, 0x08, 0x30, 
  0x31, 0x32, 0x91, 0x00, 0x09, 0x92, 0x5A, 0x1B, 0x91, 0x00, 0x04, 0x92, 0x04, 0x05, 0x91, 0x00, 
  0x0D, 0x2E, 0x91, 0x00, 0x0A, 0x2F, 0x91, 0x00, 0x0C, 0x38, 0x39, 0x91, 0x00, 0x18, 0x92, 0x04, 
  0x05, 0x00, 0x00, 0x3E, 0x91, 0x3F, 0x0A, 0x3E, 0x91, 0x00, 0x19, 0x2C, 0x2D, 0x91, 0x00, 0x0D, 
  0x2C, 0x2D, 0x91, 0x00, 0x04, 0x92, 0x1B, 0x05, 0x91, 0x00, 0x04, 0x01, 0x02, 0x03, 0x00, 0x92, 
  0x00, 0x04, 0x91, 0x00, 0x1C, 0x2E, 0x91, 0x00, 0x0A, 0x2F, 0x91, 0x00, 0x09, 0x2C, 0x2D, 0x91, 
  0x00, 0x04, 0x92, 0x1B, 0x05, 0x00, 0x00, 0x92, 0x1B, 0x05, 0x00, 0x00, 0x92, 0x1B, 0x05, 0x91, 
  0x00, 0x25, 0x36, 0x37, 0x91, 0x00, 0x0C, 0x33, 0x34, 0x35, 0x91, 0x00, 0x04, 0x3C, 0x91, 0x3D, 
  0x18, 0x3C, 0x91, 0x00, 0x06, 0x3A, 0x91, 0x3B, 0x18, 0x3A, 0x00, 0x00, 0x00, 0x2E, 0x91, 0x00, 
  0x09, 0x92, 0x09, 0x09, 0x00, 0x00, 0x92, 0x09, 0x09, 0x00, 0x00, 0x92, 0x09, 0x09, 0x00, 0x00, 
  0x92, 0x09, 0x09, 0x00, 0x00, 0x92, 0x09, 0x09, 0x00, 0x00, 0x3E, 0x91, 0x3F, 0x0A, 0x3E, 0x91, 
  0x00, 0x11, 0x38, 0x39, 0x91, 0x00, 0x0E, 0x30, 0x31, 0x32, 0x91, 0x00, 0x0F, 0x33, 0x34, 0x35, 
  0x91, 0x00, 0x0E, 0x30, 0x31, 0x32, 0x91, 0x00, 0x05, 0x33, 0x34, 0x35, 0x91, 0x00, 0x08, 0x30, 
  0x31, 0x32, 0x91, 0x00, 0x0C, 0x30, 0x31, 0x32, 0x91, 0x00, 0x04, 0x33, 0x34, 0x35, 0x91, 0x00, 
  0x09, 0x2C, 0x2D, 0x91, 0x00, 0x05, 0x92, 0x75, 0x0C, 
  0x02, // Number of Laps
};

unsigned char Compressed_Track5[] = {
  0x00, 0x32, 
  0x02, 0xC1, // Length of the Uncompressed Track 5 (unsigned short 705)
  0x91, 0x00, 0x75, 0x36, 0x37, 0x91, 0x00, 0x0F, 0x30, 0x31, 0x32, 0x91, 0x00, 0x08, 0x2C, 0x2D, 
  0x00, 0x00, 0x3E, 0x91, 0x3F, 0x0E, 0x3E, 0x00, 0x00, 0x92, 0x26, 0x06, 0x91, 0x00, 0x06, 0x92, 
  0x40, 0x1A, 0x00, 0x92, 0x00, 0x04, 0x00, 0x00, 0x92, 0x00, 0x04, 0x00, 0x00, 0x92, 0x00, 0x04, 
  0x91, 0x00, 0x14, 0x2F, 0x91, 0x00, 0x09, 0x38, 0x39, 0x91, 0x00, 0x11, 0x33, 0x34, 0x35, 0x91, 
  0x00, 0x13, 0x30, 0x31, 0x32, 0x91, 0x00, 0x14, 0x2C, 0x2D, 0x91, 0x00, 0x13, 0x92, 0x09, 0x09, 
  0x91, 0x00, 0x04, 0x92, 0x09, 0x09, 0x91, 0x00, 0x05, 0x92, 0x09, 0x09, 0x91, 0x00, 0x04, 0x92, 
  0x20, 0x06, 0x00, 0x00, 0x3E, 0x91, 0x3F, 0x0A, 0x3E, 0x91, 0x00, 0x05, 0x30, 0x31, 0x32, 0x00, 
  0x00, 0x00, 0x92, 0x5A, 0x1B, 0x91, 0x00, 0x05, 0x92, 0x04, 0x05, 0x91, 0x00, 0x07, 0x2C, 0x2D, 
  0x91, 0x00, 0x1A, 0x2C, 0x2D, 0x00, 0x00, 0x3E, 0x91, 0x3F, 0x05, 0x3E, 0x00, 0x00, 0x92, 0x1B, 
  0x05, 0x91, 0x00, 0x06, 0x92, 0x12, 0x09, 0x00, 0x00, 0x92, 0x12, 0x09, 0x00, 0x92, 0x00, 0x04, 
  0x00, 0x00, 0x92, 0x12, 0x09, 0x00, 0x00, 0x92, 0x12, 0x09, 0x00, 0x00, 0x92, 0x12, 0x09, 0x00, 
  0x00, 0x92, 0x12, 0x09, 0x00, 0x00, 0x92, 0x09, 0x09, 0x91, 0x00, 0x21, 0x30, 0x31, 0x32, 0x91, 
  0x00, 0x08, 0x33, 0x34, 0x35, 0x91, 0x00, 0x04, 0x2F, 0x91, 0x00, 0x0B, 0x2E, 0x91, 0x00, 0x08, 
  0x33, 0x34, 0x35, 0x91, 0x00, 0x06, 0x30, 0x31, 0x32, 0x91, 0x00, 0x08, 0x33, 0x34, 0x35, 0x91, 
  0x00, 0x06, 0x2F, 0x91, 0x00, 0x06, 0x92, 0x75, 0x0C, 
  0x02, // Number of Laps
};

const Tile Tiles[129] = {
 {GrayTile_Array, GrayTile_Array + 48, 48}, 
 
 {Bump_Array[0], Bump_Array[0] + 81, 56}, 
 {Bump_Array[1], Bump_Array[1] + 81, 57}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 56}, 
 
 {Bump_Array[0], Bump_Array[0] + 81, 56}, 
 {Bump_Array[0], Bump_Array[0] + 81, 64}, 
 {Bump_Array[1], Bump_Array[1] + 81, 65}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 64}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 56}, 
 
 {Bump_Array[0], Bump_Array[0] + 81, 56}, // 10
 {Bump_Array[0], Bump_Array[0] + 81, 64}, 
 {Bump_Array[0], Bump_Array[0] + 81, 72}, 
 {Bump_Array[0], Bump_Array[0] + 81, 80}, 
 {Bump_Array[1], Bump_Array[1] + 81, 81}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 80}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 72}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 64}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 56}, 
 
 {Long_Bump_Array[0], Long_Bump_Array[0] + 64, 52}, // 19
 {Long_Bump_Array[0], Long_Bump_Array[0] + 64, 56}, 
 {Long_Bump_Array[0], Long_Bump_Array[0] + 64, 60}, 
 {Long_Bump_Array[0], Long_Bump_Array[0] + 64, 64}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 64}, 
 {Long_Bump_Array[1], Long_Bump_Array[1] + 64, 64}, 
 {Long_Bump_Array[1], Long_Bump_Array[1] + 64, 60}, 
 {Long_Bump_Array[1], Long_Bump_Array[1] + 64, 56}, 
 {Long_Bump_Array[1], Long_Bump_Array[1] + 64, 52}, 
 
 {Tall_Bump_Array[0], Tall_Bump_Array[0] + 80, 64}, // 28
 {Tall_Bump_Array[0], Tall_Bump_Array[0] + 80, 80}, 
 {Bump_Array[1], Bump_Array[1] + 81, 81}, 
 {Tall_Bump_Array[1], Tall_Bump_Array[1] + 80, 80}, 
 {Tall_Bump_Array[1], Tall_Bump_Array[1] + 80, 64}, 
 
 {Bump_Array[0], Bump_Array[0] + 81, 56}, // 33 Slanted Right
 {Bump_Array[0], Bump_Array[0] + 81, 64}, 
 {Bump_Array[0], Bump_Array[0] + 81, 72}, 
 {Bump_Array[1], Bump_Array[1] + 81, 73}, 
 {Tall_Bump_Array[1], Tall_Bump_Array[1] + 80, 72}, 
 {Slanted_Array[0], Slanted_Array[0] + 56, 56}, 
 
 {Slanted_Array[1], Slanted_Array[1] + 56, 56}, // 39 Slanted Left
 {Tall_Bump_Array[0], Tall_Bump_Array[0] + 80, 72}, 
 {Bump_Array[1], Bump_Array[1] + 81, 73}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 72}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 64}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 56}, 
 
 {Small_Jump_Array[0], Small_Jump_Array[0] + 64, 64}, // 45
 {Small_Jump_Array[1], Small_Jump_Array[1] + 64, 64}, 
 
 {Barricade0_Array, Barricade0_Array + 56, 56}, // 47
 {Barricade1_Array, Barricade1_Array + 48, 48}, 
 
 {Rock1and3_Array[0], Rock1and3_Array[0] + 48, 48}, // 49
 {Rock1and3_Array[1], Rock1and3_Array[1] + 48, 48}, 
 {Rock1and3_Array[2], Rock1and3_Array[2] + 48, 48}, 
 
 {Rock2and4_Array[0], Rock2and4_Array[0] + 48, 48}, // 52
 {Rock2and4_Array[1], Rock2and4_Array[1] + 48, 48}, 
 {Rock2and4_Array[2], Rock2and4_Array[2] + 48, 48}, 
 
 {TurboTop_Array[0], TurboTop_Array[0] + 48, 48}, // 55
 {TurboTop_Array[1], TurboTop_Array[1] + 48, 48}, 
 
 {TurboLow_Array[0], TurboLow_Array[0] + 48, 48}, // 57
 {TurboLow_Array[1], TurboLow_Array[1] + 48, 48}, 
 
 {Dry_Pit_Array[0], Dry_Pit_Array[0] + 48, 48}, // 59
 {Dry_Pit_Array[1], Dry_Pit_Array[1] + 48, 48}, 
 
 {Dry_Pit_Array[2], Dry_Pit_Array[2] + 48, 48}, // 61
 {Dry_Pit_Array[1], Dry_Pit_Array[1] + 48, 24}, 
 
 {Dry_Pit_Array[3], Dry_Pit_Array[3] + 48, 48}, // 63
 {NULL, NULL, 0}, 
 
 {Bump_Array[0], Bump_Array[0] + 81, 56}, // 65 (Monster Jump)
 {Bump_Array[0], Bump_Array[0] + 81, 64}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 64}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 64}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 64}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 64}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 64}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 64}, 
 {Monster_Jump_Array[0], Monster_Jump_Array[0] + 96, 80}, 
 {Monster_Jump_Array[0], Monster_Jump_Array[0] + 96, 96}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 96}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 96}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 96}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 96}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 96}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 96}, // 80
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 96}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 96}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 96}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 88}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 80}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 72}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 64}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 64}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 64}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 56}, 
 
 {Monster_Jump_Array[0], Monster_Jump_Array[0] + 96, 64}, // 91 (Monster Ramp)
 {Monster_Jump_Array[0], Monster_Jump_Array[0] + 96, 80}, 
 {Monster_Jump_Array[0], Monster_Jump_Array[0] + 96, 96}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 96}, 
 {Monster_Jump_Array[1], Monster_Jump_Array[1] + 96, 96}, 
 {Monster_Ramp_Array[0], Monster_Ramp_Array[0] + 96, 96}, 
 {Monster_Ramp_Array[1], Monster_Ramp_Array[1] + 96, 96}, 
 {Monster_Ramp_Array[2], Monster_Ramp_Array[2] + 96, 96}, 
 {Monster_Ramp_Array[3], Monster_Ramp_Array[3] + 96, 96}, 
 {Monster_Ramp_Array[3], Monster_Ramp_Array[3] + 96, 96}, // 100
 {Monster_Ramp_Array[3], Monster_Ramp_Array[3] + 96, 96}, 
 {Monster_Ramp_Array[4], Monster_Ramp_Array[4] + 96, 96}, 
 {Monster_Ramp_Array[5], Monster_Ramp_Array[5] + 96, 96}, 
 {Monster_Ramp_Array[6], Monster_Ramp_Array[6] + 96, 96}, 
 {Monster_Ramp_Array[3], Monster_Ramp_Array[3] + 96, 96}, 
 {Monster_Ramp_Array[3], Monster_Ramp_Array[3] + 96, 96}, 
 {Monster_Ramp_Array[3], Monster_Ramp_Array[3] + 96, 96}, 
 {Monster_Ramp_Array[3], Monster_Ramp_Array[3] + 96, 96}, 
 {Monster_Ramp_Array[4], Monster_Ramp_Array[4] + 96, 96}, 
 {Monster_Ramp_Array[5], Monster_Ramp_Array[5] + 96, 96}, 
 {Monster_Ramp_Array[6], Monster_Ramp_Array[6] + 96, 96}, 
 {Monster_Ramp_Array[3], Monster_Ramp_Array[3] + 96, 96}, 
 {Monster_Ramp_Array[3], Monster_Ramp_Array[3] + 96, 96}, 
 {Monster_Ramp_Array[3], Monster_Ramp_Array[3] + 96, 96}, 
 {Monster_Ramp_Array[3], Monster_Ramp_Array[3] + 96, 96}, 
 {Monster_Ramp_Array[7], Monster_Ramp_Array[7] + 96, 96}, 
 {Dry_Pit_Array[2], Dry_Pit_Array[2] + 48, 48}, 
 
 {Bump_Array[0], Bump_Array[0] + 81, 56}, // 118
 {Bump_Array[0], Bump_Array[0] + 81, 64}, 
 {Bump_Array[0], Bump_Array[0] + 81, 72}, 
 {Finish_Line_Array[0], Finish_Line_Array[0] + 72, 72}, 
 {Finish_Line_Array[1], Finish_Line_Array[1] + 72, 72}, 
 {Finish_Line_Array[2], Finish_Line_Array[2] + 72, 72}, 
 {Finish_Line_Array[1], Finish_Line_Array[1] + 72, 72}, 
 {Finish_Line_Array[3], Finish_Line_Array[3] + 72, 72}, 
 {Finish_Line_Array[4], Finish_Line_Array[4] + 72, 72}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 72}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 64}, 
 {Monster_Jump_Array[2], Monster_Jump_Array[2] + 96, 56}, // 129
};

/* Crash Behavior */
// If Slanting Backwards  (End of a Bump) == CRASH, Roll down Bump First Head Over Heels
// If Slanting Forwards (Start of a bump) == CRASH, Roll Up and Down Bump First Head Over Heels
// If Slanting Backwards   (Landing/Flat) == CRASH
// If Slanting Forwards    (Landing/Flat) == CRASH
// Press Turbo for ~7 Seconds before Overheat, ~4 Seconds to Cool Down
// Temp is in Middle for Regular Gas

// Sin(15, 30, 45, 60, 75) * 128
unsigned char Sin_Table128[5] = { 38, 66, 90, 107, 118 };  // cos[x] = Sin_Table128[4 - x];
unsigned char *Tracks[5] = { Compressed_Track1, Compressed_Track2, Compressed_Track3, Compressed_Track4, 
Compressed_Track5 };
const char *exctimes = "exctimes";

unsigned short Default_Times[5] = 
{ 1 * 60 + 16, 1 * 60 + 16, 1 * 60 + 16, 1 * 60 + 10, 0 * 60 + 50 };

// I don't want just any joe schmoe idiot getting the codes by looking at the source, but I 
// don't care if you would like to take the time to convert these to strings to find out the 
// cheats (or write a small prog to do it for you).  Please don't pass them around, though, 
// and make them too easy to find!
const unsigned char _cheats_[8][9] = {
	{167 - 'T', '}' - 37, 88, 'o' - 27, 177 - 'X', 'z' - 37, 'h' - 18, 208 - 'z', 0}, 
	{'^' - 5, 188 - 'w', 165 - 'M', 'f' - 29, '{' - 48, 133 - '6', 'g' - 14, '&' + 27, 0}, 
	{65, 'O' - 14, 154 - 'E', 184 - 'e', '_' - 26, 205 - 't', 21 + ',', 'U' - 6, 0}, 
	{-37 + 'l', 172 - 'k', '&' + 47, 'a' - 14, 6 + '?', 127 - '&', 'L' - 11, 178 - 'b', 0}, 
	{'n' - 44, 168 - '_', 25 + '.', 196 - 'z', 178 - ']', '$' + 41, 137 - '9', 'a' - 14, 0}, 
	{168 - 'c', 51 + '#', 206 - 'y', '2' + 25, 159 - 'X', 'i' - 36, '4' + 13, 198 - 'v', 0}, 
	{'+' + 41, 171 - 'f', 'a' - 14, 20 + '3', 124 - ',', '7' + 10, 'O' - 3, 137 - 'H', 0}, 
	{'L' - 8, 160 - '^', '^' - 21, 4 + 'C', 'I' + 11, 139 - 'B', 77, 'c' - 30, 0}, 
};

const char *const cheats[8][2] = {
	{EFG("Never Overheat", "Aucune surchauffe", "Keine Überhitzung"), _cheats_[0]}, 
	{EFG("Fast Recovery", "Recouvrement rapide",  "Schnell wieder gesund" ), _cheats_[1]}, 
	{EFG("Half speed Timer", "Temps ralenti", "Verlangsamte Zeit"), _cheats_[2]}, 
	{EFG("Very slow Timer", "Temps TRÈS ralenti", "Sehr verlangsamte Zeit"), _cheats_[3]}, 
	{EFG("Moon Gravity", "Gravité lunaire", "Mondschwerkraft"), _cheats_[4]}, 
	{EFG("Fast Bikes", "Motos Rapides", "Schnelle Motorräder"), _cheats_[5]}, 
	{EFG("Background", "Arrière-plan", "Hintergrund"), _cheats_[6]}, 
	{EFG("All Cheats", "Tricherie intense", "Alle Cheats"), _cheats_[7]}, 
};