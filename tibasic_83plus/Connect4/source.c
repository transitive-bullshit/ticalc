// C Source File
// Created 3/20/2003; 6:41:33 AM

#define USE_TI89              // Compile for TI-89

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define MIN_AMS 100           // Compile for AMS 1.00 or higher

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

unsigned int player, x=0, a, b, done = 0, key, plr, winner = 0, zwinner = 0, turn;
unsigned long x2;
int y;
unsigned int board[7][7];

static unsigned int plr1d[]=
{ 0x0, 0x3E0, 0x7F0, 0xE78, 0x1C1C, 0x1C1C, 0x180C, 0x1C0C, 0x1C1C, 0xE38, 0x7F0, 0x3E0};
static unsigned int plr1l[]=
{0x0,0x3E0,0x430,0x988,0x1BEC,0x13E4,0x17F4,0x13F4,0x13E4,0x9C8,0x430,0x3E0};

static unsigned int plr2d[]=
{ 0x0, 0x3E0, 0x7F0, 0xFF8, 0x1FFC, 0x1FFC, 0x1FFC, 0x1FFC, 0x1FFC, 0xFF8, 0x7F0, 0x3E0};
static unsigned int plr2l[]=
{0x0,0x3E0,0x430,0x988,0x1BEC,0x13E4,0x17F4,0x13F4,0x13E4,0x9C8,0x430,0x3E0};

static unsigned short tiles[]=
{0xF80F,0xF007,0xE003,0xC001,0xC001,0xC001,0xC001,0xC001,0xC001,0xC001,0xE003,0xF007};

static unsigned char background[]=
{ 0x77, 0xF9, 0xBF, 0xFF, 0xF7, 0xBF, 0xFD, 0xD7};
static unsigned char backgroundl[]=
{0x49,0x4,0x70,0x4,0x89,0x50,0x2,0xA1};


void draw(short a, short b, short plr) {
	if (plr==1) {
	Sprite16( a, b, 12, plr1d, GrayGetPlane (DARK_PLANE), SPRT_XOR);
	Sprite16( a, b, 12, plr1l, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
	} else if (plr==2) {
	Sprite16( a, b, 12, plr2d, GrayGetPlane (DARK_PLANE), SPRT_XOR);
	Sprite16( a, b, 12, plr2l, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);		
	}
}

void ScrollUp(unsigned short* buffer,unsigned short lines) {  // scrolls up the screen at the end, coded in asm
    register short* dest = buffer;
    register short* src  = dest + 15;
    register short  tmplines  = lines;
    tmplines-=2;
    asm volatile (
       "0:
        move.l (%0)+,(%1)+;move.l (%0)+,(%1)+;move.l (%0)+,(%1)+
        move.l (%0)+,(%1)+;move.l (%0)+,(%1)+;move.l (%0)+,(%1)+
        move.l (%0)+,(%1)+;move.w (%0)+,(%1)+
        dbra %2,0b
        clr.l (%1)+;clr.l (%1)+;clr.l (%1)+;clr.l (%1)+
        clr.l (%1)+;clr.l (%1)+;clr.l (%1)+;clr.w (%1)+"
        : "=a" (src), "=a" (dest), "=d" (tmplines)
        : "0"  (src),  "1" (dest), "2"  (tmplines));
}

void back(short a, short b) {
	Sprite8( 113+8*a, b, 8, backgroundl, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
	Sprite8( 113+8*a, b, 8, background, GrayGetPlane (DARK_PLANE), SPRT_XOR);
	}
	
	void title(short a, short b) {
	Sprite8( a, b, 8, backgroundl, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
	Sprite8( a, b, 8, background, GrayGetPlane (DARK_PLANE), SPRT_XOR);
	}	

void _main(void)
{

 menu:
	GrayOn();
 memset (GetPlane (DARK_PLANE), 0, LCD_SIZE);
 memset (GetPlane (LIGHT_PLANE), 0, LCD_SIZE);
 
 for (a=0; a<20; a++)
 	for (b=0; b<13; b++)
 		title(8*a, 8*b);
 		
 GraySetAMSPlane (DARK_PLANE);
 ScrRectFill (&(SCR_RECT){{49,30,109,70}}, ScrRect, A_REVERSE);
 FontSetSys(F_6x8);

 DrawStr(51,31,"Connect 4",A_NORMAL);
 DrawStr(51,41,"Play!",A_NORMAL);
 DrawStr(51,51,"About",A_NORMAL);
 DrawStr(51,61,"Exit",A_NORMAL);
 a=0;
 for (b=0; b<10; b++)
	DrawLine(50, 40+10*a+b, 108, 40+10*a+b, A_XOR);
 
 while (((key=ngetchx())!=KEY_ENTER) && (key!=KEY_ESC)) { // wait til you press enter or escape
	
	for (b=0; b<10; b++)
	DrawLine(50, 40+10*a+b, 108, 40+10*a+b, A_XOR);
	
	if ((key==KEY_UP)*a)
	a-=1;
	else if ((key==KEY_DOWN) && (a!=2))
	a+=1;
	
	for (b=0; b<10; b++)
	DrawLine(50, 40+10*a+b, 108, 40+10*a+b, A_XOR);
	}
	
	if (a==1) {
		GrayOff();
		ClrScr();
	
	FontSetSys(F_8x10);
 	DrawStr(0,0,"Connect 4",A_NORMAL);	
	
	FontSetSys(F_4x6);
	DrawStr(0,15,"Drop your piece down a slot in stratetic places", A_NORMAL);
	DrawStr(0,22,"to try to get 4 of your pieces in a row!", A_NORMAL);
	DrawStr(0,41,"--KEYS--", A_NORMAL);
	DrawStr(0,46,"Escape quits at any time", A_NORMAL);
	DrawStr(0,53,"Enter is to drop your piece", A_NORMAL);
	DrawStr(0,60,"Use left and right to move.", A_NORMAL);
	DrawStr(0,79,"This program was written by Travis Fischer", A_NORMAL);
	DrawStr(0,86,"22/3/03", A_NORMAL);
	DrawStr(0,93,"Thanks to the TIGCC team", A_NORMAL);
	while (((key=ngetchx())!=KEY_ENTER) && (key!=KEY_ESC));		
	}
	if (a==1)
	goto menu;
	
	if ((a!=2)&&(key!=KEY_ESC)) {
	
 reset:
 for (a=0; a<7; a++)
 		for (b=0; b<7; b++)
 			board[a][b]=0;
 x=0;
 done=0;
 winner=0;
 turn=0;
 
  memset (GetPlane (DARK_PLANE), 0, LCD_SIZE);
  memset (GetPlane (LIGHT_PLANE), 0, LCD_SIZE);

	for (a=0; a<7; a++)
		for (b=0; b<8; b++)
			Sprite16( 16*a, 15+12*b, 12, tiles, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
	
	GraySetAMSPlane (LIGHT_PLANE); 				
	DrawLine(112, 15, 112, 99, A_NORMAL);
	
	for (a=0; a<6; a++) {
	back(a, 53);
	for (b=0; b<4; b++)
	back(a, 8*b);
	for (b=0; b<5; b++)
	back(a, 70+8*b);
	}

	while ((!done) && (!winner)) {  // loops the game until the value done is not equal to zero, AKA until not the value done
		turn++;
		FontSetSys(F_6x8);  // sets the small font.  All words displayed will be 6x8
		GraySetAMSPlane (LIGHT_PLANE);  // all drawings will be on LIGHT plane in memory
		printf_xy(116,62,"turn %d",turn);
		GraySetAMSPlane (DARK_PLANE);
		printf_xy(116,62,"turn %d",turn);
		
		for (player=1; player<3-done-winner; player ++) {  // loop for the two players.  the minus done is just if player one presses escape, the loop will not go on to the second player, because done will have a value
			draw(16*x, 1, player);  // draws sprite onto screen
			y=-1;
			
			for (key=0; key<2; key++) {  // loop to write words at right on both light and dark planes, so it will be black, not gray
			if (key)
			GraySetAMSPlane (LIGHT_PLANE);  // all drawings will be on LIGHT plane in memory
			else 
			GraySetAMSPlane (DARK_PLANE);  // all drawings will be on DARK plane in memory
			FontSetSys(F_6x8);  // sets the small font.  All words displayed will be 6x8
			DrawStr(116,33,"Player",A_NORMAL);  // draws the word "Player" at the coordinates (120,0)
			FontSetSys(F_8x10);  // sets the big font.  All words displayed will be 8x10
			printf_xy(132,43,"%d",player);  // displays the current player number at coordinates (135,10). (in an 8x10 font)
			}
			while ((((key=ngetchx())!=KEY_ENTER) || (board[x][0])) && (key!=KEY_ESC) && (key!=KEY_CLEAR)) {  // loop to make move piece along top and check it user presses clear, escape, or enter and if they pressed enter, if there is already a piece in the very top of the board at "x" 's current position. 
			draw(16*x, 1, player);  // erases sprite at top of screen so we can move it
			if ((key==KEY_LEFT) && (x!=0))  // checks to see if you pressed left and make sure you're not off the screen
			x-=1; // move you left one
			if ((key==KEY_RIGHT) && (x!=6))  // checks to see if you pressed right and make sure you're not too far right
			x+=1;	 // move you right one
			draw(16*x, 1, player);  // redraw your sprite at top of screen, at new  x  coordinate.
			}  //end of moving right or left, to pick slot
			
			if ((key==KEY_ESC) || (key==KEY_CLEAR))  // if you pressed escape or ( || is the same thing as saying or) if you pressed clear, assigne done a 1, to end the main loop.
			done=1;
			
			draw(16*x, 1, player);  // erase your piece at top of screen
			
			while ((y!=6) && (!board[x][y+1]))  {// checks to see, starting at the top of the board if there are pieces
			  y++;
			  draw(16*x, 15+12*y, player);
			  for (x2=0; x2<9999; x2++);
			  draw(16*x, 15+12*y, player);
			}
			board[x][y]=player;  // assigns the current player's value to the resting spot of the piece
			draw(16*x, 15+12*y, player);  // draws the piece to end that player's turn
			for (b=0; b<7; b++)
			for (a=0; a<4; a++)
			winner+= ((player==board[a][b])*(player==board[a+1][b])*(player==board[a+2][b])*(player==board[a+3][b])) + ((player==board[b][a])*(player==board[b][a+1])*(player==board[b][a+2])*(player==board[b][a+3]));
			
			for (b=0; b<4; b++)
			for (a=0; a<4; a++)
			winner+= ((player==board[a][b])*(player==board[a+1][b+1])*(player==board[a+2][b+2])*(player==board[a+3][b+3])) + ((player==board[a][b+3])*(player==board[a+1][b+2])*(player==board[a+2][b+1])*(player==board[a+3][b]));
			
			if (winner)
			zwinner=player;
			
			}
			
	}  //you pressed escape or clear after this. (while loop end)

if (key==KEY_CLEAR)  // checks if you want to reset the game, (if you pressed clear)
goto reset;

if (winner) {
	GraySetAMSPlane (LIGHT_PLANE);  
			
	FontSetSys(F_8x10);  // sets the big font.  All words displayed will be 8x10
	printf_xy(5,2,"WINNER! Plr %d",zwinner); 
	GraySetAMSPlane (DARK_PLANE);  
			
	printf_xy(4,1,"WINNER! Plr %d",zwinner); 

	while (((key=ngetchx())!=KEY_ENTER) && (key!=KEY_ESC));
	
	for (x = 0; x<90; x ++) {
	memset (GetPlane (DARK_PLANE), x+random(125), LCD_SIZE);
	for (x2 =0; x2<4000; x2 ++);  // slows down the shuffle
	}
 }
}
GrayOff();  // turns gray mode off, so your calc will not mess up

ClrScr();
FontSetSys(F_8x10);
DrawStr(20,30,"Programmed by:",A_REPLACE);
DrawStr(30,45,"Travis Fischer",A_REPLACE); // that's me :)

while (((key=ngetchx())!=KEY_ENTER) && (key!=KEY_ESC)); // wait til you press enter or escape
 FontSetSys(F_6x8);

}  // end of main void and game