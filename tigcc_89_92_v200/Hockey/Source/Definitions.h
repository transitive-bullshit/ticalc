// C Header File
// Created 9/7/2003; 7:17:49 PM
/* ------------------------------------------------------------------------- */
/* For Debugging purposes, just prints info at certain spots during the game */
//#define DEBUG
/* ------------------------------------------------------------------------- */
/*#define Hiddenl								GrayDBufGetHiddenPlane (LIGHT_PLANE)
#define Hiddend								GrayDBufGetHiddenPlane (DARK_PLANE)
#define Activel								GrayDBufGetActivePlane (LIGHT_PLANE)
#define Actived								GrayDBufGetActivePlane (DARK_PLANE)*/
#define Activel								GrayGetPlane (LIGHT_PLANE)
#define Actived								GrayGetPlane (DARK_PLANE)
#define QUIT								 -1
#define Normal								0
#define Level_Scored					1
#define Level_Failed					2
#define Level_Setup						3
#define Game_Finished					4
#define Game_Paused						5
#define Fast_Quit							6
#define Average								0
#define Fat										10
#define Goalie								20
#define Skinny							 -1
#define Width									29
#define Height								14
#define Max_x									441
#define Max_y									192
#define Max_Camera_y					128
//#define LINK_NO								13
#define HOST_NO								80  // Signal given by Host to Joining Calc  (random)
#define JOIN_NO								43   // Signal given by Joining Calc to Host (random)
#define Zerox									((LCD_WIDTH - 160) >> 1)
#define Zeroy									((LCD_HEIGHT - 100) >> 1)
/* Hit Detection for the Player and the Goals */
#define Hit_Goal1(x, y) 			((x) > 35 && (x) < 66 && (y) > 67 && (y) < 138)
#define Hit_Goal2(x, y) 			((x) > 383 && (x) < 414 && (y) > 67 && (y) < 138)
/* Hit Detection for the Puck and the Goals */
#define Puck_Hit_Goal1(x, y)	((x) > 46 && (x) < 66 && (y) > 80 && (y) < 138)
#define Puck_Hit_Goal2(x, y)	((x) > 394 && (x) < 414 && (y) > 80 && (y) < 138)
#define ABS(a)								(((a)<0) ? -(a): (a))
#define Dist(x1, y1, x2, y2)	Fastsqrt(((long)((x1)-(x2)))*((long)((x1)-(x2)))+((long)((y1)-(y2)))* \
															((long)((y1)-(y2))))

#define Clip_Gray_Pix(x, y)		if ((x) >= 0 && (x) < LCD_WIDTH && (y) >= 0 && (y) <= g->PlaneHeight - 1) { \
																EXT_SETPIX(Hiddenl, x, y); \
																EXT_SETPIX(Hiddend, x, y); \
															}
#define CopyScreens92Plus(p1,p2,p3,p4)	{ FastCopyScreen_R(p1,p3); FastCopyScreen_R(p2,p4); }
#define Toggle_Screens_Fast()	CopyGrayScreens(Hiddenl, Hiddend, Activel, Actived)
#define Toggle_Screens()			CopyScreens92Plus(Hiddenl, Hiddend, Activel, Actived)
#define CopyGrayScreens(p1,p2,p3,p4)	if (TI89) \
																				CopyScreens89(); \
																			else CopyScreens92Plus(p1,p2,p3,p4)

/*#define Safe_Toggle()					{GrayDBufToggleSync(); 
															if (g->HW1) 
																GrayWaitNSwitches(2);}*/
#define _custom_keytest(k) 		(_keytest(g->Keys[k].Row, g->Keys[k].Key))
#define _KeyCheck(k) 					(g->keys & (k))

// Hidden gray planes are oversized by one line to allow correct drawing of the letter 'g' in team names
// This fixes some erratic behavior described and reported by Kevin Kofler
#define HIDDEN_SIZE						(LCD_SIZE + 30)
#define Bulk_Size							(sizeof(Team) * 2) + \
															sizeof(Game) + \
															sizeof(Clock) + \
															HIDDEN_SIZE * 2

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* Thanks to Lionel Debroux, defining these saves around 1,188 Bytes!!!      */
#define COMPRESSION  // Hockey will not work right if this is not defined
//#define BULK_ALLOCATION  //(Default now, doesn't need a #define anymore)
//#define EXTERNAL_GFX // External file to hold gfx/data
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */


// Calculator Key Constants
enum Game_Keys      { UP, LEFT, DOWN, RIGHT, SHOT, PASS_, CHANGE, PAUSE, ENTER, TEACHER, ESC };
#define UPKEY				(1 << (UP))
#define LEFTKEY			(1 << (LEFT))
#define DOWNKEY			(1 << (DOWN))
#define RIGHTKEY		(1 << (RIGHT))
#define SHOTKEY			(1 << (SHOT))
#define PASSKEY			(1 << (PASS_))
#define CHANGEKEY		(1 << (CHANGE))
#define PAUSEKEY		(1 << (PAUSE))
#define ENTERKEY		(1 << (ENTER))
#define TEACHERKEY	(1 << (TEACHER))
#define ESCKEY			(1 << (ESC))

enum Speeds		{Min_yspeed = -3, Max_yspeed = 3, Min_xspeed = -3, Max_xspeed = 3};
enum Controls	{Computer, Host, Join};
enum Actions	{Shooting, Fall};
enum Errors   {MEMORY, LINK_ERROR, EXTERNAL_FILE_HOCKGFX, FONT_LOADING};
enum Modes		{EXHIBITION, SEASON, TOURNAMENT, PRACTICE, SHOOTOUT};
enum Division {WESTERN, EASTERN};

/* - - - - - - - - - -
	 FUNCTION PROTOTYPES
	 - - - - - - - - - - */

/*					Main.c					*/
void Compatibility(void);
void Allocate_Players(Team*);
void ERROR(const unsigned char);
void LeadOut(void) __attribute__ ((noreturn));
void LeadOut2(void)__attribute__ ((noreturn));

/*				ScrollLib.c				*/
void DrawMap(void);
void Center_Camera(void);
void Shift_Horizontal(const short);

/*				 Player.c					*/
void Update_Plrs(void);
void New_Control(const short);
void Draw_Plr(Plr*);
void Move_Computer_Plr(Plr*);
void Move_Cur_Plr(Plr*);
void Shoot_Puck(Plr*, const short);
short Closest_Plr(const Plr*);
void Friction(Plr*);
void Clipped_Plr(const short, short, unsigned short*, short);

/*					Game.c					*/
void MAIN_LOOP(void);
short Outer_ShootOut(void);
short Run_ShootOut(short);
void Set_Default_Game(void);
void Start_Over_At_Center(void);

/*					Modes.c					*/
void Exhibition(void);
short Choose_GamePlay(void);
void Season(void);
void Tournament(void);
void Practice(void);
void ShootOut(void);
void Outer_Loop(void);
void Post_Game(void);
short Choose_Team(Team*, const unsigned char);
void Show_Team_Stats(Names);
void Choose_Random_Team(Team*);
short Show_Tournament(unsigned char [8][3], short);
void Draw_TeamName(unsigned char [8][3], short, short, short, short);
void Editor_Menu(void);
short Edit_Team(Names*);

/*					Misc.c					*/
void WaitKey(void);
void KeyScan(void);
void while_keypress(unsigned short);
//__attribute__((always_inline)) inline void GrayCircle_Clipped(const short, const short, const short);
void Dissolve(void);
void Delay(unsigned long);
void Fill_Outside(void);
void Show_Title(void);
unsigned long Fastsqrt(unsigned long);
void Message(const char* const [], const short);
short Pause_Game(void);
void FS_DrawGrayString(short,short,const char*,short,short);
void Get_Input(const char*, char*, short);
void Draw_User_Input(const char*, char*, short);
void BubbleSort_Teams(Names*[], short);
void Swap_Teams(Names*, Names*);
short Distorted_Screen(void);
void Distort_Screen(unsigned short, unsigned short, unsigned char, unsigned char*, unsigned char*);
void Distort_ScreenSafe(unsigned short, unsigned short, unsigned char, unsigned char*, unsigned char*);

/*					Link.c					*/
void Transfer_Data(void);
short Multiplayer(void);
void Synchronize_Calcs(void);
short Align_Calcs(void);
void Exit_Linkplay(void);

/*					Puck.c					*/
void Update_Puck(void);

/*			 Interface.c				*/
void Main_Menu(void);
short Menu(const char*, char[], char, char, short);
void Options(void);
void Info(void);

/*				External.c				*/
void Save_Config(void);
void Load_Config(void);
void Load_Teams(void);
void Save_Teams(void);
unsigned char *Get_Data_Ptr(SYM_STR, short);
void Archive(SYM_STR);
void UnArchive(SYM_STR);
void RLE_Decompress(unsigned char*, unsigned char*, short);

/*					Font.h					*/
void SpriteString(short, short, unsigned char*, char*, char*, short);
void ScrollText(short);

// by TICT
void FS_DrawString(register short asm("%d0"),register short asm("%d1"),register const unsigned char* 
asm("%a0"),register void* asm("%a1"),register short asm("%d2"));


/*	  GraySprite32_MASK.s	 	*/
/*void GraySprite32_MASK_N(register short x asm("%d0"),register short y asm("%d1"),
register short h asm("%d2"),long *sprt0,long *sprt1,long *mask0,long *mask1,
register void *dest0 asm("%a0"),register void *dest1 asm("%a1"));*/