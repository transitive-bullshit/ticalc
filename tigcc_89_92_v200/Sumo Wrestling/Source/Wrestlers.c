// C Source File
// Created 6/14/2004; 2:19:41 PM
#include "All.h"
/* ------------------------------------------------------------------------ */
/* For Debugging purposes, allows the user to change certain things in-game */
//#define DEBUG
/* ------------------------------------------------------------------------ */
static inline void Handle_User_Wrestler(wrestler*);
static inline void Handle_Wrestler_AI(wrestler*);
static inline void Handle_Actions(void);
static inline void Handle_Finish(void);
static inline void Animate_Movement(wrestler*, unsigned char, unsigned short);
static inline void Display_Wrestlers(void);

#define Initiate_Move(Move, action)		(*Move = action)
#define Check_Move(action, a)					(((action) == (a)))
#define PUMMEL_MOVES									(0x7)

#define M_PUMMELPUSH		0b00000001
#define M_PUMMELTHROW		0b00000010
#define M_PUMMELLIFT		0b00000011
#define M_SHOVELEFT			0b00000100
#define M_LIFTMOVE			0b00000101
#define M_TAUNT1				0b00001000
#define M_PUNCH					0b00010000
#define M_MOVERIGHT			0b00011000
#define M_MOVELEFT			0b00100000
#define M_TAUNT2				0b00101000
//#define M_SQUAT2				0b00110000
#define M_WALK1					0b00111000
#define M_CHARGE				0b01000000
#define M_NOTCHARGE			0b10000000

//Have Person hit be able to walk a few frames before other can hit again (still hitting from before)???

short Reversal(wrestler *w0) {
	wrestler  *w1 = &g->wrestlers[!NO(w0)];
	Character *c0 = w0->c;
	Character *c1 = w1->c;
	
	//if (c0->Stamina > 0) c0->Stamina--;
	
	// w1 is the wrestler attempting to Reverse the move of w0
	return ((random(5) && c1->Strength + (w1->Health>>2) - 2 * random(c0->Stamina) > c0->Strength + 
	(w0->Health>>2))
	#ifdef DEBUG
	|| _KeyCheck(ENTERKEY)
	#endif
	);
}

#if 0
char Strength;
char Health;  // 41 (min) + Amount of starting Health
char Stamina;
char Speed;
#endif

enum Finisher_Weights { LITTLE_CHANCE = 16, NO_WEIGHT = 3, WEIGHTED = 2 };

short Finish(wrestler *w0, short Weighted_move) {
	wrestler  *w1 = &g->wrestlers[!NO(w0)];
	Character *c0 = w0->c;
	Character *c1 = w1->c;
	
	#ifdef CAPTURE_SREENSHOT
	if (w0->Control != g->Calc || Weighted_move != WEIGHTED || g->Mixed_Gfx < PummelLift1 || g->Mixed_Gfx > 
	PummelLift6)
		return 0;
	#endif
	
	// w0 is the wrestler attempting to Finish w1
	return ((((w1->Health <= 0 && Weighted_move != LITTLE_CHANCE) || !random(Weighted_move)) && 
	(w1->Health < 40 || !random(16)) && 
	(w1->Health <= 0 ||	w0->Health > 2 * w1->Health || 
	c0->Strength + (w0->Health>>1) - random(c1->Stamina) > c1->Strength + (w1->Health>>1)))
	#ifdef DEBUG
	|| _keytest(RR_PLUS)
	#endif
	);
}

/*For a HUD, have two life bars on the bottom (one for each wrestler), each 72 pixels long.

24 Black/24 Dark/24 Light                24 Light/24 Dark/24 Black
(Time takes ~13 pixels + 1 white on each side to display = 15 pixels total)
*/

//Go by Wrestler Level, Difficulty of opponent, current health/power meter, and distance to 
//edge of ring for Finishing opponent
//Have reversals with 'B' button (like when thrown and coming down for Earthquake, to land on your 
//opponent  -- Maybe have opponent able to move left/right in air to try and aim to smash 
//the thrower)  Smush!

/* ------------------------------------------------- */
/* --- Update and Draw both Wrestlers each frame --- */
/* ------------------------------------------------- */
void Update_Wrestlers(void) {
	
	if (++g->delay >= 8)
		g->delay = 0;
	
	// Reset all Move initiations from last frame
	g->Move0 = g->Move1 = 0;
	// Handle possible keystrokes/AI of light wrestler
	Handle_Wrestler(&g->wrestlers[0]);
	// Handle possible keystrokes/AI of Dark wrestler
	Handle_Wrestler(&g->wrestlers[1]);
	
	// Exchange data between calcs in Linkplay
	if (g->Link) Transfer_Data();
	
	/*#warning "debugging here"
	memset(Hiddenl, 0, 20 * 30);
	memset(Hiddend, 0, 20 * 30);
	g->Move0 |= M_WALK1;
	char buf[50];
	sprintf(buf, "%d %d %d %d", g->Move0, g->Move1, Check_Move(g->Move0, M_WALK1), 
		Check_Move(g->Move1, M_WALK1));
	DrawGrayString(0, 0, buf, A_NORMAL, F_6x8);*/
	
	// Take care of moves which effect both wrestlers
	if (g->Status == M_Normal)
		Handle_Actions();
	// Update finisher moves at the end of a match
	if (g->Status >= M_Finish)
		Handle_Finish();
	
	if (g->Status != Game_Paused)
		Display_Wrestlers();
}

/* -------------------------------------------------------------------------------------- */
/* --- Update Introductions and handle wrestlers depending on their control (User/AI) --- */
/* -------------------------------------------------------------------------------------- */
void Handle_Wrestler(wrestler *w) {
	/* -------------------- */
	/* Ritual Introductions */
	/* -------------------- */
	if (g->Status == M_Start) {
		register short Seconds = count / 361;
		short Mseconds50 = (count % 361) / 19;
		
		if (Seconds > 9 || _KeyCheck(ENTERKEY)) {  // Press enter to bypass Introductions
			if (NO(w))  // End Introduction Rituals
				End_Introductions();
		} else {
			register short No = NO(w);
			
			if (Seconds < 6) {
				
				if (Intro(w) == Stomp1) {  // Stomping feet at top of ring
					w->Graphic = Stomp1;
					w->x = 54 + 96 * No;//54 + 96 * No;
					
					if (Seconds == 5) {
						if (Mseconds50 < 4 && No == FALSE)
							w->Graphic = Stomp2;
					} else if (Seconds) {
						if (Mseconds50 < 4 && (Seconds == 1 && No == FALSE) == FALSE)
							w->Graphic = Stomp2;
						else if (((Seconds + No) & 1) == FALSE)
							w->Graphic = Stomp3 + (Seconds - 2 == No);
					}
					
					w->y = 58 - Wrestler_Height(w->Graphic);
				} else if (Intro(w) == 1) {  // Throwing Salt at top of ring
					w->Graphic = Salt1;
					w->x = 60 + 97 * No;
					w->y = 26;
					// 44 - 60, sitting there with Salt laying on Ring // There are 8 Salt sprites
					if (Seconds > !No) {
						short a = 8;//, x_start = 86 + 60 * No - g->x_off;
						short y_end = 93;
						short No_Frames = 32 - salt_x[No][8];
						
						if (No_Frames == FALSE)
							w->Graphic = Salt1;
						else {
							salt_x[No][8]++;
							w->Graphic = Salt3;
						}
						for(; a--; y_end-=5) {
							if (No_Frames != FALSE) {
								short x_end = (No ? 101 + 5 * a : 131 - 5 * a);
								salt_x[No][a] += (x_end - salt_x[No][a]) / No_Frames;
								salt_y[No][a] += (y_end - salt_y[No][a]) / No_Frames;
							}
							
							GraySprite8_MASK_R(salt_x[No][a] - g->x_off, salt_y[No][a] - g->y_off, 8, g->p_maskval, salt, 
								salt+8,salt+8, Hiddenl, Hiddend);
							//GraySprite8_AND(salt_x[No][a] - g->x_off, salt_y[No][a] - g->y_off, 8, salt, salt, 
							//	Hiddenl, Hiddend);
						}
						
						g->Force_Update = TRUE;  // Force a screen update since the Salt is being displayed
					}
					
					#if 0
					left start  (86,  24)
					right start (147, 24)
					
					
					after right throws, top is at (101, 58) last at (142, 87)
					down 5, over 5 for each one
					(136, 83)
					
					after left throws, top is at (131, 58) last at (90, 87)
					#endif
					
				} else if (Intro(w) == 2) {  // Kneeling at edge of ring
					w->Graphic = Kneel1;
					w->x = 34 + 152 * No;
					w->y = 50;
					
					if (Seconds && Seconds < 5 && ((Seconds + No) & 1) == FALSE)
						w->Graphic = Kneel5 - 2 * (Mseconds50 < 4);
				}
			} else if (Seconds == 6 || Seconds == 7) {  // Kneeling, facing each other
				w->Graphic = Kneel1;
				w->x = 81 + 58 * No;
				w->y = 50;
			} else {  // Tackle position, facing each other smiling :)
				w->Graphic = Tackle1;
				w->x = 84 + 45 * No;
				w->y = 50;
			}
			
			if (w->Graphic > Stomp4)
				w->Graphic += !No;
		}
	} else if (g->Status == M_Normal) {
		
		if (w->Control == g->Calc)
			Handle_User_Wrestler(w); // Read input for this Wrestler
		else if (g->Link == FALSE && g->Mode != PRACTICE)
			Handle_Wrestler_AI(w);   // A.I.
		// else this wrestler is being controlled by the other calc in a Linkplay match
	}
}

/* ------------------------------------------------------------------ */
/* --- Handle key input from user and update wrestler accordingly --- */
/* ------------------------------------------------------------------ */
static inline void Handle_User_Wrestler(wrestler *w) {
	char *Move = NO(w) ? &g->Move1 : &g->Move0;
	
	if (w->Graphic <= Tackle4) {
		if (g->keys || Hit_Detection(13))
			Initiate_Move(Move, M_WALK1);//Direction(w) ? M_MOVERIGHT : M_MOVELEFT);
	} else {
		
		#ifdef DEBUG
		wrestler *opponent = &g->wrestlers[!NO(w)];
		
		if (_KeyCheck(ENTERKEY)) {  // For Debugging gfx code
			if (_KeyCheck(LEFTKEY)) {
				w->attr &= ~W_DIRECTION;
				if (Direction(opponent) == FALSE) {
					opponent->attr |= W_DIRECTION;
					opponent->Graphic++;
				}
			} else if (_KeyCheck(RIGHTKEY)) {
				w->attr |= W_DIRECTION;
				if (Direction(opponent)) {
					opponent->attr &= ~W_DIRECTION;
					opponent->Graphic--;
				}
			}
		} else if (_keytest(RR_5)) {
			if (_KeyCheck(LEFTKEY))
				w->attr &= ~W_ANIM;
			else if (_KeyCheck(RIGHTKEY))
				w->attr |= W_ANIM;
		}
		#endif
		
		if (g->Mixed_Gfx || _ACTION_(w->attr)) {
			short Action = ABS(g->Mixed_Gfx);
			short right  = _KeyCheck(RIGHTKEY);
			short left   = _KeyCheck(LEFTKEY);
			short dir_key = right, opp_key = left;
			if ((w->attr & W_DIRECTION) == FALSE) {
				dir_key = left, opp_key = right;
			}
			if (Action == Pummel1) { // Pummeling for position
				if (_KeyCheck_Tap(A_KEY)) {
					if (dir_key)								// Pressing 'A' + Into opponent
						Initiate_Move(Move, M_PUMMELPUSH);
					else if (opp_key)						// Pressing 'A' + Away from opponent
						Initiate_Move(Move, M_PUMMELTHROW);
					else if (_KeyCheck(UPKEY))	// Pressing 'A' + Up
						Initiate_Move(Move, M_PUMMELLIFT);
				} else if (dir_key && _KeyCheck_Tap(B_KEY))  // Pressing 'B' + Into opponent
					Initiate_Move(Move, M_SHOVELEFT);
			} else if (Action >= PummelLift3 && Action <= PummelLift6 && dir_key && 
			(g->wrestlers[0].attr & W_ANIM) >> 4 == NO(w))
				Initiate_Move(Move, M_LIFTMOVE);//g->wrestlers[0].x += 1 - 2 * ((w->attr & W_DIRECTION) == FALSE);
		} else if (_KeyCheck_Tap(A_KEY)) {  // Punch or Taunt
			if (_KeyCheck(DOWNKEY))
				Initiate_Move(Move, M_TAUNT1);  // Wrestler gains Health by taunting
			else if (_KeyCheck(UPKEY))
				Initiate_Move(Move, M_TAUNT2);
			else Initiate_Move(Move, M_PUNCH);  // Wrestler is Punching
		} else if (_KeyCheck(LEFTKEY) && w->x > 32) {
			//Initiate_Move(Move, M_MOVELEFT);
			
			*Move = M_MOVELEFT | M_NOTCHARGE * ((w->attr & W_CHARGE) && Direction(w));
			
			//if (_KeyCheck(B_KEY))
			//	*Move |= (((w->attr & W_DIRECTION) == FALSE) ? M_CHARGE : M_NOTCHARGE);
		} else if (_KeyCheck(RIGHTKEY) && w->x < 180) {
			*Move = M_MOVERIGHT | M_NOTCHARGE * ((w->attr & W_CHARGE) && (w->attr & W_DIRECTION) == FALSE);
			//if (_KeyCheck(B_KEY))
			//	*Move |= (Direction(w) ? M_CHARGE : M_NOTCHARGE);
		} else if (_KeyCheck(B_KEY))
			Initiate_Move(Move, M_CHARGE);
		else if (w->attr & W_CHARGE)
			Initiate_Move(Move, M_NOTCHARGE);
		
		
		//else if ((w->attr & W_CHARGE) && _KeyCheck(B_KEY) == FALSE)
		//	Initiate_Move(Move, M_NOTCHARGE);
		//else if ((w->attr & W_SQUAT1) && _KeyCheck(UPKEY) && _KeyCheck_Tap(B_KEY))
		//	Initiate_Move(Move, M_SQUAT2);
		//else if (_KeyCheck(DOWNKEY) && _KeyCheck_Tap(B_KEY))
		//	Initiate_Move(Move, M_SQUAT1);
	}
}

/* -------------------------- */
/* --- Update Wrestler AI --- */
/* -------------------------- */
static inline void Handle_Wrestler_AI(wrestler *w) {
	Character *c = w->c;  // Current Wrestler
	wrestler *opponent = &g->wrestlers[!NO(w)];  // Opponent
	Character *o = opponent->c;  // Opponent's CHaracter attributes
	short Dir = Direction(w);
	char *Move = NO(w) ? &g->Move1 : &g->Move0;
	
	// Have different strategies for when an AI wrestler has low health vs high health as well
	// Add some sort of RISK variable which will determine how much risk/what level of move a 
	// wrestler can take based on health and other attributes
	
	#if 0
	char Strength; // For reversals, finishes, and affects how much damage/health to inflict with each move
	char Health;   // 41 (min) + Amount of starting Health
	char Stamina;  // For reversals and finishes
	char Speed;    // For AI Charging, how quickly AI will perform Pummel moves
	#endif
	
	if (w->Graphic <= Tackle4) {
		// Get up and charge at opponent if...
		
		short hit = Hit_Detection(13);
		if (hit || random(c->Speed + 2) > random(o->Speed + 2)) {
			w->attr |= W_MOVEABLE;
			// Random, weighted charges
			*Move = M_WALK1 | M_CHARGE * (hit == FALSE && random(random(3) + (c->Speed >> 3)));
		}
	} else if (g->Mixed_Gfx || _ACTION_(w->attr)) {
		short Action = ABS(g->Mixed_Gfx);
		
		//(NO(w) ? _keytest(RR_2) : _keytest(RR_1))
		
		//if (Action == Pummel1 && _KeyCheck(ENTERKEY))
		//	Initiate_Move(Move, M_PUMMELTHROW);
		
		if (Action == Pummel1 && random(((54 - c->Speed) >> 1) - (g->Difficulty << 3)) == FALSE) {
			Action = random(4);
			
			if (Action == 0)
				Initiate_Move(Move, M_PUMMELPUSH);
			else if (Action == 1)
				Initiate_Move(Move, M_PUMMELTHROW);
			else if (Action == 2) {
				Initiate_Move(Move, M_PUMMELLIFT);
				w->attr = (w->attr & ~W_MOVEABLE) | W_MOVEABLE * (g->Difficulty || random(3));
			} else if (Action == 3)
				Initiate_Move(Move, M_SHOVELEFT);
		} else if (Action >= PummelLift3 && Action <= PummelLift6 && (w->attr & W_MOVEABLE) && 
		(g->wrestlers[0].attr & W_ANIM) >> 4 == NO(w)) {
			Initiate_Move(Move, M_LIFTMOVE);//g->wrestlers[0].x += -1 + (Dir + Dir);
		} else *Move = M_CHARGE * (opponent->Control != AI && (w->attr & W_THROWN) && (g->Difficulty || 
		!random(4)));  // Initiate random charges
	} else {
		w->y = 50;
		// if (moving) {  else if (waiting) { if waiting has elapsed, then start moving
		
		if (opponent->attr & (W_EDGE | W_THROWN)) { // AI stands still
			w->Counter = FALSE;
			// if (difficulty/randomness) { Taunt/Gain Health }
			if (opponent->Counter <= 10 && g->Difficulty && w->Health < MAX_HEALTH && !random(3) && 
			Hit_Detection(1) == FALSE)
				Initiate_Move(Move, M_TAUNT1);  // Wrestler gains Health by taunting
			else w->attr &= ~W_MOVEABLE;
		} else if (w->attr & W_MOVEABLE) {  // AI moves toward opponent
			if (Hit_Detection(2 + random(10)) && random(18 - (g->Difficulty << 3)) == FALSE) {  // Duplicate
				Initiate_Move(Move, M_PUNCH);  // Begin a Punch
			} else Initiate_Move(Move, Dir ? M_MOVERIGHT : M_MOVELEFT);//w->x += -1 + (Dir + Dir);
		} else {
			if (w->Counter == FALSE)  // Provide a short, random delay before charging at opponent again
				w->Counter = random(40);
			if (--w->Counter <= 0) {
				w->attr |= W_MOVEABLE;
				// Random, weighted charges
				*Move = M_CHARGE * (g->Difficulty && random(random(3) + (c->Speed >> 3)));
			} else if (random(15 - (g->Difficulty << 3)) == FALSE && Hit_Detection(3))
				Initiate_Move(Move, M_PUNCH);  // Begin a Punch
		}
	}
	
	// Think this was Scott Noveck talking:
	/*I talked to Thomas Corvazier way back when about SF2TI. The AI is really deceptively simple for a 
	fighter -- the AI character determines how far it is from the player (I think it chooses from like 
	very near, near, medium, and far), and for each of those positions, it has a set of actions that it 
	can choose from. The actions might be moves - say an uppercut, or a fireball - or might be things 
	like walking closer, or jumping back, or something. On different difficulty levels those moves are 
	weigthed so that the "better" ones are chosen more often. Add a little more code for like blocking, 
	and that's it. I couldn't believe how simple it was, but SF2TIs AI was pretty good.

Other than that, I think the game logic is pretty simple. It is designing the art/animations and the 
fighters moves that is really hard.*/
}

/* ---------------------------------------------------------- */
/* --- Begin a wrestler's punch and check for a collision --- */
/* ---------------------------------------------------------- */
static inline void Initiate_Punch(wrestler *w) {
	w->Graphic = PunchLeft1 + Animation(w) + Direction(w);
	//w->attr = (w->attr & ~W_ACTIONS) | (W_PUNCH | DRAW_FIRST);
	w->attr |= (W_PUNCH | DRAW_FIRST);
	
	if (Hit_Detection(2)) {  // Punch has hit other wrestler
		wrestler *opponent = &g->wrestlers[!NO(w)];
		
		if (opponent->attr & W_PUNCH)  // If both wrestlers are punching each other
			New_Pummel_Move(w, -Spinn1);
		else {
			opponent->Graphic = PunchedLeft1 + Animation(w);
			opponent->attr = (opponent->attr & ~W_ACTIONS) | W_PUNCHED | W_MOVEABLE;
			opponent->Counter = 0;
			opponent->y = 50;
			/*if ((w->attr & W_ANIM)) {
				opponent->attr |= DRAW_FIRST;
				w->attr &= ~DRAW_FIRST;
			}*/
			if (NO(w) == FALSE)
				g->Move1 = FALSE;
			
			Add_Health(opponent, -((g->delay & 1) * (w->c->Strength < 16) + (w->c->Strength >> 4)));
			// Semi-Random Reversals for punches, where the punched wrestler will not slide back
			opponent->flag = (Reversal(w) || (w->Control == Host && opponent->Control == AI && 
			!random(11 - (g->Difficulty << 3))));
			//g->Difficulty && !random(3)));
		}
	}
}

/* ------------------------------------------------ */
/* --- Edge of Ring and Pummeling hit detection --- */
/* ------------------------------------------------ */
void Align_Synchronize_Wrestler(wrestler *w) {
	if (g->Status != M_Normal)
		return;
	
	if ((w->attr & W_EDGE) == FALSE) {
		short x = w->x + Wrestlers[w->Graphic].x_off;
		short offset = ((w->attr & W_THROWN) ? 10 : 0);
		short Edge = Left_Edge - offset - ((w->attr & W_SHOVED) ? 10 : 0);
		
		if (x < Edge) {
			w->x += (Edge - x);
			
			Check_Edge(w);
		} else {
			x += Wrestlers[w->Graphic].Width;
			Edge = Right_Edge + offset;
			
			if (x > Edge) {
				w->x -= (x - Edge);
				
				Check_Edge(w);
			}
		}
	}
	
	if (g->Status >= M_Finish)
		return;
	
	//(_ACTION_(g->wrestlers[0].attr | g->wrestlers[1].attr)) == FALSE
	//W_PUNCH|W_PUNCHED|W_TAUNT1|W_TAUNT2|W_THROW|W_THROWN|W_EDGE|W_SHOVED
	
	// -- Check for a new Pummeling position --
	if (g->Mixed_Gfx == FALSE && !((g->wrestlers[0].attr | g->wrestlers[1].attr) & (W_PUNCH|W_PUNCHED|W_THROW|
	W_THROWN|W_EDGE|W_SHOVED)) && Hit_Detection(15)) {
		const short Charge_0 = ((g->wrestlers[0].attr & W_CHARGE) != FALSE);
		const short Charge_1 = ((g->wrestlers[1].attr & W_CHARGE) != FALSE);
		g->wrestlers[0].y = 50;
		g->wrestlers[1].y = 50;
		
		if (Charge_0 && Charge_1) {  // Both wrestlers were charging and have now collided
			New_Pummel_Move(w, -Spinn1);  // Make them spin backwards away from each other
		} else if (Charge_0 || Charge_1) {
			New_Pummel_Move(&g->wrestlers[Charge_1], -Charge1);
			g->wrestlers[0].Counter = 5 + random(w->c->Strength >> 2);
		} else {  // Initiate a regular Pummel position
			g->Mixed_Gfx = Pummel1;
			g->wrestlers[1].attr &= ~(W_ANIM | W_ACTIONS);
			//if (NO(w))
			//	g->wrestlers[0].attr |= W_ANIM;
			//else g->wrestlers[0].attr &= ~W_ANIM;
			g->wrestlers[0].attr = (g->wrestlers[0].attr & ~(W_ANIM | W_ACTIONS)) | ((w->attr & W_NO) << 4) | 
				W_MOVEABLE;
			if (Direction(&g->wrestlers[1]))
				g->wrestlers[0].x = g->wrestlers[1].x;
		}
	}
}

/* --------------------------------------------------------------------- */
/* --- Update Actions being performed by one wrestler, independently --- */
/* --------------------------------------------------------------------- */
void Check_ACTION(wrestler *w) {
	const short Dir_w = Direction(w), Default_Graphic = Walk1 + Animation(w) + Dir_w;
	w->Counter++;
	
	if ((w->attr & W_PUNCH) && w->Counter > 9) {  // Wrestler is punching
		w->attr = (w->attr & ~(W_PUNCH | DRAW_FIRST)) ^ W_ANIM;
		w->Graphic = Walk1 + Animation(w) + Dir_w;
	} else if (w->attr & W_PUNCHED) {  // Wrestler is being punched
		if (w->Counter >= 8 - w->flag) {
			w->attr &= ~(W_PUNCHED | DRAW_FIRST);
			w->Graphic = Default_Graphic;
		} else if (w->flag == FALSE) {  // Show punched animation
			w->x += 1 - (Dir_w + Dir_w);  // Make wrestler reel backwards
			if (w->Counter == 2 && Finish(&g->wrestlers[!NO(w)], LITTLE_CHANCE))
				Initiate_Finisher(w, Spinn1);  // Check for finish
			
			static const char x_off[2][2] = {
				{ 21, 19 }, 
				{ -2, -7 }, 
			};
			
			//facing you is punchedLeft2
			const unsigned char gfx = w->Graphic;
			// Draw wrestler's blood
			Draw_Blood(w->x - g->x_off + x_off[Dir_w][(gfx >= PunchedLeft2)], w->y - 1 - g->y_off, Dir_w);
		}
	} else if (w->attr & W_TAUNT1) {  // Wrestler is taunting opponent
		if (w->Counter == 11)
			w->Graphic += 2;
		if (w->Counter > 24) {
			w->attr &= ~W_TAUNT1;
			w->Graphic = Default_Graphic;
		}
	} else if (w->attr & W_TAUNT2) {
		#if 0
		w->Graphic = GrowlLeft2 + Dir_w;
		if (w->Counter > 18)
			w->Graphic = Default_Graphic;
		
		short x = w->x + Wrestlers[w->Graphic].x_off;
		wrestler *o = &g->wrestlers[!NO(w)];
		short x2 = o->x + Wrestlers[o->Graphic].x_off;
		
		if (Dir_w ? (x + Wrestlers[w->Graphic].Width + 14 + w->Counter >= x2) : (x2 + 
		Wrestlers[o->Graphic].Width <= x - (14 + w->Counter))) {
			w->attr &= ~W_SQUAT2;
			o->Graphic = PunchedLeft1 + Animation(w);
			o->attr = (o->attr & ~W_ACTIONS) | W_PUNCHED;
			o->Counter = 0;
			Add_Health(o, -((g->delay & 1) * (w->c->Strength < 16) + (w->c->Strength >> 4)));
			o->flag = 0;  // Can't Reverse
		}
		
		// Display FireBall
		short gfx = FireBallLeft + Dir_w;
		GraySpriteX8_MASK(Dir_w ? x + Wrestlers[w->Graphic].Width + w->Counter : x - (16 + w->Counter), 59, 16, 
			Wrestler_Light(gfx), Wrestler_Dark(gfx), Wrestler_Mask(gfx), Wrestler_Mask(gfx), 2, Hiddenl, Hiddend);
		#endif
		
		w->Graphic = GrowlLeft1 + Dir_w + 2 * (w->Counter > 11);
		//((w->Counter & (1 << 3)) != FALSE);
		if (w->Counter > 26) {
			w->attr &= ~W_TAUNT2;
			w->Graphic = Default_Graphic;
		}
	} else if (w->attr & W_THROW && w->Counter >= 10) {  // Wrestler just threw opponent
		w->Graphic = Default_Graphic;
		w->attr   &= ~W_THROW;
	} else if (w->attr & W_THROWN) {  // Wrestler has been thrown and is now hopping
		static const char _throw[2][7] = {
		//{ 8, 10, 11, 11, 10, 9, 8}, y offsets from bottom of white chalk lines
			{-2, -1, -1,  0,  1, 1, 2}, 
			{ 0,  1,  1,  2,  1, 1, 1}, 
		};
		short counter = (w->Counter % 10);
		if (counter <= 6) {
			w->x += (Dir_w ? -_throw[1][counter] : _throw[1][counter]);
			w->y += _throw[0][counter];
		}
		if (w->Counter > 27) {
			w->Graphic = Default_Graphic;
			w->attr &= ~W_THROWN;
		}
	} else if (w->attr & W_EDGE) {
		if (w->Counter <= 1) {
			w->Graphic = ThrownRight + Dir_w;
			//14, 9, 12
		} else if (w->Counter > 25) {
			w->Graphic = Default_Graphic;
			w->attr &= ~W_EDGE;
		} else if (((w->Counter - 2) & 7) == FALSE) {
			//up/down/up
			short add = (((w->attr ^= W_ANIM) & W_ANIM) != FALSE);
			w->Graphic = ShovedRightEdge1 - Dir_w + add + add;
			if (Dir_w)
				w->x = w->x = 23 + 3 * add;
			else w->x = 187 - 3 * add;
		}
	} else if (w->attr & W_SHOVED) {
		if (w->Counter > 5) {
			w->Graphic = Default_Graphic;
			w->x -= 3 + (Dir_w << 2);
			w->attr &= ~(DRAW_FIRST | W_SHOVED);
		} else w->x += 1 - (Dir_w + Dir_w);
	}
}

void Initiate_Actions(wrestler *w, unsigned char action) {
	//short action = NO(w) ? g->Move1 : g->Move0;//, Is_AI = (w->Control == AI);
	
	// Ensure that wrestlers are not both attempting to begin pummel moves at the same time
	if (w->c->Speed < g->wrestlers[!NO(w)].c->Speed && (g->Move0 & PUMMEL_MOVES) && (g->Move1 & PUMMEL_MOVES))
		return;  // ^ XOR
	
	short NOTCHARGE = FALSE;
	if (action & M_CHARGE)
		w->attr |= W_CHARGE | W_MOVEABLE;
	else if (action & M_NOTCHARGE) {
		w->attr &= ~W_CHARGE;
		NOTCHARGE = TRUE;
	}
	action &= ~(M_CHARGE | M_NOTCHARGE);
	
	/*memset(Activel, 0, LCD_SIZE);
	memset(Actived, 0, LCD_SIZE);
	PortSet(Actived, 239, 127);
	printf("%d %d %d %d", action, Check_Move(action, M_MOVELEFT), Check_Move(action, M_MOVERIGHT), 
		((w->attr & W_CHARGE) != FALSE));
	WaitKey();*/
	
	short dif = (Check_Move(action, M_MOVERIGHT) && w->x < 180) - (Check_Move(action, M_MOVELEFT) && w->x > 32);
	if (dif || NOTCHARGE) {
		w->x += dif;
		w->y = 50;
		Animate_Movement(w, (w->attr & W_CHARGE) ? Charge1 : Walk1, (dif > 0));
	} else if (Check_Move(action, M_PUMMELPUSH))
		New_Pummel_Move(w, PummelPush1);
	else if (Check_Move(action, M_PUMMELTHROW))
		New_Pummel_Move(w, PummelThrowLeft);
	else if (Check_Move(action, M_PUMMELLIFT))
		New_Pummel_Move(w, PummelLift1);
	else if (Check_Move(action, M_SHOVELEFT))
		New_Pummel_Move(w, -ShoveLeft);
	 else if (Check_Move(action, M_LIFTMOVE))
		g->wrestlers[0].x += 1 - 2 * ((w->attr & W_DIRECTION) == FALSE);
	else if (Check_Move(action, M_TAUNT1)) {
		w->Counter = 0;
		w->Graphic = TauntLeft1 + Direction(w);
		w->attr   |= (W_TAUNT1 | W_MOVEABLE);
		Add_Health(w, 1 + (w->c->Health / 6));  // Wrestler gains Health by taunting
	} else if (Check_Move(action, M_PUNCH)) {
		w->Counter = 0;
		Initiate_Punch(w);  // Wrestler is Punching
	} else if (Check_Move(action, M_WALK1))
		w->Graphic = Walk1 + Direction(w);
	else if (Check_Move(action, M_TAUNT2)) {
		w->Counter = 0;
		w->attr = (w->attr & ~W_ACTIONS) | (W_TAUNT2 | W_MOVEABLE);
		Add_Health(w, 1 + (w->c->Health / 6));  // Wrestler gains Health by taunting
	}
}


/* ----------------------------------------------------------- */
/* --- Update all special Actions/Moves for both Wrestlers --- */
/* ----------------------------------------------------------- */
static inline void Handle_Actions(void) {
	register wrestler *w0 = &g->wrestlers[0];
	         wrestler *w1 = &g->wrestlers[1];
	if (g->Move0)
		Initiate_Actions(w0, g->Move0);
	if (g->Move1)
		Initiate_Actions(w1, g->Move1);
	wrestler *move        = &g->wrestlers[(w0->attr & W_ANIM) != FALSE];
	wrestler *moved       = &g->wrestlers[(w0->attr & W_ANIM) == FALSE];
	short Action   = ABS(g->Mixed_Gfx);
	short Dir_move = Direction(move);
	
	if (_ACTION_(w0->attr | w1->attr)) {  // Punching/being Punched/Taunting/Throw/Thrown/Edge/Shoved/Growling
		Check_ACTION(w0);
		Check_ACTION(w1);
	} else if (Action == PummelPush1 && ++w0->Counter > 6) {
		// One wrestler is pushing the other back
		if (w0->Counter == 7 && Reversal(move)) {
			g->Mixed_Gfx = Reversed;
			w0->Counter  = 0;
		} else if (w0->Counter > 14)
			g->Mixed_Gfx = Pummel1;
		else if (w0->Counter & 1) {
			const short anim = (!Dir_move);
			w0->x += 3 - (anim << 2) - (anim + anim);
			static const char y_off[4] = { -12, -4, 4, 12 };
			/*{x, y offsets after every second frame
			{3, -12}, // 3, -12
			{3, -4},  // 6, -16
			{3, 4},   // 9, -12
			{3, 12},   // 12, 0*/
			w0->y += y_off[(w0->Counter - 7) >> 1];
		}
	} else if (Action == PummelThrowLeft && ++w0->Counter >= 6) {
		// One wrestler is throwing the other in the opposite direction
		
		//if (moved->Control == g->Calc && _KeyCheck(SHIFTKEY) && Finish(moved, NO_WEIGHT)) {
		
		if (Reversal(move)) {
			g->Mixed_Gfx = PummelThrowCounter1;
			w0->attr ^= W_ANIM;
		} else {
			short fin = Finish(move, WEIGHTED);
			short x   = (Dir_move ? w0->x + 10 : w0->x - 2);
			
			if (fin) {  // Three variants of the PummelThrow finish
				fin = (random(3) ? PummelThrowLeft : BackwardsSlamLeft);
				const short dif = (Dir_move ? x - 132 : 108 - (x + Wrestlers[ThrowLeft].Width));
				
				if (dif > 0)
					fin = ((random(3) && dif <= 12) ? BackwardsSlamLeft : TossBackLeft);
			}
			
			if (fin == BackwardsSlamLeft) {
				Initiate_EarthQuake();
			} else {
				short x2 = w0->x;
				move->x  = x;
				moved->x = (Dir_move ? x2 + 31 : x2 - 30);
				
				move->Graphic  = ThrowLeft + Dir_move;
				move->attr    |= W_THROW;
				move->Counter  = -1;
				moved->Graphic = ThrownLeft - Dir_move;
				moved->attr   |= W_THROWN;
				moved->Counter = -1;
				g->Mixed_Gfx   = FALSE;
			}
			
			if (fin > 0)
				Initiate_Finisher(moved, fin);
		}
	} else if (Action >= PummelLift1 && Action <= PummelLift6) {
		// One wrestler is Lifting the other up
		if (++w0->Counter >= 41)
			g->Mixed_Gfx = Pummel1;
		else if (w0->Counter == 11) {
			short fin = Finish(move, WEIGHTED);
			short bounds = (w0->x > 86 - (Dir_move << 4) && w0->x < 140 - (Dir_move << 4));
			
			if (Reversal(move)) {
				g->Mixed_Gfx = Reversed;// + (bounds && (moved->Control == g->Calc && _KeyCheck(SHIFTKEY)));
				w0->Counter  = 0;
			} else if (fin && bounds) {
				Initiate_Finisher(moved, PummelLift1);
				
				move->Graphic  = ThrowLeft + Dir_move;
				moved->Graphic = Tumble1;
				move->x  = w0->x;
				moved->x = move->x + 16 - ((Dir_move == FALSE) << 5);
				moved->flag = -2;
			} else if (fin)
				w0->attr |= W_LIFTFINISH;
			else w0->attr &= ~W_LIFTFINISH;
		} else if (w0->Counter > 11 && ((w0->Counter - 9) & 3) == 0)
			g->Mixed_Gfx = PummelLift3 + 2 * (g->Mixed_Gfx == PummelLift3);
	} else if (Action == ShoveLeft) {
		// One wrestler is shoving the other out of the Pummel position
		
		if (++w0->Counter <= 1) {
			short x  = w0->x;
			move->x  = (Dir_move ? x - 4 : x + 8);
			moved->x = (Dir_move ? x + 15 : x - 1);
			
			move->Graphic  = ShoveLeft + Dir_move;
			moved->Graphic = ShovedLeft - Dir_move;
			moved->attr |= DRAW_FIRST;
		} else if (w0->Counter >= 5) {
			if (Reversal(move)) {
				g->Mixed_Gfx = -Reversed;
				w0->Counter  = 0;
			} else {
				moved->x += -8 + (Dir_move << 4);
				moved->attr |= W_SHOVED;
				moved->Counter = -1;
				move->Graphic = Walk1 + Dir_move;
				g->Mixed_Gfx = FALSE;
			}
		}
	} else if (Action == Spinn1) {
		// Both Wrestlers spinning backwards after charging headon and colliding
		unsigned char Dir_w0  = Direction(w0);
		
		if (w0->Counter == 0) {
			w0->Graphic = Spinn1 + Dir_w0;
			w1->Graphic = Spinn2 - Dir_w0;
		}
		if (w0->Counter == 20) {
			w0->Graphic = Walk1 + Dir_w0;
			w1->Graphic = Walk2 - Dir_w0;
			g->Mixed_Gfx = FALSE;
		} else {
			w0->x += 1 - (Dir_w0 + Dir_w0);
			w1->x -= 1 - (Dir_w0 + Dir_w0);
			
			if (++w0->Counter % 3 == FALSE) {
				if (Dir_w0) {
					w0->Graphic += 1 - ((w0->Graphic >= Spinn4) << 2);
					w1->Graphic -= 1 - ((w1->Graphic <= Spinn1) << 2);
				} else {
					w0->Graphic -= 1 - ((w0->Graphic <= Spinn1) << 2);
					w1->Graphic += 1 - ((w1->Graphic >= Spinn4) << 2);
				}
			}
		}
	} else if (Action == Charge1) {
		// One wrestler has charged into the other wrestler, who is being pushed back
		
		if (--w0->Counter <= 0) {
			move->Graphic = Walk1 + Dir_move;
			g->Mixed_Gfx  = FALSE;
		} else {
			const short add = -1 + (Dir_move + Dir_move);
			move->x  += add;
			moved->x += add;
			// Take off Health from moved here
			Add_Health(moved, -1);
		}
	} else if (Action == PummelThrowCounter1 && ++w0->Counter > 13) {
		// Handle a special, reversal finish -- where a tossed wrestler lands on and squishes his opponent
		if (w0->Counter == 14 && (w0->flag = (Finish(moved, WEIGHTED) && ((move->Control == g->Calc && 
		_KeyCheck(B_KEY)) || move->Control == AI))))
			w0->attr ^= W_ANIM;
		else if (w0->flag) {
			Initiate_Finisher(moved, PummelLift2);
			
			w0->flag = FALSE;
			move->Graphic  = ThrowLeft + Dir_move;
			moved->Graphic = Tumble1;
			move->x  = w0->x;
			moved->x = move->x + 16 - ((Dir_move == FALSE) << 5);
			moved->flag = -2;
			move->attr |= DRAW_FIRST;
		} else g->Mixed_Gfx = Pummel1;
	} else if (Action == Reversed) {
		// One wrestler has stopped/reversed the other's move
		if (g->delay == 0 && ++w0->Counter > 4) {
			if (Dir_move && move == w1 && g->Mixed_Gfx < 0)
				w0->x = w1->x;
			g->Mixed_Gfx = Pummel1;
		} else {  // Display Sweat/Blood Graphic above wrestler who is attempting the move unsuccessfully
			wrestler *m = (g->Mixed_Gfx > 0 ? w0 : move);
			unsigned short gfx = m->Graphic;
			unsigned short offset = ((gfx >= PummelLift1 && gfx <= PummelLift6) << 3);
			short x = m->x + Wrestlers[gfx].x_off - g->x_off + (Dir_move?-1-offset:Wrestlers[gfx].Width-7+offset);
			short y = m->y + Wrestlers[gfx].y_off - g->y_off;
			
			if (w0->Counter & 1) {
				y += -8 + 2 * (offset == FALSE);
				Draw_Blood(x, y + 8, Dir_move);
				Draw_Blood(x - 8 + (Dir_move << 4), y, Dir_move);
			} else {
				if (offset == FALSE)
					x -= -1 + (Dir_move + Dir_move);
				
				Draw_Blood(x, y - 3 - 2 * (g->Mixed_Gfx < 0), Dir_move + 2);
			}
		}
	}
	
	Align_Synchronize_Wrestler(w0);
	Align_Synchronize_Wrestler(w1);
}

/* ------------------------------------------------------------------------ */
/* --- Update both wrestlers during the ending Finisher Move of a Match --- */
/* ------------------------------------------------------------------------ */
static inline void Handle_Finish(void) {
	wrestler *w0   = &g->wrestlers[0];
	wrestler *move = &g->wrestlers[(w0->attr & W_ANIM) != FALSE];
	short Dir_move = Direction(move);
	short Lift2    = (g->Status == PummelLift2);
	
	if (g->Status == M_Finish) {
		// End of Match Laughing animation, where opponent lies still
		if (g->delay == FALSE)
			move->flag = !move->flag;
		move->Graphic = Laughing1 + (move->flag != FALSE);
		move->y = 50;
		
		g->Delay_Max = 2;  // Make crowd animate faster (simulating excitement/clapping)
		if (g->EarthQuake < 0 && ++w0->Counter > 130)  // Stop Match after a pause
			g->Status = Match_Won_1 - (move == w0);
	} else {
		register wrestler *moved = &g->wrestlers[!(w0->attr & W_ANIM)];
		
		if (g->Status == PummelThrowLeft) {
			// Wrestler has been thrown sideways out of the Ring
			short x = (moved->x += -2 + (Dir_move << 2));// + Wrestlers[moved->Graphic].x_off;
			
			if (x <= Left_Edge || x + Wrestlers[moved->Graphic].Width >= Right_Edge) {
				g->Status = Spinn1;
				w0->Counter = FALSE;
				return;
			} else if (w0->Counter == FALSE) {
				moved->Graphic = Tumble1;
				moved->y = 48;
			} else if (g->delay & 1) {
				if (Dir_move)
					moved->Graphic += -1 + 12 * (moved->Graphic <= Tumble1);
				else moved->Graphic += 1 - 12 * (moved->Graphic >= Tumble12);
			}
		} else if (g->Status == BackwardsSlamLeft) {
			// One wrestler is slamming the other into the Ring
			if (w0->Counter == FALSE) {
				move->Graphic  = BackwardsSlamRight - Dir_move;
				moved->Graphic = Slammed;
				move->x  = w0->x + (Dir_move ? 15 : -9);
				moved->x = move->x + (Dir_move ? 21 : 8 - 30);
				moved->attr |= DRAW_FIRST;
			}
			if (g->EarthQuake < 0)
				g->Status = M_Finish;
		} else if (g->Status == ShovedRightEdge1 || g->Status == Tumble3) {
			// After being shoved out of the ring, wrestler is falling to his back
			moved->y++;
			moved->x += -1 + (Dir_move + Dir_move);
			if (moved->x < 0)
				moved->x = 0;
			if (moved->x > (240 - 31))
				moved->x = (240 - 31);
			
			if (g->Status == ShovedRightEdge1) {
				if ((g->delay & 3) == FALSE) {
					moved->flag = !moved->flag;
					moved->Graphic = ShovedRightEdge1 - Dir_move + 2 * (moved->flag != FALSE);
				}
				if (moved->y >= 80) {
					moved->y = 85;
					moved->x = (Dir_move ? (240 - 31) : 0);
					moved->Graphic = TossBackRight - Dir_move;
					g->Status = M_Finish;
				}
			} else if (moved->y >= (128 - 48))  // Tumble3
				g->Status = M_Finish;
		} else if (g->Status == TossBackLeft) {
			// Wrestler is tossed (small) to his back
			if (w0->Counter == FALSE) {
				moved->Graphic = TossBackRight - Dir_move;
				moved->attr |= DRAW_FIRST;
				moved->flag = -1;
			}
			
			moved->y += moved->flag;
			if (moved->y <= 30)
				moved->flag = 1;
			if (moved->y >= 55)
				g->Status = M_Finish;
			else if ((g->delay & 3) == FALSE)
				moved->x += -1 + (Dir_move + Dir_move);
		} else if (Lift2 || g->Status == PummelLift1) {
			// Wrestler thrown high up offscreen, then comes crashing down into the Ring
			// PummelLift1:  Thrown wrestler lands on and indents the ring  (and loses)
			// PummelLift2:  Thrown wrestler lands on and squashes opponent (and wins)
			
			if (Lift2 && moved->flag > 0 && moved->y + 2 >= move->y) {
				moved->Graphic = BellyFlopRight2 - Dir_move;
				move->Graphic  = SmushedLeft + Dir_move;
				move->Health   = 0;
				moved->y += 1 + (g->delay & 1);
			} else {
				if ((g->delay & 3) == FALSE) {
					if (Lift2 == FALSE) {
						moved->x += -1 + (Dir_move + Dir_move);
						
						if (moved->x > 156)
							moved->x = 156;
						if (moved->x < 39)
							moved->x = 39;
					}
					
					if (Lift2 == FALSE || moved->flag < 0) {
						if (Dir_move)
							moved->Graphic += -1 + 12 * (moved->Graphic <= Tumble1);
						else moved->Graphic += 1 - 12 * (moved->Graphic >= Tumble12);
					}
					
					if (!g->delay && (Lift2 == FALSE || (move->flag = !move->flag && (moved->flag < 0 || 
					moved->y < -150))))
						moved->flag += 1 - 2 * (moved->flag < 0);
				}
				
				moved->y += moved->flag;
			}
			
			if (moved->y <= -230) {
				moved->flag = 1;
				if (Lift2) {
					moved->x = move->x + (Dir_move ? 2 : 3);
					moved->Graphic = BellyFlopRight1 - Dir_move;
					move->Graphic  = Walk1 + Dir_move;
				}
			}
			
			if (moved->y >= 66) {
				if (Lift2 == FALSE) {
					moved->Graphic = Earthquake_ThrowLeft + Dir_move;
					moved->y = 80;
					moved->x -= (Dir_move ? 12 : 3);
				} else w0->attr ^= W_ANIM;
				
				Initiate_EarthQuake();
				g->Status = M_Finish;
			}
		} else if (g->Status == Spinn1) {
			// Wrestler is spinning out of the ring
			if (w0->Counter == FALSE) {
				moved->Graphic = Spinn1;
				moved->Counter = (moved->x + (Dir_move ? (240 - 25) : 0)) >> 1;  // midpoint for spin
				moved->flag    = FALSE;
			}
			if ((g->delay & 3) == FALSE) {  // Animate spin
				if (Dir_move)
					moved->Graphic += -1 + ((moved->Graphic <= Spinn1) << 2);
				else moved->Graphic += 1 - ((moved->Graphic >= Spinn4) << 2);
			}
			
			moved->x += -1 + (Dir_move + Dir_move);
			short No_Frames = ABS(moved->Counter - moved->x);
			if (No_Frames == 0)  // Protect against 'divide by zero' errors
				moved->flag = TRUE;  // wrestler has reached middle of spin (where he will be highest in the air)
			else if (moved->flag == FALSE)
				moved->y -= (moved->y - 22)/No_Frames; // (y_min is ~22)
			else {
				No_Frames = ABS((Dir_move ? (240 - 25) : 0) - moved->x);
				
				if (No_Frames == 0) {  // Spinn has ended, wrestler will now lay on his back
					moved->y = 54;
					if (Dir_move) {
						moved->Graphic = Tumble12;
						moved->x = (240 - 34);
					} else moved->Graphic = Tumble3;
					w0->Counter = 1;
					g->Status = M_Finish;
				} else moved->y += (52 - moved->y)/No_Frames;
			}
		}// else if (g->Status == ) {
	}
	
	if (w0->Counter == FALSE)
		w0->Counter = TRUE;
}

/* ------------------------------------------------------------ */
/* --- Checks for an overlap/collision of the two wrestlers --- */
/* ------------------------------------------------------------ */
short Hit_Detection(short Hit_Amount) {
	register wrestler *w0 = &g->wrestlers[0];
	register wrestler *w1 = &g->wrestlers[1];
	short x0 = w0->x + Wrestlers[w0->Graphic].x_off;
	short x1 = w1->x + Wrestlers[w1->Graphic].x_off;
	
	return (Direction(w0) ? (x0 + Wrestlers[w0->Graphic].Width - Hit_Amount >= x1) : 
	(x1 + Wrestlers[w1->Graphic].Width - Hit_Amount >= x0));
	// Maybe return the Hit_Amount, instead of passing it as a parameter
	// Ex.  if (Hit_Detection() > 18) {  ??? Viellicht nicht auch.
}

/* ------------------------------------------------------- */
/* --- Initiate a Move/Action from the Pummel position --- */
/* ------------------------------------------------------- */
void New_Pummel_Move(wrestler *w, char Action) {
	wrestler *w0 = &g->wrestlers[0];
	
	// Move will control both wrestler's actions
	w0->attr     = (w0->attr & ~(W_ANIM | W_ACTIONS)) | ((w->attr & W_NO) << 4);
	w0->Counter  = 0;
	g->Mixed_Gfx = Action;
	g->wrestlers[1].attr &= ~W_ACTIONS;
	
	if (Action != -Charge1)
		Add_Health(&g->wrestlers[!NO(w)], -2 - (w->c->Strength >> 3));
	
	// Check if this move involves both wrestlers switching directions
	if (Action == PummelThrowLeft) {
		w0->attr ^= W_DIRECTION;
		g->wrestlers[1].attr ^= W_DIRECTION;
	}
}

/* ----------------------------------------------- */
/* --- Add or take away health from a wrestler --- */
/* ----------------------------------------------- */
void Add_Health(wrestler *w, short Amount) {
	w->Health += Amount;
	
	// boundary checks
	if (w->Health < 0)
		w->Health = 0;
	if (w->Health > MAX_HEALTH)
		w->Health = MAX_HEALTH;
}

static inline void Animate_Movement(wrestler *w, unsigned char Graphic, unsigned short Dir1) {
	unsigned short Dir2 = Direction(w);
	if (Dir1 == Dir2 && ((w->c->Speed >= 11 && (g->delay & 1)) || 
	(w->c->Speed >= 22 && (g->delay == 2 || g->delay == 6)))) {
	//if (Dir1 == Dir2 && g->delay != 0 && g->delay != 4) {// (g->delay & 1) == FALSE
		w->x += 1 - 2 * (Dir2 == FALSE);
		if (g->delay == 5)
			w->attr ^= W_ANIM;
	}
	if (g->delay == 1)
		w->attr ^= W_ANIM;
	
	w->Graphic = Graphic + Animation(w) + Dir2;
}

/* -------------------------------------------- */
/* --- Draw Wrestler Graphics to the screen --- */
/* -------------------------------------------- */
static inline void Display_Wrestlers(void) {
	register wrestler *w0 = &g->wrestlers[0];
	register wrestler *w1 = &g->wrestlers[1];
	
	// Sometimes, the two wrestlers are still being controlled together, but they do not have 
	// a mixed sprite.  Then, g->Mixed_Gfx < 0
	if (g->Mixed_Gfx > 0) {
		if (g->Mixed_Gfx != Reversed)
			w0->Graphic = g->Mixed_Gfx + (Direction(w0) == !(w0->attr & W_ANIM));
		
		if (w0->attr & W_ANIM)
			Draw_Wrestler(w0, Hiddend, Hiddenl);
		else Draw_Wrestler(w0, Hiddenl, Hiddend);
	} else if (w0->attr & DRAW_FIRST) {
		Draw_Wrestler(w0, Hiddenl, Hiddend);
		Draw_Wrestler(w1, Hiddend, Hiddenl);
	} else {
		Draw_Wrestler(w1, Hiddend, Hiddenl);
		Draw_Wrestler(w0, Hiddenl, Hiddend);
	}
}

/* ----------------------------------------------------------------- */
/* --- Save background behind wrestler and draw sprite to screen --- */
/* ----------------------------------------------------------------- */
void Draw_Wrestler(wrestler *w, void *p0, void *p1) {
	register unsigned char gfx = w->Graphic;
	
	short x = w->x + Wrestlers[gfx].x_off - g->x_off;
	short y = w->y + Wrestlers[gfx].y_off - g->y_off;
	short h = Wrestler_Height(gfx), width = Wrestler_Width(gfx);
	unsigned char *spr0 = Wrestler_Light(gfx), *spr1 = Wrestler_Dark(gfx), *mask = Wrestler_Mask(gfx);
	if (g->Force_Update == FALSE) {
		if (y < 0) {  // Vertical clipping for top of screen
			if (y + h < 0)
				return;
			
			short offset = width * y;
			spr0 -= offset;
			spr1 -= offset;
			mask -= offset;
			h    += y;
			y     = 0;
		}
		
		// Save area behind wrestler sprite first
		SpriteX8Get_R(x, y, h, Hiddenl, g->buf[NO(w)], width);
		SpriteX8Get_R(x, y, h, Hiddend, g->buf[2 + NO(w)], width);
	}
	
	// Draw wrestler over background
	GraySpriteX8_MASK(x, y, h, spr0, spr1, mask, mask, width, p0, p1);
}

/* ------------------------------------------------ */
/* --- Returns the ByteWidth of a given Graphic --- */
/* ------------------------------------------------ */
short __attribute__ ((pure)) Wrestler_Width(short index) {
	//short width = Wrestlers[index].Width;
	return (Wrestlers[index].Width >> 3) + ((Wrestlers[index].Width & 7) != FALSE);
}  // 60,117

/* --------------------------------------------------------------------------- */
/* --- Draw the blood/sweat Graphic when a wrestler is punched or reversed --- */
/* --------------------------------------------------------------------------- */
void Draw_Blood(short x, short y, unsigned char Type) {
	GraySprite8_MASK_R(x, y, 8, blood[Type], blood[Type], blood[Type]+8, blood[Type]+8, Hiddenl, Hiddend);
	
	g->Force_Update = TRUE;
}

/* ------------------------------------------------------------------------------ */
/* --- Prepares wrestlers for beginning of match (after ritual introductions) --- */
/* ------------------------------------------------------------------------------ */
void End_Introductions(void) {
	g->Status = M_Normal;
	clock->Running = TRUE;
	
	register wrestler *w0 = &g->wrestlers[0], *w1 = &g->wrestlers[1];
	w0->attr &= ~W_INTRO;
	w1->attr &= ~W_INTRO;
	// :( Frowning, mad
	w0->Graphic = Tackle4;
	w1->Graphic = Tackle3;
	w0->x = 84, w1->x = 84 + 45;
	w0->y = 50, w1->y = 50;
}

/* ------------------------------------------------- */
/* --- Setup the ending Finisher Move of a Match --- */
/* ------------------------------------------------- */
void Initiate_Finisher(wrestler *moved, short fin) {
	clock->Running = FALSE;  // Stop the Clock
	g->Mixed_Gfx = FALSE;
	g->Status = fin;
	g->Finisher = fin;
	g->wrestlers[0].attr = (g->wrestlers[0].attr & ~(W_ANIM | W_ACTIONS)) | ((!(moved->attr & W_NO)) << 4);
	g->wrestlers[0].Counter = FALSE;
	if (fin != PummelLift2)
		moved->Health = 0;
}

/* --------------------------------------------------------- */
/* --- Initiate a new EarthQuake, screen-bouncing effect --- */
/* --------------------------------------------------------- */
void Initiate_EarthQuake(void) {
	g->EarthQuake = 4;
	g->E_i = 3;
	g->E_add = -1;
	g->E_double = TRUE;
	g->Old_softy = g->softy;
}

/* ---------------------------------------------------------------- */
/* --- Edge of Ring Teetering (and check for a possible finish) --- */
/* ---------------------------------------------------------------- */
void Check_Edge(wrestler *w) {
	wrestler *w0    = &g->wrestlers[0];
	wrestler *move  = &g->wrestlers[(w0->attr & W_ANIM) != FALSE];
	wrestler *moved = &g->wrestlers[(w0->attr & W_ANIM) == FALSE];
	short x = w0->x, Dir_move = Direction(move);
	
	if ((w0->attr & W_LIFTFINISH) && g->Mixed_Gfx >= PummelLift1 && g->Mixed_Gfx <= PummelLift6) {
		move->x  = (Dir_move ? x + 10 : x - 2);
		moved->x = (Dir_move ? move->x + 6 : move->x - 5);
		moved->y = 53;
		moved->attr |= DRAW_FIRST;
		
		move->Graphic  = ThrowLeft + Dir_move;
		moved->Graphic = (Dir_move ? Tumble12 : Tumble3);
		Initiate_Finisher(moved, Tumble3);
	} else if (Finish(move, NO_WEIGHT)) {
		if (g->Mixed_Gfx == PummelPush1) {
			move->x  = (Dir_move ? x - 4 : x + 8);
			moved->x = (Dir_move ? x + 15 : x - 1);
			moved->y = 52;
			move->y  = 50;
			move->Graphic = Walk1 + Dir_move;
			
			Initiate_Finisher(moved, ShovedRightEdge1);
		} else if (w->attr & W_SHOVED) {
			w->y = 52;
			Initiate_Finisher(w, ShovedRightEdge1);
		}
	} else if (w->attr & (W_THROWN | W_SHOVED)) {
		w->Counter = 0;
		w->y = 50;
		w->attr = (w->attr & ~(W_THROWN | W_SHOVED)) | W_EDGE;
	}
}