// Copyright © 2002 TICT.
// Credits go to:
// - 'hibou' (Nicolas Lalevée) for improving the original functions in his
//   program HibText, and giving me the idea to implement them slightly
//   differently.
// - 'nEUrOne' for pointing me that his functions written in assembly were
//   faster. I had a look at what GCC assembled for these functions, and saw
//   that there were two transfers to memory in the functions using masks,
//   while one was enough (that was not GCC's fault but Tom's and my fault)... 
//   Then, I rewrote these functions, that decreased  size and improved speed.
//   After that, I decided to rewrite all functions in plain assembly...

// These functions are not designed to write in planes that are not of the
// screen size ! That's because the space between two lines is hard-coded...
// Clipping and word-wrap are NOT implemented !

// CAUTION: The functions will do what you want them to do, even if you give
// them a font number for which the font address was not retrieved...
// This is very likely to end in a crash !

#ifndef  __FASTSTR_H_
#define __FASTSTR_H_

extern unsigned char* __FontsTable__;
extern unsigned char* __F_4x6_Font__;
extern unsigned char* __F_6x8_Font__;
extern unsigned char* __F_8x10_Font__;

//static unsigned char __Factors__[3]={6,8,10};
//extern unsigned char __table_MaskLS__[];

//static short Factors[]={6,8,10};

// New SetupCharSet
extern short SetupCharSet(void);
asm("
	.data
	.even
	.xdef SetupCharSet
SetupCharSet:
| The method is fast in both cases. Therefore, there is no need to suppress interrupts.

	pea (%a2)
	move.l %d3,-(%sp)
	
	movea.l 0xC8.w,%a2

| A cmpi.w would be enough...
	cmpi.l #1000,-4(%a2)
	bgt.s __Prepare_fonts_AMS_2xx__
__Prepare_fonts_AMS_1xx__:
| Get the address of DrawStr.
	movea.l 0x1A9*4(%a2),%a0
	
| Check if we have an old PedroM version.
	cmpi.l #0x50656472,-(%a0) | 'Pedr', a signature of PedroM.
	bne.s __The_ROM_we_re_running_on_is_not_PedroM__

| We have PedroM.

| Setup all fonts. This will suppress crashes consecutive to the use of a
| font that was not initialized...

| d(pc) mode is forbidden for general purpose code.
	lea __F_8x10_Font__,%a1
	move.l -(%a0),(%a1)
	move.l -(%a0),-(%a1)
	move.l -(%a0),-(%a1)
	bra.s __Prepare_fonts_end__

__The_ROM_we_re_running_on_is_not_PedroM__:
| Retrieving the addresses of the fonts is easy (but slightly nasty) on AMS 1.xx.

| Catch a pc-relative jump to a subroutine in which are the three addresses we're looking for.
	lea 58+4(%a0),%a0 | Get rid of the cmpi.l #0x50656472,-(%a0) above.
	movea.w (%a0),%a1
	adda.l %a0,%a1
| a1 now contains the absolute address of the subroutine.

| d(pc) mode is forbidden for general purpose code.
	lea __FontsTable__,%a0

| Setup all fonts. This will suppress crashes consecutive to the use of a
| font that was not initialized...
| Setup F_4x6
	move.l 122(%a1),(%a0)+
| Setup F_6x8
	move.l 106(%a1),(%a0)+
| Setup F_8x10
	move.l 82(%a1),(%a0)

	bra.s __Prepare_fonts_end__

.even
__Prepare_fonts_AMS_2xx__:
| The fonts can be redefined on AMS 2.xx. So, retrieving the addresses of the fonts must be
| performed with FlashApp functions.

	movea.l 0x45D*4(%a2),%a0 | EV_runningApp
	move.w (%a0),%d3
	beq.s __Prepare_fonts_GetAttr__
| OO_GetAppAttr calls OO_GetAttr with the frame of the given application.
| So we are going to catch the frame of the running application, to pass it
| to OO_CondGetAttr.
| We have AMS 2.xx, we can use ROM_CALL 441 'HeapTable' (smaller and faster
| than pushing d3.w on the stack, calling HeapDeref, correcting sp).
        movea.l 0x441*4(%a2),%a0 | HeapTable
	lsl.w #2,%d3
	movea.l 0(%a0,%d3.w),%a0

| The pFrame of an ACB structure is located 20 bytes after the beginning.
	movea.l 20(%a0),%a0
	
	bra.s _Prepare_fonts_GetAttr__

__Prepare_fonts_GetAttr__:
        lea 0xFF000000.l,%a0
_Prepare_fonts_GetAttr__:
	movea.l 0x3FA*4(%a2),%a2 | OO_CondGetAttr
| We use OO_CondGetAttr because it does not throw errors, unlike OO_GetAttr
| (which can call OO_CondGetAttr anyway).

| Setup all fonts. This will suppress crashes consecutive to the use of a
| font that was not initialized...
| Setup F_4x6
| d(pc) mode is forbidden for general purpose code.
	pea __F_4x6_Font__
	pea 0x300.w
	pea (%a0)

	jsr (%a2)
	tst.w %d0
	beq.s __Prepare_fonts_problem__

| Setup F_6x8
	addq.l #4,8(%sp)
	addq.w #1,6(%sp)
	jsr (%a2)
	tst.w %d0
	beq.s __Prepare_fonts_problem__

| Setup F_8x10
	addq.l #4,8(%sp)
	addq.w #1,6(%sp)
	jsr (%a2)
	tst.w %d0
	beq.s __Prepare_fonts_problem__

	lea 12(%sp),%sp

__Prepare_fonts_end__:
	moveq #1,%d0

_Prepare_fonts_end__:
| Don't forget to restore registers...
	move.l (%sp)+,%d3
	move.l (%sp)+,%a2

| This is not an inlined version of the routine...
	rts

__Prepare_fonts_problem__:
	moveq #0,%d0
	bra.s _Prepare_fonts_end__
");


// Calling this function is not necessary.
/*
void CleanupCharSet(void);
asm("
        .xdef CleanupCharSet
CleanupCharSet:
	pea (%a0)

	lea __FontsTable__,%a0

| We don't care about a few clocks, so clr.l is OK (and reduces size).
	clr.l (%a0)+
	clr.l (%a0)+
	clr.l (%a0)+

	move.l (%sp)+,%a0

| This is not an inlined version of the routine...
	rts
");
*/

asm("
| The three pointers are always stored into the program. This wastes 8 bytes maximum, but
| increases the speed of some of the functions.
| The pointer to F_4x6 also points to the character widths of F_4x6, due to the way the
| characters are stored.
	.xdef __FontsTable__
	.even
__FontsTable__:
	.xdef __F_4x6_Font__
	.even
__F_4x6_Font__:
	.long 0

	.xdef __F_6x8_Font__
	.even
__F_6x8_Font__:
	.long 0
	
	.xdef __F_8x10_Font__
	.even
__F_8x10_Font__:
	.long 0
");

#endif // __FASTSTR