// C Source File
// Created 12/14/2003; 12:03:00 PM
#include "All.h"
// Mostly written in assembly for speed reasons.  I just started learning assembly, so I'm sure that 
// this file could be heavily optimized/fixed.  As with the rest of the Excitebike project, if you 
// know of any way to improve part of this file, please e-mail me at phishfud@elknet.net

//static inline void __attribute__((__regparm__)) CopyTerrain_ToHidden(unsigned char *src0 asm("%a0"), 
//unsigned char *src1 asm("%a1"), register short h);
static inline void CopyTerrain_ToHidden(short x, short h);

// Move the Camera Horizontally
void Shift_Left(short No_Lines) {
	g->softx -= No_Lines;
	
	const short Offset = (TI89 ? 150 : 960);  // 5 * 30 : 33 * 30
	for(;No_Lines--;) {
		ScrollLeft240_R((unsigned short*)((unsigned char*)Terrainl + Offset), 96);
		ScrollLeft240_R((unsigned short*)((unsigned char*)Terraind + Offset), 96);
	}
	
	Tile tile = Tiles[g->Track[g->camera_x]];
	if (g->softx < -8) {  // Fill Next Slot
		if (CALCULATOR > 0) {
			CGraySprite8_OR_R(240 + g->softx, 128 - tile.Height, tile.Height, tile.PtrLight, tile.PtrDark);
			if (g->camera_x & 1 && tile.Height <= 56)
				CGraySprite8_OR_R(240 + g->softx, 128 - 64, 8, Hedge, Hedge + 8);
			
			if (g->Cheats & C_BACKGROUND)
				CopyTerrain_ToHidden(240 + g->softx, tile.Height == 96 ? 128 - 34 : 128 - tile.Height + 11);
		}
		
		g->softx += 8;
		if (++g->camera_x > g->Width)  // New Lap for the Camera
			g->camera_x = 0;
		tile = Tiles[g->Track[g->camera_x]];
		if (TI89) {
			const short cut = (tile.Height == 96);
			CGraySprite8_OR_R(160 + g->softx, 100 - tile.Height + cut, tile.Height + cut, tile.PtrLight + cut, 
				tile.PtrDark + cut);
			if (g->camera_x & 1 && tile.Height <= 56)
				CGraySprite8_OR_R(160 + g->softx, 100 - 64, 8, Hedge, Hedge + 8);
			
			if (g->Cheats & C_BACKGROUND)
				CopyTerrain_ToHidden(160 + g->softx, tile.Height == 96 ? 100 - 29 : (100 - tile.Height + 
				(tile.Height == 48 ? -5 : 11)));
		}
	}
	
	/*if (g->camera_x < 320)
		g->camera_x = 320;
	char buf[80];
	sprintf(buf, "%d %d %d", g->camera_x, g->softx, tile.Height);
	FS_DrawGrayString(0, 7, buf, A_NORMAL, F_6x8);*/
	
	if (CALCULATOR > 0) {
		DrawTerrain_Clipped8_OR_R(240 + g->softx, 128 - tile.Height, tile.Height, tile.PtrLight, tile.PtrDark);
		if (g->camera_x & 1 && tile.Height <= 56)
			DrawTerrain_Clipped8_OR_R(240 + g->softx, 128 - 64, 8, Hedge, Hedge + 8);
	}
}

static inline void CopyTerrain_ToHidden(short x, short h) {
	register unsigned long  offset = 150 + ((x>>4) << 1);
	register unsigned long  addr1  = (long) Terrainl + offset;
	register unsigned long  addr2  = (long) Terraind + offset;
	register unsigned short cnt    = 24 - (x & 15);
	
	register unsigned char Back0 = g->Back0, Back1 = g->Back1;
	for (;h--;addr1+=30,addr2+=30) {
		unsigned char temp1 = ~(char)(*(long*)addr1 >> cnt);
		
		*(long*)addr1|=(long)(Back0 & ~(char)(*(long*)addr2 >> cnt)) << cnt;
		*(long*)addr2|=(long)(Back1 & temp1) << cnt;
		
		//Back0 = ~Back0; Back1 = ~Back1;
	}
}

// Modified ExtGraph function ClipSprite8_OR_R
void __attribute__((__regparm__)) DrawTerrain_Clipped8_OR_R(short x asm("%d0"), short y asm("%d1"), 
short h asm("%d2"), unsigned char* sprite1 asm("%a2"), unsigned char *sprite2 asm("%a3"));
asm("
.data
.globl DrawTerrain_Clipped8_OR_R
.even

DrawTerrain_Clipped8_OR_R:
    dbf %d2,__ContinueDrawTerrain_Clipped8_OR_R
    rts

__ContinueDrawTerrain_Clipped8_OR_R:
		movem.l  %d3/%a0-%a3,-(%sp);
		
		move.l   Terrainl,%a0;
		move.l   Terraind,%a1;
		
		|subq.w 	 #1,%d2;
    andi.w   #7,%d0;		| shiftx = x & 7
    
    move.w   %d1,%d3;		| Calculate Offsets
    lsl.w    #4,%d1;
    sub.w    %d3,%d1;		| %d1 = y*15
    
    add.w    %d1,%d1;		| %d1 = y*30
    addi.w   #29,%d1;
    adda.w   %d1,%a0;		| dest0 += offset
    adda.w   %d1,%a1;		| dest1 += offset
__LoopClipR_DrawTerrain_Clipped8_OR_R:
    move.b   (%a2)+,%d1;
    lsr.b    %d0,%d1;
    or.b     %d1,(%a0);	| Dest0

    move.b   (%a3)+,%d1;
    lsr.b    %d0,%d1;
    or.b     %d1,(%a1);	| Dest1

    lea.l    30(%a0),%a0;
    lea.l    30(%a1),%a1;
    dbf      %d2,__LoopClipR_DrawTerrain_Clipped8_OR_R
    
    movem.l  (%sp)+,%d3/%a0-%a3;
    rts
");

// Original by Thomas Nussbaumer - from the Game Construction Kit
/*void __attribute__((__regparm__)) GrayTile_Clipped8(short x asm("%d3"), short h asm("%d2"), 
unsigned char* sprite1 asm("%a3"), unsigned char *sprite2 asm("%a2")) {
	register unsigned short cnt    = 24 - (x & 15);
	register short					y      = LCD_HEIGHT - h;
	register long           offset = (y<<5)-(y+y)+2*(x>>4);  // Suggested change by Bremen
	register long           addr1  = (long) Terrainl + offset;
	register long           addr2  = (long) Terraind + offset;
	
	if (x > 232) {  // Clip Right
		register unsigned char clip = 0xff << (x - 232);
		for (;h--;addr1+=30,addr2+=30) {
			*(long*)addr1|=(long)(*sprite1++ & clip)<<cnt;
			*(long*)addr2|=(long)(*sprite2++ & clip)<<cnt;
		}
	} else {
		for (;h--;addr1+=30,addr2+=30) {
			*(long*)addr1|=(long)(*sprite1++)<<cnt;
			*(long*)addr2|=(long)(*sprite2++)<<cnt;
		}
	}
}*/

/*void __attribute__((__regparm__)) CopyGrayScreens(unsigned char *src0 asm("%a0"), 
unsigned char *src1 asm("%a1"), unsigned char *dest0 asm("%a2"), unsigned char *dest1 asm("%a3"));
asm("
.data
.globl CopyGrayScreens
.even

CopyGrayScreens:
   link.w %a6,#0
   
   |movm.l #0x1e20,-(%sp)
   |tst.w __calculator
   jbne ._CopyGrayScreens92Plus_
   
   movem.l %d0-%d5,-(%sp);
   moveq #99,%d0;
   0:;
   movem.l (%a0)+,%d1-%d5;		| Plane 1
   movem.l %d1-%d5,(%a2);
   movem.l (%a1)+,%d1-%d5;		| Plane 2
   movem.l %d1-%d5,(%a3);
   lea (%a2,30),%a2;
   adda.w #10,%a0;
   lea (%a3,30),%a3;
   adda.w #10,%a1;
   
   dbra %d0,0b;								| Loop until we do all 100 Lines
   
   movem.l  (%sp)+,%d0-%d5		| Clean up and return
   unlk %a6
   rts
   
._CopyGrayScreens92Plus_:
   movem.l %d0-%d7,-(%sp);
   moveq #127,%d0;
   0:;
   movem.l (%a0)+,%d1-%d7;		| Plane 1
   movem.l %d1-%d7,(%a2);
   movem.l (%a1)+,%d1-%d7;		| Plane 2
   movem.l %d1-%d7,(%a3);
   lea (%a2,28),%a2;
   lea (%a3,28),%a3;
   
   move.w (%a0)+,%d1;					| Extra 2 Bytes on the Right of the Screen
   move.w %d1,(%a2)+;
   move.w (%a1)+,%d1;
   move.w %d1,(%a3)+;
   
   dbra %d0,0b;								| Loop until we do all 128 Lines
   
   movem.l  (%sp)+,%d0-%d7		| Clean up and return
   unlk %a6
   rts
");*/

//-//-// //-//-// //-//-// Much Faster than GrayDBufToggleSync(); //-//-// //-//-// //-//-//
void __attribute__((__regparm__)) CopyScreens89(unsigned char *src0 asm("%a0"), 
unsigned char *src1 asm("%a1"), unsigned char *dest0 asm("%a2"), unsigned char *dest1 asm("%a3"));
asm("
.data
.globl CopyScreens89
.even

CopyScreens89:
	movem.l %d0-%d5/%a0-%a3,-(%sp);
  moveq #99,%d0;
  0:;
  movem.l (%a0)+,%d1-%d5;		| Plane 1
  movem.l %d1-%d5,(%a2);
  movem.l (%a1)+,%d1-%d5;		| Plane 2
  movem.l %d1-%d5,(%a3);
  lea (%a2,30),%a2;
  adda.w #10,%a0;
  lea (%a3,30),%a3;
  adda.w #10,%a1;
  
  dbra %d0,0b;							| Loop until we do all 100 Lines
  
  movem.l  (%sp)+,%d0-%d5/%a0-%a3;		| Restore previous register contents
  rts
");

/*void __attribute__((__regparm__)) CopyScreens92Plus(unsigned char *src0 asm("%a0"), 
unsigned char *src1 asm("%a1"), unsigned char *dest0 asm("%a2"), unsigned char *dest1 asm("%a3"));
asm("
.data
.globl CopyScreens92Plus
.even

CopyScreens92Plus:
	movem.l %d0-%d7,-(%sp);
	moveq #127,%d0;
	0:;
	movem.l (%a0)+,%d1-%d7;  | Light
	movem.l %d1-%d7,(%a2);
	movem.l (%a1)+,%d1-%d7;  | Dark
	movem.l %d1-%d7,(%a3);
	lea (%a2,28),%a2;
	lea (%a3,28),%a3;
	move.w (%a0)+,(%a2)+;    | Extra 2 Bytes on the Right of the Screen
	move.w (%a1)+,(%a3)+;
	
	dbra %d0,0b;             | Loop until we do all 128 Lines
	
	movem.l  (%sp)+,%d0-%d7
	rts
");*/

/*void  __attribute__((__stkparm__)) CopyScreens92Plus(unsigned char *src0 asm("%a0"), unsigned char *src1, 
unsigned char *dest0 asm("%a1"), unsigned char *dest1); //__attribute__((__regparm__(2)))
asm("
.data
.globl CopyScreens92Plus
.even

CopyScreens92Plus:
	movem.l %d0-%d7/%a0-%a6,-(%sp);
	move.w #13,-(%sp);
	
Copy_One_Screen:
	moveq #4,%d0;
	0:;
	movem.l (%a0)+,%d1-%d7/%a2-%a6;				| 48 Byte Chunks
	movem.l %d1-%d7/%a2-%a6,(%a1);
	movem.l (%a0)+,%d1-%d7/%a2-%a6;
	movem.l %d1-%d7/%a2-%a6,(0x30,%a1);
	movem.l (%a0)+,%d1-%d7/%a2-%a6;
	movem.l %d1-%d7/%a2-%a6,(0x60,%a1);
	movem.l (%a0)+,%d1-%d7/%a2-%a6;
	movem.l %d1-%d7/%a2-%a6,(0x90,%a1);		| 4
	movem.l (%a0)+,%d1-%d7/%a2-%a6;
	movem.l %d1-%d7/%a2-%a6,(0xC0,%a1);
	movem.l (%a0)+,%d1-%d7/%a2-%a6;
	movem.l %d1-%d7/%a2-%a6,(0xF0,%a1);
	movem.l (%a0)+,%d1-%d7/%a2-%a6;
	movem.l %d1-%d7/%a2-%a6,(0x120,%a1);
	movem.l (%a0)+,%d1-%d7/%a2-%a6;
	movem.l %d1-%d7/%a2-%a6,(0x150,%a1);	| 8
	movem.l (%a0)+,%d1-%d7/%a2-%a6;
	movem.l %d1-%d7/%a2-%a6,(0x180,%a1);
	movem.l (%a0)+,%d1-%d7/%a2-%a6;
	movem.l %d1-%d7/%a2-%a6,(0x1B0,%a1);
	movem.l (%a0)+,%d1-%d7/%a2-%a6;
	movem.l %d1-%d7/%a2-%a6,(0x1E0,%a1);
	movem.l (%a0)+,%d1-%d7/%a2-%a6;
	movem.l %d1-%d7/%a2-%a6,(0x210,%a1);	| 12
	movem.l (%a0)+,%d1-%d7/%a2-%a6;
	movem.l %d1-%d7/%a2-%a6,(0x240,%a1);
	movem.l (%a0)+,%d1-%d7/%a2-%a6;
	movem.l %d1-%d7/%a2-%a6,(0x270,%a1);
	movem.l (%a0)+,%d1-%d7/%a2-%a6;
	movem.l %d1-%d7/%a2-%a6,(0x2A0,%a1);
	movem.l (%a0)+,%d1-%d7/%a2-%a6;
	movem.l %d1-%d7/%a2-%a6,(0x2D0,%a1);	| 16
	
	lea (%a1,0x300),%a1;									| 48 * 16 = 768 = 0x300
	dbra %d0,0b;
	
	cmp.w	#13,(%sp)+;
	bne	End_CopyScreens;									| on second loop through
	
	move.w   #0,-(%sp);
	move.l   6+40+0(%sp),%a0;
	move.l   6+40+4(%sp),%a1;
	bra Copy_One_Screen;
	
End_CopyScreens:
	movem.l  (%sp)+,%d0-%d7/%a0-%a6;
	rts
");*/