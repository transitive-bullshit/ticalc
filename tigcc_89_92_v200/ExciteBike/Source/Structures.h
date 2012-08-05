// Header File
// Created 12/14/2003; 12:03:47 PM
#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#define MAX_BIKES			4

typedef struct {
	unsigned char *PtrDark;
	unsigned char *PtrLight;
	unsigned char Height;
} Tile;

/* -- MACROS -- */
/* To conserve space and retain simplicity, most of a Bike's behavioral attributes are stored */
/* in individual bits of one unsigned short attr.  These Macros make accessing them easy and  */
/* also make the source much more readable -- For You :)																			*/
#define Bike_y(b)				((b)->attr & B_Y)  // Gives 0, 1, 2, or 3
#define RealBike_y(b) 	(((b)->attr & B_Y) + (((b)->attr & B_TURNING) != 0) * (((b)->Counter < -5) - \
((b)->Counter > 5)))
#define Laps_Done(b)		(((b)->attr & B_LAPS) >> 8)
#define No_Laps					g->Track[g->Width + 1]
#define No_Tilt(b)			((b)->Tilt == None)
#define Is_Computer(c)	((c) >= AI_SLOW)
#define TEXT_DELAY			(~(1 << 15))
#define TEXT_ANIM				(1 << 15)

/* - - - - - - - - - - - - - - - - */
/* Bike Attributes                 */
/* - - - - - - - - - - - - - - - - */
#define B_Y						3							// Bits 1 and 2
#define B_JUMPING			(1<<2)				// Bit 3
#define B_CRASHING		(1<<3)				// Bit 4
#define B_OVERHEAT		(1<<4)				// Bit 5
#define B_TURBO				(1<<5)				// Bit 6
#define B_TURNING			(1<<6)				// Bit 7

#define B_MUD					(1<<7)				// Bit 8
#define B_LAPS				(15<<8)				// Bits 9, 10, 11, and 12 (max value of 15)
#define B_BLOCK				(1<<12)				// Bit 13
#define B_FLAG1				(1<<13)				// Bit 14 (used in Crashing)
#define B_FLAG2				(1<<14)				// Bit 15 (used in Crashing)
#define B_FLAG3				(1<<15)				// Bit 16 (used in Crashing)
/* - - - - - - - - - - - - - - - - */

// I wanted to keep the Bike structure as small as possible because they need to be sent and 
// received each frame in LinkPlay, but it was easier just to send the first 7 bytes, 
// which is all of the relevant information
typedef struct {
	/* These First 4 variables will be transferred each Frame in LinkPlay (7 bytes) */
	/* They MUST stay as the first 4 variables */
	// / // / // / // / // / // / //
	short x;
	short y;
	unsigned short attr;
	unsigned char Graphic;				// The current Graphic for the Bike
	// / // / // / // / // / // / //
	unsigned char Control;				// Host, Join, Computer
	short Counter;
	unsigned char Tilt;						// The Bike's Tilt
	unsigned char Temp;
	unsigned char addx;
	unsigned char Old_Graphic;
	char delay;
	unsigned char Bike_NO;
} Bike;  // The structure should be padded correctly (it's size is 16 bytes)

typedef struct {
	short y;
	short Original_x;
	long cosine_Velocity; // cos(angle) * Initial_Velocity
	long sine_Velocity;  // sin(angle) * Initial_Velocity
	short Original_y;
	short Last_Crash;
} JumpDat;

typedef struct {  //RowKey
  short Row;
  short Key;
} RowKey;

// Not included with the rest of the Global register variables because they are accessed in an 
// interrupt timer (could provide for whacky results if the cpu was using a4 at the time)
typedef struct {
	// Clock Variables
	short Mseconds50;
	short Seconds;
	short Minutes;
	short Reset_Time;
	short Running;
} Clock;

typedef struct {
	// Camera/Track Values
	short camera_x;
	short softx;
	short Width;
	short Old_x;
	short softy;  // Cisco ist hübsch!
	unsigned char *Track;
	// Current Game Values
	short No_Bikes;
	Bike bikes[MAX_BIKES];
	JumpDat jumpdat[MAX_BIKES];
	char Status;
	unsigned char delay;
	unsigned char Animation;
	char HW1;
	short Game_Progress;
	short Camera_Bike;
	short Zerox;
	// Configuration Settings
	RowKey Keys[10];
	// Miscellaneous
	unsigned char Cheats;
	unsigned char Back0;
	unsigned char Back1;
	char First;
	char Link;
	char Calc;
	unsigned char lcd_height;
	unsigned char Max_xspeed;
	short OldMseconds50;
	short OldSeconds;
	short OldMinutes;
	unsigned short text_attr;
	char LinkplayOffset;
	char Old_Status;
	unsigned char text[32];  // For text messages during the Race
	unsigned char Track_Name[9];
} GlobalVars;

/* - - - - - - - - - - - - - - - - */
/* Cheat Attributes                */
/* - - - - - - - - - - - - - - - - */
#define C_OVERHEAT		1							// Bit 1
#define C_RECOVER			(1<<1)				// Bit 2
#define C_HALF				(1<<2)				// Bit 3
#define C_QUARTER			(1<<3)				// Bit 4
#define C_MOON				(1<<4)				// Bit 5
#define C_FASTBIKES		(1<<5)				// Bit 6
#define C_BACKGROUND	(1<<6)				// Bit 7
// Bit 8 is free... Have any ideas for another cheat?
/* - - - - - - - - - - - - - - - - */

#endif