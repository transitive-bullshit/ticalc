// C Source File
// Created 9/14/2003; 3:08:27 PM
#include "All.h"

/* Communicates between the Host and the Joining Calc all of the necessary data 
   while a game is running */
void Transfer_Data(void) {
	char Control = FALSE, Other_Control;
	unsigned char a;
	
	// Must send three plrs separately
	for(a = 3; a--;) {
		if (g->Calc == Host) {
			if (LIO_SendData(&Team2->plrs[a]->Control, sizeof(char)))
				ERROR(LINK_ERROR);
			if (LIO_RecvData(&Other_Control, sizeof(char), 30))
				ERROR(LINK_ERROR);
			
			/* After a lot of trial and error, this is the only way I found that 
			will allow for one player to take the puck from another player */
			if (Other_Control < 0 && Team1->plrs[a]->Control >= 0)
				Team1->plrs[a]->Control = Other_Control;
			
			if (LIO_SendData((char*)Team1->plrs[a], sizeof(Plr)))  // Send Plr on Team1
				ERROR(LINK_ERROR);
			if (LIO_RecvData((char*)Team2->plrs[a], sizeof(Plr), 30))  // Receive Plr on Team2
				ERROR(LINK_ERROR);
		} else {
			if (LIO_RecvData(&Other_Control, sizeof(char), 30))
				ERROR(LINK_ERROR);
			if (LIO_SendData(&Team1->plrs[a]->Control, sizeof(char)))
				ERROR(LINK_ERROR);
			
			if (Other_Control < 0 && Team2->plrs[a]->Control >= 0)
				Team2->plrs[a]->Control = Other_Control;
			
			if (LIO_RecvData((char*)Team1->plrs[a], sizeof(Plr), 30))  // Receive Plr on Team1
				ERROR(LINK_ERROR);
			if (LIO_SendData((char*)Team2->plrs[a], sizeof(Plr)))  // Send Plr on Team2
				ERROR(LINK_ERROR);
		}
		
		// If the puck is controlled by the other calc, make sure g->Is_Controlled is set 
		// to the correct value and only one player has Control of the puck
		if (Control == FALSE) {
			if (Team1->plrs[a]->Control > 0) {  // Puck controlled by team 1
				g->Is_Controlled = 1;
				if (Team2->plrs[a]->Control > 0)
					Team2->plrs[a]->Control = -1;
				Control = 1;
			} else if (Team2->plrs[a]->Control > 0) {  // Puck controlled by team 2
				g->Is_Controlled = 2;
				Control = 2;
			} else g->Is_Controlled = FALSE;  // Puck is not being controlled by a player
		} else {  // Make sure two players cannot have the puck at the same time
			if (Team1->plrs[a]->Control > 0)
				Team1->plrs[a]->Control = -1;
			if (Team2->plrs[a]->Control > 0)
				Team2->plrs[a]->Control = -1;
		}
	}
	
	// Send or Receive Puck Data, depending on which team has Control of the puck
	if (g->Last_Controlled == 2 && g->Is_Controlled == 0) {
		if (g->Calc == Host && LIO_RecvData((char*)&g->Is_Controlled, 7*sizeof(short), 30))
			ERROR(LINK_ERROR);
		else if (g->Calc == Join && LIO_SendData((char*)&g->Is_Controlled, 7*sizeof(short)))
			ERROR(LINK_ERROR);
	} else if (g->Calc == Host) {
		if (g->Is_Controlled != 2) {
			if (LIO_SendData((char*)&g->Is_Controlled, 7*sizeof(short)))
				ERROR(LINK_ERROR);
		} else if (LIO_RecvData((char*)&g->Is_Controlled, 7*sizeof(short), 30))
			ERROR(LINK_ERROR);
	} else if (g->Calc == Join) {
		if (g->Is_Controlled == 2) {
			if (LIO_SendData((char*)&g->Is_Controlled, 7*sizeof(short)))
				ERROR(LINK_ERROR);
		} else if (LIO_RecvData((char*)&g->Is_Controlled, 7*sizeof(short), 30))
			ERROR(LINK_ERROR);
	}
	
	
	/*if (g->Last_Controlled == 2 && g->Is_Controlled == 0) {
		if (g->Calc == Host && LIO_RecvData((char*)&g->Is_Controlled, sizeof(short)*7, 30))
			ERROR(LINK_ERROR);
		else if (g->Calc == Join && LIO_SendData((char*)&g->Is_Controlled, sizeof(short)*7))
			ERROR(LINK_ERROR);
	} else if (g->Calc == g->Is_Controlled) {
		if (LIO_SendData((char*)&g->Is_Controlled, sizeof(short)*7))
			ERROR(LINK_ERROR);
	} else if (LIO_RecvData((char*)&g->Is_Controlled, sizeof(short)*7, 30))
		ERROR(LINK_ERROR);*/
	
	/*if (g->Calc == Host) {
		if (g->Is_Controlled != 2) {
			if ()
				ERROR(LINK_ERROR);
		} else if (LIO_RecvData((char*)&g->Is_Controlled, sizeof(short)*7, 30))
			ERROR(LINK_ERROR);
	} else if (g->Calc == Join) {
		if (g->Is_Controlled == 2) {
			if (LIO_SendData((char*)&g->Is_Controlled, sizeof(short)*7))
				ERROR(LINK_ERROR);
		} else if (LIO_RecvData((char*)&g->Is_Controlled, sizeof(short)*7, 30))
			ERROR(LINK_ERROR);
	}*/
	// Remember who had the puck last frame
	g->Last_Controlled = g->Is_Controlled;
}

/* Establishes a Connection between two calcs.  The Calc to Join the Game first will be the Host */
/*short Multiplayer(void) {
	char Send = LINK_NO, Receive = 0, ABORT = FALSE;
	short Settings[4] = { g->Mode, g->Speed, g->Friction, g->Distortion };
	Message((const char *const[]){"attempting to", "connect", NULL}, -1);
	
	// Clear the Link buffer until we empty it or receive the Host Calc's LINK_NO Signal
	while(OSReadLinkBlock(&Receive, 1) == 1) {
		if (Receive == LINK_NO)
			ABORT = -1;
		else if (Receive == QUIT)
			ABORT = TRUE;
	}
	if (ABORT == TRUE)
		Receive = 0;
	else if (ABORT == -1)
		Receive = LINK_NO;
	OSWriteLinkBlock(&Send, 1);
	if (Receive == LINK_NO) { // This calc is 2nd to join the game
		g->Calc = Join;
		if (LIO_RecvData((char*)Settings, sizeof(Settings), 30))
			ERROR(LINK_ERROR);
		g->Mode       = Settings[0];
		g->Speed      = Settings[1];
		g->Friction   = Settings[2];
		g->Distortion = Settings[3];
		
		// The Joining Calc chooses Team 2
		if (Choose_Team(Team2, 1) == QUIT)
			return QUIT;
	} else {
		// Tell the Host plr to wait and press Escape to cancel
		Message((const char *const[]){"waiting for other", "player to join", "press esc to cancel", NULL}, -1);
		
		while(Receive != LINK_NO) {  // Wait until a signal is received from the Joining Calc
			OSReadLinkBlock(&Receive, 1);
			KeyScan();
			// Check if the Host wants to exit the game and send a Signal to the Other calc if so
			if (_KeyCheck(ESCKEY | TEACHERKEY)) {
				Exit_Linkplay();
				return QUIT;
			}
		}
		g->Calc = Host;
		
		//OSWriteLinkBlock((char*)&g->Mode, sizeof(short));  (Old)
		
		// Make sure game Settings on both calcs are the same.  If one calc is in EXHIBITION Mode and 
		// the other is in SHOOTOUT Mode, there would be problems
		LIO_SendData((char*)Settings, sizeof(Settings));
		
		// The Host chooses Team 1
		if (Choose_Team(Team1, 1) == QUIT)
			return QUIT;
	}
	
	OSLinkOpen();
	return 0;
}*/

/* Establishes a Connection between two calcs.  The Calc to Join the Game first will be the Host */
short Multiplayer(void) {
	char Send, Receive = 0, Signal = FALSE;
	short Settings[4] = { g->Mode, g->Speed, g->Friction, g->Distortion };
	Message((const char *const[]){"attempting to", "connect", NULL}, -1);
	
	// Clear the Link buffer until we empty it or receive the Host Calc's LINK_NO Signal
	while(OSReadLinkBlock(&Receive, 1) == 1) {
		if (Receive == HOST_NO)
			Signal = TRUE;
		if (Receive == QUIT)
			Signal = QUIT;
	}
	
	if (Signal == TRUE) { // This calc is 2nd to join the game
		Send = JOIN_NO;
		OSWriteLinkBlock(&Send, 1);
		g->Calc = Join;
	} else {
		Send = HOST_NO;
		OSWriteLinkBlock(&Send, 1);
		// Tell the Host plr to wait and press Escape to cancel
		Message((const char *const[]){"waiting for other", "player to join", "press esc to cancel", NULL}, -1);
		
		g->Calc = Host;
		while(Receive != JOIN_NO) {  // Wait until a signal is received from the Joining Calc
			OSReadLinkBlock(&Receive, 1);
			if (Receive == HOST_NO) {
			// This calc entered Multiplayer before the other calc had even started the game.  Thus, this 
			// calc's original HOST_NO message to the other calc was disregarded by the other calc's AMS.
				OSLinkOpen();
				Send = JOIN_NO;
				OSWriteLinkBlock(&Send, 1);
				g->Calc = Join;
				break;
			}
			
			// Check if the Host wants to exit the game and send a Signal to the Other calc if so
			KeyScan();
			if (_KeyCheck(ESCKEY | TEACHERKEY)) {
				Exit_Linkplay();
				return QUIT;
			}
		}
	}
	
	if (g->Calc == Host) {
		// Make sure game Settings on both calcs are the same.  If one calc is in EXHIBITION Mode and 
		// the other is in SHOOTOUT Mode, there would be problems
		LIO_SendData((char*)Settings, sizeof(Settings));
		
		/* The Host chooses Team 1 */
		if (Choose_Team(Team1, 1) == QUIT)
			return QUIT;
	} else {
		if (LIO_RecvData((char*)Settings, sizeof(Settings), 30))
			ERROR(LINK_ERROR);
		g->Mode       = Settings[0];
		g->Speed      = Settings[1];
		g->Friction   = Settings[2];
		g->Distortion = Settings[3];
		
		/* The Joining Calc chooses Team 2 */
		if (Choose_Team(Team2, 1) == QUIT)
			return QUIT;
	}
	
	OSLinkOpen();
	return TRUE;  // Connection successfully established
}

// Synchronizes the calc's timers and Game Status
void Synchronize_Calcs(void) {
	char Other_CalcStatus;
	unsigned char Original[2] = { clock->Mseconds50, clock->Seconds };
	unsigned char Time[2]     = { clock->Mseconds50, clock->Seconds };
	
	if (g->Calc == Host) {
		// Align Timers to the Host Calc
		if (LIO_SendData(Time, sizeof(Time)))
			ERROR(LINK_ERROR);
		// Make sure if one calc stops, the other stops also
		if (LIO_RecvData((char*)&Other_CalcStatus, sizeof(char), 20))
			ERROR(LINK_ERROR);
		if (LIO_SendData((char*)&g->Status, sizeof(char)))
			ERROR(LINK_ERROR);
	} else if (g->Calc == Join) {
		if (LIO_RecvData(Time, sizeof(Time), 20))
			ERROR(LINK_ERROR);
		
		//OSWriteLinkBlock((char*)&g->Status, sizeof(char));
		if (LIO_SendData((char*)&g->Status, sizeof(char)))
			ERROR(LINK_ERROR);
		if (LIO_RecvData((char*)&Other_CalcStatus, sizeof(char), 20))
			ERROR(LINK_ERROR);
	}
	
	const char andere_mseconds = Time[0] + (clock->Mseconds50 - Original[0]);
	clock->Mseconds50 = (andere_mseconds < 0 ? 0 : andere_mseconds);
	//if (clock->Mseconds50 < 0)
	//	clock->Mseconds50 = 0;
	clock->Seconds    = Time[1] + (clock->Seconds - Original[1]);
	
	if (Other_CalcStatus != Normal && Other_CalcStatus != g->Status) {
		if (Other_CalcStatus == Fast_Quit)
			g->Status = QUIT;
		else g->Status = Other_CalcStatus;
	}
}

// Make sure Calcs are aligned before starting a game
short Align_Calcs(void) {
	char Send, Receive = 0;
	Message((const char *const[]){"waiting", NULL}, -1);
	
	if (g->Link && g->Calc == Host) {
		Send = HOST_NO;
		OSWriteLinkBlock(&Send, 1);
		// Wait until signal from the Joining calc
		while(Receive != JOIN_NO) {
			// Read the Link buffer and check if the other calc has exited the game
			if (OSReadLinkBlock(&Receive, 1) == 1 && Receive == QUIT) {
				Message((const char *const[]){"other calc has", "exited the game", NULL}, 1);
				return QUIT;
			}
			KeyScan();
			// Break out of Loop and notify other calc
			if (_KeyCheck(ESCKEY | TEACHERKEY)) {
				Exit_Linkplay();
				return QUIT;
			}
		}
	} else if (g->Calc == Join) {
		Send = JOIN_NO;
		// Wait until signal from the Host
		while(Receive != HOST_NO) {
			if (OSReadLinkBlock(&Receive, 1) == 1 && Receive == QUIT) {
				Message((const char *const[]){"other calc has", "exited the game", NULL}, 1);
				return QUIT;
			}
			KeyScan();
			// Break out of Loop and notify other calc
			if (_KeyCheck(ESCKEY | TEACHERKEY)) {
				Exit_Linkplay();
				return QUIT;
			}
		}
		if (OSReadLinkBlock(&Receive, 1) == 1 && (Receive == QUIT))
			return QUIT;
		OSWriteLinkBlock(&Send, 1);
	}
	
	return 0;
}

// Notify Other calc that the user has exited from the game
void Exit_Linkplay(void) {
	if (g->Link) {
		const char Signal = QUIT;
		OSWriteLinkBlock(&Signal, 1);
	}
	
	KeyScan();
	if (_KeyCheck(TEACHERKEY))
		LeadOut();
}

/* Old Link Routines
-- They work, but I would rather have the plrs just have one multiplayer option instead of 
-- the Users trying to figure out the Host/Join System
-- The Host/Join System is also larger in size

// Try to establish a connection with a joining Calc and choose teams
short Host_Game(void) {
	char Send = HOST_NO, Receive = 0, Wait;
	
	// Tell the Host plr to wait and press Escape to cancel
	Message((const char *const[]){"waiting for other", "player to join", "press esc to cancel", NULL}, -1);
	
	// Wait for the signal from the Joining Calc
	while(Receive != JOIN_NO) {
		OSReadLinkBlock(&Receive, 1);
		if (_rowread(ESCROW) & ESCKEY)
			return QUIT;
	}
	for(Wait = 0;Wait < 120; Wait++);  // A small Delay
	OSWriteLinkBlock(&Send, 1);
	
	// The Host chooses both teams 
	if (Choose_Team(Team1, (char*)"Choose your team") == QUIT)
		return QUIT;
	if (Choose_Team(Team2, (char*)"Choose Opponent") == QUIT)
		return QUIT;
	
	OSWriteLinkBlock(&Send, 1);  // Notify Other Calc that the Host is done choosing the teams
	return 0;  // Connection established
}

// Waits until a signal is given from the Host Calc, or the Time_Limit expires
short Wait_For_Message(void) {
	char Receive = 0;
	OSLinkOpen();
	Message((const char *const[]){"- please wait -", "for the host ", "press esc to cancel", NULL}, FALSE);
	
	do 
	{
		OSReadLinkBlock(&Receive, 1);
		if (_rowread(ESCROW) & ESCKEY)  // Exit
			return QUIT;
	} while (Receive != HOST_NO);  // Check for the signal from the Host Calc
	
	return 0;
}

// Try to establish a connection with the Host Calc
short Join_Game(void) {
	char Send = JOIN_NO;
	short Result;
	OSWriteLinkBlock(&Send, 1);
	if ((Result = Wait_For_Message(5000)) == QUIT)
		return QUIT;
	else if (Result == TRUE) {
		// Wait until Host picks teams
		if ((Wait_For_Message(0)) == QUIT)
			return QUIT;
		else return 0;  // Success!
	}
	// Error linking Calcs
	Message((const char *const[]){"link error", "please try again", NULL}, TRUE);
	return QUIT;
}*/