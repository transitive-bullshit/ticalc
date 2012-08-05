// C Source File
// Created 3/28/2003; 8:54:07 PM

#define USE_TI89              // Compile for TI-89

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define MIN_AMS 100           // Compile for AMS 1.00 or higher

#include <tigcclib.h>         // Include All Header Files

#define NO_CALC_DETECT

#define NO_EXIT_SUPPORT

static unsigned char gun[]=
{0x10,0x38,0x54,0xFE,0x54,0x38,0x10,0x0};

void ScrollLeft(unsigned short* buffer,unsigned short lines) {
    register short* tmpbuffer = buffer;
    register short  tmplines  = lines;
    tmpbuffer += (tmplines<<4) - (tmplines);
    tmplines--;
    asm volatile (
       "0:
        and.b #0xee,%%ccr
        roxl.w -(%0);roxl.w -(%0);roxl.w -(%0);roxl.w -(%0);roxl.w -(%0)
        roxl.w -(%0);roxl.w -(%0);roxl.w -(%0);roxl.w -(%0);roxl.w -(%0)
        roxl.w -(%0);roxl.w -(%0);roxl.w -(%0);roxl.w -(%0);roxl.w -(%0)
        dbra %1,0b"
        : "=a" (tmpbuffer), "=d" (tmplines)
        : "0"  (tmpbuffer), "1"  (tmplines));
}

void ScrollRight(unsigned short* buffer,unsigned short lines) {
    register short* tmpbuffer = buffer;
    register short  tmplines  = lines;
    tmplines--;
    asm volatile (
       "0:
        and.b #0xee,%%ccr
        roxr.w (%0)+;roxr.w (%0)+;roxr.w (%0)+;roxr.w (%0)+;roxr.w (%0)+;
        roxr.w (%0)+;roxr.w (%0)+;roxr.w (%0)+;roxr.w (%0)+;roxr.w (%0)+;
        roxr.w (%0)+;roxr.w (%0)+;roxr.w (%0)+;roxr.w (%0)+;roxr.w (%0)+
        dbra %1,0b"
        : "=a" (tmpbuffer), "=d" (tmplines)
        : "0"  (tmpbuffer), "1"  (tmplines));
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

void ScrollDown(unsigned short* buffer,unsigned short lines) {
    register short* dest = buffer;
    register short* src;
    register short  tmplines  = lines;

    dest += (tmplines<<4) - (tmplines);
    src   = dest - 15;

    tmplines -= 2;

    asm volatile (
       "0:
        move.l -(%0),-(%1);move.l -(%0),-(%1);move.l -(%0),-(%1)
        move.l -(%0),-(%1);move.l -(%0),-(%1);move.l -(%0),-(%1)
        move.l -(%0),-(%1);move.w -(%0),-(%1)
        dbra %2,0b
        clr.l -(%1);clr.l -(%1);clr.l -(%1);clr.l -(%1)
        clr.l -(%1);clr.l -(%1);clr.l -(%1);clr.w -(%1)"
        : "=a" (src), "=a" (dest), "=d" (tmplines)
        : "0"  (src), "1"  (dest), "2"  (tmplines));
}

void bullet(short int x, short int y, short int kind) {	
	int a, key, wait;

	//  Sprite Data for bullets
static unsigned short int bullethole[]=
{0xC,0xC,0x3000,0x3000,0x0,0x400,0xC630,0xC038,0x38,0x0,0x600,0x630,0x31,0x0,0x3004,0x300C};
static unsigned int explosiond[]=
{0x0,0xFE0,0x1EF0,0x39F8,0x707C,0x76FE,0xECFE,0xEAC6,0xF3C6,0x78C6,0x380C,0x3A1C,0x2F3C,0x2770,0x1BF0,0x60};
static unsigned int explosionl[]=
{0x0,0xFC0,0x1100,0x36F8,0x6F9C,0x4F00,0x9F30,0x9F3A,0xDFB8,0x673C,0x37FC,0x27F8,0x33C0,0x3880,0x1C30,0x60};
static unsigned int exd2[]=
{0x0,0x0,0x100,0x7F0,0xFF8,0x1FFC,0x3C8C,0x3AE4,0x1ADC,0x18BC,0xE7C,0x3F8,0x60,0x0,0x0,0x0};
static unsigned int exl2[]=
{0x0,0x0,0x100,0x7F0,0xE18,0x1804,0x3B74,0x3D1C,0x1FAC,0x1F5C,0xFFC,0x3F8,0xE0,0x0,0x0,0x0};
	
Sprite8( x, y, 7, gun, GrayGetPlane (DARK_PLANE), SPRT_XOR);
Sprite8( x, y, 7, gun, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
	if (!kind) {
		for(key=0; key<337; key++) {
			for(a=0; a<2; a++) {
				Sprite16( x-4, y-4, 16, explosiond, GrayGetPlane (DARK_PLANE), SPRT_OR);
				Sprite16( x-4, y-4, 16, explosionl, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
				Sprite16( x-4, y-4, 16, exd2, GrayGetPlane (DARK_PLANE), SPRT_XOR);
				Sprite16( x-4, y-4, 16, exl2, GrayGetPlane (LIGHT_PLANE), SPRT_OR);
			}
			for (a=0; a<100*random(10); a++)
				wait=random(2000);
		}
	} else if (kind==1) {
		Sprite16( x-4, y-4, 16, bullethole, GrayGetPlane (DARK_PLANE), SPRT_XOR);
		Sprite16( x-4, y-4, 16, bullethole, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
	} else if (kind==2) {
			for (a=0; a<24; a++) {
				for(key=0; key<7; key++)
				ScrollRight(LCD_MEM,160);
				for(key=0; key<7; key++)
				ScrollDown(LCD_MEM,100);
				for(key=0; key<7; key++)
 				ScrollUp(LCD_MEM,100);
 				for(key=0; key<7; key++)
 				ScrollLeft(LCD_MEM,160);
 			}
	}  // end if loop for scrolling
Sprite8( x, y, 7, gun, GrayGetPlane (DARK_PLANE), SPRT_XOR);
Sprite8( x, y, 7, gun, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
}


// Main Function
void _main(void)
{
int wait, x2, move, x, y, key;

int *virtual=malloc (LCD_SIZE);  // Allocate the buffer

if (!virtual)
return;

ST_helpMsg("Use 2nd, F1, and diamond to shoot!");
memcpy(virtual,LCD_MEM, 3840);

INT_HANDLER interrupt1 = GetIntVec(AUTO_INT_1);
SetIntVec(AUTO_INT_1,DUMMY_HANDLER);

if (!GrayOn()) {
	SetIntVec(AUTO_INT_1,interrupt1);
	free(virtual);
	return;
}

reset:
memcpy(GrayGetPlane (DARK_PLANE), virtual, 3840);
memcpy(GrayGetPlane (LIGHT_PLANE), virtual, 3840);

for(x2=0; x2<1000; x2++)
wait=random(2000);

x=75;
y=43;
move=16;

Sprite8( x, y, 7, gun, GrayGetPlane (DARK_PLANE), SPRT_XOR);
Sprite8( x, y, 7, gun, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);

while (1) {
	key=_rowread(0xFFFE);  // get keypress
	if (key) {
		move+=1;
		Sprite8( x, y, 7, gun, GrayGetPlane (DARK_PLANE), SPRT_XOR);
		Sprite8( x, y, 7, gun, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
		if (key&0x0001&&y>-3)  // move you up one
			y--;
		if (key&0x0004&&y<94)  // move you down
			y++;
		if (key&0x0002&&x>0)  // move you left
			x--;
		if (key&0x0008&&x<154) // move you right one
			x++;
		Sprite8( x, y, 7, gun, GrayGetPlane (DARK_PLANE), SPRT_XOR);
		Sprite8( x, y, 7, gun, GrayGetPlane (LIGHT_PLANE), SPRT_XOR);
	}  // end if loop checking if key was pressed
	if ((_rowread(~((short)(1<<1))) & (1<<6))||(_keytest (RR_ESC)))
	break;
	if (_keytest (RR_2ND) && move>10) {
		bullet(x, y, 0);
		move=0;
	} else if (_keytest (RR_F1) && move>10) {
		bullet(x, y, 1);
		move=0;
	} else if (_keytest (RR_DIAMOND) && move>10)
		bullet(x, y, 2);
	
	for(x2=0; x2<300; x2++)
	wait=random(2000);
}
if ((_rowread(~((short)(1<<1))) & (1<<6)))
goto reset;
free(virtual);
GrayOff();
SetIntVec(AUTO_INT_1,interrupt1);// turn all interrupts back on
ST_helpMsg("Ich hase schmutzig Menschen! --By Fisch");
}
