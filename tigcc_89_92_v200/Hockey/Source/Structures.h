// C Header File
// Created 9/14/2003; 9:24:11 AM
#ifndef STRUCTURES_H
#define STRUCTURES_H
#define Max_Length						20

typedef struct {
	char type;			// Goalie, Skinny, Average, or Fat
	char Control;		// Does the Plr control the puck?
	short x;				// Coordinates of Plr
	short y;
	char face;			// Which Direction the Plr is facing (Left or Right)
	char addx;			// Direction Plr is moving
	char addy;
	char anim;			// For animating the Plr
	char anim_delay;
	char Shooting;	// For animating the Plr when shooting
	char team;			// Team 1 or 2
	unsigned char No;				// 0, 1, or 2 (3 Plrs on a team)
	short delay;		// For friction with the ice
} Plr;

// Team1 and Team2
typedef struct {
	Plr *plrs[3];
	short score;
	short No;
	short Conference;
} Team;

typedef struct {  //RowKey
  short Row;
  short Key;
} RowKey;

typedef struct {
	char Link;
	char Calc;
	short Cur_Plr;
	short HW1;
	short Control_Delay;
// Game Settings
	char Status;
	char Mode;
	short Alert;
	short Shooting;
	short Last_Controlled;
	short Changing;
	unsigned char No_Teams[2];
	char Teams_Changed;
	char Friction;
	char Distortion;
// Puck
	short Is_Controlled;
	short x;
	short y;
	short addx;
	short addy;
	short Oldx;
	short Oldy;
// Scrolling Variables
	short camera_x;
	short camera_y;
	short softx;
	short softy;
// Used for calc-calc Compatability
	short SPRWIDTH;
	short SPRHEIGHT;
	short PlaneHeight;
	short Camerax_1;
	short Camerax_2;
	short Camerax_3;
	short Camerax_4;
	short Cameray_1;
	short Cameray_2;
// Configuration settings
	short keys;
	RowKey Keys[8];
	char Difficulty;
	char Time_Limit;
	char Speed;
} Game;

typedef void (*P2Func)(void);

typedef struct {
	char Item[Max_Length];
} menu;

typedef struct {
	volatile short Mseconds50;
	volatile short Seconds;
	volatile short Minutes;
	volatile short Reset_Time;
	volatile short Running;
	short Time_Limit;
} Clock;

// Each team in the Western and Eastern conference is a Names struct
typedef struct {
	char TeamName[13];
	char PlrNames[3][23];
	char Formation;
	char Types[2];
} Names;

#endif