// C Source File
// Created 9/7/2003; 12:10:41 PM
#include "All.h"
/* ------------------------------------------------------------------------- */
/* Defining this, will show the fps after each stoppage in play						   */
//#define BENCHMARK
/* ------------------------------------------------------------------------- */
/* Inlining Functions which are called each Frame will produce a slight speed increase */
static inline void Draw_HUD(void);
static inline void CopyScreens89(void);

/* The Main Game Loop, updates Player and Camera positions and runs the game */
void MAIN_LOOP(void) {
	short Starting = TRUE, Delay;
	#ifdef BENCHMARK
	unsigned long cycles = 0;
	#endif
	
	do /* Loop while the Game's Status is Normal
				(No Goals scored, Exiting the Game, or 
				Pausing of the Game) */
	{
		g->Oldx = g->x;
		g->Oldy = g->y;
		DrawMap();  // Draw the Rink
		if (g->Distortion != FALSE) {
			if (g->Distortion++ > 15)
				g->Distortion = 1;
			Distort_ScreenSafe(g->PlaneHeight, g->Distortion - 1, 30, Hiddenl, Hiddenl);  // Distortion :)
			Distort_ScreenSafe(g->PlaneHeight, g->Distortion - 1, 30, Hiddend, Hiddend);
		}
		
		KeyScan();
		Update_Plrs();
		Update_Puck();
		if (_KeyCheck(ESCKEY))  // Quit :(
			g->Status = QUIT;
		if (_custom_keytest(PAUSE))  // Pause the Game
			g->Status = Game_Paused;
		if (_KeyCheck(TEACHERKEY))  // Fast Quit
			g->Status = Fast_Quit;
		#if !defined(DEBUG)
		// Check if the Game Time has exceed the Game's Time Limit
		if (clock->Minutes >= g->Time_Limit && g->Mode != PRACTICE)
			g->Status = Game_Finished;
		#endif
		if (g->Link) {  // Take care of Multiplayer Games
			Synchronize_Calcs();
			if (g->Status == Normal)
				Transfer_Data();
		}
		Draw_HUD();
		
		Center_Camera();  // Make the 'Camera' follow the puck, for a scrolling Rink
		// If this is the first Frame of the game, dissolve in the Hidden Gray Planes
		if (Starting == TRUE) {
			Dissolve();
			clock->Running = 1;
			Starting = FALSE;
		} else {
			Toggle_Screens_Fast();
		}
		
		for(Delay = 15 - g->Speed; Delay--;) {
			pokeIO(0x600005,0b11101); /* low power mode, wake up CPU only on AI 1 
	                                 (grayscale), 3 (AMS clock), 4 (Link), 5 
	                                 (AMS timer base, needed for APD) and 6 (ON, 
	                                 always wakes up the CPU) 
	                                 -- Saves Battery Power -- */
    }
		#ifdef BENCHMARK
		cycles++;
		#endif
	} while (g->Status == Normal);
	clock->Running = 0;  // Stop the clock when the Game is not running
	
	/*if (g->Mode == TOURNAMENT && g->Status == QUIT && 
	Menu("save progress", (char*)((menu[]){ {"yes"}, {"no"} }), 2, 0, TRUE) == 0)
		Save_Tournament();*/
	
	if (g->Status == Fast_Quit)
		LeadOut();
	//  For benchmarking the fps
	#ifdef BENCHMARK
	char buffer[50];
	sprintf(buffer, "%lu cycles", cycles);
	ClearGrayScreen2B_R(Hiddenl, Hiddend);
	FS_DrawGrayString(0, 0, buffer, A_NORMAL, F_6x8);
	sprintf(buffer, "%g", (float)cycles/(float)(60*(float)clock->Minutes + 
	(float)clock->Seconds + ((float)19/(float)100) * (float)clock->Mseconds50));
	FS_DrawGrayString(0, 10, buffer, A_NORMAL, F_6x8);
	if (clock->Seconds > 9)
		sprintf(buffer, "Time:  %d:%d:%d", clock->Minutes, clock->Seconds, clock->Mseconds50);
	else sprintf(buffer, "Time:  %d:0%d:%d", clock->Minutes, clock->Seconds, clock->Mseconds50);
	FS_DrawGrayString(TI89?54:86, LCD_HEIGHT - 5, buffer, A_NORMAL, F_4x6);
	Toggle_Screens();
	WaitKey();
	#endif
}

/* ---------------- 
   By Malcolm Smith
   ---------------- */
static inline void CopyScreens89(void) {
	char *src0 = Hiddenl, *dest0 = Activel;
	char *src1 = Hiddend, *dest1 = Actived;
	asm volatile(
	"movem.l %%d0-%%d5,-(%%sp);"\
	"moveq #99,%%d0;"\
	"0:;"\
	"movem.l (%0)+,%%d1-%%d5;"\
	"movem.l %%d1-%%d5,(%1);"\
	"movem.l (%2)+,%%d1-%%d5;"\
	"movem.l %%d1-%%d5,(%3);"\
	"lea (%1,30),%1;"\
	"adda.w #10,%0;"\
	"lea (%3,30),%3;"\
	"adda.w #10,%2;"\
	"dbra %%d0,0b;"\
	"movem.l  (%%sp)+,%%d0-%%d5"
	: "=a" (src0),"=a" (dest0),"=a" (src1),"=a" (dest1)
	: "0" (src0),"1" (dest0),"2" (src1),"3" (dest1));
}

// By me, based off the CopyScreens89 by Malcolm Smith :)
// My first attempt at writing anything in ASM, ever!!!
/*static inline void CopyScreens92Plus(void) {
	char *src0 = Hiddenl, *dest0 = Activel;
	char *src1 = Hiddend, *dest1 = Actived;
	asm volatile("
	movem.l %%d0-%%d7,-(%%sp);
	moveq #127,%%d0;
	0:;
	movem.l (%0)+,%%d1-%%d7;  | Light
	movem.l %%d1-%%d7,(%1);
	movem.l (%2)+,%%d1-%%d7;  | Dark
	movem.l %%d1-%%d7,(%3);
	lea (%1,28),%1;
	lea (%3,28),%3;
	move.w (%0)+,%%d1;        | Extra 2 Bytes on the Right of the Screen
	move.w %%d1,(%1)+;
	move.w (%2)+,%%d1;
	move.w %%d1,(%3)+;
	dbra %%d0,0b;             | Loop until we do all 128 Lines
	movem.l  (%%sp)+,%%d0-%%d7"
	: "=a" (src0),"=a" (dest0),"=a" (src1),"=a" (dest1)
	: "0" (src0),"1" (dest0),"2" (src1),"3" (dest1));
}*/

// Runs the Outer 'Skeleton' of the ShootOut, inbetween Shots
short Outer_ShootOut(void) {
	char buffer[30], Sudden_Death = FALSE, Receive = 0, Signal = FALSE;
	short Turn = 0, team, Score_Dif;
	
	Set_Default_Game();
	while(Team1->score == Team2->score) { // For Sudden Death Situations
		for(Turn = 1; Turn < 6; Turn++) {
			for(team = 1; team < 3; team++) {  // Alternate between Team1 and Team2
				clock->Reset_Time = 1;
				Start_Over_At_Center();
				
				// Show a little Status screen inbetween shots
				Fill_Outside();
				SlowerSpriteX8_OR_R(Zerox, Zeroy, 100, NHLd, 20, Hiddenl);
				SpriteString(0, Zeroy, (char*)"- shootout -", Hiddenl, Hiddend, A_CENTERED);
				sprintf(buffer, "Turn %d", Turn + 5 * Sudden_Death);
				SpriteString(0, Zeroy + 13, buffer, Hiddenl, Hiddend, A_CENTERED);
				sprintf(buffer, "%d -- %d", Team1->score, Team2->score);
				SpriteString(0, Zeroy + 25, buffer, Hiddenl, Hiddend, A_CENTERED);
				// Difference in Timer speed, at least on VTI which is a HW1
				sprintf(buffer, "%d seconds to score", 4 + (g->HW1 != FALSE));
				SpriteString(0, Zeroy + 35, buffer, Hiddenl, Hiddend, A_CENTERED);
				if (Sudden_Death == TRUE)
					SpriteString(0, Zeroy + 45, (char*)"sudden death", Hiddenl, Hiddend, A_CENTERED);
				SpriteString(0, Zeroy + 92, (char*)"get ready", Hiddenl, Hiddend, A_CENTERED);
				
				Dissolve();
				/* -------------------------------------------------------------------------------- */
				/* Wait for a keypress and check for a Quit signal from the Other g->Calc in Link Mode */
				/* -------------------------------------------------------------------------------- */
				while(_rowread(0)) continue;
				while(!_rowread(0)) {
					// Read the Link buffer and check if the other calc has exited/entered the ShootOut
					if (g->Link && OSReadLinkBlock(&Receive, 1) == 1) {
						if (Receive == QUIT) {
							Message((const char *const[]){"other calc has", "exited the game", NULL}, 1);
							return QUIT;
						}
						if (g->Calc == Join && Receive == HOST_NO)
							Signal = TRUE;
					}
				}
				while(_rowread(0)) {
					KeyScan();
					
					if (_KeyCheck(ESCKEY | TEACHERKEY)) {
						Exit_Linkplay();
						return QUIT;
					}
				}
				if (g->Link) {
					if (Signal == TRUE) {  // Other g->Calc is already Ready and Waiting
						Signal = JOIN_NO;
						OSWriteLinkBlock(&Signal, 1);
					} else if (Align_Calcs() == QUIT)  // Wait for the Other g->Calc to continue the ShootOut
						return QUIT;
				}
				// Call the Main ShootOut function where a Player has 5 Seconds to score 1 vs 1 against the 
				// Other Team's Goalie
				if (Run_ShootOut(team) == QUIT)
					return QUIT;
				
				if (g->Status == QUIT)
					return QUIT;
				
				// Increment the Correct Team's score after a goal
				if (g->Status == Level_Scored && g->x < 200)
					Team2->score++;
				else if (g->Status == Level_Scored)
					Team1->score++;
			}
			// Other Team has no Hope of Winning the ShootOut
			Score_Dif = ABS(Team1->score - Team2->score);
			if (Score_Dif > 5 - Turn || (Sudden_Death && Team1->score != Team2->score))
				break;
		}
		Sudden_Death++;
	}
	g->Status = Game_Finished;
	
	// Show a Game Summary screen at the end of the ShootOut
	if (g->Mode == SHOOTOUT)
		Post_Game();
	return 0;
}

// Runs the 5 second, 1 on 1 ShootOut
short Run_ShootOut(short Team_Shooting) {
	short Plr1 = (Team_Shooting == 1);
	short Plr2 = (Team_Shooting == 2);
	short Delay;
	
	/* Start Shooter out with puck */
	g->Is_Controlled = 1;
	if (Team_Shooting == 1) {
		Team1->plrs[Plr1]->x = 217;
		Team1->plrs[Plr1]->y = 98 - ((Team1->plrs[Plr1]->type == Skinny) * 2);
		New_Control(Plr1);
	} else {
		Team2->plrs[Plr2]->x = 229;
		Team2->plrs[Plr2]->y = 98 - ((Team2->plrs[Plr2]->type == Skinny) * 2);
		New_Control(Plr2 + 3);
	}
	// Which Player will the User control?
	if (Host)
		g->Cur_Plr = Plr1;
	else if (Join)
		g->Cur_Plr = Plr2;
	
	clock->Running = 1;  // Start the Clock
	do 
	{
		g->Oldx = g->x;
		g->Oldy = g->y;
		DrawMap();  // Draw the Rink
		if (g->Distortion != FALSE) {
			if (g->Distortion++ > 16)
				g->Distortion = 1;
			Distort_ScreenSafe(g->PlaneHeight, g->Distortion - 1, 30, Hiddenl, Hiddenl);  // Distortion :)
			Distort_ScreenSafe(g->PlaneHeight, g->Distortion - 1, 30, Hiddend, Hiddend);
		}
		
		/* Update two players during the ShootOut */
		if (BOUNDS_COLLIDE(g->x - 2, g->y - 2, Team1->plrs[Plr1]->x + 12*(Team1->plrs[Plr1]->face), 
		Team1->plrs[Plr1]->y + 11, 8, 7) && Team1->plrs[Plr1]->Control == FALSE)
			New_Control(Plr1);
		if (BOUNDS_COLLIDE(g->x - 2, g->y - 2, Team2->plrs[Plr2]->x + 12*(Team2->plrs[Plr2]->face), 
		Team2->plrs[Plr2]->y + 11, 8, 7) && Team2->plrs[Plr2]->Control == FALSE)
			New_Control(Plr2 + 3);
		
		KeyScan();
		if (g->Calc == Host) {
			Move_Cur_Plr(Team1->plrs[Plr1]);
			
			if (g->Link == FALSE)
				Move_Computer_Plr(Team2->plrs[Plr2]);  // Plr vs g->Calc
		} else if (g->Calc == Join) {
			Move_Cur_Plr(Team2->plrs[Plr2]);
		} else {  // Computer vs Computer
			Move_Computer_Plr(Team1->plrs[Plr1]);
			Move_Computer_Plr(Team2->plrs[Plr2]);
		}
		// Draw the two Players
		Draw_Plr(Team1->plrs[Plr1]);
		Draw_Plr(Team2->plrs[Plr2]);
		
		Update_Puck();
		
		if (_KeyCheck(ESCKEY))
			g->Status = QUIT;
		if (_KeyCheck(TEACHERKEY))
			g->Status = Fast_Quit;
		// Check if the ShootOut has Expired
		if (clock->Seconds > 4 + (g->HW1 != FALSE) && g->Status == Normal)
			g->Status = Game_Finished;
		
		Draw_HUD();
		if (g->Link) {
			Synchronize_Calcs();
			if (g->Status == Normal)
				Transfer_Data();
		}
		Center_Camera();
		Toggle_Screens_Fast();
		
		for(Delay = 15 - g->Speed; Delay--;) {
			pokeIO(0x600005,0b11101); /* low power mode, wake up CPU only on AI 1 
	                                 (grayscale), 2 (keypress), 3 (AMS clock), 5 
	                                 (AMS timer base, needed for APD) and 6 (ON, 
	                                 always wakes up the CPU) */
    }
		
	} while (g->Status == Normal);
	clock->Running = 0;  // Stop the Clock
	clock->Reset_Time = 1;
	if (g->Status == Fast_Quit)
		LeadOut();
	
	return 0;
}

// Sets up a Game and Exchanges Team Information in Multiplayer Mode
void Set_Default_Game(void) {
	Start_Over_At_Center();  // Center camera/puck and initiate plrs on both teams
	short Link_Info[2];
	clock->Reset_Time = 1;
	Team1->score      = 0;  // Reset both team's scores
	Team2->score      = 0;
	
	if (g->Link) {
		if (g->Calc == Host) {
			Link_Info[0] = Team1->No;
			Link_Info[1] = Team1->Conference;
			LIO_SendData((char*)Link_Info, sizeof(Link_Info));
			if (LIO_RecvData((char*)Link_Info, sizeof(Link_Info), 40))
				ERROR(LINK_ERROR);
			Team2->No = Link_Info[0];
			Team2->Conference = Link_Info[1];
		} else if (g->Calc == Join) {
			if (LIO_RecvData((char*)Link_Info, sizeof(Link_Info), 40))
				ERROR(LINK_ERROR);
			Team1->No = Link_Info[0];
			Team1->Conference = Link_Info[1];
			Link_Info[0] = Team2->No;
			Link_Info[1] = Team2->Conference;
			LIO_SendData((char*)Link_Info, sizeof(Link_Info));
		}
	}
}

/* Resets the Camera and player's positions */
void Start_Over_At_Center(void) {
	short Starting_Puck[7]  = { 0, 229, 110, 0, 0, 231, 127 };
	char Scores[2]          = { Team1->score, Team2->score };
	short a;
	
	memcpy(&g->Is_Controlled, &Starting_Puck, sizeof(Starting_Puck));
	register const short ti89 = TI89;
	g->camera_x = 7 + ti89*2;
	g->camera_y = 3 + ti89*2;
	g->softx    = 7*ti89;
	g->softy    = (ti89 ? -9 : 4);
	//memcpy(&g->camera_x, (TI89 ? (short[]) { 9, 5, 7, -9 } : (short[]) { 7, 3, 0, 4 }), sizeof(short) * 4);
	
	clock->Running   = 0;
	g->Control_Delay = 0;
	g->Shooting  = FALSE;
	g->Status    = Normal;
	g->Alert     = FALSE;
	g->Changing  = FALSE;
	
	if (g->Link) {
		if (g->Calc == Host) {
			for(a = 2; a--;);
			LIO_SendData(Scores, sizeof(Scores));
		} else if (g->Calc == Join) {
			if (LIO_RecvData(Scores, sizeof(Scores), 30))
				ERROR(LINK_ERROR);
		}
		Team1->score = Scores[0];
		Team2->score = Scores[1];
	}
	
	// Loop through the Three Players on both teams and initialize them
	for(a = 3; a--;) {
		memset(Team1->plrs[a], 0, sizeof(Plr));
			if (a == 0)
				Team1->plrs[a]->type = Goalie;
			else {
				if (Team1->Conference == WESTERN)
					Team1->plrs[a]->type = Western[Team1->No].Types[a - 1];
				else Team1->plrs[a]->type = Eastern[Team1->No].Types[a - 1];
			}
			if (Team1->Conference == WESTERN) {
				Team1->plrs[a]->x = (Starting_Pos[3*Western[Team1->No].Formation + a][0]<<4);
				Team1->plrs[a]->y = (Starting_Pos[3*Western[Team1->No].Formation + a][1]<<4);
			} else {
				Team1->plrs[a]->x = (Starting_Pos[3*Eastern[Team1->No].Formation + a][0]<<4);
				Team1->plrs[a]->y = (Starting_Pos[3*Eastern[Team1->No].Formation + a][1]<<4);
			}
			Team1->plrs[a]->face = 1;
			Team1->plrs[a]->team = 1;
			Team1->plrs[a]->No   = a;
			Team1->plrs[a]->anim = Team1->plrs[a]->type == Goalie ? 0 : random(2);
			Team1->plrs[a]->anim_delay = random(3);
			memset(Team2->plrs[a], 0, sizeof(Plr));
			if (a == 0)
				Team2->plrs[a]->type = Goalie;
			else {
				if (Team2->Conference == WESTERN)
					Team2->plrs[a]->type = Western[Team2->No].Types[a - 1];
				else Team2->plrs[a]->type = Eastern[Team2->No].Types[a - 1];
			}
			if (Team2->Conference == WESTERN) {
				Team2->plrs[a]->x = (Width<<4) - (Starting_Pos[3*Western[Team2->No].Formation + a][0]<<4) - 16;
				Team2->plrs[a]->y = (Starting_Pos[3*Western[Team2->No].Formation + a][1]<<4);
			} else {
				Team2->plrs[a]->x = (Width<<4) - (Starting_Pos[3*Eastern[Team2->No].Formation + a][0]<<4) - 16;
				Team2->plrs[a]->y = (Starting_Pos[3*Eastern[Team2->No].Formation + a][1]<<4);
			}
			Team2->plrs[a]->face = 0;
			Team2->plrs[a]->team = 2;
			Team2->plrs[a]->No   = a;
			Team2->plrs[a]->anim = Team1->plrs[a]->type == Goalie ? 0 : random(2);
			Team2->plrs[a]->anim_delay = random(3);
	}
	
	g->Last_Controlled = 0;
	g->Cur_Plr = 1;
}

/* Print all of the Information in the HUD each Frame */
static inline void Draw_HUD(void) {
	char buffer[40];
	
	#ifdef DEBUG
	sprintf(buffer, "%d %d %d %d", g->camera_x, g->softx, g->camera_y, g->softy);
	//Team1->plrs[g->Cur_Plr]->x, Team1->plrs[g->Cur_Plr]->y, g->x, g->y);
	FS_DrawGrayString(0, LCD_HEIGHT - 5, buffer, A_NORMAL, F_4x6);
	#else
	// Print both Team Names and their scores
	sprintf(buffer, "%d %s", Team1->score, 
		Team1->Conference ? Eastern[Team1->No].TeamName : Western[Team1->No].TeamName);
	FS_DrawGrayString(0, LCD_HEIGHT - 5, buffer, A_NORMAL, F_4x6);
	if (g->Mode != PRACTICE) {  // There is no Second Team in PRACTICE Mode
		sprintf(buffer, "%s %d", 
			Team2->Conference ? Eastern[Team2->No].TeamName : Western[Team2->No].TeamName, Team2->score);
		FS_DrawGrayString(LCD_WIDTH-DrawStrWidth(buffer,F_4x6)-1,LCD_HEIGHT - 5,buffer,A_NORMAL,F_4x6);
	}
	#endif
	
	// Take care of Printing the Correct Time
	sprintf(buffer, "Time:  %02d:%02d", clock->Minutes, clock->Seconds);
	FS_DrawGrayString(TI89?58:90, LCD_HEIGHT - 5, buffer, A_NORMAL, F_4x6);
}