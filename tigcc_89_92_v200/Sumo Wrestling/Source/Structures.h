// Header File
// Created 5/22/2004; 12:29:25 PM
#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

// Holds a wrestler graphic and it's accompanying misc info
typedef struct {
	void *Light;
	void *Dark;
	void *Mask;
	
	unsigned char Width;
	unsigned char Height;
	char x_off;
	char y_off;
} WRESTLER;  // 16 bytes

typedef struct {
	// Clock Variables
	short Mseconds50;
	short Seconds;
	short Old_Seconds;
	short Running;
	char Time_Limit;
	char pad;
} Clock;

typedef struct {
	char Index;  // if (Index > External_Slot) External Wrestler
	char Strength;
	char Health;  // 41 (min) + Amount of starting Health
	char Stamina;
	char Speed;
	unsigned char Age;  // if (Age == FALSE) disp "Unknown"
	unsigned short Weight;
} Character;

typedef struct {
	short x;
	short y;
	unsigned short attr;
	unsigned char Graphic;
	unsigned char Control;				// Host, Join, Computer AI
	short Counter;
	char Health;
	char flag;
	Character *c;  // Customizable Stats (Weight, toughness, etc)
} wrestler;

#define NO(w)						((w)->attr & W_NO)
#define Intro(w)				(((w)->attr & W_INTRO) >> 2)
#define Direction(w)		(((w)->attr & W_DIRECTION) != FALSE)
#define Animation(w)		(((w)->attr & W_ANIM) >> 3)
//((((w)->attr & W_ANIM) != FALSE) << 1)
#define _ACTION_(a)			(((a)&(W_PUNCH|W_PUNCHED|W_TAUNT1|W_TAUNT2|W_THROW|W_THROWN|W_EDGE|W_SHOVED)) != FALSE)
#define W_ACTIONS				\
(W_CHARGE|W_PUNCH|W_PUNCHED|W_TAUNT1|W_TAUNT2|W_THROW|W_THROWN|W_EDGE|W_SHOVED|DRAW_FIRST)

/* - - - - - - - - - - - - - - - - */
/* Wrestler Attributes             */
/* - - - - - - - - - - - - - - - - */
// If change W_NO or W_ANIM, change where Initiate Pummel is in Wrestlers.c
#define W_NO					1							// Bit 1  // Change NO(w) if change this one too
#define W_DIRECTION		(1<<1)				// Bit 2 (Facing Left or Right)
#define W_INTRO				(3<<2)				// Bits 3 and 4, change Initiate_Match if change this one and #Intro(w)
//#define W_SQUAT				(3<<2)				// Bits 3 and 4 (only applicable after Introductions are finished)
//#define W_SQUAT1			(1<<2)				// Bit 3 (only applicable after Introductions are finished)
//#define W_SQUAT2			(1<<3)				// Bit 4 (only applicable after Introductions are finished)
#define W_TAUNT2			(1<<3)				// Bit 4 (only applicable after Introductions are finished)
#define W_ANIM				(1<<4)				// Bit 5  // also need to change in W_EDGE
#define W_CHARGE			(1<<5)				// Bit 6
#define W_PUNCH				(1<<6)				// Bit 7
#define W_PUNCHED			(1<<7)				// Bit 8
#define W_TAUNT1			(1<<8)				// Bit 9
#define W_THROW				(1<<9)				// Bit 10
#define W_THROWN			(1<<10)				// Bit 11
#define W_EDGE				(1<<11)				// Bit 12
#define W_SHOVED			(1<<12)				// Bit 13
#define DRAW_FIRST		(1<<13)				// Bit 14
#define W_MOVEABLE		(1<<14)				// Bit 15
#define W_LIFTFINISH	(1<<15)				// Bit 16

/*#define W_PUMMEL			1  (Instead, these will all be used by g->Mixed_Gfx)
#define W_LIFT				2
#define W_LIFTED			3
#define W_THROW				4
#define W_THROWN			5
#define W_PUSH				6
#define W_PUSHED			7
#define W_SHOVE				8
#define W_SHOVED			9
#define W_PUNCH				10
#define W_PUNCHED			11

#define W_POSITIONS		15						// Bits 1, 2, 3, and 4
#define W_NO					(1<<4)				// Bit 5
#define W_DIRECTION		(1<<5)				// Bit 6 (Facing Left or Right)
#define W_INTRO				(3<<6)				// Bits 7 and 8, change Initiate_Match if change this one
#define W_ANIM				(1<<8)				// Bit 9
#define W_CHARGE			(1<<9)				// Bit 10*/
/* - - - - - - - - - - - - - - - - */

/* - - - - - - - - - - - - - - - - */
/* Wrestler Statistics             */
/* - - - - - - - - - - - - - - - - */
//#define W_WEIGHT			7							// Bits 1, 2, and 3 // Weights[(w->stats & W_WEIGHT)] + random(17) - 8
//#define W_AGE				(15<<3)					// Bits 4, 5, 6, and 7  // 20 + (w->stats & W_AGE)
//#define W_LEVEL			(15<<7)					// Bits 8, 9, 10, and 11

/* - - - - - - - - - - - - - - - - */

typedef struct {
	short No;
	const unsigned char *text;
} Finish_Text;

typedef struct {
	unsigned char name[16];  // Max 14 letters + 1 Null byte
	unsigned short score;
} HSC;

// Cisco ist hübsch!
typedef struct {
	/* Main Wrestler Data */
	wrestler wrestlers[2];	// Structures which hold information for the two wrestlers during a match
	Character **Characters;	// Information for different characters (both default and external)
	char **strings;					// Character strings:  Name, Origin, Smack Talk (3)  (both default and external)
	/* Current Game Values */
	char Status;						// Holds game data like Paused, Normal, Exit, Fast Exit, etc.
	unsigned char delay;		// Ongoing delay counter to sync wrestler animations
	char Mixed_Gfx;					// Holds data for when both wrestlers are cooperating their behavior (Pummeling)
	char Move0;							// Current actions initiated by Wrestler 0
	char Move1;							// Current actions initiated by Wrestler 1
	char Mode;							// Current Game Mode (Exhibition/Tournament/Practice)
	unsigned char NO_WRESTLERS;			// Number of available wrestlers to choose from
	unsigned char Finisher;					// Records the Finisher Move used to end a Match
	unsigned char lcd_width;				// LCD_WIDTH
	unsigned char lcd_height;				// LCD_HEIGHT
	char calculator;				// Holds value of (CALCULATOR > 0)
	char Old_Status;				// Holds value of Game Status while paused
	short Cur_Health_0;			// Currently shown Health of Wrestler 0 (may not equal g->wrestlers[0].Health)
	short Cur_Health_1;			// Currently shown Health of Wrestler 1 (may not equal g->wrestlers[1].Health)
	short Zerox;						// For displaying the interface correctly on the 92+/v200
	short Zeroy;						// For displaying the interface correctly on the 92+/v200
	short Old_count;				// Holds AI_1 timer info while Paused
	/* Map/Camera offsets for scrollable ring */
	short Force_Update;			// Background is not redrawn every frame (only updated if Force_Update == TRUE)
	short camera_x;
	short camera_y;
	short Old_camera_y;
	short softx;
	short softy;
	short Old_softy;
	short plane_width;
	short x_off;
	short y_off;
	/* Variables needed for Animation */
	short Animation;				// Set to TRUE to animate the Audience
	unsigned short Plane;		// Two Planes of animation alternating
	short Anim_Delay;				// When to alternate Planes
	short Delay_Max;				// How long for the Delay to wait inbetween switches
	/* 'EarthQuake' effect variables */
	short EarthQuake;
	short E_i;
	short E_add;
	short E_double;
	/* Tournament variables */
	short Progress;
	short x;
	short Parallax;
	short *T_Parallax;
	/* Miscellaneous */
	short Link;							// Information on Linkplay (TRUE/FALSE)
	short Calc;							// Who this calculator is currently being controlled by (Host/Join/AI)
	unsigned short keys;		// First 12 bits hold information about keypresses received from KeyScan()
	unsigned char (*buf)[192];
	char *p_maskval;				// Holds (char*)Tiles[0] which is referenced often by GraySpriteX8_BLIT
	/* Variables used by the Text Box */
	unsigned char *text;		// Pointer to allocated memory, holding the text string
	unsigned char *box0;		// Pointer to allocated memory, holding the contents of the light plane text box
	unsigned char *box1;		// Pointer to allocated memory, holding the contents of the dark plane text box
	short text_delay;				// Used to synchronize the text output
	short text_pause;				// Used to pause the text output
	short text_i;						// Index for current position of character being displayed
	short text_x;						// Current horizontal coordinate of character being displayed
	short text_y;						// Current vertical coordinate of character being displayed
	char Sumo_Name[9];			// Holds Name of external Wrestler currently being edited
	unsigned char Remaining_Pts;// Holds the number of remaining points in the Editor for a Loaded wrestler
	/* User-defined Configuration settings */
	char Game_Speed;				// Defines the amount to slow the game each frame (0-15)
	char Time_Limit;				// Match Time Limit (optional)
	char Smack_Talk;				// PreMatch Smack Talk (On/Off)
	char Difficulty;				// AI Difficulty Level
	char Distortion;				// Wavy screen effect
	HSC high[5];						// Game High Scores
} GlobalVars;  // 'g' stored globally in "%a4" (!huge size optimization!)

#ifdef NICHTS
/* -------------------- */
/* Sumo Wrestling Notes */
/* -------------------- */

List of all Sumo Ranked wrestlers (Banzuke)
http://sumo.goo.ne.jp/eng/hon_basho/banzuke/ban_1_1.html

sprite setup
char g1[x] = {0, 5}, g2[y] = {3, 4, 8};

const char *gfx[] = {
	g1,  // Left
	NULL,// Space for mirrored g1
	g2,  // Left
	NULL,// Space for mirrored g2
};
malloc(entire memory for sideways sprites)
copy g1 mirror into bulk
gfx[1] = (Bulk + 0);
copy g2 mirror into bulk + sizeof(g1);
gfx[3] = (char*)(Bulk + sizeof(g1));
etc...
Must have only left or right sprites, then, at game startup, calculate other 
direction sprites by mirroring them with SpriteX8_MIRROR_H_R().
// Drawing Wrestlers
if (sprite_array[index].width <= 4) // 32
	GraySprite32_OR_R();
else GraySpriteX8_OR();



two planes interchangable for different suits

2nd = A = Key 'Z'
Shift = B = Key 'X'


// Behavior Notes
When pressing walking Left or right already and attempting to press Shift to 
charge, will not work.  Must be pressing charge before starting to walk towards
the enemy.

Both the toss up and the throw sideways finishes use the 4 rotating off-balance 
gfx.  With sideways finisher throw, rotating 4 until guy reaches end of ring, 
then go to 2-anim flipping left/right graphic to go up, then down and eventually
lay there with one of the two upright off-balance gfx.  Sometimes, when being 
tossed sideways, go to belly-up graphic with gritting teeth (not the old one), 
and get tossed up just a bit, then land on your back all with the same graphic.

Pressing Down, !then! 2nd -> hands go up crossed, then down like before starting 
poses.  If pressing 2nd, then down, will not work.  Taunt oder etwas?

Before match, when both wrestlers are preparing, powerbar gradually fills up to 
its current max.  That max gets larger as you progress and also during 
individual matches, when you are doing good/knocking out the other guy.

Order:  Preparing (throwing salt/stomping/Clapping) -> Both sitting in the middle 
in beginning clapping stance -> both in Football position (not angry) -> both 
wrestlers become angry as they frown -> small wait and then start to wrestle.

Standing, to punch press 2nd to punch.  Switches between punching with left and 
right hands.  After punching (if did not hit anything), go to standing still 
stance with near foot forwards.  If you both smack each other at the same time, 
fall back a few feet and both spin (same if both charge each other and hit).

Pummeling position, pressing into direction of opponent and then 2nd, will push 
opponent in that direction.  Pull up on his belt-thingy and then hop that way.  
Does not work if pressing 2nd and then a direction.

Pressing Up, then 2nd -> Pick opponent up and his legs dangle (switch between 
two gfx).

In Pummel, pressing Left, then Shift -> Shove (sortof a punch) opponent left.  
He falls back a bit, then gets into regular stance.  If opponent is on edge of 
ring, he will teeter on edge, waving arms.

Pressing opposite direction of opponent, then 2nd, to do a throw.  If opponent 
is strong, PummelThrowCounter will occur and wrestlers will switch sides.

---

For Each Sumo Wrestler the user can choose from (and also for user-created wrestlers), 
have a couple of sayings for trash-talking before matches when the pre-match splash screen 
is up.  Also keep a cumulative record for each wrestler (0-0)

For a big sumo, you have to an equally big brain to play Super Sumo Wrestling since a lot of strategy is 
involved. Before entering each tournament, you have to weigh around your attributes. Rage is great for 
building up your offensive power, but strength helps solidify that power. You can also customize a sumo 
of great size, but you can lose valuable speed and momentum if size is all you think about.

Each individual match also offers some level of strategy. Depending on the statistics of your opponent, 
it would do you well to either move around or go straight for the kill. When battles start to take a long 
time, the meter fills and you never know who is going to come in for the final attack. Super Sumo 
Wrestling really shines in the sense that the gameplay is a lot deeper than you first see it

Have some sort of 3-wrestler vs 3-wrestlers Battle, where when one loses, another comes in from that 
team until all wrestlers on one side are out

Maybe add wavy effect to VS sprite (and 8 pixels on each side, then put wrestler sprites on)

"easy pick-up-and-play style"
#endif

#endif