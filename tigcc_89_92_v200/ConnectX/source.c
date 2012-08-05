// C Source File
// Created 3/23/2003; 10:48:44 AM

#define USE_TI89              // Compile for TI-89

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define MIN_AMS 100           // Compile for AMS 1.00 or higher

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

unsigned short int player, a, b, done, winner, key, turn, oldx, zwinner;
unsigned long x2;
short int x, y;

unsigned int board[14][10];

unsigned int spotx[]=
{10,18,26,34,42,50,58,66,74,82,90,98,106,114, 123,123,123,123,123,123,123,123,123,123, 114,106,98,90,82,74,66,58,50,42,34,26,18,10, 0,0,0,0,0,0,0,0,0,0};

unsigned int spoty[]=
{0,0,0,0,0,0,0,0,0,0,0,0,0,0, 10,18,26,34,42,50,58,66,74,82, 91,91,91,91,91,91,91,91,91,91,91,91,91,91, 82,74,66,58,50,42,34,26,18,10}; 

// dimensions of spot arrays are 48

static unsigned char plr1d[]=
{ 0x0, 0x3C, 0x3A, 0x7D, 0x7F, 0x7F, 0x7F, 0x3C};
static unsigned char plr1l[]=
{0x0,0x3C,0x22,0x41,0x41,0x41,0x63,0x3C};


static unsigned char plr2d[]=
{ 0x0, 0x3C, 0x22, 0x41, 0x41, 0x41, 0x63, 0x3C};
static unsigned char plr2l[]=
{0x0,0x3C,0x3A,0x7D,0x7F,0x7F,0x7F,0x3C};

static unsigned char tiles[]=
{0xE7,0xC3,0x81,0x81,0x81,0x81,0xC3,0xE7};

static unsigned char background[]=
{ 0x77, 0xF9, 0xBF, 0xFF, 0xF7, 0xBF, 0xFD, 0xD7};
static unsigned char backgroundl[]=
{0x49,0x4,0x70,0x4,0x89,0x50,0x2,0xA1};

void title(short a, short b) {
Sprite8( a, b, 8, backgroundl, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
Sprite8( a, b, 8, background, GrayGetPlane (DARK_PLANE), SPRT_XOR);
}	
	
void draw(short a, short b, short plr) {
	if (plr==1) {
	Sprite8( a, b, 8, plr1d, GrayGetPlane (DARK_PLANE), SPRT_XOR);
	Sprite8( a, b, 8, plr1l, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
	} else if (plr==2) {
	Sprite8( a, b, 8, plr2d, GrayGetPlane (DARK_PLANE), SPRT_XOR);
	Sprite8( a, b, 8, plr2l, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);		
	}
}


// Main Function
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
 ScrRectFill (&(SCR_RECT){{49,30,112,70}}, ScrRect, A_REVERSE);
 FontSetSys(F_6x8);

 DrawStr(51,31,"Connect 4X",A_NORMAL);
 DrawStr(51,41,"Play!",A_NORMAL);
 DrawStr(51,51,"About",A_NORMAL);
 DrawStr(51,61,"Exit",A_NORMAL);
 a=0;
 for (b=0; b<10; b++)
	DrawLine(50, 40+10*a+b, 111, 40+10*a+b, A_XOR);
 
 while (((key=ngetchx())!=KEY_ENTER) && (key!=KEY_ESC)) { // wait til you press enter or escape
	
	for (b=0; b<10; b++)
	DrawLine(50, 40+10*a+b, 111, 40+10*a+b, A_XOR);
	
	if ((key==KEY_UP)*a)
	a-=1;
	else if ((key==KEY_DOWN) && (a!=2))
	a+=1;
	
	for (b=0; b<10; b++)
	DrawLine(50, 40+10*a+b, 111, 40+10*a+b, A_XOR);
	}
	
	if (a==1) {
		GrayOff();
		ClrScr();
	
	FontSetSys(F_8x10);
 	DrawStr(0,0,"Connect 4X",A_NORMAL);	
	
	FontSetSys(F_4x6);	
	DrawStr(0,15,"Drop your piece into a slot in strategic places", A_NORMAL);
	DrawStr(0,22,"to try to get 4 of your pieces in a row!", A_NORMAL);
	DrawStr(0,29,"Dropping them from the top, they fall down.", A_NORMAL);
	DrawStr(0,36,"Dropping them from the bottom, they fall up.", A_NORMAL);
	DrawStr(0,43,"Dropping them from the right, they fall left.", A_NORMAL);
	DrawStr(0,50,"Dropping them from the left, they fall right.", A_NORMAL);
	DrawStr(0,60,"Just try it once, and you'll see how it works.", A_NORMAL);
	DrawStr(0,79,"This program was written by Travis Fischer", A_NORMAL);
	DrawStr(0,86,"22/3/03", A_NORMAL);
	DrawStr(0,93,"Thanks to the TIGCC team", A_NORMAL);
	while (((key=ngetchx())!=KEY_ENTER) && (key!=KEY_ESC));
	ClrScr();
	FontSetSys(F_8x10);
 	DrawStr(0,0,"Connect 4X",A_NORMAL);	
	FontSetSys(F_6x8);
	DrawStr(0,15,"--KEYS--", A_NORMAL);
	FontSetSys(F_4x6);
	DrawStr(0,27,"Escape quits at any time", A_NORMAL);
	DrawStr(0,34,"Enter is to drop your piece", A_NORMAL);
	DrawStr(0,41,"Left moves counter-clockwise.", A_NORMAL);
	DrawStr(0,48,"Right moves clockwise.", A_NORMAL);
	DrawStr(0,55,"Clear restarts the game.", A_NORMAL);
	DrawStr(0,79,"This program was written by Travis Fischer", A_NORMAL);
	DrawStr(0,86,"22/3/03", A_NORMAL);
	DrawStr(0,93,"Thanks to the TIGCC team", A_NORMAL);
	while (((key=ngetchx())!=KEY_ENTER) && (key!=KEY_ESC));		
	}
	if (a==1)
	goto menu;
	
	if ((a!=2)&&(key!=KEY_ESC)) {
 
	reset:
	for(a=0; a<80; a++) {
	memset (GetPlane (DARK_PLANE), random(a+155), LCD_SIZE);
	for (x2 =0; x2<2000; x2 ++);  // slows down the shuffle
	}
	
	for (a=0; a<14; a++)
 		for (b=0; b<10; b++)
 			board[a][b]=0;
 	
 oldx=0;
 done=0;
 winner=0;
 turn=0;
 
 memset (GetPlane (DARK_PLANE), 0, LCD_SIZE);
 memset (GetPlane (LIGHT_PLANE), 0, LCD_SIZE);
 
 for(a=0; a<14; a++)
 	 for(b=0; b<10; b++)
 		 Sprite8(10+8*a, 10+8*b, 8, tiles, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
 
 while ((!done) && (!winner)) {
 	
 	turn++;
		FontSetSys(F_4x6);  // sets the small font.  All words displayed will be 6x8
		GraySetAMSPlane (LIGHT_PLANE);  // all drawings will be on LIGHT plane in memory
		printf_xy(132,62,"turn %d",turn);
		GraySetAMSPlane (DARK_PLANE);
		printf_xy(132,62,"turn %d",turn);
 	
 	for (player=1; player<3-done-winner; player++) {
 	
 	for (key=0; key<2; key++) {  // loop to write words at right on both light and dark planes, so it will be black, not gray
			if (key)
			GraySetAMSPlane (LIGHT_PLANE);  // all drawings will be on LIGHT plane in memory
			else 
			GraySetAMSPlane (DARK_PLANE);  // all drawings will be on DARK plane in memory
			FontSetSys(F_4x6);  // sets the small font.  All words displayed will be 6x8
			DrawStr(132,33,"Player",A_NORMAL);  // draws the word "Player" at the coordinates (120,0)
			FontSetSys(F_6x8);  // sets the big font.  All words displayed will be 8x10
			printf_xy(142,43,"%d",player);  // displays the current player number at coordinates (135,10). (in an 8x10 font)
			}

 	oops:
 	x=oldx;
 	draw(spotx[x], spoty[x], player);
 	
 	while (((key=ngetchx())!=KEY_ENTER) && (key!=KEY_ESC) && (key!=KEY_CLEAR)) {
 		draw(spotx[x], spoty[x], player);  // erase your piece
 		if (key==KEY_RIGHT) {
 			x+=1;
 			if (x>47)
 			x-=48;
 		} else if (key==KEY_LEFT) {
 			x-=1;
 			if (x<0)
 			x+=48;
 		}  // end of if statements to check for movement.
 		draw(spotx[x], spoty[x], player);  // redraw your piece at new coordinates
 	}  // end loop for moving left or right
 		if ((key==KEY_ESC)||(key==KEY_CLEAR))
 		done=1;
 		
 		oldx=x;
 		draw(spotx[x], spoty[x], player);  // erase your piece
 		if (oldx<14) {  // check if piece is dropping down
 			y=-1;
 			while ((y!=9) && (!board[oldx][y+1]))  {
			  y++;
			  draw(spotx[oldx], 10+8*y, player);
			  for (x2=0; x2<11000; x2++);
			  draw(spotx[oldx], 10+8*y, player);
			}
 			if (y!=-1) {
 			  board[oldx][y]=player;
 				draw(spotx[oldx], 10+8*y, player);
 				} else
 				goto oops;
 		} else if ((oldx>13) && (oldx<24)) {  // check if piece is going from right to left
 			x=14;
 			while ((x!=0) && (!board[x-1][oldx-14])) {
 				x--;
 				draw(10+8*x, spoty[oldx], player);
 				for (x2=0; x2<11000; x2++);
 				draw(10+8*x, spoty[oldx], player);
 			}
 			if (x!=14) {
 				board[x][oldx-14]=player;
 				draw(10+8*x, spoty[oldx], player);
 			} else 
 			goto oops;
 		} else if ((x>23) && (x<38)) {  // check if piece is falling up
 			y=10;
 			while ((y!=0) && (!board[37-oldx][y-1])) {
 				y--;
 				draw(spotx[oldx],10+8*y, player);
 				for (x2=0; x2<11000; x2++);
 				draw(spotx[oldx],10+8*y, player);
 			}
 			if (y!=10) {
 				board[37-oldx][y]=player;
 				draw(spotx[oldx],10+8*y, player);
 			} else
 			goto oops;
 		} else if (x>37) {  // check if piece is going from left to right
 			x=-1;
 			while ((x!=13) && (!board[x+1][47-oldx])) {
 				x++;
 				draw(10+8*x, spoty[oldx], player);
 				for (x2=0; x2<11000; x2++);
 				draw(10+8*x, spoty[oldx], player);
 			}
 			if (x!=-1) {
 			board[x][47-oldx]=player;
 			draw(10+8*x, spoty[oldx], player);	
 			} else
 			goto oops;
 		}
 		x=oldx;
 		
 		for (b=0; b<10; b++)
			for (a=0; a<11; a++)
			winner+= ((player==board[a][b])*(player==board[a+1][b])*(player==board[a+2][b])*(player==board[a+3][b]));
			
		for (b=0; b<7; b++)
			for (a=0; a<14; a++)	
			winner+= ((player==board[a][b])*(player==board[a][b+1])*(player==board[a][b+2])*(player==board[a][b+3]));
			
			for (b=0; b<7; b++)
			for (a=0; a<11; a++)
			winner+= ((player==board[a][b])*(player==board[a+1][b+1])*(player==board[a+2][b+2])*(player==board[a+3][b+3])) + ((player==board[a][b+3])*(player==board[a+1][b+2])*(player==board[a+2][b+1])*(player==board[a+3][b]));
			
			if (winner)
			zwinner=player;
			
 	} //end player loop, for loop
 	
 }  //end while loop
 if (key==KEY_CLEAR)
 goto reset;
 
 if (winner) {
	GraySetAMSPlane (LIGHT_PLANE);  
			
	FontSetSys(F_8x10);  // sets the big font.  All words displayed will be 8x10
	printf_xy(15,1,"WINNER! Plr %d",zwinner); 
	GraySetAMSPlane (DARK_PLANE);  
			
	printf_xy(14,0,"WINNER! Plr %d",zwinner); 

	while (((key=ngetchx())!=KEY_ENTER) && (key!=KEY_ESC));
	
	for (x = 0; x<90; x ++) {
	memset (GetPlane (DARK_PLANE), x+random(125), LCD_SIZE);
	for (x2 =0; x2<4000; x2 ++);  // slows down the shuffle
	}
 }
}
 memset (GetPlane (DARK_PLANE), 0, LCD_SIZE);
 memset (GetPlane (LIGHT_PLANE), 0, LCD_SIZE);
FontSetSys(F_8x10);
GraySetAMSPlane (DARK_PLANE); 
 DrawStr(20,30,"Programmed by:",A_REPLACE);
  DrawStr(30,45,"Travis Fischer",A_REPLACE);
GraySetAMSPlane (LIGHT_PLANE);  
 DrawStr(21,31,"Programmed by:",A_REPLACE);
  DrawStr(31,46,"Travis Fischer",A_REPLACE); // that's me :)

while (((key=ngetchx())!=KEY_ENTER) && (key!=KEY_ESC)); // wait til you press enter or escape
 FontSetSys(F_6x8); 
 GrayOff();
 for (a=0; a<80; a++) {
 	DrawLine(a,0,a,99, A_NORMAL);
 	DrawLine(159-a,0,159-a,99, A_NORMAL);
 	DrawLine(0,a,159,a, A_NORMAL);
 	DrawLine(0,100-a,159,100-a, A_NORMAL);
 }
}  // end main void and game
