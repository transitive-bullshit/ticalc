// C Source File
// Created 10/5/2003; 7:54:10 PM
#include "All.h"
/* 
	 All Game Setup Functions for each Mode of play
*/

/* One game, with a multiplayer option */
void Exhibition(void) {
	g->Mode = EXHIBITION;
	
	if (Choose_GamePlay() == QUIT) // Choose who will be playing
		return;
	// Check if this calc is the joining calc and the Host calc has changed the game Mode
	if (g->Mode == SHOOTOUT) {
		Outer_ShootOut();
		return;
	}
	Outer_Loop();  // Start the Game
}

/* Called by Exhibition() and ShootOut(), preps the game by asking the user which of three 
   Game Modes he/she would like to play */
short Choose_GamePlay(void) {
	static menu Game_Play[] = { {"plr vs calc"}, {"calc vs calc"}, {"two player"}, {"- back -"} };
	short Choice = 0, Redo;
	
	do 
	{
		Redo = FALSE;
		Choice = Menu(g->Mode == EXHIBITION ? "- exhibition -" : "- shootout -", 
								 (char*)Game_Play, 4, Choice, TRUE);
		
		switch(Choice) {
			case 0:
				/* Host     vs Computer */
				g->Calc = Host;
				g->Link = FALSE;
				if (Choose_Team(Team1, 0) == QUIT)
					Redo = TRUE;
				Choose_Random_Team(Team2);
				break;
			case 1:
				/* Computer vs Computer */
				g->Calc = Computer;
				g->Link = FALSE;
				Choose_Random_Team(Team1);
				Choose_Random_Team(Team2);
				break;
			case 2:
				/* Host     vs Join */
				g->Link = TRUE;
				if (Multiplayer() == QUIT) {
					Redo = TRUE;
				}
				break;
			default:
				return QUIT;
				break;
		}
	} while (Redo == TRUE);
	
	return Choice;
}

void Tournament(void) {
	g->Mode = TOURNAMENT;
	short a, b, Redo, Round = 0, Old_Difficulty = g->Difficulty;
	unsigned char Teams[8][3];
	
	/* Host vs Computer */
	g->Calc = Host;
	g->Link = FALSE;
	if (Choose_Team(Team1, 0) == QUIT)
		return;
	Teams[0][0] = Team1->Conference;
	Teams[0][1] = Team1->No;
	Teams[0][2] = TRUE;
	for(a = 1; a < 8; a++) {  // Choose random teams for the Tournament
		Teams[a][2] = Round;
		/* We do not want two of the same team in the tournamt */
		do 
		{
			Redo = FALSE;
			Teams[a][0] = random(2);  // Random West or East Conference
			Teams[a][1] = random(15); // Random Team Number
			for(b = a; b--;) {
				if (Teams[a][0] == Teams[b][0] && Teams[a][1] == Teams[b][1])
					Redo = TRUE;
			}
		} while (Redo == TRUE);
	}
	
	while(1) {
		if (Show_Tournament(Teams, Round) == QUIT)  // Show the Tournament Bracket
			return;
		// Find the User's current Opponent
		a = 0;
		while(Teams[++a][2] < Round);
		
		Team2->Conference = Teams[a][0];
		Team2->No         = Teams[a][1];
		
		Outer_Loop();  // Play the Game
		if (g->Status != Game_Finished) {  // Check if there was an error of the User quit the game
			g->Difficulty = Old_Difficulty;
			return;
		} else if (Team2->score > Team1->score) {  // The Plr has lost the game and the Tournament
			Message((const char *const[]){"you have lost", "the tournament", NULL}, 1);
			g->Difficulty = Old_Difficulty;
			return;
		} else if (++Round > 2) {  // The Plr has won three consecutive matches and the Tournament!
			Message((const char *const[]){"- winner -", NULL}, 1);
			g->Difficulty = Old_Difficulty;
			return;
		}
		
		// Eliminate Teams and Move the (random) Winners to the next round
		Teams[0][2] = Round;
		if (Round == 1) {
			Teams[2 + random(2)][2] = Round;
			Teams[4 + random(2)][2] = Round;
			Teams[7 - random(2)][2] = Round;
		} else if (Round == 2) {
			g->Difficulty = TRUE;  // Championship Game will always be Difficult
			if (random(2)) {
				if (Teams[4][2] == 1)
					Teams[4][2] = Round;
				else Teams[5][2] = Round;
			} else {
				if (Teams[6][2] == 1)
					Teams[6][2] = Round;
				else Teams[7][2] = Round;
			}
		}
	}
}

/* Get a feel for Hockey or make up Plays */
/* There is no Time Limit in Practice Mode */
void Practice(void) {
	g->Mode = PRACTICE;
	
	/* Host vs Noone */
	g->Calc       = Host;
	g->Link       = FALSE;
	if (Choose_Team(Team1, 0) == QUIT)
		return;
	
	Outer_Loop();  // Start the Practice
}

/* Five Rounds of alternating 1 on 1 Shots, culminating in a Sudden Death Finish */
/* Choose the Multiplayer Option to play against a Friend */
void ShootOut(void) {
	g->Mode = SHOOTOUT;
	
	if (Choose_GamePlay() == QUIT) // Choose who will be playing
		return;
	if (g->Mode == EXHIBITION) {  // Two Plr game in which the Other calc was in EXHIBITION Mode
		Outer_Loop();
		return;
	}
	
	Outer_ShootOut();  // Start the Shootout
}

/* Controls an individual Game's Progress */
void Outer_Loop(void) {
	Set_Default_Game();  // New game
	
	while(g->Status != Game_Finished) {  // Loop until the game is finished
		Start_Over_At_Center();  // Center camera/puck and initiate plrs on both teams
		
		do // Loop while the Game is Paused
		{
			g->Status = Normal;
			
			MAIN_LOOP();
			// Check if a team has Scored and increment the correct team's score
			if (g->Status == Level_Scored) {
				if (g->x < 200)
					Team2->score++;
				else Team1->score++;
			}
			
			if (g->Status == Game_Paused && Pause_Game() == QUIT)
				g->Status = QUIT;
		} while (g->Status == Game_Paused);
		
		// ESC quits.  Also, in a Multiplayer game, if one calc quits, 
		// a signal is sent and both calcs will quit
		if (g->Status == QUIT)
			return;
	}
	Post_Game();
}

/* Go into a ShootOut if necessary, and then show a Game Summary Screen */
void Post_Game(void) {
	char buffer[20];
	clock->Reset_Time = 1;
	
	while(1) {
		Fill_Outside();
		SlowerSpriteX8_OR_R(Zerox, Zeroy, 100, NHLd, 20, Hiddenl);
		// Print the Game Summary
		SpriteString(0, Zeroy, (char*)"- game over -", Hiddenl, Hiddend, A_CENTERED);
		sprintf(buffer, "%d -- %d", Team1->score, Team2->score);
		SpriteString(0, Zeroy + 13, buffer, Hiddenl, Hiddend, A_CENTERED);
		// Check if the Score is tied and if a ShootOut is necessary
		if (Team1->score == Team2->score && g->Mode != PRACTICE) {
			SpriteString(0, Zeroy + 25, (char*)"shootout", Hiddenl, Hiddend, A_CENTERED);
			Dissolve();
			WaitKey();
			if (Outer_ShootOut() == QUIT)
				return;
		} else break;  // No ShootOut is necessary
	}
	// Who won the game?
	if (Team1->score > Team2->score)
		sprintf(buffer,"%s won",Team1->Conference?Eastern[Team1->No].TeamName:Western[Team1->No].TeamName);
	else sprintf(buffer,"%s won",Team2->Conference?Eastern[Team2->No].TeamName:Western[Team2->No].TeamName);
	if (Team1->score != Team2->score)
		SpriteString(0, Zeroy + 25, buffer, Hiddenl, Hiddend, A_CENTERED);
	
	Dissolve();
	WaitKey();
}

/*

Allow the User to choose from 31 Official NHL Teams 
Each Team has:

1) A spot in either the Western or Eastern Conference
2) A Roster of Three Real Players on that actual team
3) A Goalie, and two other players with distinct Attributes
4) Their own Formation

*/
short Choose_Team(Team *team, const unsigned char Command) {
	static const char *const Commands[3] = { "Choose your team", "Edit which Team", "Delete which Team" };
	short Starting = TRUE, Conference = 0, Main_y1 = 0, Main_y2 = 0, y = 0;
	char Signal = FALSE, Receive = 0;
	register short a;
	
	short WidthWestern = 0, WidthEastern = 0;
	for(a = g->No_Teams[0]; a--;)
		if (DrawStrWidth(Western[a].TeamName, F_4x6) > WidthWestern)
			WidthWestern = DrawStrWidth(Western[a].TeamName, F_4x6);
	for(a = g->No_Teams[1]; a--;)
		if (DrawStrWidth(Eastern[a].TeamName, F_4x6) > WidthEastern)
			WidthEastern = DrawStrWidth(Eastern[a].TeamName, F_4x6);
	WidthEastern = 159 - WidthEastern;
	if (g->Link)
		OSLinkOpen();
	
	do 
	{
		
		Fill_Outside();
		SlowerSpriteX8_OR_R(Zerox, Zeroy, 100, NHLd, 20, Hiddenl);  // Draw the NHL 2004 Background
		// Mask the Western and Eastern Conference Logos over the top of the Background
		GraySpriteX8_MASK(Zerox, Zeroy, 38, Westernl, Westernd, 
		C_Mask, C_Mask, 6, Hiddenl, Hiddend);
		GraySpriteX8_MASK(LCD_WIDTH - 46 - Zerox, Zeroy, 38, Easternl, Easternd, 
		C_Mask, C_Mask, 6, Hiddenl, Hiddend);
		FS_DrawGrayString(0, Zeroy + 95, Commands[Command], A_NORMAL|A_CENTERED, F_4x6);
		
		// Check for a Quit signal from the Other g->Calc in Link Mode
		if (g->Link && OSReadLinkBlock(&Receive, 1) == 1) {
			if (Receive == QUIT) {
				Message((const char *const[]){"other calc has", "exited the game", NULL}, 1);
				return QUIT;
			}
			if (g->Calc == Join && Receive == HOST_NO)
				Signal = TRUE;
		}
		
		KeyScan();
		if (Conference == WESTERN) { 	// Western Conference
			if (_KeyCheck(UPKEY) && --y < 0) {
				y = 0;
				if (--Main_y1 < 0)
					Main_y1 = 0;
			}
			if (_KeyCheck(DOWNKEY) && ++y > 9) {
				y = 9;
				if (++Main_y1 > g->No_Teams[0] - 10)
					Main_y1 = g->No_Teams[0] - 10;
			}
		} else {           						// Eastern Conference
			if (_KeyCheck(UPKEY) && --y < 0) {
				y = 0;
				if (--Main_y2 < 0)
					Main_y2 = 0;
			}
			if (_KeyCheck(DOWNKEY) && ++y > 9) {
				y = 9;
				if (++Main_y2 > g->No_Teams[1] - 10)
					Main_y2 = g->No_Teams[1] - 10;
			}
		}
		
		// The Current Team's Conference
		if (_KeyCheck(LEFTKEY))
			Conference = 0;
		if (_KeyCheck(RIGHTKEY))
			Conference = 1;
		
		for(a = 10; a--;) {
			FS_DrawGrayString(Zerox, Zeroy + 40 + 6 * a, Western[Main_y1 + a].TeamName, A_NORMAL, F_4x6);
			FS_DrawGrayString(Zerox + 159 - DrawStrWidth(Eastern[Main_y2 + a].TeamName, F_4x6), 
			Zeroy + 40 + 6 * a, Eastern[Main_y2 + a].TeamName, A_NORMAL, F_4x6);
		}
		
		if (Conference == WESTERN) {
			Show_Team_Stats(Western[Main_y1 + y]);
			InvertGrayRect2B(Zerox, Zeroy + 39 + 6 * y, Zerox + WidthWestern, Zeroy + 45 + 6 * y, Hiddenl, Hiddend);
		} else {
			Show_Team_Stats(Eastern[Main_y2 + y]);
			InvertGrayRect2B(Zerox + WidthEastern, Zeroy + 39 + 6 * y, Zerox + 159, Zeroy + 45 + 6 * y, 
				Hiddenl, Hiddend);
		}
		
		if (Starting) {
			Dissolve();
			Starting = FALSE;
		} else {
			Toggle_Screens();
		}
		for(a = 15; a--;)
			pokeIO(0x600005,0b11101);
		while(!_rowread(0));
		KeyScan();
		
		if (_KeyCheck(ESCKEY | TEACHERKEY)) {
			// Notify Other calc that this one has exited from the game
			Exit_Linkplay();
			while_keypress(ESCKEY);
			return QUIT;
		}
	} while (!_KeyCheck(SHOTKEY | ENTERKEY));
	while(_rowread(0));
	
	// Copy the Team chosen by the User into the Team's Conference and No Data
	team->Conference = Conference;
	if (Conference == WESTERN)
		team->No = Main_y1 + y;
	else team->No = Main_y2 + y;
	
	if (g->Link) {
		if (Signal == TRUE) {
			Signal = JOIN_NO;
			OSWriteLinkBlock(&Signal, 1);
		} else if (Align_Calcs() == QUIT)
			return QUIT;
	}
	return 0;
}

void Small_Circle(void *p, short x, short y) {
	EXT_SETPIX(p, x - 1, y);
	EXT_SETPIX(p, x + 1, y);
	EXT_SETPIX(p, x, y - 1);
	EXT_SETPIX(p, x, y + 1);
}

/* Show the Stats of a specified Team */
void Show_Team_Stats(Names Cur_Team) {
	register short a;
	
	FS_DrawGrayString(0, Zeroy + 25, "Roster", A_NORMAL|A_SHADOWED|A_CENTERED, F_6x8);
	FastDrawGrayHLine2B(Zerox + 63, Zerox + 97, Zeroy + 33, COLOR_BLACK, Hiddenl, Hiddend);
	
	FS_DrawGrayString(0, Zeroy + 62, "Style", A_NORMAL|A_SHADOWED|A_CENTERED, F_6x8);
	FastDrawGrayHLine2B(Zerox + 63, Zerox + 97, Zeroy + 70, COLOR_BLACK, Hiddenl, Hiddend);
	DrawGrayRect2B(Zerox + 65, Zeroy + 74, Zerox + 95, Zeroy + 89, COLOR_WHITE, RECT_FILLED, 
	Hiddenl, Hiddend);
	DrawGrayRect2B(Zerox + 65, Zeroy + 74, Zerox + 95, Zeroy + 89, COLOR_BLACK, RECT_EMPTY, 
	Hiddenl, Hiddend);
	
	FastDrawGrayLine2B(Zerox + 80, Zeroy + 74, Zerox + 80, Zeroy + 89, COLOR_BLACK, Hiddenl, Hiddend);
	//GrayCircle_Clipped(Zerox + 80, Zeroy + 82, 2);
	
	short x = Zerox + 80, y = Zeroy + 82;
	ClipFastOutlinedCircle_R(Hiddenl, x, y, 2);
	ClipFastOutlinedCircle_R(Hiddend, x, y, 2);
	
	// Print Player Names, Types, and Formation for Currently selected team
	for(a = 3; a--;) {
		FS_DrawGrayString(0, Zeroy + 36 + 6 * a, Cur_Team.PlrNames[a], A_NORMAL|A_CENTERED, F_4x6);
		//GrayCircle_Clipped(Zerox + 65 + Starting_Pos[3*Cur_Team.Formation+a][0], 
		//Zeroy + 75 + Starting_Pos[3*Cur_Team.Formation+a][1], 1);
		
		short x = Zerox + 65 + Starting_Pos[3*Cur_Team.Formation+a][0];
		short y = Zeroy + 75 + Starting_Pos[3*Cur_Team.Formation+a][1];
		
		Small_Circle(Hiddenl, x, y);
		Small_Circle(Hiddend, x, y);
		if (!a) {
			Clipped_Plr(Zerox + 50, Zeroy, PlrGfx[Goalie + 1], 1);
		} else {
			if (Cur_Team.Types[a - 1] == Skinny)
				Clipped_Plr(Zerox + 50 + 20 * a, Zeroy, SkinnyGfx[8], 3);
			else Clipped_Plr(Zerox + 50 + 20 * a, Zeroy, PlrGfx[Cur_Team.Types[a - 1] + 8], 1);
		}
	}
}

// Records a Team with a random conference and Number into the Team Paramater
void Choose_Random_Team(Team *team) {
	do  // If this is Team2, then loop until Team2 is different than Team1
	{
		team->Conference = random(2);
		team->No = random(g->No_Teams[team->Conference]);
	} while (team == Team2 && Team1->No == team->No && Team1->Conference == team->Conference);
}

/* Shows the Tournament Board with all Team Names on it */
short Show_Tournament(unsigned char Teams[8][3], short Round) {
	short a, y = Zeroy + 69, x, addx;
	unsigned char Name_Pos[4] = { y - 6, y + 4, y + 14, y + 24 };
	char buffer[30];
	
	Fill_Outside();
	SlowerSpriteX8_OR_R(Zerox, Zeroy, 100, NHLd, 20, Hiddenl);
	SpriteString(0, Zeroy, (char*)"- tournament -", Hiddenl, Hiddend, A_CENTERED);
	
	sprintf(buffer, "Round %d", Round + 1);
	FS_DrawGrayString(0, Zeroy + 12, buffer, A_NORMAL|A_SHADOWED|A_CENTERED, F_6x8);
//	sprintf(buffer, "%s", Teams[0][0] ? Eastern[Teams[0][1]].TeamName : Western[Teams[0][1]].TeamName);
//	FS_DrawGrayString(0, Zeroy + 32, buffer, A_NORMAL|A_SHADOWED|A_CENTERED, F_4x6);
	FS_DrawGrayString(0, Zeroy + 32, ((Teams[0][0]) ? (Eastern[Teams[0][1]].TeamName) : 
		(Western[Teams[0][1]].TeamName)), A_NORMAL|A_SHADOWED|A_CENTERED, F_4x6);
	FS_DrawGrayString(0, Zeroy + 38, "vs", A_NORMAL|A_SHADOWED|A_CENTERED, F_4x6);
	Draw_TeamName(Teams, -1, Zeroy + 44, 1, Round);  // Print the Opponent's Team Name
	
	// Draw all of the Lines for the Left side and then Mirror the Lines to draw the Chart
	for(a = 2; a--;) {
		addx = 1 - (a * 2);
		x    = Zerox + 159 * a;
		FastDrawGrayHLine2B(x, x + addx * 31, y, COLOR_BLACK, Hiddenl, Hiddend);
		FastDrawGrayLine2B(x + addx * 31, y, x + addx * 31, y + 10, COLOR_BLACK, Hiddenl, Hiddend);
		FastDrawGrayHLine2B(x, x + addx * 31, y + 10, COLOR_BLACK, Hiddenl, Hiddend);
		
		FastDrawGrayHLine2B(x, x + addx * 31, y + 20, COLOR_BLACK, Hiddenl, Hiddend);
		FastDrawGrayLine2B(x + addx * 31, y + 20, x + addx * 31, y + 30, COLOR_BLACK, Hiddenl, Hiddend);
		FastDrawGrayHLine2B(x, x + addx * 31, y + 30, COLOR_BLACK, Hiddenl, Hiddend);
		
		FastDrawGrayHLine2B(x + addx * 31, x + addx * 63, y + 5, COLOR_BLACK, Hiddenl, Hiddend);
		FastDrawGrayLine2B(x + addx * 63, y + 5, x + addx * 63, y + 25, COLOR_BLACK, Hiddenl, Hiddend);
		FastDrawGrayHLine2B(x + addx * 31, x + addx * 63, y + 25, COLOR_BLACK, Hiddenl, Hiddend);
		
		FastDrawGrayHLine2B(x + addx * 63, x + addx * 79, y + 15, COLOR_BLACK, Hiddenl, Hiddend);
	}
	
	// Print the Original Eight Team Names in their Respective spots
	for(a = 8; a--;) {
		Draw_TeamName(Teams, Zerox + (a > 3) * (160 - DrawStrWidth(Teams[a][0] ? Eastern[Teams[a][1]].TeamName : 
		Western[Teams[a][1]].TeamName, F_4x6)), Name_Pos[a - ((a > 3) << 2)], a, 0);
	}
	
	// Print the Names of the Teams who have Progressed in the Tournament in Later Rounds
	if (Round > 0) {
		Draw_TeamName(Teams, Zerox + 32, y - 1, 0, 1);
		Draw_TeamName(Teams, Zerox + 32, y + 19, 2, 1);
		Draw_TeamName(Teams, Zerox + 96, y - 1, 4, 1);
		Draw_TeamName(Teams, Zerox + 96, y + 19, 6, 1);
		if (Round > 1) {
			Draw_TeamName(Teams, Zerox + 64, y + 9, 0, 2);
			Draw_TeamName(Teams, Zerox + 64, y + 17, 4, 2);
		}
	}
	
	Dissolve();
	while(_rowread(0)) {
		KeyScan();
		if (_KeyCheck(TEACHERKEY))
			LeadOut();
		else if (_KeyCheck(ESCKEY))
			return QUIT;
	}
	while(!_rowread(0));
	while(_rowread(0)) {
		KeyScan();
		
		if (_KeyCheck(TEACHERKEY))
			LeadOut();
		else if (_KeyCheck(ESCKEY))
			return QUIT;
	}
	return 0;
}

// Calculate and Print the Correct Team Name for a spot in the Tournament Chart
void Draw_TeamName(unsigned char Teams[8][3], short x, short y, short No, short Round) {
	while(Teams[No][2] < Round) {
		if (++No > 7)
			return;
	}
	
	if (x >= Zerox) {
		FS_DrawGrayString(x, y, Teams[No][0] ? Eastern[Teams[No][1]].TeamName : Western[Teams[No][1]].TeamName, 
			A_NORMAL|A_SHADOWED, F_4x6);
	} else {
		FS_DrawGrayString(x, y, Teams[No][0] ? Eastern[Teams[No][1]].TeamName : Western[Teams[No][1]].TeamName, 
			A_NORMAL|A_SHADOWED|A_CENTERED, F_4x6);
	}
}

/* Menu for modifying, adding, and/or deleting External Teams */
void Editor_Menu(void) {
	static menu Edit_Menu[] = { {"edit team"}, {"new team"}, {"delete team"}, {"- back -"} };
	short Choice = 0, Choice2, Redo, a;
	Names *Temp = NULL;
	
	do 
	{
		Redo = FALSE;
		Choice = Menu("- team editor -", (char*)Edit_Menu, 4, Choice, TRUE);
		
		if (Choice == 0) {
			/* Edit an Old Team */
			/* ---------------- */
			if (Choose_Team(Team1, 1) != QUIT) {
				if (Edit_Team(Team1->Conference ? &Eastern[Team1->No] : &Western[Team1->No]) == 0)
					BubbleSort_Teams(Team1->Conference ? &Eastern : &Western, g->No_Teams[Team1->Conference] - 1);
			}
			Redo = TRUE;
		} else if (Choice == 1) {
			/* Create a New Team */
			/* ----------------- */
			Choice2 = Menu("- conference -", (char*)((menu[]){ {"western"}, {"eastern"}, {"- back -"} }), 
										3, 0, TRUE);
			if (Choice2 == QUIT || Choice2 == 2) {
				Redo = TRUE;
			} else {
				if ((Temp = malloc(sizeof(Names[g->No_Teams[Choice2] + 1]))) == NULL) {
					Message((const char *const[]){"error", "", "not enough memory", "free up ram", NULL}, 1);
				} else {
					memcpy(Temp, Choice2 ? Eastern : Western, sizeof(Names[g->No_Teams[Choice2]]));
					Team1->Conference = Choice2;
					Team1->No = g->No_Teams[Choice2];
					
					strcpy(Temp[Team1->No].TeamName, "New");
					for(a = 3; a--;)
						strcpy(Temp[Team1->No].PlrNames[a], "New Player");
					Temp[Team1->No].Formation = random(3);
					Temp[Team1->No].Types[0] = Average;
					Temp[Team1->No].Types[1] = Average;
					Redo = TRUE;
					if (Edit_Team(&Temp[Team1->No]) == 0) { // If the User saves the changes
						g->No_Teams[Choice2]++;
						
						Swap_Teams(&Temp[Team1->No], &Temp[Team1->No - 1]);
						BubbleSort_Teams(&Temp, g->No_Teams[Choice2] - 1);
						if (Choice2 == WESTERN) {
							free(Western);
							Western = Temp;
						} else {
							free(Eastern);
							Eastern = Temp;
						}
					} else free(Temp);
				}
			}
		} else if (Choice == 2) {
			/* Delete an Old Team */
			/* ------------------ */
			if (Choose_Team(Team1, 2) != QUIT) {
				char buffer[25];
				sprintf(buffer, "%s", Team1->Conference ? Eastern[Team1->No].TeamName : Western[Team1->No].TeamName);
				if (g->No_Teams[Team1->Conference] <= 10) {
					Message((const char *const[]){"- error -", "", "the team", buffer, "cannot be deleted", NULL}, 1);
				} else {
					for(a = Team1->No; a < g->No_Teams[Team1->Conference]; a++) {
						if (Team1->Conference == WESTERN)
							memcpy(&Western[a], &Western[a + 1], sizeof(Names));
						else memcpy(&Eastern[a], &Eastern[a + 1], sizeof(Names));
					}
					g->No_Teams[Team1->Conference]--;
					g->Teams_Changed = TRUE;
					
					Message((const char *const[]){"- success -", "", "the team", buffer, "has been deleted", NULL}, 1);
				}
			}
			Redo = TRUE;
		}
	} while(Redo == TRUE);
}

/* The Team Editor Interface, where the user may make modifications to a Team */
short Edit_Team(Names *Cur_Team) {
	short Starting = TRUE, Pos = 0, width, a, b, Exit;
	char buffer[30];
	Names Temp;
	memcpy(&Temp, Cur_Team, sizeof(Names));
	const unsigned char editor[] = { 'e' - 97, 'd' - 97, 'i' - 97, 't' - 97, 'o' - 97, 'r' - 97 };
	
	do 
	{
		do 
		{
			Exit = FALSE;
			
			KeyScan();
			if (_KeyCheck(UPKEY | LEFTKEY) && (Starting = -1) && --Pos < 0)
				Pos = 7;
			if (_KeyCheck(DOWNKEY | RIGHTKEY) && (Starting = -2) && ++Pos > 7)
				Pos = 0;
			
			Fill_Outside();
			SlowerSpriteX8_OR_R(Zerox, Zeroy, 100, NHLd, 20, Hiddenl);  // Draw the NHL 2004 Background
			Show_Team_Stats(Temp);
			strcpy(buffer, Temp.TeamName);
			SpriteString(0, Zeroy + 92, buffer, Hiddenl, Hiddend, A_CENTERED);
			FS_DrawGrayString(Zerox + 133, Zeroy + 94, "-- Back", A_NORMAL, F_4x6);
			for(a = 2; a--;)
				for(b = 6; b--;)
					GraySprite8_MASK_R(Zerox+a*152,Zeroy+(b<<3),8,Font[editor[b]],Font[editor[b]]+8,Font[editor[b]]+16,
					Font[editor[b]]+16,Hiddenl,Hiddend);
			
			// Invert the current selection
			switch(Pos) {
				case 0:
				case 1:
					InvertGrayRect2B(Zerox + 69 + 20 * Pos, Zeroy, Zerox + 86 + 20 * Pos - ((Temp.Types[Pos] != Fat) 
					* 2), Zeroy + 16 + ((Temp.Types[Pos] == Skinny) * 2), Hiddenl, Hiddend);
					break;
				case 2 ... 4:
					width = 0;
					for(a = 3; a--;) {
						if (DrawStrWidth(Temp.PlrNames[a], F_4x6) > width)
							width = DrawStrWidth(Temp.PlrNames[a], F_4x6);
					}
					InvertGrayRect2B((LCD_WIDTH-width)/2, Zeroy + 23 + 6 * Pos, (LCD_WIDTH-width)/2 + width, 
					Zeroy + 29 + 6 * Pos, Hiddenl, Hiddend);
					break;
				case 5:
					InvertGrayRect2B(Zerox + 65, Zeroy + 74, Zerox + 95, Zeroy + 89, Hiddenl, Hiddend);
					break;
				case 6:
					width = 8*strlen(Temp.TeamName);
//					width = DrawStrWidth(Temp.TeamName, F_8x10);
					InvertGrayRect2B((LCD_WIDTH-width)/2 - 1, Zeroy + 91, (LCD_WIDTH-width)/2 + width, Zeroy + 99, 
						Hiddenl, Hiddend);
					break;
				case 7:
					InvertGrayRect2B(Zerox + 133, Zeroy + 93, Zerox + 159, Zeroy + 99, 
						Hiddenl, Hiddend);
					break;
			}
			
			if (Starting == TRUE) {
				Dissolve();
				Starting = FALSE;
			} else {
				Toggle_Screens();
			}
			if (Starting == -1) {
				while_keypress(UPKEY | LEFTKEY);
				Starting = FALSE;
			} else if (Starting == -2) {
				while_keypress(DOWNKEY | RIGHTKEY);
				Starting = FALSE;
			} else if (Starting == -3) {
				while_keypress(SHOTKEY | ENTERKEY);
				Starting = FALSE;
			}
			
			do { KeyScan(); } while(g->keys == FALSE);
		} while (!_KeyCheck(SHOTKEY | ENTERKEY | ESCKEY | TEACHERKEY));
		if (_KeyCheck(TEACHERKEY))  // Quick Exit without saving Changes
			LeadOut();
		if (_KeyCheck(ESCKEY)) {
			Exit = TRUE;
		} else {
			switch(Pos) {  // The user has selected an attribute to change
				case 0:
				case 1:
					if (Temp.Types[Pos] == Skinny)
						Temp.Types[Pos] = Average;
					else if (Temp.Types[Pos] == Average)
						Temp.Types[Pos] = Fat;
					else if (Temp.Types[Pos] == Fat)
						Temp.Types[Pos] = Skinny;
					Starting = -3;
					break;
				case 2 ... 4:
					Get_Input("Change Player Name:", Temp.PlrNames[Pos - 2], 22);
					Starting = TRUE;
					break;
				case 5:
					if (++Temp.Formation > 2)
						Temp.Formation = 0;
					Starting = -3;
					break;
				case 6:
					Get_Input("Change Team Name:", Temp.TeamName, 12);
					Starting = TRUE;
					break;
				default:
					Exit = TRUE;
			}
		}
		if (Exit == TRUE) {
			a = Menu("- save changes -", (char*)((menu[]){ {"yes"}, {"no"}, {"cancel"} }), 3, 0, TRUE);
			if (a == 0) {
				memcpy(Cur_Team, &Temp, sizeof(Names));
				g->Teams_Changed = TRUE;
			} else if (a == 2) {
				Exit = FALSE;
				Starting = TRUE;
			}
		}
	} while (Exit == FALSE);
	
	return a;  // Return whether or not the User chose to save the Changes
}