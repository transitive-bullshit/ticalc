// Header File
// Created 12/14/2003; 12:02:14 PM
//#define Hiddenl								GrayDBufGetHiddenPlane (LIGHT_PLANE)
//#define Hiddend								GrayDBufGetHiddenPlane (DARK_PLANE)
#define Activel								GrayGetPlane (LIGHT_PLANE)
#define Actived								GrayGetPlane (DARK_PLANE)
#define QUIT								 -1
#define Normal								0
#define End_Wheelie						1
#define Race_Finished					2
#define Game_Paused						3
#define Fast_Quit							4
#define Race_is_Running				(g->Status == Normal || g->Status == End_Wheelie)
#define LINK_NO								93 // random
#define Track_Magic						0x7D4  // 2004 :)
#define Best_Minutes					Best_Times(0)  // Track Records
#define Best_Seconds					Best_Times(1)
#define Best_Mseconds50				Best_Times(2)
#define Zeroy									((g->lcd_height - 100) >> 1)
#define A_DISSOLVE						(1 << 0)
#define A_WAITKEY							(1 << 1)
//#define SIGN(a)								(((a)<0) ? -1 : 1)
#define ABS(a)								(((a)<0) ? -(a) : (a))
//#define Real_x(x)							((x) - ((g->camera_x - (LCD_WIDTH >> 3)) << 3) + g->softx)
#define CopyScreens92Plus(p1,p2,p3,p4)	{ FastCopyScreen_R(p1,p3); FastCopyScreen_R(p2,p4); }
#define Toggle_Screens_Fast()	CopyGrayScreens(Hiddenl, Hiddend, Activel, Actived)
#define Toggle_Screens()			CopyScreens92Plus(Hiddenl, Hiddend, Activel, Actived)
//GrayDBufToggleSync(); if (HW1) GrayWaitNSwitches(2);
/*	Waits until the user press a key and then releases the key	*/
#define WaitKey()							while(_rowread(0)) continue; \
															while(!_rowread(0)) continue; \
															while(_rowread(0)) continue;
#define _KeyCheck(k) 					(_keytest(g->Keys[k].Row, g->Keys[k].Key))
#define SafeFree(Pointer)			if ((Pointer)) { free((Pointer)); (Pointer) = NULL; }
#define CopyGrayScreens(p1,p2,p3,p4)	if (TI89) \
																				CopyScreens89(p1,p2,p3,p4); \
																			else CopyScreens92Plus(p1,p2,p3,p4)
#define BULK_SIZE							LCD_SIZE * 4 +			\
															sizeof(Clock) +			\
															sizeof(GlobalVars)

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

enum bikegfx {
	Back1, Back2, Back3, Back4, Back5, Back6, 			// Slanting Backwards
	Regular1, Regular2, 														// Normal Riding, No Slant
	Forwards1, Forwards2, Forwards3, Forwards4, 		// Slanting Forwards
	Crash1, Crash2, Crash3, Crash4, 								// Crash Animation
	DownTurn, UpTurn, 															// Turning Down and Up
	Finish_Wheelie, 																// Race Finished Wheelie
	Crashed, 																				// Bike Crashed on its side
	Still, 																					// Bike, No Horizontal Movement
	EndCrash1, EndCrash2, 													// Rider getting back on Crashed Bike
	None = 255, 
};

// Calculator Key Constants
enum Game_Keys {
	ESCKEY, UPKEY, LEFTKEY, DOWNKEY, 
	RIGHTKEY, PAUSEKEY, GASKEY, TURBOKEY, 
	TEACHERKEY, CONFIRMKEY
};

enum Controls	{Host, Join, AI_SLOW, AI_MEDIUM, AI_FAST};
enum Errors   {MEMORY, LINK_ERROR};
enum Engine_Temps {Low_Temp = 12, Middle_Temp = 32, Max_Temp = 64};

/* - - - - - - - - - -
	 FUNCTION PROTOTYPES
	 - - - - - - - - - - */
// Many functions are not included here because they are declared static inline for optimization purposes

/*					Main.c					*/
void ERROR(const unsigned char) __attribute__ ((noreturn));
void LeadOut(void) __attribute__ ((noreturn));

/*				ScrollLib.c				*/
void Shift_Left(short);
void __attribute__((__regparm__)) DrawTerrain_Clipped8_OR_R(short x asm("%d0"), short y asm("%d1"), 
short h asm("%d2"), unsigned char* sprite1 asm("%a2"), unsigned char *sprite2 asm("%a3"));

void CopyScreens89(unsigned char *src0 asm("%a0"), unsigned char *src1 asm("%a1"), 
unsigned char *dest0 asm("%a2"), unsigned char *dest1 asm("%a3")) __attribute__((__regparm__));
/*void  __attribute__((__stkparm__)) CopyScreens92Plus(unsigned char *src0 asm("%a0"), unsigned char *src1, 
unsigned char *dest0 asm("%a1"), unsigned char *dest1);*/

/*					Game.c					*/
void MAIN_LOOP(void);
void Initialize_Race(short);

/*				 Bikes.c					*/
void Update_Bikes(void);
void Copy_Bike(Bike*, Bike*);
void Start_Crash(Bike*, short);
void Start_Jump(Bike*, unsigned long);
void Stop_Turning(Bike*);
void New_Text_Message(const char*);

/*				 Modes.c					*/
void Selection_A(void);
void Selection_B(void);
void Main_Game(short);
void Multiplayer(void);
short Choose_Track(void);
void Post_Race(short);

/*					Misc.c					*/
void Dissolve(void);
void Message(const char *const [], const short);
short Get_Input(const char*, char*, short);
void Draw_User_Input(const char*, const char*, const short);
void DecompressRLE(unsigned char*, unsigned char*, short);
short CompressRLE(unsigned char*, unsigned short);
void Display_Author(void);
void Pause_Game(void);
unsigned char Get_Number(const char*, short, short, short);
//short Delay_Calc(short);

/*				 Editor.c					*/
void Start_Track_Editor(void);
void Run_Editor(const char);
short Convert_Track(short, const unsigned char*);
void Draw_Editor(short, short*);
short Load_Track(unsigned char*);
short Select_External_Track(const char*);
short Best_Times(short);
short New_Track_Record(void);

/*					Link.c					*/
short Establish_Connection(void);
void Transfer_Data(void);
void Transfer(char*, char*, unsigned long);
void Exit_Linkplay(void);
void Notify_Linkplay_Exit(void);
short Align_Calcs(void);

/*			 Interface.c				*/
short Menu(const char*, const char[], short, short, short);
void Options(void);

/*				External.c				*/
short Save_File(const char*, unsigned char*, unsigned long);
unsigned char *Get_Data_Ptr(const char*, short);
void Archive(const char*);
void UnArchive(const char*);

void FS_DrawString(short asm("%d5"), short asm("%d0"), const unsigned char* asm("%a0"), void* asm("%a3"), 
short asm("%d7")) __attribute__((__regparm__));  // Beta by TICT
void FS_InvertString(short asm("%d5"), short asm("%d0"), const unsigned char* asm("%a0"), void* asm("%a3"), 
short asm("%d7")) __attribute__((__regparm__));  // Beta by TICT
// Wrapper for easy Gray string drawing, using TICT's *FAST* Beta Routines
void FS_DrawGrayString(short,short,const char*,short,short) __attribute__((__stkparm__));

// Custom GraySprite8_OR_R function (modified version of the ExtGraph function)
// In some parts of Scrollib.c, the program is strapped for registers, producing 
// unoptimized code.  By customizing this function to not take two registers 
// for the destination planes, it frees up some more of those registers to make 
// for smaller/faster code.
void CGraySprite8_OR_R(register short x asm("%d0"),register short y asm("%d1"),
register short h asm("%d2"),register unsigned char *sprt0 asm("%a2"),
register unsigned char *sprt1 asm("%a3")) __attribute__((__stkparm__));