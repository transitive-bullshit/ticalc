// C Source File
// Created 6/23/2003; 5:47:18 PM

/*//////////////////////////////////*/
//				Plinko v1.0								//
//			By Travis Fischer						//
//				 / Fisch \								//
//																	//
//	email:  phishfud@elknet.net			//
/*//////////////////////////////////*/

#include "Defines.h"					// Include the Definitions and Function Prototypes
#include <tigcclib.h>         // Include All Header Files
#include "Plinkage.h"					// Include Bitmaps and Sprites
#include "extgraph.h"					// Include Thomas Nussbaumer's (excellent) Extgraph Functions
#define Hiddenl								GrayDBufGetHiddenPlane (LIGHT_PLANE)
#define Hiddend								GrayDBufGetHiddenPlane (DARK_PLANE)
#define Activel								GrayDBufGetActivePlane (LIGHT_PLANE)
#define Actived								GrayDBufGetActivePlane (DARK_PLANE)

// Global variables
unsigned char *virtual_dark, *virtual_light;
void *dbuffer;
INT_HANDLER interrupt1;

#include "High_Scores.h"			// Include all High score functions

unsigned char wait_for_keypress(void) {
	unsigned long wait2 = 0;
	
	while( !_keytest (RR_2ND) ) {
		if (wait2++ > 2000) {
			wait2 = 0;
			if ( _keytest (RR_MINUS) )
				OSContrastDn ();//  turn contrast down
			else if ( _keytest (RR_PLUS) )
				OSContrastUp ();//  turns contrast up
		} else if ( _keytest (RR_ESC) )
			return 1;
	}
	while( _keytest (RR_2ND) );
	
	return 0;
}

static inline signed char INTRO(void) {
	unsigned char loop, a;
	signed char HEIGHT = 83;
	
	ClearGrayScreen2B(virtual_dark, virtual_light);  // clear virtual gray planes
	ClearGrayScreen2B(Hiddenl,Hiddend);
	// Clears hidden Gray planes
	
	GraySpriteX8_OR(0, 0, 100, plinkol, plinkod, 20, virtual_light, virtual_dark);
	
	for(a = 10; a--;)
		FADE_LIGHTER();
	
	GraySpriteX8_OR(48, 7, HEIGHT, moneyl, moneyd, 8, Hiddenl, 
		Hiddend);
	GrayDBufToggleSync(); // switches two sets of planes
	ClearGrayScreen2B(Hiddenl,Hiddend);
	
	for(a = 10; a--;)
		FADE_DARKER();
	
	Waitkey()
	
	for(loop = 100; loop--;) {
		
		if (loop > 7) {
			HEIGHT -= (loop<90);
			GraySpriteX8_OR(48, 7, HEIGHT, moneyl, moneyd, 8, Hiddenl, 
			Hiddend);
		}
		
		memcpy((char*) Hiddend+30*loop, virtual_dark, 30*(100-loop));
		memcpy((char*) Hiddenl+30*loop, virtual_light, 30*(100-loop));
		
		GrayDBufToggleSync(); // switches two sets of planes
		DELAY(2);
	}
	
	return 0;
}

void FADE_DARKER(void) {
	OSContrastUp ();//  turn contrast up
	DELAY(30);
}

void FADE_LIGHTER(void) {
	OSContrastDn ();//  turn contrast down
	DELAY(30);
}

void DELAY(short wait) {
	for(; wait--;)
		pokeIO(0x600005,0b11111); /* low power mode, wake up CPU only on AI 1 
	     	                          (grayscale), 2 (Keypress), 3 (AMS clock), 4 
	      	                         (Link), 5 (AMS timer base, needed for APD) 
	        	                       and 6 (ON, always wakes up the CPU)
	         	                      -- Saves Battery Power -- */
}

void GRAPHIC(short x, char y, unsigned char graphic, int type) {
	unsigned char width, HEIGHT;
	
	if (!type) {
		if (graphic > 4) graphic = 8 - graphic;
		width = 32;
		HEIGHT = 14;
	} else {
		width = 12;
		HEIGHT = 16;
	}
	
	DRAW_BOX(x, y, HEIGHT, width);
	
	//if (TYPE == BIG)
	//GraySpriteX8_OR(x+4, y+4, HEIGHT, graphicl, graphicd, width/8, Activel, 
	//	Actived);
	//else  // 32x32
	
	//GraySprite32_OR(x+4, y+4, HEIGHT, (long*) graphicl, (long*) graphicd, 
	//Activel, Actived);
	if (!type)
		GraySprite32_OR(x+4, y+4, HEIGHT, gfxl[graphic], gfxd[graphic], 
		Hiddenl, Hiddend);
	else
		GraySprite16_OR(x+4, y+4, HEIGHT, numbersl[graphic], numbersd[graphic], 
		Hiddenl, Hiddend);
}

void DRAW_BOX(short x, char y, unsigned char HEIGHT, unsigned char width) {
	unsigned char a;
	
	GrayDBufSetHiddenAMSPlane (DARK_PLANE);
		ScrRectFill (&(SCR_RECT){{x, y, x+width+7, y+HEIGHT+7}}, &(SCR_RECT){{0, 0, 159, 99}}, A_REVERSE);
	GrayDBufSetHiddenAMSPlane (LIGHT_PLANE);
		ScrRectFill (&(SCR_RECT){{x, y, x+width+7, y+HEIGHT+7}}, &(SCR_RECT){{0, 0, 159, 99}}, A_REVERSE);
	
	for(a = 0; a < 3; a++) {
		LINE(x+a, y+a, x+width+7-a, y+a, BLACK-a, A_NORMAL);
		LINE(x+width+7-a, y+a, x+width+7-a, y+HEIGHT+7-a, BLACK-a, A_NORMAL); 
		LINE(x+a, y+HEIGHT+7-a, x+width+7-a, y+HEIGHT+7-a, BLACK-a, A_NORMAL);
		LINE(x+a, y+a, x+a, y+HEIGHT+7-a, BLACK-a, A_NORMAL);
	}
	/*width = width + 7;
	HEIGHT = HEIGHT + 7;
	
	DrawGrayRect2B(x, y, x+width, y + HEIGHT, COLOR_WHITE, RECT_FILLED, 
	Hiddenl, Hiddend);
	DrawGrayRect2B(x, y, x+width, y + HEIGHT, COLOR_BLACK, RECT_EMPTY, 
	Hiddenl, Hiddend);
	DrawGrayRect2B(x + 1, y + 1, x+width-1, y+HEIGHT-1, COLOR_DARKGRAY, RECT_EMPTY, 
	Hiddenl, Hiddend);
	DrawGrayRect2B(x + 2, y + 2, x+width-2, y+HEIGHT-2, COLOR_LIGHTGRAY, RECT_EMPTY, 
	Hiddenl, Hiddend);*/
}

void LINE(short x, char y, short x2, char y2, char COLOR, short attr) {
	if (COLOR>LIGHT) {
		GrayDBufSetHiddenAMSPlane (DARK_PLANE); 
		DrawLine (x, y, x2, y2, attr);
	}
	if (COLOR!=DARK) {
		GrayDBufSetHiddenAMSPlane (LIGHT_PLANE);
		DrawLine (x, y, x2, y2, attr);
	}
}

void DRAWALL(unsigned char x, unsigned char y) {
	
	FastCopyScreen(virtual_dark, Hiddend);
	FastCopyScreen(virtual_light, Hiddenl);
	
	GraySprite8_MASK(x, y, 8, chipl, chipd, chipmask, chipmask, Hiddenl, 
	Hiddend);
		
	GrayDBufToggleSync(); // switches two sets of planes
}

static inline unsigned int DROPPING(unsigned char* x) {
	unsigned char loop = 0, y = 2, STATUS = RUNNING;
	signed char sign = 1;
	BOOL PEG = FALSE;
	
	unsigned char peg_X_array[16] =
		{1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1};
	signed char peg_Y_array[16] =
		{-1, -1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 2, 1, 2};
	unsigned int money[9] =
		{100, 500, 1000, 0, 5000, 0, 1000, 500, 100};
		
	do
	{
		if (PEG == TRUE) {
			*x+= sign*peg_X_array[ loop ];
			y+= peg_Y_array[ loop ];
			loop++;
			if (loop == 16) PEG = FALSE;
		} else {
			y++;
			if (pxltst(*x, y) && y < 84) {
				PEG  = TRUE;
				loop = 0;
				sign = 1-2*random(2);
				
				if (_keytest (RR_F1)) sign = -1;  // hmm, what could this do?
				else if (_keytest (RR_F5)) sign = 1;
				
				if (*x == 3) sign = 1;  // Makes sure the ball won't go off the board
				else if (*x == 115) sign = -1;
			}
		}
		
		DRAWALL(*x, y);
		
		//for(wait = 0; wait < 130000; wait++ );  // Slow Downnnnnn
		DELAY(4);
		if (_keytest (RR_ESC)) STATUS = EXIT;
		
	}  while (y < 93 && STATUS == RUNNING);
	
	if (STATUS != RUNNING)
		return EXIT;
	
	*x = (*x - 3) / 14;
	
	return money[*x];
}

static inline char main_game(unsigned char plinkos) {
	unsigned char STATUS = RUNNING, x, y = 0, oldx = 59;
	unsigned int Amount, Total = 0;
	char buffer[15];
	
	FontSetSys(F_4x6);
	
	while( plinkos-- ) {
		
		ClearGrayScreen2B(virtual_dark, virtual_light);
		GraySpriteX8_OR(0, 0, 100, plinkol, plinkod, 20, virtual_light, virtual_dark);
		PRINT_TOTAL(Total);
		SHOW_PLINKOS(5, plinkos - 1, virtual_light, virtual_dark);
		new_chip(oldx, plinkos);
		x = oldx;
		y = 2;
		DRAWALL(x, y);
		
		
		while (STATUS == RUNNING) {
			
			if (_keytest (RR_LEFT) && x > 3)  MOVE_LEFT( &x );
			
			if (_keytest (RR_RIGHT) && x < 115)  MOVE_RIGHT( &x );
			
			if (_keytest (RR_ESC))  STATUS = EXIT;
			
			if (_keytest (RR_2ND) || _keytest (RR_DOWN))  STATUS = DROP;
		}
		
		if (STATUS == EXIT)  return -1;
		
		oldx = x;
		Amount = DROPPING( &x );  // After this, x will be from 0 to 8
		if (Amount == EXIT) return -1;
		
		Total += Amount;
		
		
		
		//GRAPHIC(30, 20, /* HEIGHT */14, /* WIDTH */32, (char*) Zerol, (char*) Zerod, SMALL);
		/*for(y = 1; y < 6; y++) {
			period = 15000*(1+random( y ));
			memset (Actived, 255, LCD_SIZE); // clear virtual gray planes
			memset (Activel, 255, LCD_SIZE);
			for(wait = 1; wait < period; wait++);
			
			period = 15000*(1+random( y ));
			DRAWALL(14 * x + 3, 93);
			for(wait = 1; wait < period; wait++);
		}*/
		
		SHUFFLE( x );
		
		DRAWALL(14 * x + 3, 93);
		DRAWALL(14 * x + 3, 93);
		GRAPHIC(45, 36, x, FALSE);//, gfx_light[x], gfx_dark[x], x); 
		GrayDBufToggleSync(); // switches two sets of planes
		Waitkey()
		
		STATUS = RUNNING;
	}
	
	sprintf(buffer, "Score %d", Total);
	x = (LCD_WIDTH - DrawStrWidth(buffer, F_6x8)) / 2 - 16;
	DrawGrayStrExt2B(x, 2, (char*)buffer, A_NORMAL|A_SHADOWED, F_6x8, 
	Activel, Actived);
	
	wait_for_keypress();
	
	if (CHECK_FOR_HIGH(Total) == QUIT)  // Checks whether or not the user got a high score, writes a new 
	// high score if they did get one, and calls a function to display the new high scores
		return QUIT;
	
	return STATUS;
}

static inline void new_chip(unsigned char oldx, unsigned char plinkos) {
	unsigned char loop, x, y;
	
	x = 145;
	y = 86 - 9 * (plinkos);
	
	for(loop = 1; loop < 30; loop++) {
		
		x -= ((x - oldx) + (30 - loop)/2) / (30 - loop);
		y -= ((y - 2) - (30 - loop)/2) / (30 - loop);
		DRAWALL(x, y);
		//for(wait = 1; wait < 30000; wait++);
		DELAY(5);
	}
}

static inline void MOVE_LEFT(unsigned char* x) {
	unsigned char loop;
	
	for(loop = 1; loop < 8; loop++) {
		--*x;
		DRAWALL(*x, 2);
		//for(wait = 0; wait < 75000; wait++ );  // Slow Down
		DELAY(5);
	}
}

static inline void MOVE_RIGHT(unsigned char* x) {
	unsigned char loop;
	
	for(loop = 1; loop < 8; loop++) {
		++*x;
		DRAWALL(*x, 2);
		//for(wait = 0; wait < 75000; wait++ );  // Slow Down
		DELAY(6);
	}
}

static inline void SHUFFLE(unsigned char x) {
	unsigned char loop, a;
	unsigned char *src1, *src2, *dest1, *dest2;
	long period, wait;
	
	for(a = 1; a<6; a++) {
		period = 13000*(1+random( a ));
		
		DRAWALL(14 * x + 3, 93);
		src1 = Actived;
		src2 = Activel;
		dest1 = Hiddend;
		dest2 = Hiddenl;
		
		for (loop=0;loop<100;loop++,src1+=30,src2+=30,dest1+=30,dest2+=30) {
	      
	     memcpy(dest1, src1 + random(2), 20);
	     memcpy(dest2, src2 + random(2), 20);
	   
	  }
	  GrayDBufToggleSync(); // switches two sets of planes
		for(wait = 1; wait < period; wait++);
	}
}

static inline void PRINT_TOTAL(unsigned int Total) {
	/*PortSet (virtual_dark, 239, 127);
		printf_xy (131, 40, "%d", Total);
	PortSet (virtual_light, 239, 127);
		printf_xy (132, 41, "%d", Total);*/
	char buffer[6];
	sprintf(buffer, "%d", Total);
	
	DrawGrayStrExt2B(131, 40, (char*)buffer, A_NORMAL|A_SHADOWED, F_4x6, 
	virtual_light, virtual_dark);
}

static inline void GET_PRODUCT(unsigned char *n1, unsigned char *n2, char *name) {
// Gives the name and two digits of a random product
	unsigned char rndnum;  // Variable for holding which random product the function will give
	const char * const products[MAX_PRODUCTS] =  // Product names, (all products and prices came directly from 
// watching the Price is Right :)
	{"Mad Gab Board Game", "Coffeemaker", "Retro style phone", "Rival Iceshaper", "Handheld Solitaire Game", 
	"Gumball Telephone", "Mini-Popcorn Machine", "Iced Tea Maker", "Blender", "Woodbury 36 inch Shelf", 
	"Adams Golf GT Spec Putter", "Manhattan Lap Desk", 	"Rotring Tanakor Pencil", 
	"InStep5K Jogging Stroller", "Camille Dragonfly Perfume", "Sony DVP-DVD Player", 
	"Men's Swiss Army Watch", "Coleman Coffee Maker", 	"Challenger Inflatable Boat", 
	"Unisex Blazer Jacket", "LL Bean Hunting Knife", 	"100 Piece Coral Puzzle", "Lego Saturn V Moon Set", 
	"10lb Chrome Dumbell Set", "White Down Comforter", 
	};
	const unsigned char prices[MAX_PRODUCTS] = // Prices that match the products
	{25, 50, 59, 28, 18, 60, 45, 18, 35, 74, 49, 80, 13, 89, 17, 87, 65, 40, 23, 48, 59, 12, 24, 32, 40};
	
	rndnum = random(MAX_PRODUCTS);
	
	*n1 = prices[rndnum]/10;  // Gives the first digit
	*n2 = prices[rndnum]%10;  // Gives the second digit
	
	sprintf (name, "%s",products[rndnum]); // Prints the name of the random product into the buffer name.  Since
	// name, n1, and n2 are pointers, we don't have to return anything.
}

// Main function for showing products and determining how many plinkos you get
static inline int PRODUCTS(void) {
	unsigned char STATUS = RUNNING, FLAG;
	unsigned char plinkos = 1;  // You automatically start out with 1 plinko
	char turn = 1;  // Start out on First turn
	unsigned char n1, n2, real;
	unsigned char x = 0;
	char PRESSED, name[30], option, sign1, sign2;
	typedef unsigned short BOOL;
	BOOL ANSWER;
	
	//const char question[42] = "Which digit is in the actual retail price?";
	
	unsigned char *LIGHT, *DARK;
	
	while(STATUS == RUNNING && turn < 5) {
		
		PRESSED = 0;
		GET_PRODUCT(&n1, &n2, name);  // Send the address of n1, n2, and name.  The reason why name doesn't have a 
		// star is because it is an array and an array automatically give a pointer to it's the first item.
		
		if (random(2)) {  // Discards one of the actual retail price digits and replaces it with a "fake" number
			real = n1;  // Remember the real number to show after a guess
			ANSWER = 1;
			do 
			{
				n1 = random(10);
			} while (real == n1 || n1 == n2);  // Make sure n1 and n2 are not equal and the fake random number is not
			// equal to the real number
		} else {
			real = n2;
			ANSWER = 0;
			do 
			{
				n2 = random(10);
			} while (real == n2 || n1 == n2);
		}
		
		option = random(3);
		
		FLAG = FALSE;
		while(STATUS == RUNNING) {
			
			if (FLAG == FALSE) {
				if (_keytest (RR_LEFT))  x = 0;
				if (_keytest (RR_RIGHT))  x = 1;
			}
			
			LIGHT = Hiddenl;
			DARK = Hiddend;
			
			UPDATE_SCREEN(option, LIGHT, DARK);
			
			sign1 = 1 - 2 * random(2);
			sign2 = 1 - 2 * random(2);
			
			GRAPHIC(56 + (!x)*(sign1 * random(3)), 40 + (!x)*(sign2 * random(3)), n1, TRUE);
			GRAPHIC(82 + (x)*(sign1 * random(3)), 40 + (x)*(sign2 * random(3)), n2, TRUE);
			
			SHOW_PLINKOS(turn, plinkos, Hiddenl, Hiddend);
			
			DrawString (0, name);
			
			if (FLAG != FALSE) {
				GRAPHIC(56 + 26 * (ANSWER == 0), FLAG, real, TRUE);
				if ((FLAG -= 3) <= 40)
					break;
			}
			
			GrayDBufToggleSync(); // switches two sets of planes
			if (FLAG == FALSE) {
				DELAY(5);
				
				if (_keytest (RR_2ND)) PRESSED = 1;
				else if (PRESSED == 1) FLAG = 85;
			}
			
			if (_keytest (RR_ESC))
				return QUIT;
		}
		
		//GRAPHIC(x2, 40, real, TRUE);
		DRAW_BOX(60, 65, 14, 32);
		
		if (ANSWER == x) {  // You guessed right
			plinkos++;
			GraySprite32_OR(64, 69, 14, WINl, WINd, Hiddenl, 
			Hiddend);  // Show WIN
		} else
			GraySprite32_OR(64, 69, 14, wrongl, wrongd, Hiddenl,
			Hiddend);  // Show Wrong
		
		GrayDBufToggleSync(); // switches two sets of planes
		Waitkey();
		
		turn++;
	}
	
	return plinkos;
}
 
static inline void UPDATE_SCREEN(unsigned char option, char *LIGHT, char *DARK) {
	static unsigned char position = 0;
	unsigned char loop;
	unsigned char gfx[8] = {0xF, 0xE1, 0xC3, 0x87, 0x1E, 0x3C, 0x78, 0xF0};
	
	if (!option)
		memset(LIGHT, 255, 3000);
	else if (option == 1)
		memset(DARK, 255, 3000);
	
	for(loop = 0; loop < 96; loop+=8) {
		if (option) {
			// Scrolling Down&Left Lightscreen
			memset(LIGHT + 30*loop, gfx[position], 20);
			memset(LIGHT + 30*loop + 30, gfx[position+3*(position == 1)], 20);
			memset(LIGHT + 30*loop + 60, gfx[position+3*(position == 1 || position == 2)], 20);
			memset(LIGHT + 30*loop + 90, gfx[position+3*(position > 0)], 20);
			
			memset(LIGHT + 30*loop +120, gfx[position+3+4*(!position)], 20);
			memset(LIGHT + 30*loop +150, gfx[position+7*(!position)+3*(position > 1)], 20);
			memset(LIGHT + 30*loop +180, gfx[position+7*(!position)+3*(position > 2)], 20);
			memset(LIGHT + 30*loop +210, gfx[position+7*(!position)], 20);
		}
		if (option != 1) {
			// Scrolling Down&Left Dark screen
			memset(DARK + 30*loop, gfx[position], 20);
			memset(DARK + 30*loop + 30, gfx[position+3*(position == 1)], 20);
			memset(DARK + 30*loop + 60, gfx[position+3*(position == 1 || position == 2)], 20);
			memset(DARK + 30*loop + 90, gfx[position+3*(position > 0)], 20);
			
			memset(DARK + 30*loop +120, gfx[position+3+4*(!position)], 20);
			memset(DARK + 30*loop +150, gfx[position+7*(!position)+3*(position > 1)], 20);
			memset(DARK + 30*loop +180, gfx[position+7*(!position)+3*(position > 2)], 20);
			memset(DARK + 30*loop +210, gfx[position+7*(!position)], 20);
		}
	}
	
	if (option) {
		// Scroll Final four lines at bottom of screen
		memset(LIGHT + 2880, gfx[position], 20);
		memset(LIGHT + 2910, gfx[position+3*(position == 1)], 20);
		memset(LIGHT + 2940, gfx[position+3*(position == 1 || position == 2)], 20);
		memset(LIGHT + 2970, gfx[position+3*(position > 0)], 20);
	}
	
	if (option != 1) {
		memset(DARK + 30*loop, gfx[position], 20);
		memset(DARK + 30*loop + 30, gfx[position+3*(position == 1)], 20);
		memset(DARK + 30*loop + 60, gfx[position+3*(position == 1 || position == 2)], 20);
		memset(DARK + 30*loop + 90, gfx[position+3*(position > 0)], 20);
	}
	
	position++;
	if (position == 4) position = 0;
}

void SHOW_PLINKOS(char total, signed char plinkos, char *destl, char *destd) {
	unsigned char loop;
	
	for(loop = 0; loop < total; loop++) {
	  if (loop <= plinkos)
	  	GraySprite8_MASK(145*(total == 5), 92 - 9*loop - 9*(total == 5), 8, chipl, chipd, chipmask, chipmask, 
	  	 destl, destd);
		else 
			GraySprite8_MASK(145*(total == 5), 92 - 9*loop - 9*(total == 5), 8, blankl, blankd, chipmask, chipmask, 
			destl, destd);
	}
}

void DrawString(short y, char* name) {
	unsigned char x;
	int width = DrawStrWidth(name, F_6x8);
	
	x = (LCD_WIDTH-width)/2 - 4;  // Centers outputted string
	
	DRAW_BOX(x, y, 8, width + 1);
	DRAW_BOX(10, 87, 5, 136);
	/*
	GrayDBufSetHiddenAMSPlane (DARK_PLANE);
		//ScrRectFill (&(SCR_RECT){{x, y, x+width+8, y+15}}, &(SCR_RECT){{0, 0, 159, 99}}, A_REVERSE);
		//ScrRectFill (&(SCR_RECT){{10, 87, 153, 99}}, &(SCR_RECT){{0, 0, 159, 99}}, A_REVERSE);
		FontSetSys (F_6x8);
			DrawStr(x + 4, y + 4, name, A_NORMAL);
		FontSetSys (F_4x6);
		DrawStr(14, 91, "Which digit is in the actual retail price?", A_NORMAL);
		
	GrayDBufSetHiddenAMSPlane (LIGHT_PLANE); 
		//ScrRectFill (&(SCR_RECT){{x, y, x+width+8, y+15}}, &(SCR_RECT){{0, 0, 159, 99}}, A_REVERSE);
		//ScrRectFill (&(SCR_RECT){{10, 87, 153, 99}}, &(SCR_RECT){{0, 0, 159, 99}}, A_REVERSE);
		FontSetSys (F_6x8);
			DrawStr(x + 4, y + 4, name, A_NORMAL);
			DrawStr(x + 5, y + 5, name, A_NORMAL);
		FontSetSys (F_4x6);
			DrawStr(14, 91, "Which digit is in the actual retail price?", A_NORMAL);
			DrawStr(15, 92, "Which digit is in the actual retail price?", A_NORMAL);*/
		
		DrawGrayStrExt2B(x + 4, y + 4, (char*)name, A_NORMAL|A_SHADOWED|A_CENTERED, F_6x8, 
		Hiddenl, Hiddend);
		
		DrawGrayStrExt2B(14, 91, (char*)"Which digit is in the actual retail price?", 
		A_NORMAL|A_SHADOWED, F_4x6, Hiddenl, 
		Hiddend);
}

static inline int MENU(void) {
	int STATUS = RUNNING, choice = 0;
	FastCopyScreen(Actived, virtual_light);
	memset(virtual_dark, 0, LCD_SIZE);
	GraySpriteX8_OR(48, 16, 67, menul, menud, 8, virtual_light, virtual_dark);
	char buf[2] = { 0, 0 };
	
	do  // Loops until you choose new game, exit, or press Home
	{
		do
		{
			DRAW_MENU(choice);
			
			/*if (_keytest (RR_UP) && choice > 0) {
				choice--;
			}
			if (_keytest (RR_DOWN) && choice < 3) {
				choice++;
			}*/
			if (_keytest (RR_DOWN)) {
				if (buf[0] == FALSE) {
					buf[0] = TRUE;
					if (++choice > 3) choice = 0;
				}
			} else buf[0] = FALSE;
			if (_keytest (RR_UP)) {
				if (buf[1] == FALSE) {
					buf[1] = TRUE;
					if (--choice < 0) choice = 3;
				}
			} else buf[1] = FALSE;
			
			if (_keytest (RR_2ND) || _keytest (RR_ENTER)) {
				STATUS = DROP;
				while(_keytest (RR_2ND) || _keytest (RR_ENTER));
			}
			if (_keytest (RR_ESC)) STATUS = EXIT;
		} while (STATUS == RUNNING);
		
		if (STATUS == DROP && choice != 3) {
			STATUS = RUNNING;
			
			switch(choice) {
				case 1:
					if (DISPLAY_SCORES() == QUIT)
						return QUIT;
					break;
				case 2:
					info();
					break;
			}
		}
		
		if (STATUS == EXIT || choice == 3) return -1;
		
	} while(choice);
	
	return STATUS;
}

static inline void DRAW_MENU(int choice) {
	//unsigned char loop;
	unsigned char y = 31 + 13 * choice + 2 * (choice > 1);
	FastCopyScreen(virtual_light, Hiddenl);
	FastCopyScreen(virtual_dark,  Hiddend);
	//memset(Hiddend, 0, LCD_SIZE);
	
	//GraySpriteX8_OR(48, 16, 67, menul, menud, 8, Hiddenl, 
	//Hiddend);
	
	InvertGrayRect2B(48, y, 112, y + 12, Hiddenl, Hiddend);
	//for(loop = 0; loop < 12; loop++)
	//	LINE(48, y + loop, 112, y + loop, BLACK, A_XOR);
	
	GrayDBufToggleSync(); // switches two sets of planes
}

static inline void info(void) {
	int loop;
	const char * const str[12] = 
	{"Plinko V 1.0", "By Travis Fischer", "", "Win as many plinko chips as you can", 
	"by guessing the correct digit in the", "price of the given product.", 
	"Then, drop your chips down the plinko board", "and win BIG!!!", "--July 6, 2003--", "", 
	"Thanks to the TIGCC team and everyone at tict", 
	"phishfud@elknet.net"};
	char buffer[45];
	
	ClearGrayScreen2B(Hiddenl, Hiddend);
	
	FontSetSys (F_4x6);
	
	for(loop = 0; loop < 12; loop ++) {
		sprintf (buffer, "%s", str[loop]);
		
		GrayDBufSetHiddenAMSPlane (LIGHT_PLANE);
			DrawStr((LCD_WIDTH - DrawStrWidth(buffer, F_4x6))/2 + 1, 1 + 8 * loop, buffer, A_NORMAL);
		GrayDBufSetHiddenAMSPlane (DARK_PLANE);
			DrawStr((LCD_WIDTH - DrawStrWidth(buffer, F_4x6))/2, 8 * loop, buffer, A_NORMAL);
		//DrawGrayStrExt2B(0, 8 * loop, (char*)str[loop], A_NORMAL|A_SHADOWED|A_CENTERED, F_4x6, 
		//Hiddenl, Hiddend);
	}
	GrayDBufToggleSync(); // switches two sets of planes
	wait_for_keypress();
}

static inline int Gray_prep(void) {
	// turn interrupts off and set up a fake interrupt for graymode to use.
	interrupt1 = GetIntVec(AUTO_INT_1);
	SetIntVec(AUTO_INT_1, DUMMY_HANDLER);
	
	if (!GrayOn())
		return QUIT;
	
	dbuffer = malloc(GRAYDBUFFER_SIZE); // allocate space for double-buffering
	if (!dbuffer)
		return QUIT;
	
	GrayDBufInit(dbuffer); // enable double-buffering
	return 0;
}

static inline void Cleanup(void) {
	GrayOff();
	SetIntVec(AUTO_INT_1,interrupt1);
	free(virtual_dark);
	free(virtual_light);
	free(dbuffer);
}

void Error_Memory(void) {
	ST_helpMsg("Error, Not Enough Memory.  Free up RAM");
}

// Main Function
void _main(void)
{
signed char STATUS, a;
int plinkos;
unsigned char *Home = malloc(LCD_SIZE); // Allocate the SAVE_SCREEN buffer
if (Home == NULL) {
	Error_Memory();
	return;
}
LCD_save(Home);  // Save LCD HomeScreen Contents

virtual_dark = malloc ( LCD_SIZE );  // Allocate the dark buffer
if (virtual_dark == NULL) {Error_Memory(); return; }
virtual_light=malloc ( LCD_SIZE );  // Allocate the light buffer
if (virtual_light == NULL)	{
	free(virtual_dark);
	Error_Memory();
	return;
}

randomize();

if (Gray_prep() == QUIT) {
	Error_Memory();
	goto end;
}

if (makefile() == QUIT)  // Tests whether or not we need a default save file and installs one if the 
// user doesn't already have one on their calc
	goto end;

for(a = 10; a--;)
	FADE_LIGHTER();
// Clears both Hidden Gray planes
ClearGrayScreen2B(Hiddenl, Hiddend);

GraySpriteX8_OR(52, 37, 27, titlel, titled, 7, Hiddenl, Hiddend);

FontSetSys (F_4x6);
GrayDBufSetHiddenAMSPlane (DARK_PLANE); 
	DrawStr(122, 94, "-- By Fisch", A_NORMAL);
GrayDBufSetHiddenAMSPlane (LIGHT_PLANE); 
	DrawStr(123, 95, "-- By Fisch", A_NORMAL);
GrayDBufToggleSync(); // switches two sets of planes

for(a = 10; a--;)
	FADE_DARKER();
Waitkey();

NEW_GAME:

if ((STATUS = MENU()) == QUIT)
	goto end;

if ((plinkos = PRODUCTS()) == QUIT)
	goto end;

if ((STATUS = INTRO()) == QUIT)
	goto end;

if ((STATUS = main_game(plinkos)) == QUIT)
	goto end;

goto NEW_GAME;

end:
// Clean up at end of Program
Cleanup();
if (Home) {
	LCD_restore(Home);
	free(Home);
	
	GKeyFlush();
	ST_helpMsg("Plinko  --By Travis Fischer");
}
}

// By Fisch