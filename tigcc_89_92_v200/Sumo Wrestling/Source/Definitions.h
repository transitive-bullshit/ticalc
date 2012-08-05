// Header File
// Created 5/22/2004; 11:57:03 AM
/* ------------------------------------------------------------------------- */
/* Defining this will modify some values for uniform/easier screenshotting	 */
//#define CAPTURE_SCREENSHOT
/* ------------------------------------------------------------------------- */
#define Activel								GrayGetPlane (LIGHT_PLANE)
#define Actived								GrayGetPlane (DARK_PLANE)
#define Normal_Match_Process	(g->Status == M_Start || g->Status == M_Normal || g->Status >= M_Finish)
#define QUIT_AUTO						 -2
#define QUIT								 -1
#define M_Start								0
#define M_Normal							1
#define Match_Won_0						2
#define Match_Won_1						3
#define Time_Expired					4
#define Game_Paused						5
#define Fast_Quit							6
#define M_Finish							7
#define Match_Over						(g->Status == Match_Won_0 || g->Status == Match_Won_1)
#define Sumo_Magic						0x7D5  // 2005 :)
#define NO_DEFAULT_WRESTLERS	(16)
#define Load_Slot							(NO_DEFAULT_WRESTLERS)
#define External_Slot					(Load_Slot + 1)
#define One_String_Size				(32 * 2 + 256 * 3)
#define NO_SPRITES						(117)  // Number of large gfx sprites in the game (including mirrors)
#define NO_MASKS							(111)  // Number of masks to calculate at runtime
#define MAX_RING_SPRITES			109
#define map_width							15 // Dimensions of Ring map (15x8 tiles, 240x128 pixels)
#define map_height						8
#define MAX_HEALTH						(72)
#define MAX_GAME_SPEED				(15)
#define MAX_STAT							(31)  // 0 - 31 = 32 possible
#define Left_Edge							32
#define Right_Edge						208
#define A_DISSOLVE						(1 << 0)
#define A_WAITKEY							(1 << 1)
#define BOX_WIDTH							(240/8)
#define BOX_SIZE							(sizeof(char) * 19 * BOX_WIDTH)
#define TILE_SIZE							(sizeof(short) * 16 * 2)
#define NO_EDITOR							(QUIT)
#define A_SPECIAL							(1 << 5)
// How to access a string within a Character struct
//#define Char_String(c, No)		(Char_String_Addr(5 * (c)->Index + (No)))
#define Char_String(c, No)		(g->strings[5 * (c)->Index + (No)])

//#define _KeyCheck(k) 					(_keytest(g->Keys[k].Row, g->Keys[k].Key))
//#define _KeyCheck(k) 					(g->keys & (1 << (k)))
// Detects if this is the first frame a keypress has been held down
//#define _KeyCheck_Tap(k)			((_KeyCheck(k) | (g->keys & (1 << ((k) + 2)))) == (1 << (k)))

#define _KeyCheck(k) 					(g->keys & (k))
// Detects if this is the first frame a keypress has been held down
#define _KeyCheck_Tap(k)			((_KeyCheck(k) | (g->keys & ((k) << 2))) == (k))
#define Wrestler_Light(index) (Wrestlers[(index)].Light)
#define Wrestler_Dark(index)	(Wrestlers[(index)].Dark)
#define Wrestler_Mask(index)	(Wrestlers[(index)].Mask)
#define Wrestler_Height(index) (Wrestlers[(index)].Height)
// Preprocessor directives pertaining to Text Messages
#define TEXT_DELAY						(20)
#define PAUSE_DELAY						(50 * TEXT_DELAY)
#define PAUSE_TEXT(No)				(g->text_delay = count + TEXT_DELAY * (No);)

#define CopyScreens92Plus(p1,p2,p3,p4)	{ FastCopyScreen_R(p1,p3); FastCopyScreen_R(p2,p4); }
#define Toggle_Screens_Fast()	CopyGrayScreens(Hiddenl, Hiddend, Activel, Actived)
#define Toggle_Screens()			CopyScreens92Plus(Hiddenl, Hiddend, Activel, Actived)
#define CopyGrayScreens(p1,p2,p3,p4)	if (g->calculator) \
																				CopyScreens92Plus(p1,p2,p3,p4) \
																			else CopyScreens89(p1,p2,p3,p4);

/*#define Safe_Toggle()					{GrayDBufToggleSync(); 
															if (g->HW1) 
																GrayWaitNSwitches(2);}*/
#define ABS(a)								(((a)<0) ? -(a) : (a))
#define SafeFree(Pointer)			if ((Pointer)) { free((Pointer)); (Pointer) = NULL; }

// Preset amount of memory allocated for program initializations
#define BULK_SIZE							2 * LCD_SIZE + \
															sizeof(GlobalVars) + \
															sizeof(Clock) + \
															(2 * sizeof(Ring_Matrix)) + \
															2 * BOX_SIZE + \
															sizeof(char) * 192 * 4 + \
															TILE_SIZE
															// + sizeof mirrored sprites calculated at runtine

// All Mirrored sprites (57) will be calculated at runtime for optimal program size
// All sprite masks (106) will also be calculated at runtime for optimal program size
enum Rikishi_gfx {
/*----------       									-----------       		----------  ---------		--------
  Name/Alias      									Description						Dimensions	ByteWidth		Mirrored
  ----------       									-----------       		----------  ---------		-------- */
	// Ritual Introductions:
		Stomp1,										// 0) Buddah sitting				28x28				4						No
		Stomp2,										// 1) Buddah standing				28x31				4						No
		Stomp3,										// 2) Leg up Left						30x32				4						No
		Stomp4,										// 3) Leg up Right					30x32				4						Yes (Stomp2)
		
		Salt1,										// 4) Facing Left						23x32				3						No
		Salt2,										// 5) Facing Right					23x32				3						Yes (Salt1)
		Salt3,										// 6) Throwing Left					29x32				4						No
		Salt4,										// 7) Throwing Right				29x32				4						Yes (Salt3)
		
		Kneel1,										// 8) Kneeling Left					20x31				3						No
		Kneel2,										// 9) Kneeling Right				20x31				3						Yes (Kneel1)
		Kneel3,									// 10) Hand Back Left					23x31				3						No
		Kneel4,									// 11) Hand Back Right				23x31				3						Yes (Kneel3)
		Kneel5,									// 12) Clap Left							22x31				3						No
		Kneel6,									// 13) Clap Right							22x31				3						Yes (Kneel5)
	
	// Stances:
		Tackle1,								// 14) Tackle stance Left :)	27x28				4						No
		Tackle2,								// 15) Tackle stance Right :)	27x28				4						Yes (Tackle1)
		Tackle3,								// 16) Tackle stance Left :(	28x26				4						No
		Tackle4,								// 17) Tackle stance Right :(	28x26				4						Yes (Tackle3)
	
	// Movement:
		Walk1,									// 18) Stepping Left 1				28x31				4						No
		Walk2,									// 19) Stepping Right 1				28x31				4						Yes (Walk1)
		Walk3,									// 20) Stepping Left 2				29x31				4						No
		Walk4,									// 21) Stepping Right 2				29x31				4						Yes (Walk3)
		
		Charge1,								// 22) Charge Left 1					27x28				4						No
		Charge2,								// 23) Charge Right 1					27x28				4						Yes (Charge1)
		Charge3,								// 24) Charge Left 2					28x28				4						No
		Charge4,								// 25) Charge Right 2					28x28				4						Yes (Charge3)
	
	// Tossed:
		Tumble1,								// 26) Tumbling (small)				34x32				5						No
		Tumble2,								// 27) Tumbling (small)				34x32				5						Yes (Tumble1)
		Tumble3,								// 28) Tumbling (large)				36x34				5						No
		Tumble4,								// 29) Tumbling (small)				32x34				4						No
		Tumble5,								// 30) Tumbling (small)				32x34				4						Yes (Tumble4) v
		Tumble6,								// 31) Tumbling (large)				36x34				5						Yes (Tumble3) v
		Tumble7,								// 32) Tumbling (small)				34x32				5						Yes (Tumble2) v
		Tumble8,								// 33) Tumbling (small)				34x32				5						Yes (Tumble7)
		Tumble9,								// 34) Tumbling (large)				34x36				5						No
		Tumble10,								// 35) Tumbling (small)				32x34				4						Yes (Tumble5)
		Tumble11,								// 36) Tumbling (small)				32x34				4						Yes (Tumble10) v
		Tumble12,								// 37) Tumbling (large)				34x36				5						Yes (Tumble9) v
		
		Spinn1,									// 38) Spinning (Left)				24x32				3						No
		Spinn2,									// 39) Spinning (Right)				24x32				3						Yes (Spinn1)
		Spinn3,									// 40) Spinning (Turned right)25x32				4						No
		Spinn4,									// 41) Spinning (Turned left) 25x32				4						Yes (Spinn3)
		
		TossBackLeft,						// 42) On Back Gritting teeth 31x27				4						No
		TossBackRight,					// 43) Same, Facing Right			31x27				4						Yes (TossBackLeft)
	
	// Pummeling Stance (Both wrestlers in mixed sprite):
		Pummel1,								// 44) Light Facing Left 
														// (normal, default Pummel)		36x32				5						No
		Pummel2,								// 45) Light Facing Right
														// (normal, default Pummel)		36x32				5						Yes (Pummel1)
		
		PummelPush1,						// 46) Light Facing Left
														// (Pulling up on belt & hop) 33x34				5						No
		PummelPush2,						// 47 Light Facing Right
														// (Pulling up on belt & hop) 33x34				5						Yes (PummelPush1)
		
		PummelLift1,						// 48) Attempting Lift Left		31x35				4						No
		PummelLift2,						// 49) Attempting Lift Right	31x35				4						Yes (PummelLift1)
		PummelLift3,						// 50) Lift up in air Left 1	32x37				4						No
		PummelLift4,						// 51) Lift up in air Right 1 32x37				4						Yes (PummelLift3)
		PummelLift5,						// 52) Lift in air Left 2			34x37				5						No
		PummelLift6,						// 53) Lift in air Right 2		34x37				5						Yes (PummelLift5)
		
		Earthquake_ThrowLeft,		// 54) Slammed into ground 1	48x24				6						No
		Earthquake_ThrowRight,	// 55) Slammed into ground 2	48x24				6						Yes (Earthquake_ThrowLeft)
		
		ShoveLeft,							// 56) Shoving opponent Left	31x32				4						No
		ShoveRight,							// 57) Shoving opponent Right	31x32				4						Yes (ShoveLeft)
		ShovedRight,						// 58) Getting shoved Right (Facing Left, must be drawn 2nd)
														// Falling back a few pixels	24x32				3						No
		ShovedLeft,							// 59) Getting shoved Left (Facing Right, must be drawn 2nd)
														// Falling back a few pixels	24x32				3						Yes (ShovedRight)
		ShovedLeftEdge1,				// 60) Almost Falling Off Left (When 
														// shoved by edge of ring)		31x39				4						No
		ShovedRightEdge1,				// 61) Almost Falling Off Right (When 
														// shoved by edge of ring)		31x39				4						Yes (ShovedLeftEdge1)
		ShovedLeftEdge2,				// 62) Almost Falling Off Left (2nd 
														// animation graphic)					32x40				4						No
		ShovedRightEdge2,				// 63) Almost Falling Off Right (2nd 
														// animation graphic)					32x40				4						Yes (ShovedLeftEdge2)
		
		PummelThrowLeft,				// 64) Attempting Throw Left	37x32				5						No
		PummelThrowRight,				// 65) Attempting Throw Right	37x32				5						Yes (PummelThrowLeft)
		ThrowLeft,							// 66) After Throw Left				32x30				4						No
		ThrowRight,							// 67) After Throw Right			32x30				4						Yes (ThrowLeft)
		ThrownRight,						// 68) Being thrown Right			37x31				5						No
		ThrownLeft,							// 69) Being thrown Left			37x31				5						Yes (ThrownRight)
		PummelThrowCounter1,		// 70) Counter Left Throw			41x32				6						No
		PummelThrowCounter2,		// 71) Counter Right Throw		41x32				6						Yes (PummelThrowCounter1)
	
	// Punch:
		PunchLeft1,							// 72) Punch Left near arm		32x32				4						No
		PunchRight1,						// 73) Punch Right near arm		32x32				4						Yes (PunchLeft1)
		PunchLeft2,							// 74) Punch Left far arm			33x32				5						No
		PunchRight2,						// 75) Punch Right far arm		33x32				5						Yes (PunchLeft2)
		PunchedLeft1,						// 76) Hit, face turned, blood
														// (Facing Left)							29x32				4						No
		PunchedRight1,					// 77) Hit, face turned, blood
														// (Facing Right)							29x32				4						Yes (PunchedLeft)
		PunchedLeft2,						// 78) Hit, face turned, blood
														// (Facing Left)							29x32				4						No
		PunchedRight2,					// 79 Hit, face turned, blood
														// (Facing Right)							29x32				4						Yes (PunchedLeft2)
	
	// Taunt:
		TauntLeft1,							// 80) Arms crossed up in air 23x32				3						No
		TauntRight1,						// 81) Arms crossed up in air 23x32				3						Yes (TauntLeft1)
		TauntLeft2,							// 82) 2nd anim, arms down		28x32				4						No
		TauntRight2,						// 83) 2nd anim, arms down		28x32				4						Yes (TauntLeft2)
	
	// Celebration:
		Laughing1,							// 84) Laughing/Line					32x30				4						No
		Laughing2,							// 85) Laughing/Smiling				32x32				4						No
	
	// Eating:
		//SumoEating1,						// 86) Eating Rice						24x27				3						No
		//SumoEating2,						// 87) Eating Rice anim 2			24x27				3						No
	
	// Pause Screen:
		SweepingLeft,						// 86) Sweeping with a broom	24x32				3						No
		SweepingRight,					// 87) Sweeping with a broom	24x32				3						Yes (SweepingLeft)
	
	// Random Finishes:
		SmushedLeft,						// 88) Smushed facing left		37x22				5						No
		SmushedRight,						// 89) Smushed facing right		37x22				5						Yes (SmushedLeft)
		BellyFlopLeft1,					// 90) Falling down, mouth slightly open
														//														35x24				5						No
		BellyFlopRight1,				// 91) Falling down, mouth slightly open
														//														35x24				5						Yes (BellyFlopLeft1)
		BellyFlopLeft2,					// 92) Smushing opponent, mouth wide open
														//														32x23				5						No
		BellyFlopRight2,				// 93) Smushing opponent, mouth wide open
														//														32x23				5						Yes (BellyFlopLeft2)
	
	// Backwards Slam:
		BackwardsSlamLeft,			// 94) All the way back				30x21				4						No
		BackwardsSlamRight,			// 95) All the way back				30x21				4						Yes (BackwardsSlamLeft)
		Slammed,								// 96) Head slammed into ring	30x37				4						No
	
	// Growl Taunt/Finish:
		GrowlLeft1,							// 97) Leaning back left			30x32				4						No
		GrowlRight1,						// 98) Leaning back right			30x32				4						Yes (GrowlLeft1)
		GrowlLeft2,							// 99) Standing up, growling	24x38				3						No
		GrowlRight2,					// 100) Standing up, growling		24x38				3						Yes (GrowlLeft2)
	
	// Match Summary:
		MatchWinner1,					// 101) Squatting/smiling				24x27				3						No
		MatchWinner2,					// 102) Jumping up							24x27				3						No
		
		MatchLoser1,					// 103) Facing away							24x24				3						No
		MatchLoser2Left,			// 104) Looking back left				24x27				3						No
		MatchLoser2Right,			// 105) Looking back right			24x27				3						Yes (MatchLoser2Left)
	
	// Misc Graphics which have masks
		Sign,									// 106) Right Arrow Sign				16x32				2						No
	
		Star1,								// 107) Light										16x16				2						No
		Star2,								// 108) Dark										16x16				2						No
		Star3,								// 109) Light2									16x16				2						Yes (Star1)
		Star4,								// 110) Dark2										16x16				3						Yes (Star2) v
	
	// Misc other non-wrestler Graphics which have to be mirrored (masked are not calculated):
		Pre_Match0,						// 111) Pre-Match UpLeft				80x50				10					No
		Pre_Match1,						// 112) Pre-Match UpRight				80x50				10					Yes (Pre_Match0)
		Pre_Match2,						// 113) Pre-Match LowLeft				80x50				10					Yes (Pre_Match0) v
		Pre_Match3,						// 114) Pre-Match LowRight			80x50				10					Yes (Pre_Match2)
		
		Text_Box0,						// 115) Text Box Graphic Left		56x21				7						No
		Text_Box1,						// 116) Text Box Graphic Right	56x21				7						Yes (Text_Box0)
	// Non-Existent Graphic whichexists only as a Placeholder
		Reversed,							// 117) cannot take out!  (should equal NO_SPRITES)
};

enum ring_index {
	Whit, // 0x00, Blank
	//Blac, // 0xFF
	Low1, // Bottom Left of Ring
	Low2, // Bottom Middle (repeated) along width of Ring
	Low3, // Bottom Right of Ring
	Cir1, Cir1M, // Bottom Left (M)
	Cir2, Cir2M, Cir3, Cir4, Cir5, Cir5M, Cir6, Cir7, Cir9, 
	Ci10, Ci10M, Ci11, Ci11M, Ci12, Ci12M, Ci13, Ci13M, 
	Ci14, Ci14M, Ci15, Ci15M, Ci16, Ci16M, Ci17, Ci17M, Ci18, Ci18M, 
	Ci19, Ci19M, Ci20, Ci20M, Ci21, Ci21M, Ci22, Ci22M, Ci23, Ci24, 
	
	Top1, Top2, Top3, Top4, Top5, 
	Pillar, PillarM, 
	Lin1, Lin1M, Lin2, Lin2M, 
	
	// Small Face Fillers which also include part of the Ring
	Eye1, Eye1M, Eye2, Eye2M, Eye3, Eye3M, Eye4, Eye4M, 
	// 'Outside' audience sprites (higher bar)
	Audience1, Audience1M, Audience2, Audience2M, // Black Hair
	Audience3, Audience3M, Audience4, Audience4M, // Light Hair
	Audience5, Audience5M, Audience6, Audience6M, // Bald, No Hair
	Audience7, Audience7M, Audience8, Audience8M, // Frizzy, Dark Gray Hair
	// 'Inside' audience sprites (lower bar)
	Aud1, Aud1M, Aud2, Aud2M, // Dark Gray Hair
	Aud3, Aud3M, Aud4, Aud4M, // Frizzy, Dark Gray Hair (low)
	Aud5, Aud5M, Aud6, Aud6M, // Male, Bald Announcer
	Aud7, Aud7M, Aud8, Aud8M, // Female Announcer
	Aud9, Aud10, // DogMan
	// Misc
	PicLeft, PicRight, // Girl with Camera (not animated)
	// Small Faces
	Face1, Face2, 
	Face3, Face4, 
	// In Middle, under Top of 'roof'-thingy
	Face5, Face6, 
	
	// Placeholders for audience sprites in the Ring_Matrix
	// Before every Match, these will be replaced by random/appropriate Audience 
	// sprites from above, creating a different audience each match
	Outs, Insi, Camera, Smal, Smal2, 
};

enum Controls	      { Host, Join, AI };
enum Errors         { MEMORY, memory, LINK_ERROR, link_error };
enum Modes		      { EXHIBITION, TOURNAMENT, LINKPLAY, PRACTICE, NONE };
enum char_strings   { C_Name, C_Origin, C_Smack };

// Calculator Key Constants
enum Game_Keys      { UP, LEFT, DOWN, RIGHT, A_, B_, A_REPEAT, B_REPEAT, ENTER, TEACHER, ESC };
#define UPKEY				(1 << (UP))
#define LEFTKEY			(1 << (LEFT))
#define DOWNKEY			(1 << (DOWN))
#define RIGHTKEY		(1 << (RIGHT))
#define A_KEY				(1 << (A_))
#define B_KEY				(1 << (B_))
#define ENTERKEY		(1 << (ENTER))
#define TEACHERKEY	(1 << (TEACHER))
#define ESCKEY			(1 << (ESC))

/* - - - - - - - - - -
	 FUNCTION PROTOTYPES
	 - - - - - - - - - - */
// Many functions are not included here because they are declared static inline for optimization purposes
// These functions are then declared at the top of the C source file they are located in

/*					Main.c					*/
void Load_Wrestlers(void);
void ERROR(const unsigned char) __attribute__ ((noreturn));
void LeadOut(void) __attribute__ ((noreturn));

/*				ScrollLib.c				*/
void DrawBackground(void);
void Center_Camera(void);
void Check_Bounds(const short);
void Shift_Horizontal(const short);
void GrayTile_Clipped(short, short, unsigned short*);
//void CopyScreens89(unsigned char *src0 asm("%a0"), unsigned char *src1 asm("%a1"), 
//unsigned char *dest0 asm("%a2"), unsigned char *dest1 asm("%a3")) __attribute__((__regparm__));

/*					Game.c					*/
void MAIN_LOOP(void);
void Health_Bar(unsigned char*, const short, const short);
//void Health_BarRight(unsigned char*, short);

/*				Wrestlers.c				*/
void Update_Wrestlers(void);
void Handle_Wrestler(wrestler*);
void Align_Synchronize_Wrestler(wrestler*);
void Check_ACTION(wrestler*);
short Hit_Detection(short);
void New_Pummel_Move(wrestler*, char);
void Add_Health(wrestler*, short);
void Draw_Wrestler(wrestler*, void*, void*);
void Draw_Blood(short, short, unsigned char);
void End_Introductions(void);
void Initiate_Finisher(wrestler*, short);
void Initiate_EarthQuake(void);
void Check_Edge(wrestler*);
short Wrestler_Width(short) __attribute__ ((pure));

/*			PrePostMatch.c			*/
short Choose_Wrestler(const char*, short, short, short);
short Pre_Match(void);
void Draw_Pre_Match(short, short);
void Post_Match(void);
void Display_Loser(short, short, void*, void*);
void Display_Winner(short, short, char);

/*			 Tournament.c				*/
void Tournament(short);

/*				Messages.c				*/
void New_Text(const char*, short);
void Run_Text(void);

/*					Misc.c					*/
void Dissolve(void);
void KeyScan(void);
void while_keypress(unsigned short);
//void Gallery(void);
void Display_Interface_Design(short);
void Pause_Game(void);
void WaitKey(void);
void Message(const char *const [], const short);
void Display_Author(void);
short Get_Input(const char*, char*, short, short*);
void Draw_User_Input(const char*, const char*, const short);
void DrawGrayString(short,short,const char*,short,short);
void Reset_Time(void);

/*				 Editor.c					*/
void Wrestler_Editor(void);
void ValidateSumoFileName(char*, short);
short Find_All_Files(char (*)[9], const char);

/*					Link.c					*/
short Establish_Connection(void);
void Transfer_Data(void);
void Exit_Linkplay(void);
void Notify_Linkplay_Exit(void);
short Align_Calcs(void);

/*			 Interface.c				*/
short Menu(const char*, const char *const[], short, short, char);
void ScrollText(void*, const short);

/*				External.c				*/
void Save_Config(void);
void Load_Config(void);
short Save_Tournament(void);
short Load_Tournament(void);
unsigned char *Get_Data_Ptr(const char*, short);
void Archive(const char*);
void UnArchive(const char*);

/*		 itoa_ushort_10.s			*/
short itoa_ushort_10(register unsigned char *str asm("%a0"), register unsigned short number asm("%d0"));

/*			 Wrestlers.h				*/
//unsigned char * __attribute__((__regparm__)) Char_String_Addr(unsigned short Index asm("%d0"));
unsigned char * __attribute__((__regparm__)) STRADDR_MASTER(unsigned short Index asm("%d0"), 
void *array asm("%a1"));