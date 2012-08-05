// Header File
// Created 10/13/2004; 6:51:28 PM
/*       c_strings v1.0       */
// By:  Travis Fischer (Fisch)
/*       c_strings v1.0       */
#ifndef info_text_h
#define info_text_h
#define NO_INFO_STRINGS 	(77)
extern void *info_text;

/********************************************************
*
* Array name:  info_text
* No strings:  77 (C), 49 (asm)
*
********************************************************/
#ifdef ENGLISH
asm("
info_text:
.word info_text0   - info_text
.word info_text1   - info_text
.word info_text2   - info_text
.word info_text3   - info_text
.word info_text4   - info_text
.word info_text5   - info_text
.word info_text6   - info_text
.word info_text3   - info_text
.word info_text3   - info_text
.word info_text7   - info_text
.word info_text8   - info_text
.word info_text3   - info_text
.word info_text3   - info_text
.word info_text9   - info_text
.word info_text3   - info_text
.word info_text10  - info_text
.word info_text11  - info_text
.word info_text3   - info_text
.word info_text12  - info_text
.word info_text13  - info_text
.word info_text14  - info_text
.word info_text15  - info_text
.word info_text3   - info_text
.word info_text16  - info_text
.word info_text17  - info_text
.word info_text18  - info_text
.word info_text19  - info_text
.word info_text20  - info_text
.word info_text3   - info_text
.word info_text21  - info_text
.word info_text3   - info_text
.word info_text22  - info_text
.word info_text23  - info_text
.word info_text24  - info_text
.word info_text25  - info_text
.word info_text3   - info_text
.word info_text26  - info_text
.word info_text17  - info_text
.word info_text27  - info_text
.word info_text19  - info_text
.word info_text20  - info_text
.word info_text3   - info_text
.word info_text28  - info_text
.word info_text3   - info_text
.word info_text29  - info_text
.word info_text30  - info_text
.word info_text31  - info_text
.word info_text32  - info_text
.word info_text3   - info_text
.word info_text33  - info_text
.word info_text17  - info_text
.word info_text34  - info_text
.word info_text19  - info_text
.word info_text20  - info_text
.word info_text3   - info_text
.word info_text35  - info_text
.word info_text2   - info_text
.word info_text3   - info_text
.word info_text3   - info_text
.word info_text36  - info_text
.word info_text3   - info_text
.word info_text37  - info_text
.word info_text38  - info_text
.word info_text39  - info_text
.word info_text40  - info_text
.word info_text41  - info_text
.word info_text42  - info_text
.word info_text43  - info_text
.word info_text46  - info_text
.word info_text47  - info_text
.word info_text48  - info_text
.word info_text3   - info_text
.word info_text44  - info_text
.word info_text2   - info_text
.word info_text3   - info_text
.word info_text3   - info_text
.word info_text45  - info_text

info_text0:
.asciz \"Sumo Wrestling\"
info_text1:
.asciz \"v0.20\"
info_text2:
.asciz \"--\"
info_text3:
.asciz \"\"
info_text4:
.asciz \"Programmed By\"
info_text5:
.asciz \"- Fisch -\"
info_text6:
.asciz \"Travis Fischer\"
info_text7:
.asciz \"Released In\"
info_text8:
.asciz \"May 2005\"
info_text9:
.asciz \"- Keys -\"
info_text10:
.asciz \"APPS Pauses\"
info_text11:
.asciz \"ESC Quits\"
info_text12:
.asciz \"TI89:\"
info_text13:
.asciz \"2nd punches\"
info_text14:
.asciz \"Shift charges\"
info_text15:
.asciz \"2nd+Up/Down Taunts\"
info_text16:
.asciz \"2nd+Right/Left/Up\"
info_text17:
.asciz \"and\"
info_text18:
.asciz \"Shift+Forwards\"
info_text19:
.asciz \"perform moves\"
info_text20:
.asciz \"when pummeling\"
info_text21:
.asciz \"F5 Teacher Key\"
info_text22:
.asciz \"TI92Plus:\"
info_text23:
.asciz \"F1 punches\"
info_text24:
.asciz \"F5 charges\"
info_text25:
.asciz \"F1+Up/Down Taunts\"
info_text26:
.asciz \"F1+Right/Left/Up\"
info_text27:
.asciz \"F5+Forwards\"
info_text28:
.asciz \"F4 Teacher Key\"
info_text29:
.asciz \"v200:\"
info_text30:
.asciz \"Hand punches\"
info_text31:
.asciz \"Q charges\"
info_text32:
.asciz \"Hand+Up/Down Taunts\"
info_text33:
.asciz \"Hand+Right/Left/Up\"
info_text34:
.asciz \"Q+Forwards\"
info_text35:
.asciz \"F3 Teacher Key\"
info_text36:
.asciz \"Thanks To:\"
info_text37:
.asciz \"TICT,\"
info_text38:
.asciz \"The TIGCC Team,\"
info_text39:
.asciz \"Nintendo,\"
info_text40:
.asciz \"TechnoPlaza,\"
info_text41:
.asciz \"Kevin Kofler,\"
info_text42:
.asciz \"Lionel Debroux,\"
info_text43:
.asciz \"Squale92,\"
info_text46:
.asciz \"mhubi,\"
info_text47:
.asciz \"Lionel Anton,\"
info_text48:
.asciz \"240-185 (naPO),\"

info_text44:
.asciz \"And Cisco, My Cat\"
info_text45:
.asciz \"phishfud@elknet.net\"
");

/* C equivalent: (less optimized storage format)

static const char * const info_text[] = {
"Sumo Wrestling", "v0.20", "--", "", "Programmed By", "- Fisch -", "Travis Fischer", "", "", 
"Released In", "February 2005", "", "", 
"- Keys -", 
"", 
"APPS Pauses", "ESC Quits", 
"", 
"TI89:", 
"2nd punches", "Shift charges", "2nd+Up/Down Taunts", "", "2nd+Right/Left/Up", "and", "Shift+Forwards", 
"perform moves", "when pummeling", "", "F5 Teacher Key", 
"", 
"TI92Plus:", 
"F1 punches", "F5 charges", "F1+Up/Down Taunts", "", "F1+Right/Left/Up", "and", "F5+Forwards", 
"perform moves", "when pummeling", "", "F4 Teacher Key", 
"", 
"v200:", 
"Hand punches", "Q charges", "Hand+Up/Down Taunts", "", "Hand+Right/Left/Up", "and", "Q+Forwards", 
"perform moves", "when pummeling", "", "F3 Teacher Key", 
"--", "", "", 
"Thanks To:", 
"", 
"TICT,", "The TIGCC Team,", "Nintendo,", "TechnoPlaza,", "Kevin Kofler,", 
"Lionel Debroux,", "Squale92,", "mhubi,", "Lionel Anton,", "240-185 (naPO),", 
"", 
"And Cisco, My Cat", 
"--", 
"", "", 
"phishfud@elknet.net", NULL};*/
#elif defined(FRENCH)

#elif defined(GERMAN)

#endif

#endif