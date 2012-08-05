// C Source File
// Created 9/14/2003; 3:07:05 PM
#include "All.h"

void Update_Plrs(void) {
	short a, New = 0;
	
	/*	Team 1	*/
	for(a = 0; a < 3; a++) {
		if (g->Calc == Join) {
			Draw_Plr(Team1->plrs[a]);
			continue;
		}
		
		// Has this plr just won control of the puck?
		if (BOUNDS_COLLIDE(g->x - 2, g->y - 2, Team1->plrs[a]->x + 12*(Team1->plrs[a]->face), 
		Team1->plrs[a]->y + 11, 8, 7) && Team1->plrs[a]->Control == FALSE && New == FALSE && 
		g->Is_Controlled != 1) {
			New_Control(a);
			New = 1;
		}
		
		if (g->Calc == Host && g->Cur_Plr == a)
			Move_Cur_Plr(Team1->plrs[a]);
		else Move_Computer_Plr(Team1->plrs[a]);
		
		Draw_Plr(Team1->plrs[a]);
	}
	
	// Team 2 is not updated or drawn in PRACTICE Mode
	if (g->Mode == PRACTICE)
		return;
	
	/*	Team 2	*/
	for(a = 0; a < 3; a++) {
		if (g->Calc == Host && g->Link == TRUE) {
			Draw_Plr(Team2->plrs[a]);
			continue;
		}
		
		#if !defined(DEBUG)  // Team1 is 'invincible' in DEBUG mode and Team2 cannot acquire the puck
		if (BOUNDS_COLLIDE(g->x - 2, g->y - 2, Team2->plrs[a]->x + 12*(Team2->plrs[a]->face), 
		Team2->plrs[a]->y + 11, 8, 7) && Team2->plrs[a]->Control == FALSE && New == FALSE && 
		g->Is_Controlled != 2) {
			New_Control(a + 3);
			New = 1;
		}
		#endif
		
		if (g->Cur_Plr == a && g->Link == TRUE)
			Move_Cur_Plr(Team2->plrs[a]);  // Move current player if this is the Joining calc in Multiplayer
		else Move_Computer_Plr(Team2->plrs[a]);
		
		Draw_Plr(Team2->plrs[a]);
	}
}

// Transfers control of the puck from one plr to another
void New_Control(const short Player) {
	short a;
	
	// Prevent two players from controlling the puck at the same time
	for(a = 3; a--;) {
		if (Team1->plrs[a]->Control > 0)
			Team1->plrs[a]->Control = -1;
		if (Team2->plrs[a]->Control > 0)
			Team2->plrs[a]->Control = -1;
	}
	
	for(a = 3; a--;) {
		if (Player == a) {
			Team1->plrs[a]->Control = TRUE;
			if (g->Calc == Host)
				g->Cur_Plr = a;
		} else if (Player - 3 == a) {
			Team2->plrs[a]->Control = TRUE;
			if (g->Calc == Join)
				g->Cur_Plr = a;
		}
	}
	g->Is_Controlled = 1 + (Player > 2);
	g->Alert         = 0;
}

void Draw_Plr(Plr *plr) {
	if (plr->Shooting) {  // Move the player to the Shooting Animation
		plr->anim = 4;
		if (plr->Control <= 0) {
			g->Shooting = FALSE;
			plr->Shooting  = FALSE;
		}
	} else if (plr->addx || plr->addy) {  // Update Plr Animation only if moving
		if (++plr->anim_delay > 3) {
			plr->anim_delay = 0;
			plr->anim = !plr->anim;
		}
	} else {  // Plr is standing still
		plr->anim = 2;
	}
	if (plr->Control > 0) {  // Plr has control of Puck
		g->x = plr->x + 14 * (plr->face) - ((plr->type == Skinny) * 2);
		g->y = plr->y + 12 + ((plr->type == Skinny) * 2);
		g->addx = plr->addx << 5;
		g->addy = plr->addy << 5;
	} else if (plr->Control < 0 && --plr->Control < -15 + ((plr->type == Fat) << 3)-((plr->type==Skinny)<<2))
		plr->Control = FALSE;  // Plr just lost puck and cannot get it back for 5 frames
	
	if (plr->Control > 0) {
#if 0
		register short x = plr->x-(g->camera_x<<4)-g->softx + ((16 - DrawStrWidth(buf, F_4x6)) >> 1);
		if (x + DrawStrWidth(buf, F_4x6) > 239)
			x -= (x + DrawStrWidth(buf, F_4x6)) - 239;
		FS_DrawGrayString(x, plr->y - (g->camera_y << 4) - g->softy + 16 + ((plr->type == Skinny) * 2), 
		buf, A_NORMAL, F_4x6);  // Show Player's Name underneath their Sprite
#endif
		unsigned char *str;
		if (plr->team == 1) {
			str = Team1->Conference ? Eastern[Team1->No].PlrNames[plr->No] : Western[Team1->No].PlrNames[plr->No];
		} else {
			str = Team2->Conference ? Eastern[Team2->No].PlrNames[plr->No] : Western[Team2->No].PlrNames[plr->No];
		}
		
		// Show Player's Name Centered at the Top of the Screen
		FS_DrawGrayString((LCD_WIDTH - DrawStrWidth(str, F_4x6)) >> 1, 0, str, A_NORMAL, F_4x6);
		
		//GrayCircle_Clipped(g->x-(g->camera_x<<4)-g->softx + ((!plr->face)<<3) - (plr->face<<3), 
		//g->y-(g->camera_y<<4)-g->softy-3, 24);
		
		short x = g->x-(g->camera_x<<4)-g->softx + ((!plr->face)<<3) - (plr->face<<3);
		short y = g->y-(g->camera_y<<4)-g->softy-3;
		ClipFastOutlinedCircle_R(Hiddenl, x, y, 24);
		ClipFastOutlinedCircle_R(Hiddend, x, y, 24);
	}
	
	// Make plr blink if they have the puck
	if (plr->Control > 0 && ++g->Control_Delay > 1)
		g->Control_Delay = 0;
	else {
		if (plr->type == Skinny)
			Clipped_Plr(plr->x-(g->camera_x<<4)-g->softx, plr->y-(g->camera_y<<4)-g->softy, 
			SkinnyGfx[plr->anim+5*(plr->face)], plr->team + 2);
		else if (plr->type == Goalie)
			Clipped_Plr(plr->x-(g->camera_x<<4)-g->softx, plr->y-(g->camera_y<<4)-g->softy, 
			PlrGfx[Goalie + plr->face], plr->team);
		else Clipped_Plr(plr->x-(g->camera_x<<4)-g->softx, plr->y-(g->camera_y<<4)-g->softy, 
				 PlrGfx[plr->type + plr->anim+5*(plr->face)], plr->team);
	}
	
	if (g->Cur_Plr == plr->No && ((g->Calc == Host && plr->team == 1) || (g->Calc == Join && plr->team == 2)))
		Clipped_Plr(plr->x-(g->camera_x<<4)-g->softx + 16, plr->y-(g->camera_y<<4)-g->softy, Hand, 1);
}

/*	-----------------------	 */
/*	ARTIFICIAL INTELLIGENCE	 */
/*	-----------------------	 */
void Move_Computer_Plr(Plr *plr) {
	register short x = plr->x + (13 - ((plr->type == Skinny) * 2)) * (plr->face);
	register short y = plr->y + 13 + ((plr->type == Skinny) * 2);
	
	/* Check if Plr is close enough to the puck to go after it */
	if ((plr->type != Goalie && g->Is_Controlled == FALSE) || (Dist(x, y, g->x, g->y) < (plr->type == 
	Goalie ? 50 : 90) && g->Is_Controlled != plr->team)) {
		if (g->Is_Controlled && random(10) > 4 && plr->type != Goalie) {
			Friction(plr);
			return;
		}
		/* Check if Plr is impeded by a goal to get to the puck */
		if (plr->y > 65 && plr->y < 140 && ((g->x >= 54 && plr->x < 67) || 
		(g->x > 409 && plr->x < 415 && ((plr->x > 409 && (plr->y < 69 || plr->y > 134)) || plr->x < 410)))) {
			plr->addx += 3;
			plr->addy += ((plr->y > 94) * 2) - ((plr->y < 94) * 2);
		} else if (plr->y > 65 && plr->y < 140 && ((plr->x > 381 && g->x < 400) || (g->x <= 54 && 
		plr->x > 32 && ((plr->x < 38 && (plr->y < 69 || plr->y > 134)) || plr->x > 38)))) {
			plr->addx -= 3;
			plr->addy += ((plr->y > 94) * 2) - ((plr->y < 94) * 2);
		} else {
			/* Check if puck is right by the net, which can sometimes cause players to get caught on 
			the edges of the goals */
			if (!(g->x < 69 && plr->x < 69 && (plr->y>134 || plr->y<69))) {
				if (g->x < 54)
					plr->addx += (g->x - 4 > x) - (g->x - 4 < x);
				else plr->addx += (g->x + 4 > x) - (g->x + 4 < x);
			} else if (!(g->x > 395 && plr->x > 381 && (plr->y>134 || plr->y<69))) {
				if (g->x > 409)
					plr->addx += (g->x + 4 > x) - (g->x + 4 < x);
				else plr->addx += (g->x - 4 > x) - (g->x - 4 < x);
			} else plr->addx += (g->x > x) - (g->x < x);
			/* Make Goalies react to where the puck is going, if the puck is close enough to make the save */
			if (plr->type == Goalie) {
				if (plr->team == 1 && g->x < 87 && g->addx < 0) {
					if (g->addy < 0 && y >= g->y - 7)
						plr->addy -= 2;
					else if (g->addy > 0 && y <= g->y + 7)
						plr->addy += 2;
				} else if (plr->team == 2 && g->x > 371 && g->addx > 0) {
					if (g->addy < 0 && y >= g->y - 7)
						plr->addy -= 2;
					else if (g->addy > 0 && y <= g->y + 7)
						plr->addy += 2;
				} else {
					plr->addy += (g->y > y) - (g->y < y);
					if (g->Calc == Host && g->Link == FALSE && plr->team == 2 && g->Difficulty == TRUE)
						plr->addy += (g->y > y) - (g->y < y);  // Make the Computer Goalie Slightly Better
				}
			} else plr->addy += (g->y > y) - (g->y < y);
		}
		if (plr->type != Goalie)
			plr->face = (plr->addx >= 0);  // Which direction is the plr facing
		else plr->face = (g->x > plr->x + 4 + 9 * (plr->face));
	} else if (plr->Control <= 0) {
		/* Make computer Plr skate back to it's original position in the Formation */
		/* Check if the Plr is stuck behind one of the Goals */
		if (plr->x < 38 && (g->x > 38 || (g->Is_Controlled == plr->team && g->x < 38))) {
			plr->addx++;  // Plr is behind one of the Left Goal
			plr->addy += ((plr->y > 94) * 2) - ((plr->y < 94) * 2);
		} else if (plr->x > 410 && (g->x < 410 || (g->Is_Controlled == plr->team && g->x > 410))) {
			plr->addx--;  // Plr is behind the Right Goal
			plr->addy += (plr->y > 94) - (plr->y < 94);
			if (g->Difficulty == TRUE || g->Calc == Computer)
				plr->addy += (plr->y > 94) - (plr->y < 94);
		} else if (plr->team == 1) {
			/* Goalies will either try to cover the top half, bottom half, or middle of the goal 
		   depending on where the puck is */
			if (plr->type == Goalie) {
				x = 66;
				y = 100 + 12 * (g->Is_Controlled != plr->team && g->y > 124) - 
				22 * (g->Is_Controlled != plr->team && g->y < 66);
			} else if (Team1->Conference == WESTERN) {
				x = (Starting_Pos[3*Western[Team1->No].Formation + plr->No][0]<<4);
				y = (Starting_Pos[3*Western[Team1->No].Formation + plr->No][1]<<4);
			} else {
				x = (Starting_Pos[3*Eastern[Team1->No].Formation + plr->No][0]<<4);
				y = (Starting_Pos[3*Eastern[Team1->No].Formation + plr->No][1]<<4);
			}
			
			if (plr->type == Goalie || plr->x < x - 2 || plr->x > x + 2)
				plr->addx += (plr->x < x) - (plr->x > x);
			if (plr->type == Goalie || plr->y < y - 2 || plr->y > y + 2)
				plr->addy += (plr->y < y) - (plr->y > y);
			plr->face = 1;
		} else if (plr->team == 2) {
			if (plr->type == Goalie) {
				x = 383;
				y = 100 + 12 * (g->Is_Controlled != plr->team && g->y > 124) - 
				22 * (g->Is_Controlled != plr->team && g->y < 66);
				
				if (g->Alert) {
					if (g->Is_Controlled == TRUE || g->addx <= 0)
						g->Alert = FALSE;
					else if (g->addy < 0)
						plr->addy += ((95 > plr->y) * 2) - ((95 < plr->y) * 2);
					else if (g->addy > 0)
						plr->addy += ((100 > plr->y) * 2) - ((100 < plr->y) * 2);
				}
			} else if (Team2->Conference == WESTERN) {
				x = (Width<<4) - (Starting_Pos[3*Western[Team2->No].Formation + plr->No][0]<<4) - 16;
				y = (Starting_Pos[3*Western[Team2->No].Formation + plr->No][1]<<4);
			} else {
				x = (Width<<4) - (Starting_Pos[3*Eastern[Team2->No].Formation + plr->No][0]<<4) - 16;
				y = (Starting_Pos[3*Eastern[Team2->No].Formation + plr->No][1]<<4);
			}
			
			if (plr->type == Goalie || plr->x < x - 2 || plr->x > x + 2)
				plr->addx += (plr->x < x) - (plr->x > x);
			if (plr->type == Goalie || plr->y < y - 2 || plr->y > y + 2)
				plr->addy += (plr->y < y) - (plr->y > y);
			if (g->Alert && plr->type == Goalie)
				plr->face  = (g->x > plr->x + 7 * (plr->face));
			else plr->face = 0;
		}
	}
	
	/* Player has control of the puck */
	/* Plr will try to shoot if the other team's goalie is out or they are close enough */
	if (plr->Control > 0) {
		/* Check if the Plr is stuck behind one of the Goals */
		if (plr->x < 38) {  // Plr is behind one of the Left Goal
			plr->addx++;
			plr->addy += ((plr->y > 94) * 2) - ((plr->y < 94) * 2);
		} else if (plr->x > 410) {  // Plr is behind the Right Goal
			plr->addx--;
			plr->addy += ((plr->y > 94) * 2) - ((plr->y < 94) * 2);
		} else if (plr->team == 1) {  // Team 1
			plr->addx++;
			if (x > Team2->plrs[0]->x && plr->face)  // Team 2's goalie is out
				Shoot_Puck(plr, (plr->x > 304 && plr->x < 396 && (g->Difficulty || g->Calc == Computer)));
			else 
			if (plr->x>298 && (random(16)==FALSE || (plr->x>324 && plr->face && plr->y > 61 && plr->y < 130)))
				Shoot_Puck(plr, (g->Difficulty || g->Calc == Computer));
			if (plr->y < 69 && random(2) == TRUE)
				plr->addy++;
			else if (plr->y > 121 && random(2) == TRUE)
				plr->addy--;
			else if (plr->y >= 69 && plr->y <= 121 && random(2) == TRUE)
				plr->addy += 1 - (random(2) * 2);
		} else {  // Team 2
			plr->addx--;
			if (x < Team1->plrs[0]->x && !plr->face)  // Team 1's goalie is out
				Shoot_Puck(plr, (plr->x < 160 && plr->x > 66 && (g->Difficulty || g->Calc == Computer)));
			else  // Check if Plr is close enough to shoot the puck
			if (plr->x<151 && (random(16)==FALSE || (plr->x<125 && !plr->face && plr->y > 61 && plr->y < 130)))
				Shoot_Puck(plr, (g->Difficulty || g->Calc == Computer));
			if (plr->y < 69 && random(2) == TRUE)
				plr->addy++;
			else if (plr->y > 121 && random(2) == TRUE)
				plr->addy--;
			else if (plr->y >= 69 && plr->y <= 122 && random(2) == TRUE)
				plr->addy += 1 - (random(2) * 2);
		}
		plr->face = (plr->addx >= 0);  // Which direction is the plr facing
	}
	
	Friction(plr);  // Handle ice physics and Check for Collisions
}

// Move the Player that the User is currently Controlling
void Move_Cur_Plr(Plr *plr) {
	plr->addx += (_KeyCheck(RIGHTKEY) != FALSE) - (_KeyCheck(LEFTKEY) != FALSE);
	plr->addy += (_KeyCheck(DOWNKEY) != FALSE)  - (_KeyCheck(UPKEY) != FALSE);
	
	if (plr->addx != 0)
		plr->face = (plr->addx >= 0);  // Which direction is the plr facing
	
	if (plr->Shooting && (!(_KeyCheck(SHOTKEY)) || ++g->Shooting > 3)) {
		if (plr->Shooting == 1) {
			Shoot_Puck(plr, FALSE);
			if (g->Link == FALSE && g->Calc == Host && g->Difficulty == TRUE && plr->x > 270 && 
			plr->x < 288 && plr->face && (plr->y > 182 || plr->y < 14))
				g->Alert = TRUE;  /* Alert Other team's Goalie of a Corner bank shot.  This was added 
				because it was REALLY easy to score by skating down the side of the rink and then 
				banking the puck off of the side so it goes into the opposite corner, if that makes sense */
		} else {  // Pass to the Closest Plr
			short Closest;
			Plr *Cur;
			
			if (plr->type == Goalie)  // Goalies will pass to their closest Teammate
				Closest = Closest_Plr(plr);
			else Closest = (!(plr->No - 1)) + 1;  // Other Players will pass between each other
			g->x = plr->x - 2 + 18 * (plr->face);
			g->y = plr->y + 13 + ((plr->type == Skinny) * 2);
			if (g->Calc == Host)
				Cur = Team1->plrs[Closest];
			else Cur = Team2->plrs[Closest];
			if (plr->x > Cur->x && plr->face)
				g->x -= 3;
			else if (plr->x < Cur->x && !plr->face)
				g->x += 3;
			
			// Calculate the New puck speed
			if (plr->x > Cur->x) {
				g->addx = ((Cur->x + 5 + 16 * (Cur->face)) - g->x) * 3;
			} else g->addx = ((Cur->x - 4 + 12 * (Cur->face)) - g->x) * 3;
			if (plr->y > Cur->y) {
				g->addy = ((Cur->y + 18 + ((Cur->type == Skinny) * 2)) - g->y) * 3;
			} else g->addy = ((Cur->y + 8 + ((Cur->type == Skinny) * 2)) - g->y) * 3;
			
			// Restrict the speed of the puck
			if (g->addx > 220)
				g->addx = 220;
			else if (g->addx < -220)
				g->addx = -220;
			if (g->addy > 220)
				g->addy = 220;
			else if (g->addy < -220)
				g->addy = -220;
			
			plr->Control = -1;
			g->Is_Controlled = FALSE;
		}
		g->Shooting = FALSE;
		plr->Shooting  = FALSE;
	}
	
	if (plr->Control > 0) {  // Player has control of Puck
		if (_KeyCheck(SHOTKEY) && plr->Shooting == FALSE) {  // Plr is shooting
			plr->Shooting  = 1;
			g->Shooting = 1 + ((plr->type == Goalie) * 2);
		} else if (_KeyCheck(PASSKEY) && plr->Shooting == FALSE) {  // Plr is passing
			plr->Shooting  = 2;
			g->Shooting = 1 + ((plr->type == Goalie) * 2);
		}
	} else {
		if (_KeyCheck(CHANGEKEY)) {
			// Allow tapping the ChangeKey twice to switch control to the Goalie
			if (g->Changing > 0 && g->Changing < 5) {
				g->Cur_Plr = 0;  // Goalie
				g->Changing = -2;
			} else if (g->Changing > -2) {
				g->Cur_Plr = Closest_Plr(NULL);  // Find closest plr to puck
				g->Changing = -1;
			}
		} else if (g->Changing && (g->Changing==-2 || (g->Changing+=1+(g->Changing==-1))>5)) {
			g->Changing = 0;
		}
		
		/* ---------------------------------- */
		/* SHOW ARROWS IF PLAYER IS OFFSCREEN */
		/* ---------------------------------- */
		register short x = plr->x-(g->camera_x<<4)-g->softx;
		register short y = plr->y-(g->camera_y<<4)-g->softy;
		if (x < -16) {
			if (y < 0)
				//GrayTile_Clipped(0, 0, Arrows[1]);
				Clipped_Plr(0, 0, Arrows[1], 1);
			else if (y > g->PlaneHeight - 13)
				//GrayTile_Clipped(0, g->PlaneHeight - 13, Arrows[1]);
				Clipped_Plr(0, g->PlaneHeight - 13, Arrows[1], 1);
			else //GrayTile_Clipped(0, y, Arrows[1]);
			Clipped_Plr(0, y, Arrows[1], 1);
		} else if (x > LCD_WIDTH) {
			if (y < 0)
				//GrayTile_Clipped(LCD_WIDTH - 13, 0, Arrows[0]);
				Clipped_Plr(LCD_WIDTH - 13, 0, Arrows[0], 1);
			else if (y > g->PlaneHeight - 13)
				//GrayTile_Clipped(LCD_WIDTH - 13, g->PlaneHeight - 13, Arrows[0]);
				Clipped_Plr(LCD_WIDTH - 13, g->PlaneHeight - 13, Arrows[0], 1);
			else //GrayTile_Clipped(LCD_WIDTH - 13, y, Arrows[0]);
			Clipped_Plr(LCD_WIDTH - 13, y, Arrows[0], 1);
		} else if (y < -16) {
			if (x < 0)
				//GrayTile_Clipped(0, 0, Arrows[3]);
				Clipped_Plr(0, y, Arrows[3], 1);
			else if (x > LCD_WIDTH - 13)
				//GrayTile_Clipped(LCD_WIDTH - 13, 0, Arrows[3]);
				Clipped_Plr(LCD_WIDTH - 13, 0, Arrows[3], 1);
			else //GrayTile_Clipped(x, 0, Arrows[3]);
			Clipped_Plr(x, 0, Arrows[3], 1);
		} else if (y > g->PlaneHeight) {
			if (x < 0)
				//GrayTile_Clipped(0, g->PlaneHeight - 13, Arrows[2]);
				Clipped_Plr(0, g->PlaneHeight - 13, Arrows[2], 1);
			else if (x > LCD_WIDTH - 13)
				//GrayTile_Clipped(LCD_WIDTH - 13, g->PlaneHeight - 13, Arrows[2]);
				Clipped_Plr(LCD_WIDTH - 13, g->PlaneHeight - 13, Arrows[2], 1);
			else //GrayTile_Clipped(x, g->PlaneHeight - 13, Arrows[2]);
			Clipped_Plr(x, g->PlaneHeight - 13, Arrows[2], 1);
		}
	}
	
	Friction(plr);  // Player physics/Hit Detection
}

void Shoot_Puck(Plr *plr, const short Smart_Shot) {
	g->x = plr->x - 2 + 18 * (plr->face);
	g->y = plr->y + 13 + ((plr->type == Skinny) * 2);
	
	/* Determine the Puck's Direction and Speed */
	// Puck's New Horizontal Speed
	if (!plr->face)
		g->addx = (Min_xspeed << 6) + ((plr->type == Skinny) << 3) - ((plr->type == Fat) << 3);
	else if (plr->face)
		g->addx = (Max_xspeed << 6) - ((plr->type == Skinny) << 3) + ((plr->type == Fat) << 3);
	else g->addx = 0;
	// Puck's New Vertical Speed
	if (Smart_Shot == TRUE) {
		/* --------------------------------------------------
		   Make Computer plr try to shoot around the goalie. 
		   Computer plr will shoot at either the top or the 
		   bottom of the goal, depending on which half the 
		   goalie is covering.  This makes it Very difficult
		   to stop the computer from scoring on break aways 
		   and in ShootOuts! The Computer shoots 'smart' only
		   when the Difficulty is set to Hard.
		   -------------------------------------------------- */
		if ((plr->team == 1 && Team2->plrs[0]->y < 100) || (plr->team == 2 && Team1->plrs[0]->y < 100))
			g->addy = (133 - g->y) << 2;
		else g->addy = ((91 + ((g->y < 94) * 2)) - g->y) * 3;
		if (g->addy > 200)
			g->addy = 200;
		else if (g->addy < -200)
			g->addy = -200;
		
	} else if (plr->addy < -1) {  // Puck's new vertical speed will be Negative
		if (plr->addy <= Min_yspeed)
			g->addy = (Min_yspeed << 6) + ((plr->type == Skinny) << 3) - ((plr->type == Fat) << 3);
		else g->addy = ((plr->addy) << 6) + ((plr->type == Skinny) << 3) - ((plr->type == Fat) << 3) + 
		((!_KeyCheck(UPKEY | DOWNKEY)) << 5);
		g->y   -= 10;  // Make the puck shoot from the Plr's head
	} else if (plr->addy > 1) {  // Puck's new vertical speed will be Positive
		if (plr->addy >= Max_yspeed)
			g->addy = (Max_yspeed << 6) - ((plr->type == Skinny) << 3) + ((plr->type == Fat) << 3);
		else g->addy = ((plr->addy) << 6) - ((plr->type == Skinny) << 3) + ((plr->type == Fat) << 3) - 
		((!_KeyCheck(UPKEY | DOWNKEY)) << 5);
	} else g->addy = 0;			 // Puck's new vertical speed will be Zero
	
	plr->Control = -1;  /* Plr cannot pick up puck again for fifteen frames, this is to ensure that
	the player will not pick up the puck immediately after shooting/passing it */
	g->Is_Controlled = FALSE;
}

// Returns the Closest teammate to a given player or to the puck if the plr Paramater is NULL
short Closest_Plr(const Plr *plr) {
	register short Smallest = -1, Cur_Smallest = 0, a, D, x, y;
	Plr *Cur;
	
	if (plr != NULL) {  // Finding the Closest Teammate to a Player
		x = plr->x + (13 - ((plr->type == Skinny) * 2)) * plr->face;
		y = plr->y + 13 + ((plr->type == Skinny) * 2);
	} else {  // Finding closest player to the puck
		x = g->x;
		y = g->y;
	}
	
	for(a = 3; a--;) {  // Calculate closest plr using Distance Formula
		if (g->Calc == Host)
			Cur = Team1->plrs[a];
		else Cur = Team2->plrs[a];
		if ((a!=g->Cur_Plr||plr==NULL)&&((D=Dist(x,y,Cur->x+13*(Cur->face),Cur->y+13))<Smallest||Smallest==-1)) {
			Cur_Smallest = a;
			Smallest = D;
		}
	}
	
	return Cur_Smallest;
}

/*	Make Player Slide to stop and check for collisions	*/
void Friction(Plr *plr) {
	register short Fast_Plr = plr->type == Skinny && (random(3) || (g->Difficulty && 
	(plr->team == 1 ? (g->Calc == Computer) : (g->Calc == Host))));
	
	if (++plr->delay > 4 - (Fast_Plr == FALSE)) {  // Friction with ice
		plr->delay = 0;
		plr->addx += (plr->addx < 0) - (plr->addx > 0);
		plr->addy += (plr->addy < 0) - (plr->addy > 0);
	}
	
	// Players can only skate so fast
	if (plr->addx > Max_xspeed + Fast_Plr - (plr->type == Fat))
		plr->addx = Max_xspeed + (random(2) << Fast_Plr);
	else if (plr->addx < Min_xspeed - Fast_Plr + (plr->type == Fat))
		plr->addx = Min_xspeed - (random(2) << Fast_Plr);
	if (plr->addy > Max_yspeed + Fast_Plr - (plr->type == Fat))
		plr->addy = Max_yspeed + (random(2) << Fast_Plr);
	else if (plr->addy < Min_yspeed - Fast_Plr + (plr->type == Fat))
		plr->addy = Min_yspeed - (random(2) << Fast_Plr);
	
	// Update plr's x and y coordinates
	plr->x += plr->addx;
	plr->y += plr->addy;
	
	// Check if plr hit Left Goal
	if (Hit_Goal1(plr->x, plr->y + ((plr->type == Skinny && plr->y < 74) * 2))) {
		if (plr->y < 136 && plr->y - ((plr->type == Skinny) * 2) >= 69) {
			if (plr->addx > 0 && plr->x <= 39 + ((plr->type == Skinny) * 2)) {
				plr->x = 35;
				plr->addx = 0;
			} else if (plr->addx < 0 && plr->x >= 61 - ((plr->type == Skinny) * 2)) {
				plr->x = 66;
				plr->addx = 0;
			}
		}
		if (Hit_Goal1(plr->x, plr->y + ((plr->type == Skinny && plr->y < 74) * 2))) {
			if (plr->addy > 0 && plr->y - ((plr->type == Skinny) * 2) <= 71) {
				plr->y = 67 - ((plr->type == Skinny) * 2);
				plr->addy = 0;
			} else if (plr->addy < 0 && plr->y > 133) {
				plr->y = 138;
				plr->addy = 0;
			}
		}
	} else if (Hit_Goal2(plr->x, plr->y + ((plr->type == Skinny && plr->y < 74) * 2))) {
		// Check if plr hit Right Goal
		if (plr->y < 136 && plr->y - ((plr->type == Skinny) * 2) >= 69) {
			if (plr->addx > 0 && plr->x <= 387 + ((plr->type == Skinny) * 2)) {
				plr->x = 383;
				plr->addx = 0;
			} else if (plr->addx < 0 && plr->x > 409) {
				plr->x = 414;
				plr->addx = 0;
			}
		}
		if (Hit_Goal2(plr->x, plr->y + ((plr->type == Skinny && plr->y < 74) * 2))) {
			if (plr->addy > 0 && plr->y - ((plr->type == Skinny) * 2) <= 71) {
				plr->y = 67 - ((plr->type == Skinny) * 2);
				plr->addy = 0;
			} else if (plr->addy < 0 && plr->y > 133) {
				plr->y = 138;
				plr->addy = 0;
			}
		}
	}
	
	// Check if Plr hit sides of Rink
	if (plr->x < 8) {
		plr->x = 8;
		plr->addx = 0;
	} else if (plr->x > Max_x) {
		plr->x = Max_x;
		plr->addx = 0;
	} if (plr->y < 3) {
		plr->y = 3;
		plr->addy = 0;
	} else if (plr->y > Max_y - ((plr->type == Skinny) * 2)) {
		plr->y = Max_y - ((plr->type == Skinny) * 2);
		plr->addy = 0;
	}
}

// By Thomas Nussbaumer - from the Game Construction Kit
void Clipped_Plr(const short x, short y, unsigned short* spritedata, short Team) {
	register long            addr1   = (long) Hiddenl;
	register long            addr2   = (long) Hiddend;
	register unsigned short* sprite1 = spritedata;
	register unsigned short* sprite2 = sprite1+16;
	register short           h       = 16;
	register long            offset;
	register unsigned short  cnt;
	
	if (y <= -16 || y > g->PlaneHeight || x <= -16 || x > LCD_WIDTH)
		return;
	
	if (Team > 2) {  // Skinny Plr
		Team    -= 2;
		sprite2 += 2;
		h        = 18;
	}
	
	// Switch Light and Dark sprites for Team 2
	if (Team == 2) {
		sprite1 = sprite2;
		sprite2 = spritedata;
	}
	
	if (y<0) {  // Clip Above
		sprite1 -= y;
		sprite2 -= y;
		h       += y;
		y        = 0;
	} else if (y > g->PlaneHeight - 1 - h)  // Clip Below
		h = g->PlaneHeight - y;
	
	if (x<0) {  // Clip Left
		cnt    = -x+16;
		offset = (y<<5)-(y<<1);
		addr1  += offset;
		addr2  += offset;
		for (;h;--h,addr1+=30,addr2+=30) {
			register long val = ~(((long)(*sprite1) | (long)(*sprite2))<<cnt);
			*(long*)addr1&=val;
			*(long*)addr1|=(long)(*sprite1++)<<cnt;
			*(long*)addr2&=val;
			*(long*)addr2|=(long)(*sprite2++)<<cnt;
		}
	} else if (x+16>LCD_WIDTH) {  // Clip Right
		register unsigned short mask2=0xffff<<(16-(LCD_WIDTH-x));
		cnt    = 16-(x&15);
		//offset = (y<<5)-(y<<1)+((x>>3)&0x1e);
		offset = (y<<5)-(y+y)+2*(x>>4);
		addr1  += offset;
		addr2  += offset;
		for (;h;--h,addr1+=30,addr2+=30) {
			register long val = ~((long)((long)((long)(*sprite1) | (long)(*sprite2)) & (long)mask2)<<cnt);
			*(long*)addr1&=val;
			*(long*)addr1|=(long)(*sprite1++ & mask2)<<cnt;
			*(long*)addr2&=val;
			*(long*)addr2|=(long)(*sprite2++ & mask2)<<cnt;
		}
	} else {
		cnt    = 16-(x&15);
		//offset = (y<<5)-(y<<1)+((x>>3)&0x1e);
		offset = (y<<5)-(y+y)+2*(x>>4);
		addr1  += offset;
		addr2  += offset;
		for (;h;--h,addr1+=30,addr2+=30) {
			register long val = ~(((long)((long)(*sprite1) | (long)(*sprite2)))<<cnt);
			*(long*)addr1&=val;
			*(long*)addr1|=(long)(*sprite1++)<<cnt;
			*(long*)addr2&=val;
			*(long*)addr2|=(long)(*sprite2++)<<cnt;
		}
	}
}