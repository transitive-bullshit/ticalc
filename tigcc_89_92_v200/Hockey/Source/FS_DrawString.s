.text

    .even
    .global FS_DrawString
FS_DrawString:
    movem.l  %d3/%d5/%d7/%a2-%a4,-(%sp)
| Quick hack.
    move.w   %d0,%d5
    move.w   %d1,%d0
    movea.l  %a1,%a3
    move.w   %d2,%d7

    add.w    %d0,%d0
    move.w   %d0,%d2
    lsl.w    #4,%d2
    sub.w    %d0,%d2
    adda.w   %d2,%a3

    | factor = 6 on F_4x6, 8 on F_6x8, 10 on F_8x10 = 6 + font * 2.
    move.w   %d7,%d0
    add.w    %d0,%d0
    move.w   %d0,%d3
    addq.w   #6,%d3

    add.w    %d0,%d0
| A pc-relative lea doesn't suit for a generic routine, __FontsTable__ can be 
| more than 32K away from this lea.
    lea      __FontsTable__,%a1
    move.l   0(%a1,%d0.w),%a4

_beginning_of_loop_FS_DS_:
    moveq    #0,%d0
    move.b   (%a0)+,%d0
    jbeq     _finished_FS_DS_

    move.w   %d0,%d1
    
    mulu.w   %d3,%d0 | > 38.

| mulu and that sequence seem equivalent for F_4x6...
| I will keep mulu for size (and probably speed, because mulu may be faster
| for F_8x10)...

| F_4x6:  4 + 12 + 8 + 8 + 10 + 4 = 46.
| F_6x8:  4 + 12 + 8 + 10 = 34.
| F_8x10: 4 + 12 + 8 + 8 + 8 + 4 + 10 = 54.
/*    add.w    %d1,%d1 | 4
    lsl.w    #3,%d0  | 6+3*2 = 12
    
    cmpi.w   #1,%d7  | 8
    beq.s    _F_6x8_FS_DS_ | 8 or 10
    blt.s    _F_4x6_FS_DS_ | 8 or 10
    add.w    %d1,%d0       | 4
    bra.s    _F_6x8_FS_DS_ | 10
_F_4x6_FS_DS_:
    sub.w    %d1,%d0       | 4
_F_6x8_FS_DS_:*/
    lea      0(%a4,%d0.w),%a2

    move.w   %d5,%d2
    lsr.w    #4,%d2
    add.w    %d2,%d2
    lea      0(%a3,%d2.w),%a1

    tst.w    %d7
    bne.s    _not_F_4x6_FS_DS_1_
    addq.l   #1,%a2
    cmpi.b   #'g',%d1
    bne.s    _not_0x67_FS_DS_
    lea      30(%a1),%a1

_not_F_4x6_FS_DS_1_:
_not_0x67_FS_DS_:

    move.w   %d3,%d1
    subq.w   #1,%d1 | subtract 1 because of the dbf in the loop.
    tst.w    %d7
    bne.s    _not_F_4x6_FS_DS_2_
    subq.w   #1,%d1 | subtract 1 if F_4x6.

_not_F_4x6_FS_DS_2_:

    move.w   %d5,%d0
    andi.w   #15,%d0

    moveq    #8,%d2
    sub.w    %d0,%d2
    
    cmpi.w   #8,%d0
    ble.s    _one_word_FS_DS_

    neg.w    %d2

_loop_one_long_FS_DS_:
    moveq    #0,%d0
    move.b   (%a2)+,%d0
    swap     %d0
    lsr.l    %d2,%d0
    or.l     %d0,(%a1)
    lea      30(%a1),%a1
    
    dbf      %d1,_loop_one_long_FS_DS_

    bra.s    _next_letter_FS_DS_

_loop_one_word_FS_DS_:
    lea      30(%a1),%a1

_one_word_FS_DS_:
    clr.w    %d0
    move.b   (%a2)+,%d0
    lsl.w    %d2,%d0
    or.w     %d0,(%a1)
    
    dbf      %d1,_loop_one_word_FS_DS_

_next_letter_FS_DS_:
    cmpi.w   #1,%d7
    blt.s    _next_letter_F_4x6_FS_DS_
    bgt.s    _next_letter_F_8x10_FS_DS_
_next_letter_F_6x8_FS_DS_:
    addq.w   #6,%d5
    jbra     _beginning_of_loop_FS_DS_
_next_letter_F_4x6_FS_DS_:
    moveq    #0,%d0
    move.b   -6(%a2),%d0
    add.w    %d0,%d5
    jbra     _beginning_of_loop_FS_DS_
_next_letter_F_8x10_FS_DS_:
    addq.w   #8,%d5
    jbra     _beginning_of_loop_FS_DS_

_finished_FS_DS_:
    movem.l (%sp)+,%d3/%d5/%d7/%a2-%a4
    rts
