// C Source File
// Created 7/30/2004; 12:24:33 PM
#include "All.h"

// Returns the wrestler chosen by the user
short Choose_Wrestler(const char *text, short No, short External_Only, short Starting) {
	short pause = PAUSE_DELAY, left = TRUE, right = TRUE, pos;
	const short x = g->Zerox + 26 + 80 * No, y = g->Zeroy + 8;
	char buf[100];
	
	pos = g->wrestlers[No].c ? g->wrestlers[No].c->Index : 0;
	if (External_Only) {  // User may only select from External wrestlers (for Editing purposes)
		if (g->NO_WRESTLERS <= NO_DEFAULT_WRESTLERS + 1) {
			#ifdef ENGLISH
			Message((const char *const[]){ "- Error -", "", "No External",  "Wrestlers", "Located on Calc", NULL }, 
				A_DISSOLVE | A_WAITKEY);
			#elif defined(FRENCH)
			
			#elif defined(GERMAN)
			
			#endif
			
			return QUIT;
		}
		
		if (pos <= NO_DEFAULT_WRESTLERS)
			pos = NO_DEFAULT_WRESTLERS + 1;
	}
	while_keypress(ESCKEY);
	
	do 
	{
		
		if (pos >= 0) {  // Print custom information about wrestler in text box
			g->wrestlers[No].c = g->Characters[pos];
			
			if (pause) {
				sprintf(buf, EFG("%s\x0A(Left/Right cycles through)"), text);
			} else {
				const Character *c = g->wrestlers[No].c;
				char weight[10], age[5];
				
				if (c->Weight > 0) {
					sprintf(weight, "%dlbs", c->Weight);
				} else strcpy(weight, "?");
				if (c->Age > 0) {
					sprintf(age, "%d", c->Age);
				} else strcpy(age, "?");
				
				sprintf(buf, EFG("Weight:  %s,  Age: %s\x0A""Origin:  %s"), weight, age, Char_String(c, C_Origin));
			}
		} else {
			// Get a 'Random' graphic!
			strcpy(buf, EFG("Random"));
			g->wrestlers[No].c = NULL;
		}
		
		New_Text(buf, pause);pause = FALSE;
		Draw_Pre_Match(FALSE, NO_EDITOR);
		/*char buf[30];
		sprintf(buf, "%d %d", pos, g->NO_WRESTLERS);
		DrawGrayString(0, y + 45 - 8, buf, A_NORMAL | A_CENTERED | A_SHADOWED, F_4x6);*/
		
		if (left == FALSE)
			left = TRUE;
		if (right == FALSE)
			right = TRUE;
		
		do {
			
			KeyScan();
			//if (pause || (!left != !right && _KeyCheck(LEFTKEY) == FALSE && _KeyCheck(RIGHTKEY) == FALSE))
			
			if (_KeyCheck(LEFTKEY)) {
				GraySprite8_MASK_R(x, y, 6, Arrow[0], Arrow[0], Arrow[2], Arrow[2], Hiddenl, Hiddend);
				
				if (left == FALSE || ++left > 40) {
					GraySprite8_MASK_R(x, y, 6, Arrow[0], Arrow[0], Arrow[2], Arrow[2], Activel, Actived);
					left = TRUE;
					if (--pos < (External_Only ? NO_DEFAULT_WRESTLERS + 1 : -1)) pos = g->NO_WRESTLERS - 1;
					if (pos == Load_Slot) pos--;
					break;
				}
			} else if (left != FALSE) {
				GraySprite8_MASK_R(x, y, 6, Arrow[0], Arrow[0]+6, Arrow[2], Arrow[2], Hiddenl, Hiddend);
				left = FALSE;
			}
			
			if (_KeyCheck(RIGHTKEY)) {
				GraySprite8_MASK_R(x + 23, y, 6, Arrow[1], Arrow[1], Arrow[2]+6, Arrow[2]+6, Hiddenl, Hiddend);
				
				if (right == FALSE || ++right > 40) {
					GraySprite8_MASK_R(x + 23, y, 6, Arrow[1], Arrow[1], Arrow[2]+6, Arrow[2]+6, Activel, Actived);
					right = TRUE;
					if (++pos > g->NO_WRESTLERS - 1 - (g->NO_WRESTLERS == NO_DEFAULT_WRESTLERS + 1))
						pos = (External_Only ? NO_DEFAULT_WRESTLERS + 1 : -1);
					if (pos == Load_Slot) pos++;
					break;
				}
			} else if (right != FALSE) {
				GraySprite8_MASK_R(x + 23, y, 6, Arrow[1], Arrow[1]+6, Arrow[2]+6, Arrow[2]+6, Hiddenl, Hiddend);
				right = FALSE;
			}
			
			if (_KeyCheck(ESCKEY | TEACHERKEY)) {
				Exit_Linkplay();
				return QUIT;
			}
			if (_KeyCheck(ENTERKEY | A_KEY)) {
				if (g->wrestlers[No].c == NULL) // Random wrestler
					g->wrestlers[No].c = g->Characters[random(NO_DEFAULT_WRESTLERS)];
				
				if (External_Only)
					ValidateSumoFileName(g->Sumo_Name, pos);
				
				while_keypress(ENTERKEY | A_KEY);
				return 0;
			}
			
			Run_Text();
			
			if (Starting == TRUE) {
				Dissolve();
				Starting = FALSE;
			} else Toggle_Screens();
			
			pokeIO(0x600005,0b11111);
		} while(1);
	} while(1);
}

short Pre_Match(void) {
	// PreMatch Smack Talk may be toggled On/Off via the Options Menu
	if (g->Smack_Talk == FALSE)
		return 0;
	
	/* For debugging:
	
	const Character *c0 = g->wrestlers[0].c, *c1 = g->wrestlers[1].c;
	memset(Activel, 0, LCD_SIZE);
	memset(Actived, 0, LCD_SIZE);
	PortSet(Actived, 239, 127);
	printf("%d) %d %d\n%d %d\n%d %d\n%d %d\n%d %d\n%d %d\n%d %d\n", g->Calc, c0->Index, c1->Index, 
	c0->Strength, c1->Strength, 
	c0->Health, c1->Health, 
	c0->Stamina, c1->Stamina, 
	c0->Speed, c1->Speed, 
	c0->Age, c1->Age, 
	c0->Weight, c1->Weight
	);
	WaitKey();
	Align_Calcs();*/
	
	char *smack_talk[2] = {
		Char_String(g->wrestlers[0].c, C_Smack + 
		#ifdef CAPTURE_SREENSHOT
		0
		#else
		random(3)
		#endif
		), 
		Char_String(g->wrestlers[1].c, C_Smack + 
		#ifdef CAPTURE_SREENSHOT
		2
		#else
		random(3)
		#endif
		), 
	};
	unsigned short pos = 0, enter = TRUE, Starting = TRUE;
	unsigned char names[2][40];
	sprintf(names[0], "%s:", Char_String(g->wrestlers[0].c, C_Name));
	sprintf(names[1], "%s:", Char_String(g->wrestlers[1].c, C_Name));
	
	Draw_Pre_Match(TRUE, NO_EDITOR);
	do 
	{
		if (g->text == NULL || enter == TRUE) {
			if (pos > 3)
				return 0;
			
			if ((pos & 1) == FALSE) {
				while(smack_talk[pos>>1] == NULL || *smack_talk[pos>>1] == '\0') {
					if ((pos += 2) > 3)
						return 0;
				}
			}
			
			New_Text((pos & 1) ? (char*)smack_talk[pos>>1] : (char*)names[pos>>1], 400);
			pos++;
		}
		
		Run_Text();
		if (Starting && g->Mode == TOURNAMENT) {
			Dissolve();
			Starting = FALSE;
		} else Toggle_Screens();
		pokeIO(0x600005,0b11111);
		
		KeyScan();
		if (_KeyCheck(ENTERKEY | A_KEY))
			enter++;
		else enter = FALSE;
	} while (_KeyCheck(TEACHERKEY | ESCKEY) == FALSE);
	
	Exit_Linkplay();
	return QUIT;
}

void Draw_Pre_Match(short Wrestlers_Chosen, short pos) {
	const Character *c0 = g->wrestlers[0].c, *c1 = g->wrestlers[1].c;
	const short Disp0 = Wrestlers_Chosen || (c0 != NULL && (g->Link == FALSE || g->Calc == Host));
	const short Disp1 = Wrestlers_Chosen || (c1 != NULL && (g->Link == FALSE || g->Calc == Join));
	short x = g->Zerox, y = g->Zeroy, width = 0;
	
	// Display Background and text box
	Display_Interface_Design(FALSE);
	
	// Strings
	DrawGrayString(0, y + 45, EFG("Strength"), A_NORMAL | A_CENTERED | A_SHADOWED, F_4x6);
	DrawGrayString(0, y + 52, EFG("Health"), A_NORMAL | A_CENTERED | A_SHADOWED, F_4x6);
	DrawGrayString(0, y + 59, EFG("Stamina"), A_NORMAL | A_CENTERED | A_SHADOWED, F_4x6);
	DrawGrayString(0, y + 66, EFG("Speed"), A_NORMAL | A_CENTERED | A_SHADOWED, F_4x6);
	
	// Wrestler One
	if (Disp0) {  // Width of Name must be <= 52
		const char *Name = Char_String(c0, C_Name);
		DrawGrayString(x + 25, y + 39, Name, A_NORMAL, F_4x6);
		width = DrawStrWidth(Name, F_4x6);
	}
	GraySpriteX8_MASK(x + 22, y + 8, 28, Wrestler_Light(Stomp1), Wrestler_Dark(Stomp1), Wrestler_Mask(Stomp1), 
		Wrestler_Mask(Stomp1), 4, Hiddenl, Hiddend);
	// VS
	if (pos == NO_EDITOR) {
		// Wrestler Two
		if (Disp1) {
			const char *Name = Char_String(c1, C_Name);
			DrawGrayString(x + 134 - DrawStrWidth(Name, F_4x6), y + 39, Name, A_NORMAL, F_4x6);
		}
		GraySpriteX8_BLIT(x + 102, y + 8, 28, Wrestler_Light(Stomp1), Wrestler_Dark(Stomp1), g->p_maskval, 4, 
			Hiddend, Hiddenl);
		
		GraySpriteX8_BLIT(x + 56, y + 8, 30, vs[0], vs[1], g->p_maskval, 48/8, Hiddenl, Hiddend);
	} else {
		DrawGrayString(0, y + 8, EFG("- Sumo Editor -"), A_NORMAL | A_CENTERED | A_SHADOWED, F_4x6);
		char buf[100];
		short x0 = x;
		sprintf(buf, EFG("Origin: %s"), Char_String(c0, C_Origin));
		DrawGrayString(x + 56, y + 17, buf, A_NORMAL | A_SHADOWED, F_4x6);
		
		if (pos == 0)
			width = DrawStrWidth(buf, F_4x6) + 1;
		
		if (c0->Age > 0) {
			sprintf(buf, EFG("Age: %d"), c0->Age);
		} else strcpy(buf, EFG("Age: ?"));
		DrawGrayString(x + 56, y + 24, buf, A_NORMAL | A_SHADOWED, F_4x6);
		if (pos == 1)
			width = DrawStrWidth(buf, F_4x6) + 1;
		if (c0->Weight > 0) {
			sprintf(buf, EFG("Weight: %dlbs"), c0->Weight);
		} else strcpy(buf, EFG("Weight: ?"));
		DrawGrayString(x + 56, y + 31, buf, A_NORMAL | A_SHADOWED, F_4x6);
		if (pos == 2)
			width = DrawStrWidth(buf, F_4x6) + 1;
		
		DrawGrayString(x + 95, y + 39, EFG("Smack Talk"),  A_NORMAL, F_4x6);
		DrawGrayString(x + 108, y + 66, EFG("-- Back"), A_NORMAL, F_4x6);
		short pos9 = (pos == 9), y0 = y + 17 + 7 * (pos - pos9 - (pos > 3));
		if (pos > 4) {
			width = 31 - 6 * pos9;
			x0 += (pos9 ? 108 : 65);
		} else if (pos == 3)
			x0 += 26;
		else if (pos == 4) {
			x0 += 95;
			width = 38;
		} else x0 += 56;
		
		InvertGrayRect2B(x0, y0, x0 + width, y0 + 5, Hiddenl, Hiddend);
		/*if (pos <= 2)
			y0 += 15 + 7 * pos;
		else if (pos == 3)
			y0 += 39;
		else if (pos <= 7)
			y0 += 45 + 7 * (pos - 4);
		else y0 += 66;*/
		
		// + 9, for F_6x8 if selected
	}
	
	short a;
	// Stat Bars
	for(a = 4; a--;) {
		short y0 = y + 47 + 7 * a;
		GrayDrawRect2B(x + 26, y0, x + 56, y0 + 2, COLOR_LIGHTGRAY, RECT_FILLED, Hiddenl, Hiddend);
		if (Disp0) {
			char x0 = *((char*)(&c0->Strength) + a);
			if (x0 > 0)
				GrayDrawRect2B(x + 26, y0, x + 25 + x0, y0 + 2, COLOR_BLACK, RECT_FILLED, Hiddenl, Hiddend);
		}
		
		if (pos == NO_EDITOR) {
			GrayDrawRect2B(x + 133, y0, x + 103, y0 + 2, COLOR_LIGHTGRAY, RECT_FILLED, Hiddenl, Hiddend);
			if (Disp1) {
				char x1 = *((char*)(&c1->Strength) + a);
				if (x1 > 0)
					GrayDrawRect2B(x + 133, y0, x + 134 - x1, y0 + 2, COLOR_BLACK, RECT_FILLED, Hiddenl, Hiddend);
			}
		}
		
		/*memset(Activel, 0, LCD_SIZE);
		memset(Actived, 0, LCD_SIZE);
		PortSet(Actived, 239, 127);
		printf("%d) %d == %d", a, g->Zeroy, y);
		WaitKey();*/
	}
}

// Add a shop, with credits to buy more power (get credits after winning tournament matches)
// Tournament screen, have Sumo Face sprites represent a Win or a Loss there (Smiling or Frowning)
void Post_Match(void) {
	unsigned char Result = g->Status - Match_Won_0, time_expired = (g->Status == Time_Expired), star_anim = 0;
	short anim = FALSE, lose_anim = FALSE, a;
	
	/* ------------------------------------------------------------ */
	/* --- Draw Background, Text, and Sumo Winner/Loser sprites --- */
	/* ------------------------------------------------------------ */
	Display_Interface_Design(FALSE);  // Draw the Background
	DrawGrayString(0, g->Zeroy + 8, EFG("Match Summary"), A_NORMAL | A_CENTERED | A_SHADOWED, F_4x6);
	DrawGrayString(g->Zerox + 25, g->Zeroy + 40, Char_String(g->wrestlers[0].c, C_Name), A_NORMAL | A_SHADOWED, 
		F_4x6);
	
	const char *Name = Char_String(g->wrestlers[1].c, C_Name);
	DrawGrayString(g->Zerox + 134 - DrawStrWidth(Name, F_4x6), g->Zeroy + 40, Name, A_NORMAL | A_SHADOWED, 
		F_4x6);
	
	char buf[100];
	for(a = 0; a < 3; a++) {
		short b = clock->Seconds;
		if (g->Time_Limit)
			b = g->Time_Limit - b;
		
		sprintf(buf, EFG("Match Time: %d sec"), b);
		
		if (a == 0) {
			if (time_expired) {
				strcpy(buf, EFG("Time Expired"));
				a++;
			} else sprintf(buf, EFG("Winner: %s"), Char_String(g->wrestlers[Result].c, C_Name));
		} else if (a == 1) {
			b = 0;
			while(g->Finisher != finish_text[b].No) b++;
			sprintf(buf, EFG("Finish Move: %s"), finish_text[b].text);
		}
		
		DrawGrayString(g->Zerox + 40, g->Zeroy + 49 + (a << 3), buf, A_NORMAL | A_SHADOWED, F_4x6);
	}
	
	if (time_expired)
		New_Text(EFG("Match ends in a tie!"), PAUSE_DELAY);
	else if (g->wrestlers[0].Control != AI || g->wrestlers[1].Control != AI) {
		New_Text((g->wrestlers[Result].Control == g->Calc) ? EFG("Congratulations!") : 
		(random(2) ? EFG("Better luck next time") : EFG("You lost!")), PAUSE_DELAY);
	} else New_Text(EFG("The calculator is victorious"), PAUSE_DELAY);
	
	// Display hidden wrestler loser sprites
	Display_Loser(!Result, -1, Hiddenl, Hiddend);
	if (time_expired)  // Both wrestlers lose
		Display_Loser(!!Result, -1, Hiddenl, Hiddend);
	
	Dissolve();
	do {
		
		Run_Text();
		Toggle_Screens();
		if (time_expired) {
			Display_Loser(1, lose_anim, Activel, Actived);
			Display_Loser(0, lose_anim, Activel, Actived);
		} else {
			Display_Loser(!Result, lose_anim, Activel, Actived);
			Display_Winner(Result, anim, star_anim);
		}
		
		if (++star_anim >= 4) {
			star_anim = 0;
			anim = !anim;
		}
		if (lose_anim >= 9)
			lose_anim = 0;
		
		lose_anim += (star_anim + anim == FALSE);
		
		for(a = 32; a--;) {
			pokeIO(0x600005,0b11101);
			if (_rowread(0)) {
				KeyScan();
				if (_KeyCheck(TEACHERKEY))
					LeadOut();
				
				if (_KeyCheck(ENTERKEY | A_KEY | ESCKEY))
					return;
			}
		}
		/*if (_KeyCheck(ENTERKEY | A_KEY)) {
			if (enter == FALSE && g->Mode == TOURNAMENT)
				New_Text("", PAUSE_DELAY);
			enter++;
		}
		else enter = FALSE;*/
	} while(1);
	
	/*Winner is jumping the whole time
	Loser facing away, turns head, pause - cries - pause, turns back facing away again
	- 4 for jumping sprite
	Have a 32x32 rotating star (perhaps mix of light grey/white )(light grey or white / mix) behind winner
	
	
	Just added different sprites (winner, loser, star)*/
	//bah
	
	//star at 25,7
}

void Display_Loser(short No, short lose_anim, void *p0, void *p1) {
	short gfx = ((lose_anim >= 5 && lose_anim <= 7) ? MatchLoser2Right - No : MatchLoser1);
	if (gfx == MatchLoser1 && lose_anim >= 0)
		return;
	
	if (No) {
		void *temp = p0;
		p0 = p1;
		p1 = temp;
	}
	WRESTLER *w = &Wrestlers[gfx];
	GraySpriteX8_BLIT(g->Zerox + 26 + 84 * No, g->Zeroy + 39 - w->Height, w->Height, w->Light, w->Dark, 
		g->p_maskval, 3, p0, p1);
	
	if (lose_anim == 7) {  // Draw tear on losing wrestler's face
		GraySprite8_MASK_R(g->Zerox + 37 + 82 * No, g->Zeroy + 20, 3, g->p_maskval, g->p_maskval, 
			tear, tear, p0, p1);
	}
}

void Display_Winner(short No, short anim, char star_anim) {
	WRESTLER *star = &Wrestlers[Star1 + star_anim];
	GraySpriteX8_BLIT(g->Zerox + 54 + 34 * No, g->Zeroy + 20, 16, star->Light, star->Dark, 
		g->p_maskval, 2, Activel, Actived);
	
	WRESTLER *w = &Wrestlers[MatchWinner1 + anim];
	GraySpriteX8_BLIT(g->Zerox + 26 + 84 * No, g->Zeroy + 12 - 4 * anim, 27, w->Light, w->Dark, 
		g->p_maskval, 3, (No == 0 ? Activel : Actived), (No == 0 ? Actived : Activel));
}

/*GraySpriteX8_OR(3, 4, 92, Wrestler_Light(Design_Left), Wrestler_Dark(Design_Left), 
		Wrestler_Width(Design_Left), Hiddenl, Hiddend);
	GraySpriteX8_OR(138, 4, 92, Wrestler_Light(Design_Right), Wrestler_Dark(Design_Right), 
		Wrestler_Width(Design_Right), Hiddenl, Hiddend);
	
	GrayDrawRect2B(0, 0, 159, 99, COLOR_BLACK,     RECT_EMPTY, Hiddenl, Hiddend);
	GrayDrawRect2B(1, 1, 158, 98, COLOR_DARKGRAY,  RECT_EMPTY, Hiddenl, Hiddend);
	GrayDrawRect2B(2, 2, 157, 97, COLOR_LIGHTGRAY, RECT_EMPTY, Hiddenl, Hiddend);
	
	GrayDrawRect2B(22, 4, 137, 95, COLOR_LIGHTGRAY, RECT_EMPTY, Hiddenl, Hiddend);
	GrayDrawRect2B(23, 5, 136, 94, COLOR_DARKGRAY,  RECT_EMPTY, Hiddenl, Hiddend);
	GrayDrawRect2B(24, 6, 135, 93, COLOR_BLACK,     RECT_EMPTY, Hiddenl, Hiddend);
	
	GrayDrawLine2B(29, 63, 130, 63,  COLOR_LIGHTGRAY, Hiddenl, Hiddend);
	GrayDrawLine2B(29, 91, 130, 91,  COLOR_LIGHTGRAY, Hiddenl, Hiddend);
	GrayDrawLine2B(31, 65, 31, 89,   COLOR_DARKGRAY,  Hiddenl, Hiddend);
	GrayDrawLine2B(128, 65, 128, 89, COLOR_DARKGRAY,  Hiddenl, Hiddend);
	
	GrayDrawRect2B(30, 64, 129, 90, COLOR_BLACK,     RECT_EMPTY, Hiddenl, Hiddend);
	GrayDrawRect2B(32, 65, 127, 89, COLOR_LIGHTGRAY, RECT_EMPTY, Hiddenl, Hiddend);
	
	short a = 64, anim = 0;
	for(; a < 91; a += 3, anim = !anim) {
		GraySprite8_OR(25, a, 3, design_2[0] + 3 * anim, design_2[1] + 3 * anim, Hiddenl, Hiddend);
		GraySprite8_OR(130, a, 3, design_2[0] + 3 * (!anim), design_2[1] + 3 * (!anim), Hiddenl, 
			Hiddend);
	}*/