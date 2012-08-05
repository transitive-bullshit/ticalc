// Header File
// Created 8/15/2004; 11:20:00 AM
/*       c_strings v1.0       */
// By:  Travis Fischer (Fisch)
/*       c_strings v1.0       */
#ifndef asm_strings_h
#define asm_strings_h
extern void *info, *keys_89, *keys_92plus;

// This function may be called just like any regular C function
// Input:  %d0/%a1  Output:  %a0
unsigned char * __attribute__((__regparm__)) STRADDR(unsigned short Index asm("%d0"), 
void *array asm("%a1"));
asm("
.text | Should be better than .data thanks to section reordering
.globl STRADDR
.even

| Returns the address of array[Index] in %a0 (not an inline version)
STRADDR:
add.w   %d0,%d0
movea.w 0(%a1,%d0.w),%a0
adda.l %a1,%a0

rts
");

/********************************************************
*
* Array name:  info
* No strings:  81
*
********************************************************/
asm("
.data
.even
info:
.word info0   - info
.word info1   - info
.word info2   - info
.word info3   - info
.word info4   - info
.word info4_5 - info
.word info5   - info
.word info3   - info
.word info3   - info
.word info6   - info
.word info7   - info
.word info3   - info
.word info3   - info
.word info8   - info
.word info3   - info
.word info9   - info
.word info10  - info
.word info11  - info
.word info12  - info
.word info13  - info
.word info14  - info
.word info15  - info
.word info16  - info
.word info17  - info
.word info18  - info
.word info19  - info
.word info20  - info
.word info21  - info
.word info22  - info
.word info23  - info
.word info3   - info
.word info24  - info
.word info2   - info
.word info3   - info
.word info3   - info
.word info25  - info
.word info3   - info
.word info26  - info
.word info27  - info
.word info28  - info
.word info29  - info
.word info30  - info
.word info31  - info
.word info3   - info
.word info32  - info
.word info33  - info
.word info34  - info
.word info39  - info
.word info35  - info
.word info36  - info
.word info3   - info
.word info37  - info
.word info39  - info
.word info38  - info
.word info39  - info
.word info39  - info
.word info40  - info
.word info41  - info
.word info3   - info
.word info42  - info
.word info3   - info
.word info43  - info
.word info2   - info
.word info3   - info
.word info3   - info
.word info44  - info
.word info3   - info
.word info45  - info
.word info46  - info
.word info22  - info
.word info24  - info
.word info47  - info
.word info48  - info
.word info49  - info
.word info50  - info
.word info28  - info
.word info51  - info
.word info52  - info
.word info53  - info
.word info54  - info
.word info2   - info

info0:
.asciz \"ice hockey\"
info1:
.asciz \"v2.75\"
info2:
.asciz \"--\"
info3:
.asciz \"\"
info4:
.asciz \"programmed by\"
info4_5:
.asciz \"travis fischer\"
info5:
.asciz \"- fisch -\"
info6:
.asciz \"released in\"
info7:
.asciz \"october 2004\"
info8:
.asciz \"thanks to\"
info9:
.asciz \"thomas nussbaumer,\"
info10:
.asciz \"everyone at tict,\"
info11:
.asciz \"the tigcc team,\"
info12:
.asciz \"apogee software,\"
info13:
.asciz \"nintendo,\"
info14:
.asciz \"jesystems,\"
info15:
.asciz \"open source,\"
info16:
.asciz \"mr. mobius,\"
info17:
.asciz \"malcolm smith,\"
info18:
.asciz \"zeljko juric,\"
info19:
.asciz \"kevin kofler,\"
info20:
.asciz \"sebastian reichelt,\"
info21:
.asciz \"greg dietsche,\"
info22:
.asciz \"scott noveck,\"
info23:
.asciz \"lionel debroux,\"
info24:
.asciz \"and cisco, my cat\"
info25:
.asciz \"default keys\"
info26:
.asciz \"- TI89 -\"
info27:
.asciz \"2nd shoots\"
info28:
.asciz \"diamond switches\"
info29:
.asciz \"plrs on defense\"
info30:
.asciz \"shift passes\"
info31:
.asciz \"f5 teacher key\"
info32:
.asciz \"- TI92PLUS -\"
info33:
.asciz \"f1 shoots\"
info34:
.asciz \"f2 switches\"
info35:
.asciz \"f5 passes\"
info36:
.asciz \"f4 teacher key\"
info37:
.asciz \"- v200 -\"
info38:
.asciz \"hand shoots\"
info39:
.asciz \"q switches\"
info40:
.asciz \"f1 passes\"
info41:
.asciz \"f3 teacher key\"
info42:
.asciz \"escape quits\"
info43:
.asciz \"apps pauses\"
info44:
.asciz \"beta testers\"
info45:
.asciz \"clawsonia,\"
info46:
.asciz \"deadbird,\"
info47:
.asciz \"stoopidguy,\"
info48:
.asciz \"adrian bader,\"
info49:
.asciz \"digital,\"
info50:
.asciz \"jordan krage,\"
info51:
.asciz \"shadow\"
info52:
.asciz \"henry barrow,\"
info53:
.asciz \"dan barr\"
info54:
.asciz \"jesse frey\"
");

/********************************************************
*
* Array name:  keys_89
* No strings:  56
*
********************************************************/
asm("
.data
.even
keys_89:
.word keys_890   - keys_89
.word keys_891   - keys_89
.word keys_892   - keys_89
.word keys_893   - keys_89
.word keys_894   - keys_89
.word keys_895   - keys_89
.word keys_896   - keys_89
.word keys_897   - keys_89
.word keys_898   - keys_89
.word keys_899   - keys_89
.word keys_8910  - keys_89
.word keys_8911  - keys_89
.word keys_8912  - keys_89
.word keys_8913  - keys_89
.word keys_8914  - keys_89
.word keys_8915  - keys_89
.word keys_8916  - keys_89
.word keys_8917  - keys_89
.word keys_8918  - keys_89
.word keys_8919  - keys_89
.word keys_8920  - keys_89
.word keys_8921  - keys_89
.word keys_8922  - keys_89
.word keys_8923  - keys_89
.word keys_8924  - keys_89
.word keys_8925  - keys_89
.word keys_8926  - keys_89
.word keys_8927  - keys_89
.word keys_8928  - keys_89
.word keys_8929  - keys_89
.word keys_8930  - keys_89
.word keys_8931  - keys_89
.word keys_8932  - keys_89
.word keys_8933  - keys_89
.word keys_8934  - keys_89
.word keys_8935  - keys_89
.word keys_8936  - keys_89
.word keys_8937  - keys_89
.word keys_8938  - keys_89
.word keys_8939  - keys_89
.word keys_8940  - keys_89
.word keys_8941  - keys_89
.word keys_8942  - keys_89
.word keys_8943  - keys_89
.word keys_8944  - keys_89
.word keys_8945  - keys_89
.word keys_8946  - keys_89
.word keys_8947  - keys_89
.word keys_8948  - keys_89
.word keys_8948  - keys_89
.word keys_8948  - keys_89
.word keys_8948  - keys_89
.word keys_8948  - keys_89
.word keys_8948  - keys_89
.word keys_8948  - keys_89
.word keys_8949  - keys_89

keys_890:
.asciz \"Up\"
keys_891:
.asciz \"Left\"
keys_892:
.asciz \"Down\"
keys_893:
.asciz \"Right\"
keys_894:
.asciz \"2nd\"
keys_895:
.asciz \"Shift\"
keys_896:
.asciz \"Diamond\"
keys_897:
.asciz \"Alpha\"
keys_898:
.asciz \"Enter\"
keys_899:
.asciz \"+\"
keys_8910:
.asciz \"-\"
keys_8911:
.asciz \"*\"
keys_8912:
.asciz \"/\"
keys_8913:
.asciz \"^\"
keys_8914:
.asciz \"Clear\"
keys_8915:
.asciz \"F5\"
keys_8916:
.asciz \"(-)\"
keys_8917:
.asciz \"3\"
keys_8918:
.asciz \"6\"
keys_8919:
.asciz \"9\"
keys_8920:
.asciz \",\"
keys_8921:
.asciz \"T\"
keys_8922:
.asciz \"<-\"
keys_8923:
.asciz \"F4\"
keys_8924:
.asciz \".\"
keys_8925:
.asciz \"2\"
keys_8926:
.asciz \"5\"
keys_8927:
.asciz \"8\"
keys_8928:
.asciz \")\"
keys_8929:
.asciz \"Z\"
keys_8930:
.asciz \"CATALOG\"
keys_8931:
.asciz \"F3\"
keys_8932:
.asciz \"0\"
keys_8933:
.asciz \"1\"
keys_8934:
.asciz \"4\"
keys_8935:
.asciz \"7\"
keys_8936:
.asciz \"(\"
keys_8937:
.asciz \"Y\"
keys_8938:
.asciz \"MODE\"
keys_8939:
.asciz \"F2\"
keys_8940:
.asciz \"APPS\"
keys_8941:
.asciz \"STO\"
keys_8942:
.asciz \"EE\"
keys_8943:
.asciz \"|\"
keys_8944:
.asciz \"=\"
keys_8945:
.asciz \"X\"
keys_8946:
.asciz \"HOME\"
keys_8947:
.asciz \"F1\"
keys_8948:
.asciz \"\"
keys_8949:
.asciz \"ESC\"
");

/********************************************************
*
* Array name:  keys_92plus
* No strings:  80
*
********************************************************/
asm("
.data
.even
keys_92plus:
.word keys_92plus0   - keys_92plus
.word keys_92plus1   - keys_92plus
.word keys_92plus2   - keys_92plus
.word keys_92plus3   - keys_92plus
.word keys_92plus4   - keys_92plus
.word keys_92plus5   - keys_92plus
.word keys_92plus6   - keys_92plus
.word keys_92plus7   - keys_92plus
.word keys_92plus8   - keys_92plus
.word keys_92plus9   - keys_92plus
.word keys_92plus10  - keys_92plus
.word keys_92plus11  - keys_92plus
.word keys_92plus12  - keys_92plus
.word keys_92plus13  - keys_92plus
.word keys_92plus14  - keys_92plus
.word keys_92plus15  - keys_92plus
.word keys_92plus8   - keys_92plus
.word keys_92plus16  - keys_92plus
.word keys_92plus17  - keys_92plus
.word keys_92plus18  - keys_92plus
.word keys_92plus19  - keys_92plus
.word keys_92plus20  - keys_92plus
.word keys_92plus21  - keys_92plus
.word keys_92plus22  - keys_92plus
.word keys_92plus23  - keys_92plus
.word keys_92plus24  - keys_92plus
.word keys_92plus25  - keys_92plus
.word keys_92plus26  - keys_92plus
.word keys_92plus27  - keys_92plus
.word keys_92plus28  - keys_92plus
.word keys_92plus29  - keys_92plus
.word keys_92plus30  - keys_92plus
.word keys_92plus31  - keys_92plus
.word keys_92plus32  - keys_92plus
.word keys_92plus33  - keys_92plus
.word keys_92plus34  - keys_92plus
.word keys_92plus35  - keys_92plus
.word keys_92plus36  - keys_92plus
.word keys_92plus37  - keys_92plus
.word keys_92plus38  - keys_92plus
.word keys_92plus39  - keys_92plus
.word keys_92plus40  - keys_92plus
.word keys_92plus41  - keys_92plus
.word keys_92plus42  - keys_92plus
.word keys_92plus43  - keys_92plus
.word keys_92plus44  - keys_92plus
.word keys_92plus45  - keys_92plus
.word keys_92plus46  - keys_92plus
.word keys_92plus47  - keys_92plus
.word keys_92plus48  - keys_92plus
.word keys_92plus49  - keys_92plus
.word keys_92plus50  - keys_92plus
.word keys_92plus51  - keys_92plus
.word keys_92plus52  - keys_92plus
.word keys_92plus53  - keys_92plus
.word keys_92plus54  - keys_92plus
.word keys_92plus55  - keys_92plus
.word keys_92plus56  - keys_92plus
.word keys_92plus57  - keys_92plus
.word keys_92plus58  - keys_92plus
.word keys_92plus59  - keys_92plus
.word keys_92plus60  - keys_92plus
.word keys_92plus61  - keys_92plus
.word keys_92plus62  - keys_92plus
.word keys_92plus63  - keys_92plus
.word keys_92plus64  - keys_92plus
.word keys_92plus65  - keys_92plus
.word keys_92plus66  - keys_92plus
.word keys_92plus67  - keys_92plus
.word keys_92plus68  - keys_92plus
.word keys_92plus69  - keys_92plus
.word keys_92plus8   - keys_92plus
.word keys_92plus70  - keys_92plus
.word keys_92plus71  - keys_92plus
.word keys_92plus72  - keys_92plus
.word keys_92plus73  - keys_92plus
.word keys_92plus74  - keys_92plus
.word keys_92plus75  - keys_92plus
.word keys_92plus76  - keys_92plus
.word keys_92plus77  - keys_92plus

keys_92plus0:
.asciz \"2nd\"
keys_92plus1:
.asciz \"Diamond\"
keys_92plus2:
.asciz \"Shift\"
keys_92plus3:
.asciz \"Hand\"
keys_92plus4:
.asciz \"Left\"
keys_92plus5:
.asciz \"Up\"
keys_92plus6:
.asciz \"Right\"
keys_92plus7:
.asciz \"Down\"
keys_92plus8:
.asciz \"\"
keys_92plus9:
.asciz \"Z\"
keys_92plus10:
.asciz \"S\"
keys_92plus11:
.asciz \"W\"
keys_92plus12:
.asciz \"F8\"
keys_92plus13:
.asciz \"1\"
keys_92plus14:
.asciz \"2\"
keys_92plus15:
.asciz \"3\"
keys_92plus16:
.asciz \"X\"
keys_92plus17:
.asciz \"D\"
keys_92plus18:
.asciz \"E\"
keys_92plus19:
.asciz \"F3\"
keys_92plus20:
.asciz \"4\"
keys_92plus21:
.asciz \"5\"
keys_92plus22:
.asciz \"6\"
keys_92plus23:
.asciz \"STO\"
keys_92plus24:
.asciz \"C\"
keys_92plus25:
.asciz \"F\"
keys_92plus26:
.asciz \"R\"
keys_92plus27:
.asciz \"F7\"
keys_92plus28:
.asciz \"7\"
keys_92plus29:
.asciz \"8\"
keys_92plus30:
.asciz \"9\"
keys_92plus31:
.asciz \"Space\"
keys_92plus32:
.asciz \"V\"
keys_92plus33:
.asciz \"G\"
keys_92plus34:
.asciz \"T\"
keys_92plus35:
.asciz \"F2\"
keys_92plus36:
.asciz \"(\"
keys_92plus37:
.asciz \")\"
keys_92plus38:
.asciz \",\"
keys_92plus39:
.asciz \"/\"
keys_92plus40:
.asciz \"B\"
keys_92plus41:
.asciz \"H\"
keys_92plus42:
.asciz \"Y\"
keys_92plus43:
.asciz \"F6\"
keys_92plus44:
.asciz \"sin\"
keys_92plus45:
.asciz \"cos\"
keys_92plus46:
.asciz \"tan\"
keys_92plus47:
.asciz \"^\"
keys_92plus48:
.asciz \"N\"
keys_92plus49:
.asciz \"J\"
keys_92plus50:
.asciz \"U\"
keys_92plus51:
.asciz \"F1\"
keys_92plus52:
.asciz \"ln\"
keys_92plus53:
.asciz \"Enter2\"
keys_92plus54:
.asciz \"P\"
keys_92plus55:
.asciz \"=\"
keys_92plus56:
.asciz \"M\"
keys_92plus57:
.asciz \"K\"
keys_92plus58:
.asciz \"I\"
keys_92plus59:
.asciz \"F5\"
keys_92plus60:
.asciz \"Clear\"
keys_92plus61:
.asciz \"APPS\"
keys_92plus62:
.asciz \"*\"
keys_92plus63:
.asciz \"BckSpc\"
keys_92plus64:
.asciz \"theta\"
keys_92plus65:
.asciz \"L\"
keys_92plus66:
.asciz \"O\"
keys_92plus67:
.asciz \"+\"
keys_92plus68:
.asciz \"MODE\"
keys_92plus69:
.asciz \"ESC\"
keys_92plus70:
.asciz \"-\"
keys_92plus71:
.asciz \"Enter1\"
keys_92plus72:
.asciz \"A\"
keys_92plus73:
.asciz \"Q\"
keys_92plus74:
.asciz \"F4\"
keys_92plus75:
.asciz \"0\"
keys_92plus76:
.asciz \".\"
keys_92plus77:
.asciz \"(-)\"
");

#endif