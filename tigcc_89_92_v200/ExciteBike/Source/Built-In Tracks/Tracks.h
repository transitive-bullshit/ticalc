// Header File
// Created 3/4/2004; 3:46:30 PM

// Definitions to make declaring the Tracks MUCH easier and more readible!
enum Tile_Detections {
/*-------------       																		-----------       ----------  ---------
  Type Of Tiles       																		Description				Dimensions	ByteWidth
  -------------       																		-----------       ----------  --------- */
	Gra1,																									// Gray								8x48				1
	SL00, SL01, SL02, 																		// _Small Bump_				24x57				3
	SL10, SL11, SL12, SL13, SL14, 												// _Medium Bump_			40x65				5
	SL20, SL21, SL22, SL23, SL24, SL25, SL26, SL27, SL28, // _Large Bump_				72x81 			9
	SL30, SL31, SL32, SL33, SL34, SL35, SL36, SL37, SL38, // _Long Bump_				72x64				9
	SL40, SL41, SL42, SL43, SL44, 												// _Tall Bump_				40x81				5
	SL50, SL51, SL52, SL53, SL54, SL55,										// _Slanted Right_		48x73				6
	SL60, SL61, SL62, SL63, SL64, SL65,										// _Slanted Left_			48x73				6
	Jum1,	Jum2,																						// Small Jump					16x64				2
	Blo1,																									// Barricade Block		8x56				1
	Blo2, 																								// Barricade Block		8x48				1
	Rok1,	Rok2, Rok3,																			// Gravel 2 and 4			24x48				3
	Rok4,	Rok5, Rok6,																			// Gravel 1 and 3			24x48				3
	Tur1,	Tur2,																						// Turbo Arrow Top		16x48				2
	Tur3,	Tur4,																						// Turbo Arrow Low		16x48				2
	Dry0,	Dry1, 																					// Dry Pit Low				16x48				2
	Dry2, Dry3,																						// Dry Pit High				16x48				2
	Dry4, Dry5,																						// Dry Pit Full				16x48				2
	MO00, MO01, MO02, MO03, MO04, MO05, MO06, MO07, MO08, 
	MO09, MO10, MO11, MO12, MO13, MO14, MO15, MO16, MO17, 
	MO18, MO19, MO20, MO21, MO22, MO23, MO24, MO25, 			// Monster Jump				208x96			26
	MR00, MR01, MR02, MR03, MR04, MR05, MR06, MR07, MR08, 
	MR09, MR10, MR11, MR12, MR13, MR14, MR15, MR16, MR17, 
	MR18, MR19, MR20, MR21, MR22, MR23, MR24, MR25, MR26, // Monster Ramp				216x96			27
	Fin0, Fin1,	Fin2, Fin3, Fin4, Fin5, Fin6, Fin7, Fin8, 
	Fin9, Fi10, Fi11, 																		// Finish Line				96x72				12
};

// Definitions to make declaring the Tracks MUCH easier and more readible!
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
unsigned char Track1[] = {
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
};