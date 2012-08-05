// C Source File
// Created 4/2/2003; 7:02:15 PM

#define USE_TI89              // Compile for TI-89

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define MIN_AMS 100           // Compile for AMS 1.00 or higher

#define SAVE_SCREEN           // Save/Restore LCD Contents

#define NO_EXIT_SUPPORT

#include <tigcclib.h>         // Include All Header Files
#include "Levels.h"						// Include level data
#include "extgraph.h"
#define NUMBER_OF_LEVELS		41

//  12 by 9 left and right mouse sprites
static unsigned short leftd[]=
{ 0xA00, 0x1700, 0x3900, 0x4080, 0x80E0, 0x40B0, 0x3900, 0x1700, 0x2080, 0x0, 0x0, 0x0};
static unsigned short leftl[]=
{0x0,0x600,0x3F00,0x5F80,0x7F80,0x5F80,0x3F00,0x600,0x0,0x0,0x0,0x0};
static unsigned short rightd[]=
{ 0x1040, 0xE80, 0x9C0, 0xD020, 0x7010, 0x1020, 0x9C0, 0xE80, 0x500, 0x0, 0x0, 0x0};
static unsigned short rightl[]=
{0x0,0x600,0xFC0,0x1FA0,0x1FE0,0x1FA0,0xFC0,0x600,0x0,0x0,0x0,0x0};
static unsigned short left2d[]=
{ 0x2080, 0x1700, 0x3900, 0x40B0, 0x80E0, 0x4080, 0x3900, 0x1700, 0xA00, 0x0, 0x0, 0x0};
static unsigned short left2l[]=
{0x0,0x600,0x3F00,0x5F80,0x7F80,0x5F80,0x3F00,0x600,0x0,0x0,0x0,0x0};
static unsigned short right2d[]=
{ 0x500, 0xE80, 0x9C0, 0x1020, 0x7010, 0xD020, 0x9C0, 0xE80, 0x1040, 0x0, 0x0, 0x0};
static unsigned short right2l[]=
{0x0,0x600,0xFC0,0x1FA0,0x1FE0,0x1FA0,0xFC0,0x600,0x0,0x0,0x0,0x0};
//  special sprites for left side of screen with tail cut off (easier than making clipped sprite functions)
static unsigned short rightdsp[]=
{ 0x8200, 0x7400, 0x4E00, 0x8100, 0x8080, 0x8100, 0x4E00, 0x7400, 0x2800, 0x0, 0x0, 0x0};
static unsigned short rightlsp[]=
{0x0,0x3000,0x7E00,0xFD00,0xFF00,0xFD00,0x7E00,0x3000,0x0,0x0,0x0,0x0};
static unsigned short right2dsp[]=
{ 0x2800, 0x7400, 0x4E00, 0x8100, 0x8080, 0x8100, 0x4E00, 0x7400, 0x8200, 0x0, 0x0, 0x0};
static unsigned short right2lsp[]=
{0x0,0x3000,0x7E00,0xFD00,0xFF00,0xFD00,0x7E00,0x3000,0x0,0x0,0x0,0x0};

//  9 by 12 up and down mouse sprites
static unsigned short upd[]=
{ 0x800, 0x1400, 0x2280, 0x6300, 0xA200, 0x4100, 0xC100, 0x6300, 0x1C80, 0x800, 0xC00, 0x400};
static unsigned short upl[]=
{0x0,0x1C00,0x2A00,0x3E00,0x3E00,0x7F00,0x7F00,0x3E00,0x1C00,0x0,0x0,0x0};
static unsigned short downd[]=
{ 0x400, 0xC00, 0x800, 0x9C00, 0x6300, 0x4180, 0x4100, 0x2280, 0x6300, 0xA200, 0x1400, 0x800};
static unsigned short downl[]=
{0x0,0x0,0x0,0x1C00,0x3E00,0x7F00,0x7F00,0x3E00,0x3E00,0x2A00,0x1C00,0x0};
static unsigned short up2d[]=
{ 0x800, 0x1400, 0xA200, 0x6300, 0x2280, 0x4100, 0x4180, 0x6300, 0x9C00, 0x800, 0x1800, 0x1000};
static unsigned short up2l[]=
{0x0,0x1C00,0x2A00,0x3E00,0x3E00,0x7F00,0x7F00,0x3E00,0x1C00,0x0,0x0,0x0};
static unsigned short down2d[]=
{ 0x1000, 0x1800, 0x800, 0x1C80, 0x6300, 0xC100, 0x4100, 0xA200, 0x6300, 0x2280, 0x1400, 0x800};
static unsigned short down2l[]=
{0x0,0x0,0x0,0x1C00,0x3E00,0x7F00,0x7F00,0x3E00,0x3E00,0x2A00,0x1C00,0x0};

// 10 by 10 sprites for enemies
static unsigned short enemyd[]=
{ 0x1E00, 0x2100, 0x5480, 0x5480, 0x4080, 0x2100, 0x2100, 0x1200, 0xA00, 0x600}; // sweet ghost (moving tail)
static unsigned short enemyl[]=
{0x1200,0x1E00,0x2B40,0x2B00,0xBF00,0x1E00,0x1E00,0xC00,0x400};
static unsigned short enemy2d[]=
{ 0x1E00, 0x2100, 0x5480, 0x5480, 0x4080, 0x2100, 0x2100, 0x1200, 0x1200, 0xC00};
static unsigned short enemy2l[]=
{0x1200,0x1E00,0x2B40,0x2B00,0xBF00,0x1E00,0x1E00,0xC00,0xC00};
static unsigned short enemy3d[]=
{ 0x1E00, 0x2100, 0x4A80, 0x4A80, 0x4080, 0x2100, 0x2100, 0x1200, 0x1400, 0x1800};
static unsigned short enemy3l[]=
{0x1200,0x1E00,0xB500,0x3500,0x3F40,0x1E00,0x1E00,0xC00,0x800};
static unsigned short enemy4d[]=
{ 0x1E00, 0x2100, 0x4A80, 0x4A80, 0x4080, 0x2100, 0x2100, 0x1200, 0x1200, 0xC00};
static unsigned short enemy4l[]=
{0x1200,0x1E00,0xB500,0x3500,0x3F40,0x1E00,0x1E00,0xC00,0xC00};

// 6 by 11 sprites for arrows
static unsigned short arrow_leftd[]=
{0x400, 0x800, 0x1400, 0x2800, 0x5000, 0xA000, 0x5000, 0x2800, 0x1400, 0x800, 0x400};
static unsigned short arrow_leftl[]=
{0x400,0xC00,0x1C00,0x3800,0x7000,0xE000,0x7000,0x3800,0x1C00,0xC00,0x400};
static unsigned short arrow_rightd[]=
{ 0x8000, 0x4000, 0xA000, 0x5000, 0x2800, 0x1400, 0x2800, 0x5000, 0xA000, 0x4000, 0x8000};
static unsigned short arrow_rightl[]=
{0x8000,0xC000,0xE000,0x7000,0x3800,0x1C00,0x3800,0x7000,0xE000,0xC000,0x8000};

//  12 by 12 sprites for tiles for multiple background choices
static unsigned short blockd[]=
{ 0x380, 0x6300, 0x6300, 0xE780, 0x840, 0x1880, 0x5980, 0xE380, 0xC2C0, 0xC380};
static unsigned short blockl[]=
{0xFC40,0x94C0,0xBDC0,0x1A40,0xF780,0xE740,0xA6C0,0x1D40,0x6D00,0xBCC0};
static unsigned short block2d[]=
{ 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0};
static unsigned short block2l[]=
{0x6640,0x7FC0,0xFF80,0xFF80,0x7FC0,0x7FC0,0xFF80,0xFF80,0x7FC0,0x6640};
static unsigned short block3d[]=
{ 0xC00, 0x3F00, 0x7380, 0x6580, 0xC2C0, 0xC0C0, 0x6180, 0x7380, 0x3F00, 0xC00};
static unsigned short block3l[]=
{0x1200,0x5E80,0x2F00,0xC3C0,0x6180,0x6180,0xC0C0,0x2D00,0x5E80,0x1200};
static unsigned short block4d[]=
{ 0xD2C0, 0xAD40, 0x5E80, 0xBF40, 0x7F80, 0x7F80, 0xBF40, 0x5E80, 0xAD40, 0xD2C0};
static unsigned short block5d[]=
{ 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0};
static unsigned short block5l[]=
{0xAD40,0x5280,0xAD40,0x5E80,0xBF40,0xBF40,0x5E80,0xAD40,0x5280,0xAD40};
static unsigned short block6d[]=
{ 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0};
static unsigned short block6l[]=
{0xC0C0,0x8040,0x0,0x0,0x0,0x0,0x0,0x0,0x8040,0xC0C0};
static unsigned short block7d[]=
{ 0xFFC0, 0x8340, 0x8340, 0xFFC0, 0x8440, 0xFFC0, 0xA140, 0xFFC0, 0x8840, 0xFFC0};
static unsigned short block7l[]=
{0xFFC0,0xFCC0,0xFCC0,0x8040,0xFBC0,0x8040,0xDEC0,0x8040,0xF7C0,0xFFC0};
static unsigned short block8d[]=
{ 0xC00, 0x3F00, 0x7F80, 0x7D80, 0xFAC0, 0xFDC0, 0x7F80, 0x7F80, 0x3F00, 0xC00};
static unsigned short block8l[]=
{0x1200,0x5E80,0x2D00,0xDEC0,0x7B80,0x7D80,0xDEC0,0x2D00,0x5E80,0x1200};
static unsigned short block9d[]=
{ 0x2140, 0x8480, 0x4000, 0x880, 0x5200, 0x0, 0x8200, 0xA800, 0x0, 0x40};
static unsigned short block9l[]=
{0xA060,0x4400,0x300,0x0,0x4300,0x2000,0xB080,0x8880,0x600,0x604};

static unsigned short bad_blockd[]=
{ 0xFFC0, 0xC0C0, 0xA140, 0x9240, 0x8C40, 0x8C40, 0x9240, 0xA140, 0xC0C0, 0xFFC0};
static unsigned short bad_blockl[]=
{0xFFC0,0xBF40,0xDEC0,0xEDC0,0xF3C0,0xF3C0,0xEDC0,0xDEC0,0xBF40,0xFFC0};

// variables for ghosts and levels
unsigned char board[16][10];
short unsigned int *coordinates;

// pointer for enemy sprites
static unsigned short *enemiesd[]=
{enemyd, enemy2d, enemy3d, enemy4d};
static unsigned short *enemiesl[]=
{enemyl, enemy2l, enemy3l, enemy4l};

//  pointer for background tiles
static unsigned short *blocksd[]=
{blockd, block2d, block3d, block4d, block5d, block6d, block7d, block8d, block9d, bad_blockd};
static unsigned short *blocksl[]=
{blockl, block2l, block3l, block5l, block5l, block6l, block7l, block8l, block9l, bad_blockl};

//  pointer for the mouse sprites  :)
static unsigned short *youd[]=
{leftd,left2d, rightd,right2d, upd,up2d, downd,down2d, rightdsp,right2dsp};
static unsigned short *youl[]=
{leftl,left2l, rightl,right2l, upl,up2l, downl,down2l, rightlsp,right2lsp};


static unsigned short question1d[]=// height of 15
{0x1E00,0x3F00,0x7F80,0xE380,0xE380,0x380,0x700,0x1E00,0x1C00,0x3800,0x3000,0x0,0x3000,0x3000,0x7000};
static unsigned short question1l[]=
{0x2D00,0x7F80,0xFF00,0x7780,0xC180,0x700,0xF80,0xD00,0x3800,0x1800,0x800,0x0,0x7800,0x7800,0x3000};
static unsigned short mask1[]=
{0xC0FF,0x807F,0x7F,0x87F,0x1C7F,0xF87F,0xF07F,0xE0FF,0xC3FF,0xC7FF,0xC7FF,0xFFFF,0x87FF,0x87FF,0x8FFF};
static unsigned short question2d[]=
{0xF8,0x304,0x6E6,0x784,0x8C,0x18,0x470,0x980,0x1E00,0x1C00,0x3800,0x1800,0x1800,0x7000,0x0};
static unsigned short question2l[]=
{0x104,0x48A,0x7C0,0x76A,0x15E,0x3BC,0xAE8,0x1640,0x1900,0x1E00,0x7C00,0x6400,0xF800,0xF800,0x0};
static unsigned short mask2[]=
{0xFE03,0xF871,0xF819,0xF811,0xFE21,0xFC43,0xF107,0xE03F,0xE0FF,0xE1FF,0x83FF,0x83FF,0x7FF,0x7FF,0xFFFF};

void question(void) {
	if (random(2)) {
		GraySprite16_MASK(random(150), 16+random(75), 15, question1d, question1l, mask1, mask1,
		GrayGetPlane (DARK_PLANE), GrayGetPlane (LIGHT_PLANE));
	} else {
		GraySprite16_MASK(random(150), 16+random(75), 15, question2d, question2l, mask2, mask2,
		GrayGetPlane (DARK_PLANE), GrayGetPlane (LIGHT_PLANE));
	}
}

void drawplr(short a, short b, short int position) { // function that draws your mouse onto the screen, Xor.
// if you are already on the screen at this position, it will erase the mouse.
	Sprite16( a, b, 12, youd[position], GrayGetPlane (DARK_PLANE), SPRT_XOR);
	Sprite16( a, b, 12, youl[position], GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
}

inline void Drawblock_n(short int a, short int b, short int position) {
// function for drawing a single background block
	Sprite16( a, b, 10, blocksd[position], GrayGetPlane (DARK_PLANE), SPRT_XOR);
	Sprite16( a, b, 10, blocksl[position], GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
}

void enemy(short int a, short int b, short position) {  // function that draws one enemy onto the screen
	Sprite16( a, b, 10, enemiesd[position], GrayGetPlane (DARK_PLANE), SPRT_XOR);
	Sprite16( a, b, 10, enemiesl[position], GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
}

void initialize_level(short int type, short int level) {
// data for ghost positions in the levels. Initialize a level/display blocks and ghosts onto screen:

unsigned short int lvl_pos[10*NUMBER_OF_LEVELS]=
{15,9, 7,2, 2,7, 0,0, 0,0,   5,3, 5,6, 10,3, 10,6, 8,6,   7,5, 8,5, 3,4, 12,4, 0,0,   0,8, 15,1, 15,8, 0,0, 0,0,   
1,2, 2,2, 2,1, 0,0, 0,0,   5,7, 11,7, 12,1, 0,0, 0,0,   2,2, 4,4, 6,6, 8,8, 13,3,   7,9, 6,4, 9,4, 15,8, 0,0,   
0,9, 3,8, 5,9, 9,9, 13,8,   9,4, 14,8, 0,0, 0,0, 0,0,   6,4, 9,5, 0,0, 0,0, 0,0,   15,9, 9,9, 5,9, 11,5, 0,0,   
4,1, 7,8, 11,2, 15,9, 0,0,   4,3, 4,5, 13,4, 15,0, 0,0,   3,5, 12,5, 5,1, 14,9, 0,0,   1,9, 7,5, 8,4, 14,0, 14,9,   
8,4, 13,5, 4,6, 8,9, 0,0,   5,6, 8,9, 10,5, 12,1, 0,0,   8,3, 8,5, 0,0, 0,0, 0,0,   2,3, 15,9, 0,0, 0,0, 0,0,   
6,3, 9,5, 10,4, 7,7, 8,2,   0,8, 6,8, 9,8, 15,8, 0,0,   14,4, 14,5, 0,0, 0,0, 0,0,   7,6, 9,4, 14,7, 0,0, 0,0,   
1,5, 3,7, 12,6, 13,9, 15,7,   5,0, 1,9, 13,8, 9,7, 0,0,  8,5, 13,0, 13,8, 14,2, 15,9,   5,9, 8,6, 13,8, 15,5, 0,0,   
2,1, 2,9, 7,4, 12,0, 12,9,   5,5, 11,7, 0,0, 0,0, 0,0,   5,3, 5,6, 8,4, 13,7, 0,0,   5,5, 0,0, 0,0, 0,0, 0,0,   
9,4, 8,6, 0,0, 0,0, 0,0,   1,8, 6,9, 9,6, 14,1, 14,8,   5,1, 5,5, 11,4, 13,4, 15,5,   7,3, 14,0, 12,9, 0,0, 0,0,   
6,4, 7,6, 8,3, 9,5, 0,0,   0,2, 8,5, 0,0, 0,0, 0,0,   8,5, 13,1, 12,8, 3,2, 3,7,   6,9, 9,5, 10,1, 13,9, 14,1,   
};
//15,9, 15,8, 15,7, 15,6, 15,5};

	int a, b, c = 0;
	//unsigned char *(bigboard[NUMBER_OF_LEVELS])=
	unsigned char (*bigboard[NUMBER_OF_LEVELS])[16][10]  =
	{&board1,&board2,&board3,&board4,&board5,&board6,&board7,
	&board8,&board9,&board10,&board11,&board12,&board13,&board14,
	&board15,&board16,&board17,&board18,&board19,&board20,&board21,
	&board22,&board23,&board24,&board25,&board26,&board27,&board28,
	&board29,&board30,&board31,&board32,&board33,&board34,&board35,
	&board36,&board37,&board38, &board39,&board40};
	//unsigned char *pointer=bigboard[level-1];
	if (level!=NUMBER_OF_LEVELS) {
		memcpy(board, bigboard[level-1], sizeof(unsigned char)*16*10);
		for(a=5; a--;) {
			coordinates[2*a]=lvl_pos[2*a+10*(level-1)];
			coordinates[1+2*a]=lvl_pos[1+2*a+10*(level-1)];
		}
	} else {
			for(a=0; a<16; a++)
			for(b=0; b<10; b++)
				board[a][b]=(c=(!(!a+!b))*random(3))-(c*random(2));
			
			for(a=5; a--;)
				while(board[(coordinates[2*a]=random(16))][(coordinates[1+2*a]=random(10))]
				&&coordinates[2*a]+coordinates[1+2*a]);
	}

	memset (GetPlane (DARK_PLANE), 0, LCD_SIZE); // clear gray planes
	memset (GetPlane (LIGHT_PLANE), 0, LCD_SIZE);
	
for (a=0; a<16; a++)  // draw initial blocks to screen.
	for (b=0; b<10; b++)
		if (board[a][b])  // checks to see if there should be a block on that point of the screen
		Drawblock_n(10*a, 10*b, type-type*(board[a][b]==2)+9*(board[a][b]==2));
for(a=5; a--;)  // draw initial ghosts on screen
if (coordinates[2*a]+coordinates[2*a+1])
enemy(10*coordinates[2*a],10*coordinates[2*a+1], 0);
}

void pick_level(short int level) {  // displays the level onto the top of the screen
		FontSetSys(F_8x10);
		ScrRectFill (&(SCR_RECT){{48-2*(level>9),0,112+4*(level>9),14}}, ScrRect, A_REVERSE);
		if (level!=NUMBER_OF_LEVELS&&level)
		printf_xy(53-5*(level>9), 3,"level %d",level);
		else {
			if (level==NUMBER_OF_LEVELS)
				DrawStr(53,3,"Bonus??",A_REPLACE);
			else if (!level) {
				clrscr();
				Sprite16(102, 2, 11, arrow_rightd, GrayGetPlane (DARK_PLANE), SPRT_OR);
				Sprite16(102, 2, 11, arrow_rightl, GrayGetPlane (LIGHT_PLANE), SPRT_OR);
				DrawStr(50,3,"Random",A_REPLACE);
			}
		}
		DrawLine (47-2*(level>9), 0, 112+4*(level>9), 0, A_THICK1); 
		DrawLine (47-2*(level>9), 14, 112+4*(level>9), 14, A_THICK1); 
		DrawLine (46-2*(level>9), 1, 46-2*(level>9), 14, A_THICK1);
		DrawLine (112+4*(level>9), 1, 112+4*(level>9), 14, A_THICK1);		
}

void ScrollUp(unsigned short* buffer,unsigned short lines) {
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

// Main Function
void _main(void)
{
unsigned long int x2, wait_move=16;
unsigned short int direction, on, on2=0, key, wait_on=0, done, newx, newy, wait_ghost=0, no_of_ghosts;
unsigned short int x, y, a, b, c, move, oldx, oldy, old_direction, wait_on2=81, level=1, mode=0, oldlevel;
short int type=0, up, down, right, left;
unsigned int live[5];

unsigned short int ghost_nos[NUMBER_OF_LEVELS]=
{3, 5, 4, 3, 3, 3, 5, 4, 5, 2, 2, 4, 4, 4, 4, 5, 4, 4, 2, 2, 5, 4, 2, 3, 5, 4, 5, 4, 5, 2, 4, 1, 2, 5, 5, 3, 4, 2, 
5, 5, 5};
coordinates = (unsigned int *)malloc(sizeof(unsigned int[10]));
// allocate room for enemy coordinates pointer
if (!coordinates) {
ST_helpMsg("Not enough memory");
return;
}
//  bitmaps for light and dark CONGRATULATIONS, after you beat a level
static BITMAP congratulations_dark = {16, 147, {
	/*  to declare a bitmap, just use static BITMAP name= { height, width, {data}};
	Then just use:  BitmapPut(X, Y, & NAME_OF_BITMAP, &(SCR_RECT){{0,0,159,99}}, A_NORMAL);
	I used a paint program to write Congratulations in Cezanne font, and then used Imagestudio to convert it to hex.
	*/
  0x00,0x02,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x48,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x13,0xF0,0x00,0x00,0x00,0x00,0x00,0x01,0x20,0x00,0x3C,0x00,0x04,0x80,0x00,0x00,0x00,0x00,
  0x00,0x5F,0xC0,0x00,0x00,0x00,0x00,0x00,0x01,0xC0,0x00,0x7C,0x00,0x07,0x04,0x80,0x00,0x00,0x00,
  0x01,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x01,0xF8,0x00,0x1F,0x07,0x80,0x00,0x00,0x00,
  0x00,0x7D,0x00,0x00,0x00,0x30,0x00,0x00,0x0F,0xC0,0x03,0xF8,0x00,0x3F,0x0C,0x00,0x00,0x00,0x00,
  0x03,0xE8,0x1F,0x07,0x00,0xB8,0xD0,0x1E,0x5F,0xD2,0x03,0xF0,0x79,0x7F,0x14,0x7C,0x1C,0x01,0x70,
  0x0F,0xA0,0x1F,0x8E,0x01,0xFB,0xFC,0x3E,0x3F,0xFF,0x07,0xE0,0xF8,0xFF,0x00,0x7E,0x38,0x05,0xD0,
  0x1E,0x80,0x3F,0x9E,0xF1,0xFB,0xFC,0x7E,0xFF,0xBE,0x9F,0x81,0xFB,0xFE,0x12,0xFE,0x7B,0xCF,0xC0,
  0x1E,0x00,0x6F,0x1F,0xF3,0xF3,0xFC,0xFC,0xBE,0x3C,0xE7,0x03,0xF2,0xF8,0x7D,0xBC,0x7F,0xCF,0x80,
  0x7C,0x03,0x6F,0x9F,0xFF,0xC0,0x72,0x7E,0x7E,0xFD,0xDF,0x89,0xF9,0xF8,0xFF,0xBE,0x7F,0xFF,0x00,
  0x7B,0x8F,0x5F,0x7D,0xFF,0xE0,0x7F,0x78,0x78,0xFB,0xDE,0x1D,0xE1,0xE1,0xF9,0x7D,0xF7,0xFF,0xC0,
  0x7F,0xFE,0xFC,0x39,0xFF,0xE0,0xE7,0xFC,0xFC,0x7F,0xCF,0x1F,0xF3,0xF3,0xFF,0xF0,0xE7,0xFF,0xC0,
  0x3F,0xFB,0xF8,0xF9,0xE3,0xF1,0xCF,0xF8,0xF8,0xF7,0x8E,0x3F,0xE3,0xE3,0xF7,0xE3,0xE7,0x97,0x80,
  0x3F,0xF8,0xF0,0x03,0xE2,0xF3,0xCF,0xF0,0x7F,0xCF,0x9E,0x3F,0xC1,0xF8,0xF3,0xC0,0x0F,0x87,0x00,
  0x7F,0xF9,0xE0,0x03,0xE5,0xAF,0xDF,0x78,0xFC,0x0F,0x87,0xFD,0xE3,0xF1,0xE7,0x80,0x0F,0x84,0x00,
  0x7F,0x21,0xE0,0x03,0x4F,0x06,0x90,0x48,0x48,0x04,0x83,0xC1,0x21,0x20,0x27,0x80,0x0D,0x38,0x00}};

static BITMAP congratulations_light = {16, 148, {
  0x00,0x01,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x0F,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x78,0x00,0x03,0x00,0x00,0x00,0x00,0x00,
  0x00,0x3F,0xE0,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0xF8,0x00,0x0F,0x03,0x00,0x00,0x00,0x00,
  0x00,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0xF8,0x00,0x0E,0x0F,0x00,0x00,0x00,0x00,
  0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x80,0x01,0xF0,0x00,0x1E,0x1E,0x00,0x00,0x00,0x00,
  0x01,0xF0,0x0E,0x0E,0x00,0x71,0xE0,0x3C,0x3F,0x8F,0x03,0xE0,0xF0,0xFE,0x08,0x38,0x38,0x00,0xE0,
  0x07,0xC0,0x3F,0x1F,0x00,0xF1,0xF8,0x7E,0x7F,0x9E,0x03,0xC1,0xF9,0xFE,0x00,0xFC,0x7C,0x03,0xE0,
  0x0F,0x00,0x7F,0x8F,0x03,0xF1,0xF8,0xFE,0x7F,0x3C,0x67,0xC3,0xF9,0xFC,0x0D,0xFE,0x3C,0x07,0x80,
  0x3C,0x00,0x7F,0x9F,0xF7,0xE1,0xF9,0xFE,0x7E,0x7C,0xCF,0x87,0xF9,0xF8,0x3D,0xFE,0x7F,0xDF,0x00,
  0x38,0x00,0xE7,0x3F,0xF7,0x80,0xF9,0xFC,0x3C,0x78,0xCF,0x07,0xF0,0xF0,0x79,0x9C,0xFF,0xDF,0x80,
  0x3C,0x7F,0xEE,0x3F,0xE7,0xC0,0xF3,0xFC,0x7C,0x79,0x8F,0x0F,0xF1,0xF0,0xFB,0xB8,0xFF,0x9F,0x80,
  0x3F,0xFF,0xFE,0x7F,0xE7,0xF1,0xF7,0xF8,0x78,0xF3,0x9E,0x1F,0xE1,0xE1,0xF3,0xF9,0xFF,0x9F,0x80,
  0x7F,0xFC,0xFC,0x73,0xE1,0xF3,0xE7,0xF8,0x78,0xFF,0x9E,0x1F,0xE1,0xE1,0xF3,0xF1,0xCF,0x8F,0x00,
  0x7F,0xF9,0xF8,0x03,0xC1,0xE7,0x8F,0xF8,0xFC,0xE7,0x0E,0x3F,0xE3,0xF1,0xE7,0xE0,0x0F,0x0E,0x00,
  0x3F,0xF1,0xF0,0x03,0xC3,0xC7,0x8E,0x30,0x78,0x07,0x0F,0xB8,0xC1,0xE0,0xE7,0xC0,0x0F,0x18,0x00,
  0x3F,0xC1,0xC0,0x03,0x87,0x8F,0x0C,0x30,0x30,0x03,0x07,0x30,0xC0,0xC0,0xC7,0x00,0x0E,0x00,0x00}};

static BITMAP strategery_dark = {21, 109, {
  0x01,0x71,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x03,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x03,0xCF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x06,0x9E,0x78,0x00,0x00,0x01,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x1C,0x1E,0x78,0x00,0x00,0x01,0xE0,0x2C,0x00,0x00,0x58,0x00,0x00,0x00,
  0x1E,0x1E,0x7E,0xFF,0x97,0x01,0xF8,0xFE,0x0B,0xF1,0xFC,0xFF,0xA4,0xB0,
  0x1C,0x00,0xFF,0x7F,0x39,0xC3,0xFC,0xEE,0x0F,0xF9,0xDC,0x7F,0x79,0xF8,
  0x1F,0x82,0xFA,0x7F,0xBB,0xCB,0xE9,0x8F,0x1E,0xE3,0x1E,0x7F,0xBC,0xF8,
  0x0F,0xE1,0xF0,0xF6,0x8B,0xC7,0xC7,0x0F,0x3D,0xEE,0x1E,0xF6,0x1C,0x78,
  0x01,0xE1,0xE0,0xF7,0x2F,0x07,0x87,0xFE,0xFB,0xCF,0xFC,0xF7,0x30,0xF0,
  0x79,0xF9,0xE0,0xE0,0xFF,0x87,0x8F,0x91,0xF3,0xDF,0x20,0xE0,0x78,0xF0,
  0x78,0xF8,0xE0,0xF1,0xEF,0x83,0x87,0x00,0xE3,0xCE,0x00,0xF0,0x38,0xF0,
  0xF1,0x73,0xED,0xE3,0xE7,0x0F,0xB7,0x4B,0xC7,0xCE,0x91,0xE0,0x30,0xE0,
  0xFF,0xE3,0xFF,0xEB,0xFF,0xCF,0xF7,0xF7,0x97,0x8F,0xEB,0xE8,0x77,0xE0,
  0x6F,0x40,0xEB,0xFB,0xE6,0x83,0xAF,0x93,0xFF,0x9F,0x23,0xF8,0x3D,0xE0,
  0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xF7,0x00,0x00,0x00,0x01,0x80,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x03,0xC0,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x07,0x80,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x00,0x00,0x01,0xDF,0x80,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xBE,0x00,0x00,0x00,0xBD,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xF4,0x00,0x00,0x00,0x00,0x00}};


static BITMAP strategery_light = {21, 109, {
  0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x01,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x07,0x8E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x0F,0x0E,0x30,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x0E,0x0C,0x30,0x00,0x00,0x00,0xC0,0x1E,0x00,0x00,0x3C,0x00,0x00,0x00,
  0x1C,0x0C,0x70,0x77,0x0F,0x81,0xC0,0x7F,0x07,0xF8,0xFE,0x77,0x18,0x78,
  0x1E,0x00,0x7E,0xFF,0x9F,0x81,0xF9,0xF7,0x1F,0xF3,0xEE,0xFF,0xFC,0xF8,
  0x0F,0x01,0xFC,0x3F,0x71,0x87,0xF3,0xC6,0x3D,0xC7,0x8C,0x3F,0x78,0x70,
  0x07,0xC3,0xE0,0x7F,0x67,0x8F,0x83,0xFE,0x79,0xC7,0xFC,0x7F,0x38,0x70,
  0x03,0xF0,0xE0,0x7E,0x1F,0x83,0x87,0xFC,0x71,0xCF,0xF8,0x7E,0x78,0x70,
  0x30,0xF0,0xC0,0x70,0x7F,0x03,0x07,0xE0,0xE1,0x8F,0xC0,0x70,0x30,0x60,
  0x70,0x71,0xC0,0xE0,0xF7,0x07,0x0E,0x01,0xC3,0x9C,0x00,0xE0,0x70,0xE0,
  0x78,0xE1,0xDC,0xE1,0xDF,0x07,0x7E,0x3D,0xC3,0x9C,0x78,0xE0,0x71,0xE0,
  0x7F,0xC1,0xF9,0xF1,0xFF,0x87,0xEF,0xF9,0xCF,0x9F,0xF1,0xF0,0x33,0xC0,
  0xFF,0x81,0xF1,0xF1,0xFF,0x07,0xC7,0xE1,0xFF,0x0F,0xC1,0xF0,0x1F,0xC0,
  0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x03,0xC0,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x03,0x80,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x03,0x80,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x00,0x01,0xE7,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1C,0x00,0x00,0x00,0x7E,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0x00,0x00}};

  
// turn interrupts off and set up a fake interrupt for graymode to use.
INT_HANDLER interrupt1 = GetIntVec(AUTO_INT_1);
SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
if (!GrayOn())  {/* turns gray mode on.  Basically, in gray mode, interrupt 1 switches between a light and dark
plane (a plane is a virtual drawing space.  The one you are used to is the LCD MEM, which you see), giving 
the illusion that we are seeing gray.  You just put your sprites onto the two planes to draw in graymode and the 
calc does the actual switching of the planes with an interrupt.  You have to check to make sure gray mode 
actually turned on, because if the calc doesn't have enough free memory, it will not turn on and your game will 
be messed up.*/
free(coordinates);  /* frees the space we allocated for the coordinate array.  Without this, the space would
stay there and be taking up space when you return.*/
SetIntVec(AUTO_INT_1,interrupt1);// turn off dummy interrupt handler for grayscale
OSSetSR (0x0000);  // turn back on all interrupts
GKeyFlush();
ST_helpMsg("Not enough memory");
return;  // just quits out of your game because there wasn't enough memory available for gray mode.
}
new_game:
// clear both gray planes
memset (GetPlane (DARK_PLANE), 0, LCD_SIZE); // clears the dark plane
memset (GetPlane (LIGHT_PLANE), 0, LCD_SIZE);  /* clears the light plane ( the zero can be replaced by any
number up to 255.  All numbers will give you a slightly different pattern.  One cool effect is to make a for 
loop and memset the screen onto a random number between 0 and 255.

Example:

for (a = 0; a<160; a ++) {
	memset (GetPlane (DARK_PLANE), a+random(125), LCD_SIZE);
	for (x2 =0; x2<4000; x2 ++);  // slows down the shuffle

*/

//FontSetSys(F_8x10);
GraySetAMSPlane (LIGHT_PLANE);
BitmapPut(25, 0, &strategery_light, &(SCR_RECT){{0,0,159,99}}, A_NORMAL);
//DrawStr(38,0,"Strategery",A_REPLACE);
FontSetSys(F_4x6);
DrawStr(0,28,"Choose background:",A_NORMAL);
GraySetAMSPlane (DARK_PLANE);
BitmapPut(25, 0, &strategery_dark, &(SCR_RECT){{0,0,159,99}}, A_NORMAL);
//FontSetSys(F_8x10);
//DrawStr(38,0,"Strategery",A_REPLACE);
FontSetSys(F_4x6);
DrawStr(125,20,"--By Fisch",A_REPLACE);
DrawStr(0,28,"Choose background:",A_NORMAL);

Sprite16(55, 43, 11, arrow_leftd, GrayGetPlane (DARK_PLANE), SPRT_XOR);
Sprite16(55, 43, 11, arrow_leftl, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
Sprite16(95, 43, 11, arrow_rightd, GrayGetPlane (DARK_PLANE), SPRT_XOR);
Sprite16(95, 43, 11, arrow_rightl, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
Drawblock_n(73, 43, type);
Drawblock_n(37, 43, type-1+9*(!type));
Drawblock_n(113, 43, type+1-9*(type==8));

DrawStr(0,61,"Push the blocks to trap and vanquish the ghosts",A_REPLACE);
DrawStr(0,68,"Don't forget to use lots of strategery!", A_NORMAL);
DrawStr(0,75,"Home exits, Clear restarts, and F1 pauses.",A_REPLACE);
DrawStr(0,82,"Use the + and - keys to adjust the contrast.",A_REPLACE);
DrawStr(0,92,"You CAN'T move these        blocks",A_NORMAL);
GraySetAMSPlane (LIGHT_PLANE);
DrawStr(0,92,"You CAN'T move these        blocks",A_NORMAL);
Drawblock_n(76, 90, 9);
while (_keytest (RR_ENTER)||(_keytest (RR_HOME)));

while (!_keytest (RR_HOME)&&!_keytest (RR_2ND)&&!_keytest (RR_ENTER)) {
	if ((key=_rowread(0xFFFE))&&wait_on++>800) {
		wait_on=0;
		Drawblock_n(73, 43, type);
		Drawblock_n(37, 43, type-1+9*(!type));
		Drawblock_n(113, 43, type+1-9*(type==8));
		if (key&0x0002) {
			type--;
			if (type<0)
			type=8;
		} else if (key&0x0008) {
			type++;
			if (type==9)
			type=0;
		}
		Drawblock_n(73, 43, type);
		Drawblock_n(37, 43, type-1+9*(!type));
		Drawblock_n(113, 43, type+1-9*(type==8));
	}
	if (wait_on2++>2000) {
		wait_on2=0;
		if (_keytest (RR_MINUS))
			OSContrastDn ();//  turn contrast down
		else if (_keytest (RR_PLUS))
		OSContrastUp ();//  turns contrast up
	}
}
if (_keytest (RR_HOME))
goto end;

practice_reset:
level=1;
memset (GetPlane (DARK_PLANE), 0, LCD_SIZE);
memset (GetPlane (LIGHT_PLANE), 0, LCD_SIZE);
GraySetAMSPlane (LIGHT_PLANE);
FontSetSys(F_4x6);
DrawStr(0,28,"Choose Game type:",A_NORMAL);
BitmapPut(25, 0, &strategery_light, &(SCR_RECT){{0,0,159,99}}, A_NORMAL);
FontSetSys(F_6x8);
DrawStr(25,43,"Story Mode",A_NORMAL);
DrawStr(95,43,"Practice",A_NORMAL);
GraySetAMSPlane (DARK_PLANE);
FontSetSys(F_4x6);
DrawStr(0,28,"Choose Game type:",A_NORMAL);
DrawStr(0,61,"In story mode, you battle your way up to",A_REPLACE);
DrawStr(0,68,"become the master of Stratagery!", A_NORMAL);
DrawStr(0,75,"In practice mode, you can pick which",A_REPLACE);
DrawStr(0,82,"level you would like to practice on.",A_REPLACE);
DrawStr(0,89,"High scores will only be kept in story mode",A_REPLACE);
BitmapPut(25, 0, &strategery_dark, &(SCR_RECT){{0,0,159,99}}, A_NORMAL);
FontSetSys(F_6x8);
DrawStr(25,43,"Story Mode",A_NORMAL);
DrawStr(95,43,"Practice",A_NORMAL);

for(a=42; a<52; a++)
	DrawLine(24+68*mode, a, 85+60*mode, a, A_XOR);

	while (_keytest (RR_2ND)||_keytest (RR_ENTER)||_keytest (RR_HOME));
while (!_keytest (RR_HOME)&&!_keytest (RR_2ND)&&!_keytest (RR_ENTER)) {
	if ((key=_rowread(0xFFFE))&&wait_on++>800) {
		wait_on=0;
		for(a=42; a<52; a++)
			DrawLine(24+68*mode, a, 85+60*mode, a, A_XOR);
		if ((key&0x0002)||key&0x0008)
			mode=!mode;
		for(a=42; a<52; a++)
			DrawLine(24+68*mode, a, 85+60*mode, a, A_XOR);
	}
	if (wait_on2++>2000) {
		wait_on2=0;
		if (_keytest (RR_MINUS))
			OSContrastDn ();//  turn contrast down
		else if (_keytest (RR_PLUS))
		OSContrastUp ();//  turns contrast up
	}
}

if (_keytest (RR_HOME))
goto new_game;
a=0;
if (mode) {
	initialize_level(type, level);
	GraySetAMSPlane (LIGHT_PLANE);
pick_level( level);
	GraySetAMSPlane (DARK_PLANE);
pick_level( level);
while (_keytest (RR_2ND)||_keytest (RR_ENTER));

	while (!_keytest (RR_HOME)&&!_keytest (RR_2ND)&&!_keytest (RR_ENTER)) {
	oldlevel=level;
	if (wait_on++>800||_keytest (RR_F1)||(_keytest (RR_6)&&_keytest (RR_9))) {
		wait_on=0;
		if (_keytest (RR_6)&&_keytest (RR_9))
			level=NUMBER_OF_LEVELS;
		else if (_keytest (RR_F1))
			level=0;
		else if ((_rowread(0xFFFE)&0x0002)&&level)
			level--;
		else if ((_rowread(0xFFFE)&0x0008)&&level<NUMBER_OF_LEVELS-1)
			level++;
		if (oldlevel!=level) {
			a=0;
			if (level)
			initialize_level(type, level);
				GraySetAMSPlane (LIGHT_PLANE);
			pick_level( level);
				GraySetAMSPlane (DARK_PLANE);
			pick_level(level);
		}
	}
	if (!level&&!random(80))
	question();
	if (!level) {
		if (!random(80))
			question();
		if (a++>2500) {
			a=0;
			Sprite16(102, 2, 11, arrow_rightd, GrayGetPlane (DARK_PLANE), SPRT_XOR);
			Sprite16(102, 2, 11, arrow_rightl, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
		}
	}
	
	if (wait_on2++>2000) {
		wait_on2=0;
		if (_keytest (RR_MINUS))
			OSContrastDn ();//  turn contrast down
		else if (_keytest (RR_PLUS))
		OSContrastUp ();//  turns contrast up
	}
 }
 if (!level) {
 	level=random(NUMBER_OF_LEVELS);
 } else level--;
	if (_keytest (RR_HOME))
		goto new_game;
} else level=0;


/////////////////////////////////////////NEW///////////////////////////////////////////////////////////////////
/////////////////////////////////////////LEVEL/////////////////////////////////////////////////////////////////

new_level:
if (level++==NUMBER_OF_LEVELS+1)
level=1;

restart_level:
if (mode)
	for (a = 0; a<240; a ++) {
	memset (GetPlane (DARK_PLANE), a+random(125), LCD_SIZE);
	for (x2 =0; x2<4300; x2 ++);  // slows down the shuffle
}
// Initialize all variables and draw to screen
initialize_level(type, level);

no_of_ghosts=ghost_nos[level-1];
for (a=0; a<5; a++)
	if (a<=no_of_ghosts)
	live[a]=1;
	else live[a]=0;

if (!mode) {
	for(a=1; a<=NUMBER_OF_LEVELS; a++) {
		initialize_level(type, a);
		for (x2 =0; x2<1900; x2 ++)
			b=random(2000);
	}
	a=1;
	while (a<=level) {
		initialize_level(type, a);
		for (x2 =0; x2<2300; x2 ++)
			b=random(2000);
		a++;
	}
	GraySetAMSPlane (LIGHT_PLANE);
		pick_level( level);
	GraySetAMSPlane (DARK_PLANE);
		pick_level( level);
		wait_on2=900;
		on2=0;
	while (!_keytest (RR_HOME)&&!_keytest (RR_2ND)&&!_keytest (RR_ENTER)) {
		if (wait_on2++>900) {
			if (_keytest (RR_MINUS))
				OSContrastDn ();//  turn contrast down
			else if (_keytest (RR_PLUS))
				OSContrastUp ();//  turns contrast up
			wait_on2=0;
			for(a=no_of_ghosts; a--;)
			enemy(10*coordinates[2*a],10*coordinates[2*a+1], on2);
			if (on2++>2)
				on2=0;
			for(a=no_of_ghosts; a--;)
				enemy(10*coordinates[2*a],10*coordinates[2*a+1], on2);
		}
	}
	if (_keytest (RR_HOME))
		goto new_game;
	initialize_level(type, level);
}

// set up variables
x=y=done=on=on2=wait_on=up=down=left=right=0;;
direction=2;  // direction=2 just means the mouse is facing right

drawplr(x-3*(direction==2&&x), y-3*(direction==6), on+direction+6*(direction==2&&!x)); // draw mouse on screen
while (!done) {  // main while loop that controls the game

for(x2=175; x2--;)  // slow downnnnnnn
	a=random(2000);
	
if ((wait_on++)>135) {
	wait_on=0;
	drawplr(x-3*(direction==2&&x), y-3*(direction==6), on+direction+6*(direction==2&&!x));
	on=!on;
	drawplr(x-3*(direction==2&&x), y-3*(direction==6), on+direction+6*(direction==2&&!x));
	if (_keytest (RR_MINUS))
		OSContrastDn ();//  turn contrast down
	else if (_keytest (RR_PLUS))
		OSContrastUp ();//  turns contrast up
}
	
if ((wait_move++)>15) {
	wait_move=0;
	key=_rowread(0xFFFE);  // get keypress
	
	if (key) {
		old_direction=direction;
		oldx=x;
		oldy=y;
		drawplr(x-3*(direction==2&&x), y-3*(direction==6), on+direction+6*(direction==2&&!x));
		if ((key&0x0001&&y>0)||up)  {// move you up one
			if (direction==4)
			y-=10;
			direction=4;
		} else if ((key&0x0004&&y<87)||down)  {// move you down
			if (direction==6)
			y+=10;
			direction=6;
		} else if ((key&0x0002&&x>0)||left) { // move you left
			if (!direction)
			x-=10;
			direction=0;
		} else if ((key&0x0008&&x<147)||right) {// move you right one
			if (direction==2)
			x+=10;
			direction=2;
		}
		up=down=left=right=0;
		if (board[x/10][y/10]) {
			move=0;
			if (board[x/10][y/10]==2)
			goto nope;
			move=1;
			newx=x/10;
			newy=y/10;
			if (!direction) {  // checks if you're pushing a block left
				while (newx&&board[newx-1][newy]&&board[newx-1][newy]!=2)
					newx--;
				if (newx&&board[newx-1][newy]!=2) {
				newx--;
				board[x/10][y/10]=0;
				Drawblock_n(x, y, type);
				board[newx][newy]=1;
				Drawblock_n(10*newx, y, type);
				} else move=0;
			} else if (direction==2) {  // checks if you're pushing a block right
				while (newx!=15&&board[newx+1][newy]&&board[newx+1][newy]!=2)
					newx++;
				if (newx!=15&&board[newx+1][newy]!=2) {
				newx++;
				board[x/10][y/10]=0;
				Drawblock_n(x, y, type);
				board[newx][newy]=1;
				Drawblock_n(10*newx, y, type);
				} else move=0;
			} else if (direction==4) {  // checks if you're pushing a block up
				while (newy&&board[newx][newy-1]&&board[newx][newy-1]!=2)
					newy--;
				if (newy&&board[newx][newy-1]!=2) {
				newy--;
				board[x/10][y/10]=0;
				Drawblock_n(x, y, type);
				board[newx][newy]=1;
				Drawblock_n(x, 10*newy, type);
				} else move=0;
			} else if (direction==6) {  // checks if you're pushing a block down
				while (newy!=9&&board[newx][newy+1]&&board[newx][newy+1]!=2)
					newy++;
				if (newy!=9&&board[newx][newy+1]!=2) {
				newy++;
				board[x/10][y/10]=0;
				Drawblock_n(x, y, type);
				board[newx][newy]=1;
				Drawblock_n(x, 10*newy, type);
				} else move=0;
			}
			nope:
			if (!move) {
				x=oldx;
				y=oldy;
				direction=old_direction;
			} else {
				for (a=no_of_ghosts; a--;) {
					if (live[a]&&board[coordinates[2*a]][coordinates[1+2*a]]) { 
					// if you pushed a block onto the ghost, then:
						oldx=coordinates[2*a];
						oldy=coordinates[2*a+1];
						x2=random(4);
						enemy(10*coordinates[2*a],10*coordinates[2*a+1], on2);
						for(b=0; b<4; b++) {
						if (x2==b&&oldx&&!board[oldx-1][oldy]&&direction!=2&&oldx==coordinates[2*a]&&oldy==coordinates[1+2*a])
							coordinates[2*a]--;
						else if (x2+1-4*(x2==3)==b&&oldx!=15&&!board[oldx+1][oldy]&&direction&&oldx==coordinates[2*a]&&oldy==coordinates[1+2*a])
							coordinates[2*a]++;
						else if (x2+2-4*(x2>=2)==b&&oldy&&!board[oldx][oldy-1]&&direction!=6&&oldx==coordinates[2*a]&&oldy==coordinates[1+2*a])
							coordinates[1+2*a]--;
						else if (x2+3-4*(x2>0)==b&&oldy!=9&&!board[oldx][oldy+1]&&direction!=4&&oldx==coordinates[2*a]&&oldy==coordinates[1+2*a])
							coordinates[1+2*a]++;
						}
						if (oldx==coordinates[2*a]&&oldy==coordinates[1+2*a])
						live[a]=0;
						else enemy(10*coordinates[2*a],10*coordinates[2*a+1], on2);
					}					
				} // end for loop checking for hit detection of blocks and ghosts
			}
		}  // end if branch to check for movement of pieces.
		drawplr(x-3*(direction==2&&x), y-3*(direction==6), on+direction+6*(direction==2&&!x));
	}  else {// end if loop checking if key was pressed
		if (_rowread(0xFFFE)&0x0001&&y>0)					up=1;
		else if (_rowread(0xFFFE)&0x0004&&y<87)		down=1;
		else if (_rowread(0xFFFE)&0x0002&&x>0)		left=1;
		else if (_rowread(0xFFFE)&0x0008&&x<147)	right=1;
	}

	for(a=no_of_ghosts; a--;) {  // loop making sure no ghosts show up on top of each other.
		for(b=no_of_ghosts; b--;) {
		if (b!=a&&coordinates[2*a]==coordinates[2*b]&&coordinates[1+2*a]==coordinates[1+2*b]&&live[b]&&live[a]){
				enemy(10*coordinates[2*a],10*coordinates[2*a+1], on2);
				oldx=coordinates[2*a];
				oldy=coordinates[2*a+1];
				c=1;
				if (oldy!=9&&!board[oldx][oldy+1]) {  // down
					c=0;
					for(x2=no_of_ghosts; x2--;)
						c+=(x2!=a&&coordinates[2*a]==coordinates[2*x2]&&coordinates[1+2*a]+1==coordinates[1+2*x2]&&live[x2]);
					if (!c)
					coordinates[1+2*a]++;
				} if (oldy&&!board[oldx][oldy-1]&&c) {  // up
					c=0;
					for(x2=no_of_ghosts; x2--;)
						c+=(x2!=a&&coordinates[2*a]==coordinates[2*x2]&&coordinates[1+2*a]-1==coordinates[1+2*x2]&&live[x2]);
					if (!c)
					coordinates[1+2*a]--;
				} if (oldx!=15&&!board[oldx+1][oldy]&&c) {  // right
					c=0;
					for(x2=no_of_ghosts; x2--;)
				 	c+=(x2!=a&&coordinates[2*a]+1==coordinates[2*x2]&&coordinates[1+2*a]==coordinates[1+2*x2]&&live[x2]);
					if (!c)
					coordinates[2*a]++;
				}	if (oldx&&!board[oldx-1][oldy]&&c) {  // left
				c=0;
					for(x2=no_of_ghosts; x2--;)
					 c+=(x2!=a&&coordinates[2*a]-1==coordinates[2*x2]&&coordinates[1+2*a]==coordinates[1+2*x2]&&live[x2]);
					if (!c)
					coordinates[2*a]--;
				}
				if (c)
					live[a]=0;
				else enemy(10*coordinates[2*a],10*coordinates[2*a+1], on2);
			}
		}
	}

	if (wait_on2++>80) {
		for(a=no_of_ghosts; a--;)
		if (live[a])
		enemy(10*coordinates[2*a],10*coordinates[2*a+1], on2);
		if (on2++>2)
		on2=0;
		for(a=no_of_ghosts; a--;)
		if (live[a])
		enemy(10*coordinates[2*a],10*coordinates[2*a+1], on2);
	}
	// ghost coordinates will only be changed only so many loops, depending on the difficulty
	if (wait_ghost++>3) {
	wait_ghost=0;
	for(a=no_of_ghosts; a--;) {  // loop for changing and updating ghost coordinates
		if (live[a]) {
		oldx=coordinates[2*a];
		oldy=coordinates[2*a+1];		
		enemy(10*coordinates[2*a],10*coordinates[2*a+1], on2); // erase ghost sprite
		if (random(2))
			coordinates[2*a]+=(oldx!=x/10)-2*(oldx>x/10);
		else coordinates[1+2*a]+=(oldy!=y/10)-2*(oldy>y/10);
		x2=0;
		for(b=no_of_ghosts; b--;)  //  checks all other ghost positions to make sure current ghost isn't on one
			x2+=(b!=a&&coordinates[2*a]==coordinates[2*b]&&coordinates[1+2*a]==coordinates[1+2*b]&&live[b]);
		
		if (board[coordinates[2*a]][coordinates[1+2*a]]||x2) {  // if you ran into a block or another ghost then:
			
			if (random(2)) {  // if branch to make more random movement, instead of just heading straight towards you
				x2=random(2);
				coordinates[2*a]=oldx+(oldx<15)-2*(oldx>0&&x2)+(oldx==15&&x2);
				coordinates[1+2*a]=oldy;
			} else {
				x2=random(2);
				coordinates[1+2*a]=oldy+(oldy<9)-2*(oldy>0&&x2)+(oldy==9&&x2);
				coordinates[2*a]=oldx;
			}
			c=0;
			for(b=no_of_ghosts; b--;)  //  checks all other ghost positions to make sure current ghost isn't on one
				c+=(b!=a&&coordinates[2*a]==coordinates[2*b]&&coordinates[1+2*a]==coordinates[1+2*b]&&live[b]);
			
			if (board[coordinates[2*a]][coordinates[1+2*a]]||c) {	
			coordinates[2*a]=oldx;
			coordinates[1+2*a]=oldy;
			}
		}
		enemy(10*coordinates[2*a],10*coordinates[2*a+1], on2); // redraw ghost sprite
	 }
	}  // end for loop changing ghost's coordinates
}  // end if branch to move ghost's coordinates
}  // end main while loop
if (_rowread(~((short)(1<<1))) & (1<<6))
done=2;
if (_keytest (RR_HOME))
done=1;
if (_keytest (RR_F1)) {
	int *virtual=malloc (LCD_SIZE);  // Allocate the buffer
	if (!virtual)
	return;
	int *virtual2=malloc (LCD_SIZE);
	if (!virtual2) {
	free(virtual);
	return;
	}
	memcpy( virtual,GrayGetPlane (DARK_PLANE), 3840);
	memcpy( virtual2,GrayGetPlane (LIGHT_PLANE), 3840);
	FontSetSys(F_8x10);
	GrayOff();
	ScrRectFill (&(SCR_RECT){{48,0,112,14}}, ScrRect, A_REVERSE);
	DrawStr(56,3,"PAUSED", A_NORMAL);
	DrawLine (49, 0, 112, 0, A_THICK1); 
	DrawLine (49, 14, 112, 14, A_THICK1); 
	DrawLine (48, 1, 48, 14, A_THICK1);
	DrawLine (112, 1, 112, 14, A_THICK1);

	while(_keytest (RR_F1));
	while(!_keytest (RR_F1)&&!done) {
	if (_keytest (RR_HOME))
		done=1;
	}
	while(_keytest (RR_F1)&&!done);
	GrayOn();
	memcpy(GrayGetPlane (DARK_PLANE),  virtual, 3840);
	memcpy(GrayGetPlane (LIGHT_PLANE),  virtual2, 3840);
	free(virtual);
	free(virtual2);
}
for(a=no_of_ghosts; a--;) { // hit detection for mouse and ghosts, to see if you die
	if (live[a]&&coordinates[2*a]==x/10&&coordinates[1+2*a]==y/10) {
	enemy(10*coordinates[2*a],10*coordinates[2*a+1], on2); // erase ghost sprite
	done=3;
	}
}  // end hit detection loop

if (!done) {  // checks to see if you've defeated all the ghosts
	b=0;
	for(a=no_of_ghosts;a--;)
	b+=live[a];
	if (!b)
	done=4;
}
}  // end main while loop
if (done==1)
	goto new_game;
if (done==3||done==2) {  // check to see if you died (hit a mouse)
	drawplr(x-3*(direction==2&&x), y-3*(direction==6), on+direction+6*(direction==2&&!x)); // erase mouse
	for (b=4; b--;) {
		for (direction=0; direction<=8; direction+=2) {
			drawplr(x-3*(direction==2&&x), y-3*(direction==6), on+direction+6*(direction==2&&!x));
			for (x2=1850; x2--;)
			a=random(2000);
			if (direction==2||direction==6) {
			if (b>=2) OSContrastUp ();//  turns contrast up
			else OSContrastDn ();//  turns contrast back down
			}
		}
	}
}  // end if loop
if (done==2&&mode)
	goto restart_level;
if (done==2)
	goto new_game;

for (b=100; b--;) {
	ScrollUp(GrayGetPlane (DARK_PLANE),100);
	if (_keytest (RR_HOME)||(_rowread(~((short)(1<<1))) & (1<<6)))
		break;
	for (x2=680; x2--;)
		a=random(2000);
}
if (_rowread(~((short)(1<<1))) & (1<<6)&&mode)
goto restart_level;
if (done==4) {
	FontSetSys(F_8x10);
GraySetAMSPlane (DARK_PLANE);
	BitmapPut(6, 24, &congratulations_dark, &(SCR_RECT){{0,0,159,99}}, A_NORMAL);
 //DrawStr(16,30,"Congratulations!",A_REPLACE);
 printf_xy(27, 46,"Level %d clear",level);
GraySetAMSPlane (LIGHT_PLANE);  
	BitmapPut(6, 24, &congratulations_light, &(SCR_RECT){{0,0,159,99}}, A_NORMAL);
  printf_xy(27, 46,"Level %d clear",level);
  
  OSFreeTimer (USER_TIMER);
	OSRegisterTimer (USER_TIMER, 20);

while (!_keytest (RR_HOME)&&!_keytest (RR_2ND)&&!_keytest (RR_ENTER)) {
 while (!OSTimerExpired (USER_TIMER)&&!_keytest (RR_HOME)&&!_keytest (RR_2ND)&&!_keytest (RR_ENTER));
 		if (_keytest (RR_MINUS))
			OSContrastDn ();//  turn contrast down
		else if (_keytest (RR_PLUS))
		OSContrastUp ();//  turns contrast up
		
  	GraySetAMSPlane (DARK_PLANE); 
 		BitmapPut(6, 24, &congratulations_dark, &(SCR_RECT){{0,0,159,99}}, A_XOR);
  	GraySetAMSPlane (LIGHT_PLANE);  
 		BitmapPut(6, 24, &congratulations_light, &(SCR_RECT){{0,0,159,99}}, A_XOR);
  } // end while

if (_keytest (RR_HOME))
goto new_game;
}  // end if branch seeing if you passed the level, trapped all the mice
if (done==4&&!mode)
goto new_level;

if ((done==4||done==3)&&mode)
goto practice_reset;
if (!mode&&done==3)
	goto new_game;
end:
GrayOff();  // turn off grayscale
free(coordinates);
SetIntVec(AUTO_INT_1,interrupt1);// turn off dummy interrupt handler for grayscale
OSSetSR (0x0000);  // turn back on all interrupts
GKeyFlush();
}

// By Fisch