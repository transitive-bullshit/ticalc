| Assembly Source File
| Created 3/18/2004; 7:25:12 PM
| Slightly specialized because, where this function is called from, registers are very 
| scarce.  By not taking up two registers in the calling function to store Terrainl and 
| Terraind, much more efficient code is produced.

.data
.globl CGraySprite8_OR_R
.even

CGraySprite8_OR_R:
    dbf      %d2,__Rest_of_CGraySprite8_OR_R
    rts

__Rest_of_CGraySprite8_OR_R:
    movem.l  %d3/%a0-%a3,-(%sp)
    
    move.l   Terrainl,%a0
    move.l   Terraind,%a1

    move.w   %d1,%d3	| d3 = y
    lsl.w    #4,%d1	| d1 = 16*y
    sub.w    %d3,%d1	| d1 = 15*y

    move.w   %d0,%d3	| d3 = x
    lsr.w    #4,%d3	| d3 = x/16

    add.w    %d3,%d1	| d1 = 15*y + x/16
    add.w    %d1,%d1	| d1 = 30*y + x/8
    adda.w   %d1,%a0	| a0 += offset
    adda.w   %d1,%a1

    and.w    #0xF,%d0
    moveq    #8,%d1
    sub.w    %d0,%d1
    bge.s    __loop_CGraySprite8_OR_R_2

    neg.w    %d1
_loop_CGraySprite8_OR_R_1:
    moveq    #0,%d0
    move.b   (%a2)+,%d0
    swap.w   %d0
    lsr.l    %d1,%d0
    or.l     %d0,(%a0)
    lea.l    30(%a0),%a0

    moveq    #0,%d0
    move.b   (%a3)+,%d0
    swap.w   %d0
    lsr.l    %d1,%d0
    or.l     %d0,(%a1)
    lea.l    30(%a1),%a1

    dbf      %d2,_loop_CGraySprite8_OR_R_1
    movem.l  (%sp)+,%d3/%a0-%a3
    rts

_loop_CGraySprite8_OR_R_2:
    lea      30(%a0),%a0
    lea      30(%a1),%a1
__loop_CGraySprite8_OR_R_2:
    moveq    #0,%d0
    move.b   (%a2)+,%d0
    lsl.w    %d1,%d0
    or.w     %d0,(%a0)

    moveq    #0,%d0
    move.b   (%a3)+,%d0
    lsl.w    %d1,%d0
    or.w     %d0,(%a1)

    dbf      %d2,_loop_CGraySprite8_OR_R_2

    movem.l  (%sp)+,%d3/%a0-%a3
    rts
