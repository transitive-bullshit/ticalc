| Fast routine for printing an unsigned short in base 10.  (used to display time during matches)
|
| Copyright © 2002-2004 Lionel Debroux, designed for TIGCCLIB.

.data
        .xdef itoa_ushort_10
        .even
itoa_ushort_10:
        subq.l   #6,%sp
        movea.l  %sp,%a1
        moveq    #10,%d1
        cmp.w    %d1,%d0
        bcc.s    __ius10_clear_upper_part_of_d0__

        addi.b   #'0',%d0            | 8 clocks, 4 bytes.
        move.b   %d0,(%a0)+          | 8 clocks, 2 bytes.
|       move.b   0(%a2,%d0.w),(%a1)+ | 18 clocks, 4 bytes.

        moveq    #1,%d0
        bra.s    __ius10_cleanup_and_bye__
__ius10_clear_upper_part_of_d0__:
        swap     %d0
        clr.w    %d0
        swap     %d0
__ius10_loop__:
        divu.w   %d1,%d0
| Take the remainder.
        swap     %d0
        addi.b   #'0',%d0
        move.b   %d0,(%a1)+
| Clear the remainder.
        clr.w    %d0
| Swap back the register.
        swap     %d0
| If whole register is less than 10, that's over.
        cmp.w    %d1,%d0
        bcc.s    __ius10_loop__

        addi.b   #'0',%d0
        move.b   %d0,(%a0)+
__ius10_prepare_copy__:
        move.l   %a1,%d1
        sub.l    %sp,%d1
        move.w   %d1,%d0
        addq.b   #1,%d0
        subq.b   #1,%d1
__ius10_copy__:
        move.b   -(%a1),(%a0)+
        dbf      %d1,__ius10_copy__
__ius10_cleanup_and_bye__:
        clr.b    (%a0)
        addq.l   #6,%sp
        rts
