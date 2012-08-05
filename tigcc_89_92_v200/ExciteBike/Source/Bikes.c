// C Source File
// Created 12/24/2003; 2:12:50 PM
#include "All.h"
static inline void Align_Bike(Bike*);
static inline void Draw_All_Bikes(void);
static inline void Draw_Bike(Bike*);
static inline void Handle_User_Bike(Bike*);
static inline void Move_Bike(Bike*);
static inline void Handle_Computer_Bike(Bike*);
static inline void Handle_Finish_Wheelie(Bike*);
static inline short Real_x(short);
#define GetTerrain(x) (g->Track[((x)>>3)+((x) < 0 ? (g->Width + 1) : 0)])
//(g->Track[((x)>>3)+((x) < 0) * (g->Width + 1)])

// http://faqs.ign.com//articles/382/382505p1.html

void Update_Bikes(void) {
	short a;
	
	// Handle KeyPresses for Calc vs Calc Races -- Changing g->Camera_Bike
	/*if (Is_Computer(g->bikes[g->Camera_Bike].Control)) {
		if (_KeyCheck(LEFTKEY) && --g->Camera_Bike < 0)
			g->Camera_Bike = g->No_Bikes - 1;
		if (_KeyCheck(RIGHTKEY) && ++g->Camera_Bike >= g->No_Bikes)
			g->Camera_Bike = 0;
	}*/
	g->Old_x = g->bikes[g->Camera_Bike].x;
	if (++g->delay >= 8) {
		g->delay = 0;
		g->Animation = !g->Animation;
	}
	
	// Run Text Messages
	if (g->text[0]) {
		if (g->delay == 0 && (++g->text_attr & 7) == 0)  // Make Message Blink
			g->text_attr ^= TEXT_ANIM;
		
		a = (g->text_attr & TEXT_DELAY);
		if (g->text_attr & TEXT_ANIM || a > 80)   // Draw Message
			FS_DrawGrayString(0, 10, g->text, A_CENTERED|A_SHADOWED, F_6x8);
		
		if (a > 56) {
			if (a > 80) {
				if (a > 90) g->text[0] = 0;
			} else if (g->bikes[g->Camera_Bike].attr & B_OVERHEAT) {
				strcpy(g->text, EFG("Go!", "Partez !", "Es geht los!"));
				g->text_attr = 81 | TEXT_ANIM;
			} else g->text[0] = 0;
		}
	}
	
	// Loop through and update each Bike
	for(a = 0; a < g->No_Bikes; a++) {
		if (Is_Computer(g->bikes[a].Control)) {
			Handle_Computer_Bike(&g->bikes[a]); // A.I.
		} else if (g->Calc == g->bikes[a].Control) {
			if (g->Status != End_Wheelie)
				Handle_User_Bike(&g->bikes[a]);  // Update Bike (being controlled by the User)
			else Handle_Finish_Wheelie(&g->bikes[a]);
		} else continue;  // This Bike will be handled by the other Calculator in LinkPlay
		
		// y - (x << 3) - (x << 2) = equals = y - 12 * x, but without the multiplication
		g->bikes[a].y = g->lcd_height - 29 - ((g->bikes[a].attr & B_Y) << 3) - ((g->bikes[a].attr & B_Y) << 2);
		Align_Bike(&g->bikes[a]);
	}
	
	if (g->Link == TRUE)
		Transfer_Data();
	Draw_All_Bikes();
	
	// Check for Front wheel to Back Wheel Bike Collisions -> Crashes
	register short b;
	for(/* a already equals g->No_Bikes from above*/; a--;) {
		/* -- Make the Computer Bike Loop around the User's Screen instead of racing the entire Track --
		 If a Computer Bike gets too far off of the Screen, it will be 'teleported' to the other 
		 side, so the Camera will always have bikes on or near it.  This gives the user the feel of 
		 racing against more bikes, when at most, there will only be 4 Bikes in the race.  This is 
		 done for speed reasons and is also the way the bikes were handled in the original.
		*/
		if (Is_Computer(g->bikes[a].Control) && a != g->Camera_Bike) {
			if (Real_x(g->bikes[a].x) > Real_x(g->bikes[g->Camera_Bike].x) + (160 * 2)) {
				g->bikes[a].attr &= ~(B_OVERHEAT | B_CRASHING | B_FLAG1 | B_FLAG2 | B_FLAG3);
				
				/*if (_keytest (RR_CLEAR)) {  // For Debugging Purposes
		PortSet(Hiddend, 239, 127);
		printf_xy(0, 0, "1) %d %d %d %d", Real_x(g->bikes[a].x), g->bikes[a].x, 
			Real_x(g->bikes[g->Camera_Bike].x) + (160 * 2), g->bikes[a].x - (160 * 4));
		PortSet(Hiddenl, 239, 127);
		printf_xy(0, 0, "1) %d %d %d %d", Real_x(g->bikes[a].x), g->bikes[a].x, 
			Real_x(g->bikes[g->Camera_Bike].x) + (160 * 2), g->bikes[a].x - (160 * 4));
		CopyGrayScreens(Hiddenl, Hiddend, Activel, Actived);
					
					while(_keytest(RR_CLEAR));
					while(!_keytest(RR_CLEAR));
					while(_keytest(RR_CLEAR));
				}*/
				
				g->bikes[a].x -= (160 * 4);
				g->jumpdat[g->bikes[a].Bike_NO].Original_x -= (160 * 4);
				g->jumpdat[g->bikes[a].Bike_NO].Last_Crash -= (160 * 4);
			} else if (Real_x(g->bikes[a].x) + (160 * 2) < Real_x(g->bikes[g->Camera_Bike].x)) {
				g->bikes[a].attr &= ~(B_OVERHEAT | B_CRASHING | B_FLAG1 | B_FLAG2 | B_FLAG3);
				
				/*if (_keytest (RR_CLEAR)) {  // For Debugging Purposes
		PortSet(Hiddend, 239, 127);
		printf_xy(0, 0, "2) %d %d %d %d", Real_x(g->bikes[a].x) + (160 * 2), g->bikes[a].x, 
			g->bikes[g->Camera_Bike].x, g->bikes[a].x - (160 * 4));
		PortSet(Hiddenl, 239, 127);
		printf_xy(0, 0, "2) %d %d %d %d", Real_x(g->bikes[a].x) + (160 * 2), g->bikes[a].x, 
			g->bikes[g->Camera_Bike].x, g->bikes[a].x - (160 * 4));
		CopyGrayScreens(Hiddenl, Hiddend, Activel, Actived);
					
					while(_keytest(RR_CLEAR));
					while(!_keytest(RR_CLEAR));
					while(_keytest(RR_CLEAR));
				}*/
				
				g->bikes[a].x += (160 * 4);
				g->jumpdat[g->bikes[a].Bike_NO].Original_x += (160 * 4);
				g->jumpdat[g->bikes[a].Bike_NO].Last_Crash += (160 * 4);
				if (g->bikes[a].x > (g->Width << 3) + 8 - LCD_WIDTH) {
					g->bikes[a].x -= (g->Width << 3) + 8;
					g->jumpdat[g->bikes[a].Bike_NO].Original_x -= (g->Width << 3) + 8;
					g->jumpdat[g->bikes[a].Bike_NO].Last_Crash -= (g->Width << 3) + 8;
				}
			}
		}
		
		if ((g->bikes[a].attr & (B_JUMPING | B_OVERHEAT | B_CRASHING)) != FALSE)
			continue;
		for(b = g->No_Bikes; b--;) {
			if (b == a || (g->bikes[b].attr & (B_JUMPING | B_OVERHEAT | B_CRASHING)) != FALSE)
				continue;
			if (g->bikes[b].y > g->bikes[a].y - 2 && g->bikes[b].y < g->bikes[a].y + 2 && 
			Real_x(g->bikes[b].x) > Real_x(g->bikes[a].x) - 21 && 
			Real_x(g->bikes[b].x) < Real_x(g->bikes[a].x) - 14) {
				Start_Crash(&g->bikes[b], 1);  // Crash!
				break;
			}
		}
	}
}

// Correct the Bike's current Tilt, x, y, and Graphic Values depending on the Terrain and the 
// Bike's current Behaviors (Jumping, Crashing, Mud...)
// This is the main Hit Detection and Bike Correction Routine
static inline void Align_Bike(Bike *b) {
	Move_Bike(b);
	
	// If the Camera is centered on this Bike, then Update the Camera's position after moving the Bike
	if (b->Bike_NO == (unsigned short)g->Camera_Bike)
		Shift_Left(b->x - g->Old_x);
	
	// Set the Bike to the Default Graphic, depending on its behavior.  After this, 
	// adjustments are made to the Bike's Graphic depending on the Terrain
	if (b->Tilt != None && (b->attr & B_JUMPING) == FALSE)
		b->Graphic = b->Tilt;													// Bike is doing a Wheelie
	else if (b->addx < 26) {
		b->Graphic = Still;														// Bike is not Moving
		b->attr   &= ~B_BLOCK;
	} else if (b->attr & B_TURNING)
		b->Graphic = DownTurn + (b->Counter < 0);			// Turning Graphic
	else b->Graphic = Regular1 + g->Animation;			// Riding Animation
	
	/* --------------------- */
	/* TERRAIN HIT DETECTION */
	/* --------------------- */
	register short x0 = GetTerrain(b->x + 6), x = GetTerrain(b->x + 10), x2 = GetTerrain(b->x + 18);
	register short xcnt = ((b->x + 2) & 7), Bikey = RealBike_y(b);  // + 2
	JumpDat *j = &g->jumpdat[b->Bike_NO];
	
	if (x0 + x + x2 != 0) {  // There is no need to check ~50 if statements if we know that the 
		// bike is on level terrain (Gra1)
		
		if (x == MR26 && Bikey > 1) {
			b->y      -= 48;
			
			b->Old_Graphic = Back5;
			Start_Jump(b, 25); // Monster Ramp //
			if (b->attr & B_JUMPING)
				b->Graphic = b->Tilt = Back6;
		}
		/* --- Front End of Bike --- */
		else if (x2 == SL00 || x2 == SL10 || x2 == SL20 || x2 == SL50 || x2 == MO00 || x2 == MR00 || x2 == Fin0)
			b->Graphic = Back6 - (GetTerrain(b->x + 16) == x2) - (GetTerrain(b->x + 13) == x2);
		else if (x2 == SL30)
			b->Graphic = Back6 - (GetTerrain(b->x + 16) == x2);
		else if (x2 == SL40)
			b->Graphic = Back6 - (GetTerrain(b->x + 17) == x2) - (GetTerrain(b->x + 16) == x2) - 
			(GetTerrain(b->x + 15) == x2);
		else if (x2 == SL60 && xcnt > 3)
			b->Graphic = Back6 - (GetTerrain(b->x + 14) == x2) - (GetTerrain(b->x + 12) == x2);
		else if (x2 == Jum1 && Bikey > 1 && xcnt > 2)
			b->Graphic = Back6 - (GetTerrain(b->x + 10) == x2);
		else if ((b->attr & B_JUMPING) == FALSE && ((Bikey == 3 && (x2 == Tur1 || x2 == Tur2)) || 
		(Bikey == 0 && (x2 == Tur3 || x2 == Tur4))))
			b->Temp = Low_Temp;  // Turbo Arrow
		else if (x2 == MO08) {
			b->Graphic = Back6 - (GetTerrain(b->x + 17) == x2) - (GetTerrain(b->x + 16) == x2) - 
			(GetTerrain(b->x + 15) == x2);
			b->y      -= 16;
		} else if (x == Jum1 && Bikey > 1) {													// Top Jump
			b->Graphic = Back5 - (xcnt == 7);
		}	else if (x == Jum2 && Bikey > 1) {
			b->Graphic = Back4;
			b->y      -= xcnt + 1;
			
			Start_Jump(b, 62); // Top Jump //
		} else if ((x2 == Blo1 && Bikey > 1) || (x2 == Blo2 && Bikey < 2)) {
			// If you're leaning backwards and hit a block, you won't crash
			if (b->Tilt > Back5 || b->Tilt == None)
				Start_Crash(b, 1);  // Crash!
			else if ((b->attr & (B_CRASHING | B_JUMPING)) == FALSE) {
				b->Tilt = None;
				j->Last_Crash = b->x + 8;
				b->attr |= B_BLOCK;
			}
		} else if ((x2 >= Rok1 && x2 <= Rok3 && (Bikey & 1)) || 
		(x2 >= Rok4 && x2 <= Rok6 && (Bikey & 1) == 0))
			b->attr |= B_MUD;
		else if ((Bikey < 2 && (x2 == Dry0 || x2 == Dry1)) || 
		(Bikey > 1 && (x2 == Dry2 || x2 == Dry3)) || (x2 == Dry4 || x2 == Dry5))
			b->attr |= B_MUD;
		/* --- Middle of Bike --- */
		else if (x == SL00 || x == SL10 || x == SL20 || x == SL50 || x == MO00 || x == Fin0) {
			b->Graphic = Back4 + (xcnt == 7 || xcnt < 2);
			b->y      -= xcnt;
		}	else if (x == SL01) {
			b->y      -= 8;
			
			Start_Jump(b, 30); // Small Jump //
		} else if (x == SL12 || x == MO02) {
			b->y      -= 16;
			
			Start_Jump(b, 40); // Medium Jump //
		} else if (x == SL24) {
			b->y      -= 32;
			
			Start_Jump(b, 50); // Large Jump //
		} else if (x == SL34) {
			b->y      -= 16;
			
			Start_Jump(b, 40); // Long Jump //
		} else if (x == SL42) {
			b->y      -= 32;
			
			Start_Jump(b, 40); // Tall Jump //
		} else if (x == SL53 || x == SL62) {
			b->y			-= 24;
			if (xcnt > 5)
				b->Graphic = Forwards1;
			
			Start_Jump(b, 45); // Slanted Jump //
		} else if (x >= Fin3 && x <= Fin8) {
			b->y			-= 24;
			if (xcnt > 5 && x == Fin8) {
				b->Graphic = Forwards1;
			} else if (x == Fin5 && GetTerrain(g->Old_x + 10) != x) {  // New Lap!!!
				if (Laps_Done(b) + 1 < No_Laps)
					b->attr = (b->attr & ~B_LAPS) | ((Laps_Done(b) + 1) << 8);
				else if (b->Control == g->Calc) {
					g->Status = End_Wheelie;
					clock->Running = FALSE;
				}
				/*if (Laps_Done(b) + 1 < No_Laps)
						sprintf(buf, "Lap %d", Laps_Done(b) + 1);
					else strcpy(buf, "Final Lap");*/
				if (b->Bike_NO == (unsigned short)g->Camera_Bike) {
					char buf[25];
					short msecs = clock->Mseconds50 - g->OldMseconds50;
					register short time = (clock->Minutes - g->OldMinutes);
					if ((time = ((time << 6) - (time << 2)) + (clock->Seconds - g->OldSeconds)) < 0)
						time = 0;
					
					sprintf(buf, EFG("Lap %d:%02d:%02d", "Tour %d:%02d:%02d", "Runde %d:%02d:%02d"), time / 60, 
						time % 60, (msecs + 19 * (msecs < 0)) << 2);
					New_Text_Message(buf);
					
					g->OldMinutes = clock->Minutes; g->OldSeconds = clock->Seconds;
					g->OldMseconds50 = clock->Mseconds50;
				}
			} else if (x == Fin3)
				Start_Jump(b, 45); // Finish Line Jump //
		} else if (x == MO10 || x == MR03) {
			b->y      -= 48;
			
			Start_Jump(b, 40); // Monster Jump //
		} else if (x == SL02 || x == SL14 || x == SL28 || x == SL65 || x == MO25 || x == Fi11) {
			b->Graphic = Forwards3;
			b->y      -= 7 - xcnt;
		} else if (x == SL11) {																				// Medium
			b->Graphic = Back4;
			b->y      -= xcnt + 8;
		} else if (x == SL13 || x == MO24) {
			b->Graphic = Forwards3;
			b->y      -= (7 - xcnt) + 8;
		} else if (x >= SL21 && x <= SL23) {													// Large
			b->Graphic = Back4;
			b->y      -= xcnt + ((x - SL20) << 3);
		} else if (x >= SL25 && x <= SL27) {
			b->Graphic = Forwards3;
			b->y      -= (7 - xcnt) + ((SL28 - x) << 3);
		} else if (x == SL30) {																				// Long
			b->Graphic = Back5 + (xcnt == 5 || xcnt == 6);
			b->y      -= (xcnt >> 1);
		} else if (x >= SL31 && x <= SL33) {
			b->Graphic = Back5;
			b->y      -= (xcnt + ((x - SL30) << 3)) >> 1;
		} else if (x >= SL35 && x <= SL37) {
			b->Graphic = Forwards2;
			b->y      -= ((7 - xcnt) + ((SL38 - x) << 3)) >> 1;
		} else if (x == SL38) {
			b->y      -= (7 - xcnt) >> 1;
			b->Graphic = Forwards2 - (xcnt == 5 || xcnt == 6);
		} else if (x == SL40 || x == SL41 || x == MR00) {							// Tall
			b->Graphic = Back2;
			b->y      -= (xcnt + xcnt) + ((x == SL41) << 3) + 4;
		} else if (x == SL43 || x == SL44) {
			b->Graphic = Forwards4;
			b->y      -= (((7 - xcnt) + ((x == SL43) << 3)) << 1) + 4;
		} else if (x == SL51 || x == SL52) {													// Slanted Right
			b->Graphic = Back4;
			b->y      -= xcnt + ((x - SL50) << 3);
		} else if (x == SL54) {
			b->Graphic = Forwards4 - (xcnt < 1) - (xcnt < 3);
			b->y      -= (26 - xcnt - xcnt);
		} else if (x == SL55) {
			b->Graphic = Forwards4;
			b->y      -= (10 - xcnt - xcnt);
		} else if (x == SL60) {																				// Slanted Left
			b->Graphic = Back2 + (xcnt < 2);
			if (xcnt > 3)
				b->y    -= (xcnt + xcnt) - 4;
		} else if (x == SL61) {
			b->Graphic = Back2 + (xcnt > 4) + (xcnt > 6);
			b->y      -= (xcnt + xcnt) + 8;
		} else if (x == SL63 || x == SL64) {
			b->Graphic = Forwards3;
			b->y      -= (7 - xcnt) + ((SL65 - x) << 3);
		} else if (x == MO01) {																				// Monster Jump
			b->Graphic = Back4;
			b->y      -= xcnt + 8;
		} else if (x == MO23 || (x >= MO03 && x <= MO07)) {
			b->y      -= 16;
		} else if (x == MO08 || x == MR01) {
			b->Graphic = Back2;
			b->y      -= 16 + xcnt + xcnt + 4;
		} else if (x == MO09 || x == MR02) {
			b->Graphic = Back2;
			b->y      -= 32 + xcnt + xcnt + 4;
		} else if (x >= MO11 && x <= MO17) {
			b->y      -= 48;
		} else if (x >= MO18 && x <= MO21) {
			b->Graphic = Forwards3;
			b->y      -= 16 + (7 - xcnt) + ((MO21 - x) << 3);
		} else if (x == MO22) {
			b->Graphic = Regular1 + ((xcnt < 2) + (xcnt < 2));
			b->y      -= 16;
		} else if (x == MR04) {																			// Monster Ramp
			if (Bikey < 2)
				b->Graphic = Regular1 + (xcnt > 2) + (xcnt > 2) + (xcnt > 4);
			b->y      -= 48;
		} else if (x >= MR05 && x <= MR07) {
			if (Bikey > 1) {
				b->y    -= 48;
			} else {
				b->Graphic = Forwards4;
				b->y      -= (((7 - xcnt) + ((MR07 - x) << 3)) << 1) + 4;
			}
		} else if (x == MR08 && Bikey > 1) {
			b->y    -= 48;
		} else if (x >= MR09 && x <= MR25) {
			if (Bikey > 1) {
				b->y    -= 48;
			} else if ((x >= MR11 && x <= MR13) || (x >= MR18 && x <= MR20)) b->attr |= B_MUD;
		} else if (xcnt < 3 && ((x == Blo1 && (Bikey > 1)) || (x == Blo2 && (Bikey < 2)))) {
			// If you're leaning backwards and hit a block, you won't crash
			if (b->Tilt > Back5 || b->Tilt == None)
				Start_Crash(b, 1);  // Crash!
			else if ((b->attr & (B_CRASHING | B_JUMPING)) == FALSE) {
				b->Tilt = None;
				j->Last_Crash = b->x + 8;
				b->attr |= B_BLOCK;
			}
		} else if ((x >= Rok1 && x <= Rok3 && (Bikey & 1)) || 				// Rock/Mud Patch
		(x >= Rok4 && x <= Rok6 && (Bikey & 1) == 0))
			b->attr |= B_MUD;
		else if ((b->attr & B_JUMPING) == FALSE && ((Bikey == 3 && (x == Tur1 || x == Tur2)) || // Turbo Strip
		(Bikey == 0 && (x == Tur3 || x == Tur4))))
			b->Temp = Low_Temp;  // Turbo Arrow
		else if ((Bikey < 2 && (x == Dry0 || x == Dry1)) || 					// Dry Patch
		(Bikey > 1 && (x == Dry2 || x == Dry3)) || (x == Dry4 || x == Dry5))
			b->attr |= B_MUD;
		else if (x == Fin1 || x == Fin2) {														// Finish Line
			b->Graphic = Back4;
			b->y      -= xcnt + ((x - Fin0) << 3);
		} else if (x == Fin9 || x == Fi10) {
			b->Graphic = Forwards3;
			b->y      -= (7 - xcnt) + ((Fi11 - x) << 3);
		}
		/* --- Rear End of Bike --- */
		else if (x0 == SL02 || x0 == SL14 || x0 == SL28 || x0 == SL65 || x0 == MO25 || x0 == Fi11)
			b->Graphic = Forwards2 - (GetTerrain(b->x + 8) != x0);
		else if (x0 == SL38)
			b->Graphic = Forwards2 - (GetTerrain(b->x + 8) != x0);
		else if (x0 == SL44 || x0 == SL55 || x0 == MR07)
			b->Graphic = Forwards4 - (GetTerrain(b->x + 7) != x0) - (GetTerrain(b->x + 8) != x0) - 
			(GetTerrain(b->x + 9) != x0);
		else if ((x0 >= Rok1 && x0 <= Rok3 && (Bikey & 1)) || 
		(x0 >= Rok4 && x0 <= Rok6 && (Bikey & 1) == 0))
			b->attr |= B_MUD;
		else if ((b->attr & B_JUMPING) == FALSE && ((Bikey == 3 && (x0 == Tur1 || x0 == Tur2)) || 
		(Bikey == 0 && (x0 == Tur3 || x0 == Tur4))))
			b->Temp = Low_Temp;  // Turbo Arrow
		else if ((Bikey < 2 && (x0 == Dry0 || x0 == Dry1)) || 
		(Bikey > 1 && (x0 == Dry2 || x0 == Dry3)) || (x0 == Dry4 || x0 == Dry5))
			b->attr |= B_MUD;
		// ~50 if statements...
	}
	
	/* ------------- */
	/* HANDLE A JUMP */
	/* ------------- */
	if (b->attr & B_JUMPING) {
		if (b->Counter > 2 && j->y >= b->y) {  // Bike is Landing
			b->attr   &= ~B_JUMPING;
			b->Counter = 0;
			
			if (b->x > j->Original_x + 12 && ((b->Tilt < Regular1 && b->Graphic > Regular2) || 
				  (b->Tilt > Forwards1 && b->Graphic < Forwards1) || 
				  (b->Control < AI_SLOW && b->Graphic < Back6 && b->Tilt > b->Graphic + 2))) {
				// B_FLAG1 = 1, B_FLAG2 = 0, Rest = 1
				Start_Crash(b, ((b->attr & B_FLAG2) == FALSE));  // Crash!
			} else if (b->attr & B_OVERHEAT) {
				b->Tilt = None;
				b->attr |= B_FLAG1;
				b->delay = 0;
			} else {
				// Slow the Bike down if its Tilt is not centered when Landing
				b->addx -= (ABS((b->Tilt - (b->Tilt > Regular1)) - (b->Graphic - (b->Graphic > Regular1)))) * 12;
				if ((signed char)b->addx < 0)
					b->addx = 0;
				
				// Check if a flip was performed during the Jump
				if (((b->attr & B_FLAG2) && b->Tilt > Back5) || ((b->attr & B_FLAG1) && b->Tilt < Back6)) {
					if (clock->Seconds > 3)  // Timer Bonus
						clock->Seconds -= 3;
					b->attr &= ~(B_FLAG1 | B_FLAG2);
				}
				
				if (b->Graphic != b->Tilt && b->Graphic != Regular1 + g->Animation)
					b->Tilt = None;
				else if (b->Tilt > Back6) {
					b->Tilt = None;
					b->Graphic = Regular1 + g->Animation;
				} else b->Graphic = b->Tilt;
			}
		} else if (b->Counter > 0) {
			// Draw Bike's Shadow on Terrain beneath Bike
			GrayClipSprite8_OR_R(Real_x(b->x + 9), b->y + 21 - g->softy, 3, Shadow, Shadow, Hiddenl, Hiddend);
			
			b->Graphic = b->Tilt;
			b->y = j->y;
		}
	} else if (b->attr & B_CRASHING) {
		
		/* -------------- */
		/* HANDLE A CRASH */
		/* -------------- */
		
		/* 
		There are 3 Stages to a Crash
			1) The Bike Tumbling
			2) The Bike and the Rider being thrown upward
			3) The Rider Running back towards the Bike
		*/
		
		if (b->Counter < 0) {
			b->Graphic = Crashed;  // Bike Laying on its side
			b->delay++;
			
			if (b->attr & B_FLAG1) {  // Rider and Bike are Sliding Upwards
				// Draw Rider on its side
				GrayClipSprite16_MASK_R(Real_x(b->x - b->Counter + 10), b->y + 10 + ((b->Counter * ((b->y + 10) - 
				j->y)) >> 4) - g->softy, 14, Rider[0], Rider[0] + 16, Rider[0] + 32, Rider[0] + 32, Hiddenl, 
				Hiddend);
				
				// Move Bike
				b->y += (b->Counter * (b->y - (g->lcd_height - 48 - 24))) >> 4;
				
				if (b->Counter < -16) {
					b->attr &= ~B_FLAG1;
					b->Counter = -1;
				} else if (b->delay >= 5) {
					b->delay = 0;
					b->Counter--;
				}
			} else {
				b->y = j->y - b->Counter;
				
				if (b->y >= g->lcd_height - 48 - 20) {  // Rider is getting Back on the Bike
					
					if (b->delay >= 0) {
						b->delay = 0;
						b->attr &= ~B_FLAG3;
					}
					if ((b->delay -= 2) < -22)
						b->attr |= B_FLAG3;
					
					if (b->delay < -44) {  // Crash is Completely Finished
						// Align Bike on the Top Edge of the Track and reset all Crashing Values
						b->attr = (b->attr & ~(B_CRASHING | B_FLAG1 | B_FLAG2 | B_FLAG3 | 3)) | B_TURNING | 
							(3 - ((x >= MR09 && x <= MR25) << 1));
						b->Counter = -5;
					}
					b->Graphic = EndCrash1 + ((b->attr & B_FLAG3) != FALSE);
					b->y = g->lcd_height - 48 - 24;
				} else {  // Rider is Running back towards Bike
					x2 = 1 + ((b->attr & B_FLAG3) == FALSE);
					
					/*  Tapping Gas or Turbo while Running Back to your Bike will speed up the Process */
					/* This is a Cheat from the Original NES Version */
					if (b->Control == g->Calc) {
						if (_KeyCheck(GASKEY) || _KeyCheck(TURBOKEY) || g->Cheats & C_RECOVER) {
							if ((b->attr & B_FLAG2) == FALSE || g->Cheats & C_RECOVER) {
								b->Counter -= 2 + (g->Cheats & C_RECOVER);
								b->delay = 20;
								b->attr |= B_FLAG2;
							}
						} else if (b->attr & B_FLAG2) b->attr &= ~B_FLAG2;
					}
					
					// Draw Rider Running
					GrayClipSprite16_MASK_R(Real_x(b->x + 26 + ((b->Counter << 4) / ((g->lcd_height - 48 - 20) - 
					j->y))),b->y - g->softy, 16, Rider[x2], Rider[x2] + 16, Rider[x2] + 32, Rider[x2] + 32, 
					Hiddenl, Hiddend);
					
					if (b->delay >= 20) {
						b->Counter--;
						b->attr ^= B_FLAG3;  // Animate Rider Running
						b->delay = 0;
					} else if (b->delay == 10) b->Counter--;
					
					b->y = g->lcd_height - 48 - 24;  // Bike sitting at Top of Track
				}
			}
		} else {  // Bike is Tumbling
			b->Graphic = (b->Counter & 15);
			if (b->attr & B_FLAG3)
				b->Graphic = 15 - b->Graphic;
			
			if (b->Counter > 7 && (g->delay & 3) == 0)
				b->Counter--;
			
			if (b->Counter <= 7 || (b->attr & B_FLAG1 && xcnt >= 5 && xcnt <= 6)) {
				if (b->y == (signed short)(g->lcd_height-29-12*Bike_y(b))) {  // First Part of Crash is Finished
					// Set up Variables for Second Part of Crash (Rider and Bike Sliding Up)
					b->attr = (b->attr & ~B_FLAG2) | B_FLAG1;
					b->Counter = -1;
					j->y = ((CALCULATOR > 0) << 5) + random(96 - 72) - (b->addx >> 2);
					//j->y = g->lcd_height - 48 - random(100 - 72) - (b->addx >> 2);
					g->jumpdat[b->Bike_NO].Last_Crash = b->x;
					b->addx = 0;
					b->delay = 0;
				} else {
					if (b->Counter <= 7)
						b->Counter += 16;
					b->attr |= B_FLAG1;
				}
			}
		}
	} else if (b->attr & B_OVERHEAT) {  // Bike has Overheated
		if ((g->text_attr & 1) == 0 && g->delay == 0)
			b->Temp = Max_Temp * (!b->Temp);
		
		if (b->Counter < 0) {  // Bike is sitting at Top of Track
			b->Counter--;
			b->y -= (43 - 12 * Bike_y(b));
			
			if (b->Counter < -412) {  // Bike's temp is back to normal  // -448
				// Align Bike on the Top Edge of the Track
				b->attr = (b->attr & ~(B_OVERHEAT | B_FLAG1 | 3)) | B_TURNING | 
					(3 - ((x >= MR09 && x <= MR25) << 1));
				b->Counter = -5;
				b->Temp = Low_Temp;
			}
		} else {  // Bike is drifting towards the top of the Track
			b->addx = (b->addx >> 1);
			
			if ((g->delay & 3) == 0)
				b->Counter++;
			// Move Bike
			b->y -= (b->Counter * (43 - 12 * Bike_y(b))) >> 4;
			
			if (b->Counter > 15)
				b->Counter = -1;
		}
	} else {  // Bike is not Jumping, Overheated, or Crashing
		if (b->Graphic != b->Tilt && b->Graphic != Regular1 + g->Animation)
			b->Tilt = None;
		
		if (b->attr & B_MUD) {
			b->attr &= ~(B_MUD | B_BLOCK);
			
			// Draw dirt flying up behind Bike
			if (b->addx > 28) {
				Bikey = b->y + 16 - g->softy - ((b->Graphic == DownTurn) * 3);
				
				if (random(7)) {
					if (random(4))
						GrayClipSprite8_OR_R(Real_x(b->x) - 9, Bikey, 8, Sparks[0], Sparks[0], Hiddenl, Hiddend);
					else GrayClipSprite8_OR_R(Real_x(b->x) - 9, Bikey, 8, Sparks[1], Sparks[1], Hiddenl, Hiddend);
				}
				if ((g->delay & 3) == 3 || random(3) == 2)
					GrayClipSprite8_OR_R(Real_x(b->x) - 17, Bikey - 8, 8, Sparks[1], Sparks[1], Hiddenl, Hiddend);
			}
			
			b->addx = (b->addx >> 1);  // Slow Bike down in Mud
			if (b->addx < 32)
				b->addx = (28 + (((g->delay & 1) == FALSE) << 2)) * 
				(Is_Computer(b->Control) || _KeyCheck(GASKEY) || _KeyCheck(TURBOKEY));
		}
	}
	
	b->Old_Graphic = b->Graphic;
	// Adjustment for when the Bike enters a new 'Camera' Lap
	if (b->x > (g->Width << 3) + 8 - LCD_WIDTH) {
		b->x -= (g->Width << 3) + 8;
		j->Original_x -= (g->Width << 3) + 8;
		j->Last_Crash -= (g->Width << 3) + 8;
	}
	
	/*if (b->x > (g->Width << 3) + 8 - (b->Bike_NO == (unsigned short)g->Camera_Bike ? LCD_WIDTH : 0)) {
		b->x -= (g->Width << 3) + 8;
		j->Original_x -= (g->Width << 3) + 8;
		j->Last_Crash -= (g->Width << 3) + 8;
	}*/
	
	/*PortSet(Hiddend, 239, 127);
		printf_xy(0, 0, "%d %d", xcnt, b->y);
			PortSet(Hiddenl, 239, 127);
				printf_xy(0, 0, "%d %d", xcnt, b->y);
			if (_keytest(RR_CLEAR)) b->addx = 38;
		Draw_Bike(b);
	CopyGrayScreens(Hiddenl, Hiddend, Activel, Actived);*/
}

/*
		PortSet(Hiddend, 239, 127);
		printf_xy(0, 0, "%d %d", xcnt, b->y);
		PortSet(Hiddenl, 239, 127);
		printf_xy(0, 0, "%d %d", xcnt, b->y);
		Draw_Bike(b);
		CopyGrayScreens(Hiddenl, Hiddend, Activel, Actived);
		
		while(!_keytest(RR_CLEAR));
		while(_keytest(RR_CLEAR));*/

/* Draw the Bikes Layered, so the Bikes closest to the Screen will be drawn Last */
static inline void Draw_All_Bikes(void) {
	unsigned char Used[MAX_BIKES] = { 0, 0, 0, 0 };
	short a, b, Back_Bike, y;
	
	for(a = 0; a < g->No_Bikes; a++) {
		Back_Bike = 0, y = 140;
		for(b = 0; b < g->No_Bikes; b++) {
			if (Used[b] == FALSE && g->bikes[b].y < y) {
				Back_Bike = b;
				y = g->bikes[b].y;
			}
		}
		
		Used[Back_Bike] = TRUE;
		Draw_Bike(&g->bikes[Back_Bike]);
	}
}

/* Draw one Clipped Bike */
static inline void Draw_Bike(Bike *b) {
	
	GrayClipSprite32_MASK_R(Real_x(b->x), b->y - g->softy, 24, BikeGfx[b->Graphic], BikeGfx[b->Graphic] + 
	24, BikeGfx[b->Graphic] + 48, BikeGfx[b->Graphic] + 48, Hiddenl, Hiddend);
	
	/*if (b->attr & B_OVERHEAT) {
		if (g->Calc == Host) {
			PortSet(Hiddend, 239, 127);
			printf_xy(0, 0, "%d %d %d %d", g->delay, b->Counter, b->y, (b->attr & B_TURNING) != FALSE);
			PortSet(Hiddenl, 239, 127);
			printf_xy(0, 0, "%d %d %d %d", g->delay, b->Counter, b->y, (b->attr & B_TURNING) != FALSE);
			CopyGrayScreens(Hiddenl, Hiddend, Activel, Actived);
			
			while(!_keytest(RR_CLEAR));
			while(_keytest(RR_CLEAR));
		}
		
		Align_Calcs();
	}*/
}

static inline void Handle_User_Bike(Bike *b) {
	if (b->attr & B_OVERHEAT)
		return;
	
	// Check for KeyPresses
	if (_KeyCheck(RIGHTKEY) && ((b->attr & B_JUMPING) || b->Tilt < Regular1)) {  // Tilt Forwards
		if (No_Tilt(b) || b->Tilt == Regular1)
			b->Tilt = Forwards1;
		else if (g->delay == 4 && _KeyCheck(UPKEY) && b->Tilt > Forwards4) {
			// Allow for Frontflips by holding Up and Right when Jumping!
			if (++b->Tilt > Crash4) {
				b->Tilt = Back1;
				b->attr |= B_FLAG1;
			}
		} else if (g->delay == 0) {
			if (b->Tilt < Forwards4)
				b->Tilt++;
			else if (b->Tilt > Forwards4 && ++b->Tilt > Crash4)
				b->Tilt = Back1;
		}
	}	
		
		
		/*else if (g->delay == 0 || (g->delay == 4 && b->Tilt > Forwards4 && _KeyCheck(UPKEY))) {
			// Allow for Frontflips by holding Up and Right when Jumping!
			if (_KeyCheck(UPKEY) || b->Tilt > Forwards4) {
				if (++b->Tilt > Crash4) {
					b->Tilt = Back1;
					if (_KeyCheck(UPKEY))
						b->attr |= B_FLAG1;
				}
			} else if (b->Tilt < Forwards4)
				b->Tilt++;
		}
	} */
	
	else if (_KeyCheck(LEFTKEY) && ((b->attr & B_JUMPING) || b->addx > 32)) {  // Tilt Backwards
		if ((b->attr & B_BLOCK) == FALSE) {
			if (No_Tilt(b))
				b->Tilt = Back6;
			else if (g->delay == 0 || (g->delay == 4 && b->Tilt > Forwards4)) {
				if (_KeyCheck(DOWNKEY)) {  // Allow for Backflips by holding Down and Left when Jumping!
					if (b->Tilt-- <= Back1) {
						if (b->attr & B_JUMPING) {
							b->Tilt = Crash4;
							b->attr |= B_FLAG2;
						} else Start_Crash(b, 0);  // Crash!
					}
				} else if (b->Tilt > ((b->attr & B_JUMPING) != FALSE)) {
					b->Tilt--;
				} else if ((b->attr & B_JUMPING) == FALSE)
					Start_Crash(b, 0);  // Crash!
			}
		}
	} else {
		b->attr &= ~B_BLOCK;
		if (b->Tilt != None && g->delay == 0 && (b->attr & B_JUMPING) == FALSE && ++b->Tilt >= Regular1)
			b->Tilt = None;
	}
	
	if (_KeyCheck(GASKEY) == FALSE && _KeyCheck(TURBOKEY) == FALSE && g->delay==0 && b->Temp>Low_Temp)
		b->Temp--;
	
	if ((b->attr & B_CRASHING) == FALSE) {
		if (_KeyCheck(TURBOKEY)) {
			b->attr |= B_TURBO;
			
			if ((b->attr & B_JUMPING) == FALSE)
				b->addx += 6;
			if (g->delay == 0 && ++b->Temp > Max_Temp) {
				b->Temp = Max_Temp;
				
				if ((g->Cheats & C_OVERHEAT) == FALSE) {  // Never Overheat Cheat
					b->attr = (b->attr & ~B_BLOCK) | B_OVERHEAT;
					New_Text_Message(EFG("OVER HEAT", "SURCHAUFFE MOTEUR", "ÜBERHITZUNG"));
					
					if ((b->attr & B_JUMPING) == FALSE) {
						b->Tilt = None;
						b->attr |= B_FLAG1;
						Stop_Turning(b);
					}
					
					return;
				}
			}
		} else {
			b->attr &= ~B_TURBO;
			
			if (_KeyCheck(GASKEY)) {
				if (g->delay == 0 && (b->Temp > Middle_Temp || ++b->Temp > Middle_Temp))
					b->Temp--;
				if ((b->attr & B_JUMPING) == FALSE)
					b->addx += 4;
			}
		}
		
		if ((b->attr & B_JUMPING) == FALSE && (b->attr & B_TURNING) == FALSE) {
			if (_KeyCheck(UPKEY)) {
				b->Counter = -1;
				b->attr |= B_TURNING;
			} else if (_KeyCheck(DOWNKEY)) {
				b->Counter = 1;
				b->attr |= B_TURNING;
			}
		}
	}
}

/* Update a Bike's x and y coordinates */
static inline void Move_Bike(Bike *b) {
	
	if (Is_Computer(b->Control) == FALSE) {
		if ((b->attr & B_JUMPING) == FALSE && (b->attr & B_CRASHING) == FALSE)
			b->addx -= ((b->addx > 0)) + (b->addx > 1);  // Friction
		
		if (b->addx > (g->Max_xspeed + ((b->attr & B_TURBO) != FALSE)) << 5)  // Restrict Speed of Bike
			b->addx = (g->Max_xspeed + ((b->attr & B_TURBO) != FALSE)) << 5;
	}
	
	// Handle a Bike's turn
	if (b->attr & B_TURNING) {
		register char Track_y = Bike_y(b) + (b->Counter < 0) - (b->Counter > 0);
		if (GetTerrain(b->x + 10) >= MR05 && GetTerrain(b->x + 10) <= MR26) {
			if (Bike_y(b) == 2 && Track_y == 1)
				Track_y = -1;
			else if (Bike_y(b) == 1 && Track_y == 2)
				Track_y = 4;
		}
		b->Counter += (b->Counter > 0) - (b->Counter < 0);
		
		// Make sure bike does not turn off of the track
		if (Track_y < 0) {
			if (!_KeyCheck(DOWNKEY) || b->Control != g->Calc) {
				b->Counter -= 2 - (b->Counter == 1);
				if (b->Counter <= 0)
					b->attr &= ~B_TURNING;  // Bike is no longer Turning
			} else if (b->Counter > 5) { b->attr |= B_MUD; b->Counter = 5; }
		} else if (Track_y > 3) {
			if (!_KeyCheck(UPKEY) || b->Control != g->Calc) {
				b->Counter += 2 - (b->Counter == -1);
				if (b->Counter >= 0)
					b->attr &= ~B_TURNING;  // Bike is no longer Turning
			} else if (b->Counter < -5) { b->attr |= B_MUD; b->Counter = -5; }
		}
		
		// Check if Bike should no longer be Turning
		if (ABS(b->Counter) >= 12) {
			Stop_Turning(b);
			if (g->Calc == b->Control) {
				if (_KeyCheck(UPKEY)) {
					b->Counter = -1;
					b->attr |= B_TURNING;
				} else if (_KeyCheck(DOWNKEY)) {
					b->Counter = 1;
					b->attr |= B_TURNING;
				}
			}
		} else b->y += b->Counter;
	}
	
	if (b->attr & B_JUMPING) {
		JumpDat *j = &g->jumpdat[b->Bike_NO];
		b->Counter++;
		
		// The Formula for Bike Jumping Physics
		// Several multiplactions and divisions are saved by only calculating certain variables once
		// at the beginning of a Jump
		b->x = j->Original_x + ((b->Counter * j->cosine_Velocity) / 12);
		j->y = j->Original_y - (((b->Counter * j->sine_Velocity) >> 4) - (((5 * ((long)b->Counter * 
					 b->Counter))) >> 7));
	} else if (b->attr & B_CRASHING) {
		if (b->Counter >= 0 && g->delay != 0)
			b->x++;
	} else b->x += (b->addx >> 5);  // Bike is moving normally
}

/*
// Copy the contents of one (16 byte) Bike struct into another
void Copy_Bike(Bike *dest, Bike *src) {
	*(long)((char*)dest + 0)  = *(long)((char*)src + 0);
	*(long)((char*)dest + 4)  = *(long)((char*)src + 4);
	*(long)((char*)dest + 8)  = *(long)((char*)src + 8);
	*(long)((char*)dest + 12) = *(long)((char*)src + 12);
}*/

/* ------------------ */
/* HANDLE COMPUTER AI */
/* ------------------ */
static inline void Handle_Computer_Bike(Bike *b) {
	if (b->attr & B_CRASHING)
		return;
	register short x = 64 + ((b->Control - AI_MEDIUM) << 5) + random(28);
	if (g->delay == 0)
		b->Temp += (b->Temp < Middle_Temp) - (b->Temp > Middle_Temp);
	
	/* Make adjustments to the Bike's behavior */
	if (b->attr & B_JUMPING) {
		if (b->Tilt != None && g->delay == 0)  // Align Bike's Tilt to Regular Terrain
			b->Tilt += (b->Tilt < Regular1) - (b->Tilt > Regular1);
	} else {
		b->addx += (random(12) != FALSE) << 2;
		if (b->addx > x)  // Restrict Speed of Bike
			b->addx = x;
		
		if ((b->attr & B_TURNING) == FALSE && b->addx > 28) {
			register short Bikey = RealBike_y(b), x2 = GetTerrain(b->x + 28 + 24);
			if (x2 == Gra1)
				x2 = GetTerrain(b->x + 28 + 12);
			x = GetTerrain(b->x + 28);
			
			// Basic Bike AI -- Avoid Hazardous Obstacles
			if ((Bikey & 1 && x >= Rok1 && x <= Rok3) || ((Bikey & 1) == FALSE && x >= Rok4 && x <= Rok6)) {
				b->Counter = (Bikey != FALSE) - (((random(2) || Bikey == FALSE) && Bikey != 3) << 1);
				b->attr |= B_TURNING;
			} else if ((Bikey>1 && (x==Blo1||x==Dry2||x==Dry3))||(Bikey==3&&(x2==Blo1||x2==Dry2||x2==Dry3))) {
				b->Counter = 1;
				b->attr |= B_TURNING;
			} else if ((Bikey<2 && (x==Blo2||x==Dry0||x==Dry1))||(Bikey==0&&(x2==Blo2||x2==Dry0||x2==Dry1))) {
				b->Counter = -1;
				b->attr |= B_TURNING;
			} else if (random(180) == 4) {
				b->Counter = 1 - (random(2) << 1);
				b->attr |= B_TURNING;
			}
		}
	}
}

/* Check if it is valid for the bike to crash and if so, Initiate the bike's crash */
void Start_Crash(Bike *b, short Direction) {
	// Check if Bike is already Jumping or Crashing
	if (b->attr & B_JUMPING || b->attr & B_CRASHING || (b->x < g->jumpdat[b->Bike_NO].Last_Crash + 8) || 
	(g->Status == End_Wheelie && g->Calc == b->Control))
		return;
	Stop_Turning(b);
	
	// Set up all variables needed to run a Crash
	b->attr    = (b->attr & ~(B_OVERHEAT | B_FLAG3 | B_BLOCK)) | B_CRASHING;
	b->Counter = (b->addx - 24 - (((b->attr & B_TURBO) != FALSE) << 4)) & ~15;
	b->Tilt    = None;
	if (b->Counter < 16)  // Bike has to flip at least once
		b->Counter = 16;
	if (Direction) {  // Bike will be flipping forwards
		b->Counter += (14 - b->Old_Graphic);
		b->attr    |= B_FLAG3;
	}
}

/* Check if it is valid for the bike to start a jump and if so, Initiate the bike's jump */
void Start_Jump(Bike *b, unsigned long Initial_Velocity) {
	register JumpDat *j  = &g->jumpdat[b->Bike_NO];
	
	// Check if Bike is able to Jump
	if (b->attr & (B_JUMPING | B_CRASHING) || (b->x < j->Original_x + 8))
		return;
	
	// Allow for the Bike's speed to (partly) determine the speed of the Bike's Initial Velocity
	if (b->addx < (g->Max_xspeed << 5))
		Initial_Velocity -= ((g->Max_xspeed << 5) - b->addx) >> 1;
	else Initial_Velocity += (b->addx >> 4);
	
	if (b->addx < 45 && b->Control != AI_SLOW)
		Initial_Velocity = 10;
	Stop_Turning(b);
	
	if (g->Cheats & C_MOON)  // Moon Gravity
		Initial_Velocity += (Initial_Velocity >> 2);
	
	b->attr              = (b->attr & ~(B_FLAG1 | B_FLAG2 | B_BLOCK)) | B_JUMPING;
	b->Tilt              = b->Old_Graphic;
	b->Graphic           = b->Old_Graphic;
	b->Counter           = 0;
	//register short Angle = (short)(15 * (Back6 - b->Old_Graphic + 1) * PI/180);
	//register short Angle = (short)((45 + ((Back4 - b->Old_Graphic) << 3)) * PI/180);
	j->Original_x        = b->x;
	j->Original_y        = b->y;
	// Tell Angle of the Jump by the Bike's Tilt  (cos[x] = Sin_Table128[4 - x];)
	j->cosine_Velocity   = (Initial_Velocity * Sin_Table128[4 - (Back6 - b->Old_Graphic)]) >> 7;
	j->sine_Velocity     = (Initial_Velocity * Sin_Table128[Back6 - b->Old_Graphic]) >> 7; // x >> 7 = x / 128
	if (b->Control == g->Calc) {
		// Allow the User to determine which type of Jump they want by which way they want 
		// the Bike to tilt.  Pressing Back (Left) will give a shorter, higher Jump.  
		// Pressing Forwards (Right) will give a longer, lower Jump.
		j->cosine_Velocity += ((_KeyCheck(RIGHTKEY)) - (_KeyCheck(LEFTKEY))) << 2;
		j->sine_Velocity   -= ((_KeyCheck(RIGHTKEY)) - (_KeyCheck(LEFTKEY))) << 2;
	} else {  // Computer AI has more random jumping
		j->cosine_Velocity += 1 - (random(2) << 1);
		j->sine_Velocity   -= 1 - (random(2) << 1);
	}
	if (j->cosine_Velocity < 0)
		j->cosine_Velocity = 0;
	if (j->sine_Velocity < 0)
		j->sine_Velocity = 0;
	
	// Original Formula for Bike Jumping Physics
	//-- Taken from my Tanks Game in Ti-83+ Basic (the missile trajectory code)
	//Time + .08 -> Time;
	//Original_y + Velocity * Time * sin(angle) - 9.8/2(Time*Time) -> y;
	//Original_x + Velocity * Time * cos(angle) -> x;
}

void Stop_Turning(Bike *b) {
	if (b->attr & B_TURNING) {
		b->attr &= ~B_TURNING;
		if (ABS(b->Counter) > 5 && !((Bike_y(b)==0 && b->Counter>0) || (Bike_y(b)==3 && b->Counter<0))) {
			b->attr = (b->attr & ~B_Y) | (Bike_y(b) + (b->Counter < 0) - (b->Counter > 0));
			b->y += 12 * ((b->Counter > 0) - (b->Counter < 0));
		}
		b->Counter = 0;
	}
}

void New_Text_Message(const char *text) {
	strcpy(g->text, text);
	g->text_attr = TEXT_ANIM;
}

static inline void Handle_Finish_Wheelie(Bike *b) {
	b->attr &= ~(B_TURNING | B_CRASHING | B_OVERHEAT);
	
	if (b->Counter < 160 || b->attr & B_JUMPING) {
		if (No_Tilt(b))
			b->Tilt = Regular1;
		else if ((b->attr & B_JUMPING) == FALSE && (b->Tilt == Back4 || b->Tilt == Finish_Wheelie))
			b->Tilt = Finish_Wheelie;
		else if (g->delay == 0)
			b->Tilt += (b->Tilt < Back4) - (b->Tilt > Back4);
		
		b->addx = 40;
		b->Counter++;
	} else if (g->Link == TRUE) {
		b->addx = 0;
		b->Tilt = None;
	} else g->Status = Race_Finished;
}

// Returns an x coordinate in relation to the Camera
static inline short Real_x(short x) {
	register short __x = x - ((g->camera_x - (LCD_WIDTH >> 3)) << 3) + g->softx;
	//register short __x = x + (LCD_WIDTH - (g->camera_x << 3)) + g->softx;
	
	// Correction if a Rider is on a different Lap than the Camera
	while(__x < -((g->Width << 3) + 8 - LCD_WIDTH))
		__x += (g->Width << 3) + 8;
	
	return __x;
}