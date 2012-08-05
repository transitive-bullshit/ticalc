// Header File
// Created 7/11/2005; 10:15:53 PM
/*       c_strings v1.0       */
// By:  Travis Fischer (Fisch)
/*       c_strings v1.0       */
#ifndef storyline_h
#define storyline_h
#define NO_STORY_STRINGS 	(43)
extern void *story;

/********************************************************
*
* Array name:  story
* No strings:  43 (C), 33 (asm)
*
********************************************************/
#ifdef ENGLISH
asm("
story:
.word story0   - story
.word story1   - story
.word story2   - story
.word story2   - story
.word story3   - story
.word story4   - story
.word story5   - story
.word story6   - story
.word story7   - story
.word story2   - story
.word story8   - story
.word story9   - story
.word story10  - story
.word story11  - story
.word story1   - story
.word story2   - story
.word story2   - story
.word story12  - story
.word story13  - story
.word story14  - story
.word story15  - story
.word story16  - story
.word story17  - story
.word story18  - story
.word story2   - story
.word story19  - story
.word story20  - story
.word story21  - story
.word story22  - story
.word story23  - story
.word story2   - story
.word story24  - story
.word story25  - story
.word story26  - story
.word story27  - story
.word story28  - story
.word story29  - story
.word story30  - story
.word story31  - story
.word story2   - story
.word story32  - story
.word story1   - story

story0:
.asciz \"Crappy Storyline\"
story1:
.asciz \"--\"
story2:
.asciz \"\"
story3:
.asciz \"Once upon a time\"
story4:
.asciz \"in Feudal Japan,\"
story5:
.asciz \"there was a\"
story6:
.asciz \"sumo wrestler\"
story7:
.asciz \"like no other...\"
story8:
.asciz \"To some, he was\"
story9:
.asciz \"called The Outcast,\"
story10:
.asciz \"but to most he was\"
story11:
.asciz \"known as Steve\"
story12:
.asciz \"After being sent\"
story13:
.asciz \"on a quest by\"
story14:
.asciz \"someone to become\"
story15:
.asciz \"a powerful warrior,\"
story16:
.asciz \"Steve battled the\"
story17:
.asciz \"infamous ancient\"
story18:
.asciz \"Japanese Cowboys..\"
story19:
.asciz \"Though he defeated\"
story20:
.asciz \"them unscathed,\"
story21:
.asciz \"his pride was hurt\"
story22:
.asciz \"when one of them\"
story23:
.asciz \"called him fat.\"
story24:
.asciz \"Steve wanted to\"
story25:
.asciz \"prove that fat was\"
story26:
.asciz \"an understatement,\"
story27:
.asciz \"that he was the\"
story28:
.asciz \"mightiest sumo of\"
story29:
.asciz \"them all, so he\"
story30:
.asciz \"challenged peoples\"
story31:
.asciz \"to a Tournament...\"
story32:
.asciz \"And so it begins\"
");

/* C equivalent: (less optimized storage format)

static const char * const info_text[] = {
"Crappy Storyline", "--", 
"", "", 
"Once upon a time", "in Feudal Japan,", 
"there was a", "sumo wrestler", "like no other...", "", "To some, he was", 
"called The Outcast,", "but to most he was", "known as Steve", "--", 
"", "", 
"After being sent", "on a quest by", "someoneto become a", "powerful warrior,", 
"Steve battled the", "infamous ancient", "Japanese Cowboys..", 
"", 
"Though he defeated", "them unscathed,", "his pride was hurt", 
"when one of them", "called him fat.", 
"", 
"Steve wanted to", "prove that fat was", "an understatement,", "that he was the", 
"mightiest sumo of", "them all, so he", "challenged peoples", "to a tournament...", 
"", 
"And so it begins", "--", 
NULL};

"so he began to", "initiate the", "prelude to the", "precedings", 
"previously", "mentioned", "- a tournament", 


//shananigans, rabble-rousers, ruckus, mischief, 
//"who sent Steve on", "this foobar mission", 
//"mission saw his", "skill and commanded", "him to challenge", "the mighty", 
//"rabblerouser army", "of sumos wrestlers...", 
*/

#elif defined(FRENCH)

#elif defined(GERMAN)

#endif

#endif