// C Source File
// Created 3/28/2003; 11:43:43 AM

#define USE_TI89              // Compile for TI-89

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define MIN_AMS 100           // Compile for AMS 1.00 or higher

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

unsigned short level, score, wait, minus=1;

static unsigned char right[]=
{0xC0,0xB8,0x56,0x69,0x69,0x56,0xB8,0xC0};
static unsigned char left[]=
{0x3,0x1D,0x6A,0x96,0x96,0x6A,0x1D,0x3};
static unsigned char up[]=
{0x18,0x24,0x24,0x5A,0x66,0x5A,0xBD,0xC3};
static unsigned char down[]=
{0xC3,0xBD,0x5A,0x66,0x5A,0x24,0x24,0x18};

static unsigned char *ball[]=
{right, left, down, up};

int *coordinates;

void newball(short int ball_number) {

	if (!ball_number) {
		coordinates[0]=-3;
		coordinates[1]=random(91);
	} else if (ball_number==1) {
		coordinates[2]=159;
		coordinates[3]=random(91);
	} else if (ball_number==2) {
		coordinates[4]=random(151);
		coordinates[5]=-3;
	} else if (ball_number==3) {
		coordinates[6]=random(151);
		coordinates[7]=103;
	}
	minus-=(3/100);
	level-=minus;
	wait-=3;
	score++;
}

inline void draw(short int x2, short int y2, short int ball_number) {
	Sprite8(x2, y2, 8, ball[ball_number], LCD_MEM, SPRT_XOR);
}


// Main Function
void _main(void)
{
static unsigned int explosiond[]=
{0x0,0xFE0,0x1EF0,0x39F8,0x707C,0x76FE,0xECFE,0xEAC6,0xF3C6,0x78C6,0x380C,0x3A1C,0x2F3C,0x2770,0x1BF0,0x60};
static unsigned int explosionl[]=
{0x0,0xFC0,0x1100,0x36F8,0x6F9C,0x4F00,0x9F30,0x9F3A,0xDFB8,0x673C,0x37FC,0x27F8,0x33C0,0x3880,0x1C30,0x60};
static unsigned int exd2[]=
{0x0,0x0,0x100,0x7F0,0xFF8,0x1FFC,0x3C8C,0x3AE4,0x1ADC,0x18BC,0xE7C,0x3F8,0x60,0x0,0x0,0x0};
static unsigned int exl2[]=
{0x0,0x0,0x100,0x7F0,0xE18,0x1804,0x3B74,0x3D1C,0x1FAC,0x1F5C,0xFFC,0x3F8,0xE0,0x0,0x0,0x0};
static unsigned char background[]=
{0x92,0x49,0xFF,0xAA,0xFF,0x49,0x92,0x0};
static unsigned char smiley[] = 
{0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C};

short int direction[8]=
{1, 0, -1, 0, 0, 1, 0, -1};

short unsigned int x=75, y=43, done, key, a, wait_move, wait_balls;
ClrScr();

for(a=0; a<=152; a+=8) {
	Sprite8(a, 0, 8, background, LCD_MEM, SPRT_XOR);
	Sprite8(a, 92, 8, background, LCD_MEM, SPRT_XOR);
}

FontSetSys(F_8x10);
DrawStr(38,13,"!!!RUN!!!",A_REPLACE);
FontSetSys(F_4x6);
DrawStr(0,28,"Use the arrow keys to move Mr. Smiley, while",A_NORMAL);
DrawStr(0,35,"avoiding the four deadly ships.",A_NORMAL);
DrawStr(0,42,"Clear restarts or quits at any time.",A_NORMAL);
DrawStr(0,66,"This program was written by Travis Fischer", A_NORMAL);
DrawStr(0,73,"28/3/03", A_NORMAL);
DrawStr(0,80,"Thanks to the TIGCC team", A_NORMAL);
while (((key=ngetchx())!=KEY_ENTER) && (key!=KEY_ESC)); // wait til you press enter or escape

if (key==KEY_ESC)
goto end;

coordinates = (int *)malloc(sizeof(int[8]));  // allocate coordinate matrix
if (!coordinates)
return;

reset:  // label used to restart game if you press clear
ClrScr();  // clears screen
level=54;
done=0;  //  set up variables
wait_balls=0;
wait_move=0;
wait=80;
x=75;
y=43;
wait=500;
score=0;
for(a=0; a<4; a++) {  // set up initial ball values and draw them to start out
newball(a);
draw(coordinates[2*a], coordinates[1+2*a], a);
}
OSSetSR (0x0700);  // turn off all interrupts
Sprite8( x, y, 8, smiley, LCD_MEM, SPRT_XOR);
while (!done) {
	if (wait_balls++>level) {
		wait_balls=0;
		for(a=0; a<4; a++) {
		draw(coordinates[2*a], coordinates[1+2*a], a);
		coordinates[2*a]+=direction[2*a];
		coordinates[1+2*a]+=direction[1+2*a];
		if ((!a && coordinates[0]>156)||(a==1 && coordinates[2]<-4)||(a==2 && coordinates[5]>96)||(a==3 && coordinates[7]<-4))
		newball(a);
		draw(coordinates[2*a], coordinates[1+2*a], a);
		}  // end for loop controlling movement of balls
	}
	
	for(a=0; a<wait; a++);
	
	wait_move++;
	if (wait_move>11) {
	wait_move=0;
	key=_rowread(0xFFFE);  // get keypress
	if (key) {
		Sprite8( x, y, 8, smiley, LCD_MEM, SPRT_XOR);
		if (key&0x0001&&y>0)  // move you up one
			y--;
		if (key&0x0004&&y<91)  // move you down
			y++;
		if (key&0x0002&&x>0)  // move you left
			x--;
		if (key&0x0008&&x<151) // move you right one
			x++;
		Sprite8( x, y, 8, smiley, LCD_MEM, SPRT_XOR);
	}  // end if loop checking if key was pressed
	
	if ((_rowread(~((short)(1<<1))) & (1<<6))||GetPix(x+1,y)||GetPix(x+6,y)||GetPix(x+1,y+7)||GetPix(x+6,y+7)||GetPix(x+2,y+3)||GetPix(x+5,y+3))
	// check if clear or escape was pressed
	done=1;
	}
}  // end main while loop.  after this, you either pressed esc, clear, or you died.
Sprite8( x, y, 8, smiley, LCD_MEM, SPRT_XOR);
OSSetSR (0x0000);  // turn back on all interrupts
GrayOn();
for(key=0; key<500; key++) {
	for(a=0; a<2; a++) {
	Sprite16( x-4, y-4, 16, explosiond, GrayGetPlane (DARK_PLANE), SPRT_XOR);
	Sprite16( x-4, y-4, 16, explosionl, GrayGetPlane (LIGHT_PLANE), SPRT_OR);
	Sprite16( x-4, y-4, 16, exd2, GrayGetPlane (DARK_PLANE), SPRT_XOR);
	Sprite16( x-4, y-4, 16, exl2, GrayGetPlane (LIGHT_PLANE), SPRT_OR);
	}
	for (a=0; a<100*random(10); a++) {
		done=random(2000);
	}
}
GrayOff();
FontSetSys(F_6x8);
printf_xy (20,0,"Score: %d",score);
FontSetSys(F_8x10);
DrawStr(43,38,"Exit",A_REPLACE);	
DrawStr(43,49,"Restart",A_REPLACE);
a=0;
 for (x=0; x<11; x++)
	DrawLine(43, 37+11*a+x, 98, 37+11*a+x, A_XOR);
GKeyFlush();
while (((key=ngetchx())!=KEY_ENTER) && (key!=KEY_ESC)){
	for (x=0; x<11; x++)
	 DrawLine(43, 37+11*a+x, 98, 37+11*a+x, A_XOR);
	if ((key==KEY_UP)*a)
	a-=1;
	else if ((key==KEY_DOWN) && (a!=1))
	a+=1;
	for (x=0; x<11; x++)
	 DrawLine(43, 37+11*a+x, 98, 37+11*a+x, A_XOR);
	}
for (x=0; x<11; x++)
	 DrawLine(42, 37+11*a+x, 98, 37+11*a+x, A_XOR);
DrawStr(43,38,"Exit",A_XOR);	
DrawStr(43,49,"Restart",A_XOR);


OSSetSR (0x0700);  // turn off all interrupts
if (a&&key!=KEY_ESC)
goto reset;

free(coordinates);

 end:
 for (a=0; a<80; a++) {
 	DrawLine(a,0,a,99, A_NORMAL);
 	DrawLine(159-a,0,159-a,99, A_NORMAL);
 	DrawLine(0,a,159,a, A_NORMAL);
 	DrawLine(0,100-a,159,100-a, A_NORMAL);
 }
OSSetSR (0x0000);  // turn back on all interrupts
}

// March 28, 2003