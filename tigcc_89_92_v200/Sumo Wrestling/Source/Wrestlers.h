// Header File
// Created 8/3/2004; 10:52:26 AM
#ifndef __WRESTLERS_H__
#define __WRESTLERS_H__

/* Wrestlers.h 
-- Contains information for every default Wrestler -- */
extern void *strings;

#ifdef ENGLISH
asm(" | 16 Default Characters * 3 = 48 Smack Talk sayings
strings:
| Name, Origin, 3 Smack Talk sayings for each Character
	.word string0_0  - strings
	.word string0_1  - strings
	.word string0_2  - strings
	.word string0_3  - strings
	.word string0_4  - strings
	.word string1_0  - strings
	.word string1_1  - strings
	.word string1_2  - strings
	.word string1_3  - strings
	.word string1_4  - strings
	.word string2_0  - strings
	.word string2_1  - strings
	.word string2_2  - strings
	.word string2_3  - strings
	.word string2_4  - strings
	.word string3_0  - strings
	.word string3_1  - strings
	.word string3_2  - strings
	.word string3_3  - strings
	.word string3_4  - strings
	.word string4_0  - strings
	.word string4_1  - strings
	.word string4_2  - strings
	.word string4_3  - strings
	.word string4_4  - strings
	.word string5_0  - strings
	.word string5_1  - strings
	.word string5_2  - strings
	.word string5_3  - strings
	.word string5_4  - strings
	.word string6_0  - strings
	.word string6_1  - strings
	.word string6_2  - strings
	.word string6_3  - strings
	.word string6_4  - strings
	.word string7_0  - strings
	.word string7_1  - strings
	.word string7_2  - strings
	.word string7_3  - strings
	.word string7_4  - strings
	.word string8_0  - strings
	.word string8_1  - strings
	.word string8_2  - strings
	.word string8_3  - strings
	.word string8_4  - strings
	.word string9_0  - strings
	.word string9_1  - strings
	.word string9_2  - strings
	.word string9_3  - strings
	.word string9_4  - strings
	.word string10_0 - strings
	.word string10_1 - strings
	.word string10_2 - strings
	.word string10_3 - strings
	.word string10_4 - strings
	.word string11_0 - strings
	.word string11_1 - strings
	.word string11_2 - strings
	.word string11_3 - strings
	.word string11_4 - strings
	.word string12_0 - strings
	.word string12_1 - strings
	.word string12_2 - strings
	.word string12_3 - strings
	.word string12_4 - strings
	.word string13_0 - strings
	.word string13_1 - strings
	.word string13_2 - strings
	.word string13_3 - strings
	.word string13_4 - strings
	.word string14_0 - strings
	.word string14_1 - strings
	.word string14_2 - strings
	.word string14_3 - strings
	.word string14_4 - strings
	.word string15_0 - strings
	.word string15_1 - strings
	.word string15_2 - strings
	.word string15_3 - strings
	.word string15_4 - strings
	
	string0_0:
		.asciz \"Hakuho\"   | Rikishi
	string0_1:
		.asciz \"Mongolia\" | Tokyo
	string0_2:
		.asciz \"I vill Terminate du!\"
    string0_3:
		.asciz \"You bloody wanker!\"
	string0_4:
		.asciz \"How tall are you?...  I didn't know they stacked sh*t that high!\"
	
	string1_0:
		.asciz \"Dejima\"   | Sasquatch
	string1_1:
		.asciz \"Ishikawa\" | Unknown
	string1_2:
		.asciz \"You'll scream louder than my wife!\"
	string1_3:
		.asciz \"If Jesus were here, I'd fight him too!\"
	string1_4:
		.asciz \"I'm not arrogant, I'm just better than you\"
	
	string2_0:
		.asciz \"Asashoryu\"
	string2_1:
		.asciz \"Mongolia\"
	string2_2:
		.asciz \"Everybody's got something to hide, except for me and my monkey\"
	string2_3:
		.asciz \"Your mom must be proud... she took a sh*t and it lived!\"
	string2_4:
		.asciz \"You have teepee for my Bunghole?\"
	
	string3_0:
		.asciz \"Chiyotaikai\"
	string3_1:
		.asciz \"Oita\"
	string3_2:
		.asciz \"Let's play house. You be the door and I'll slam you\"
	string3_3:
		.asciz \"Next time an eating contest?  I can't lose that!\"
	string3_4:
		.asciz \"I always win.. except when I lose, and then it doesn't count\"
	
	string4_0:
		.asciz \"Kaio\"
	string4_1:
		.asciz \"Fukuoka\"
	string4_2:
		.asciz \"I'm not as dumb as you look\"
	string4_3:
		.asciz \"Calling you stupid would be an insult to stupid people\"
	string4_4:
		.asciz \"I don't know what makes you so stupid, but it really works!\"
	
	string5_0:
		.asciz \"Musoyama\"
	string5_1:
		.asciz \"Ibaraki\"
	string5_2:
		.asciz \"My Buttocks will reign supreme!\"
	string5_3:
		.asciz \"Yo mama aint so bad...she'd give you the hair off of her back!\"
	string5_4:
		.asciz \"Yo mama so fat when her beeper goes off, people think she's backing up\"
	
	string6_0:
		.asciz \"Wakanosato\"
	string6_1:
		.asciz \"Aomori\"
	string6_2:
		.asciz \"I will crush you!\"
	string6_3:
		.asciz \"Is that a diaper you're wearing?\"
	string6_4:
		.asciz \"My love handles will suffocate you!\"
	
	string7_0:
		.asciz \"Hokutoriki\"
	string7_1:
		.asciz \"Tochigi\"
	string7_2:
		.asciz \"The paramedics are waiting\"
	string7_3:
		.asciz \"Pack your bags, you're about to meet your maker\"
	string7_4:
		.asciz \"Are you afraid of the dark, cos I'm about to knock your lights out\"
	
	string8_0:
		.asciz \"Shimotori\"
	string8_1:
		.asciz \"Niigata\"
	string8_2:
		.asciz \"I'm a living legend\"
	string8_3:
		.asciz \"C'mon sucka\"
	string8_4:
		.asciz \"Get ready for a whole world of hurt\"
	
	string9_0:
		.asciz \"Miyabiyama\"
	string9_1:
		.asciz \"Ibaraki\"
	string9_2:
		.asciz \"Methinks thou art a vile and wanton villain\"
	string9_3:
		.asciz \"Thou art evil, and for this offense, I dislike thee\"
	string9_4:
		.asciz \"Knowing there is no hope can give one the courage to fight and win\"
	
	string10_0:
		.asciz \"Jumonji\"
	string10_1:
		.asciz \"Aomori\"
	string10_2:
		.asciz \"Yo mama so fat she's on both sides of the family!\"
	string10_3:
		.asciz \"Yo mama so fat she jumped in the air and got stuck\"
	string10_4:
		.asciz \"Yo mama's twice the man you are\"
	
	string11_0:
		.asciz \"Kotomitsuki\"
	string11_1:
		.asciz \"Okazaki Aichi\"
	string11_2:
		.asciz \"Du hast einen Vogel!\"
	string11_3:
		.asciz \"Halt's Maul!\"
	string11_4:
		.asciz \"Grrrrrrrrr\"
	
	string12_0:
		.asciz \"Kaiho\"
	string12_1:
		.asciz \"Aomori\"
	string12_2:
		.asciz \"You are a fool to defy me!\"
	string12_3:
		.asciz \"Big? Ha! My rice bowl is bigger than you!\"
	string12_4:
		.asciz \"I've seen jack-o-lanterns with more teeth than you're gonna have\"
	
	string13_0:  | hairy feller :)
		.asciz \"Buyuzan\"
	string13_1:
		.asciz \"Aichi\"
	string13_2:
		.asciz \"I'm gonna knock you out!\"
	string13_3:
		.asciz \"Bah!\"
	string13_4:
		.asciz \"The undertaker wants to know what size coffin you'll be wanting\"

	string14_0:
		.asciz \"Tochisakae\"
	string14_1:
		.asciz \"Saga\"
	string14_2:
		.asciz \"I can't wait to touch your half naked body...\"
	string14_3:
		.asciz \"Oooooh, sexy beast!\"
	string14_4:
		.asciz \"I'll smack your butt and ride the waves!\"
	
	string15_0:
		.asciz \"Aminishiki\"
	string15_1:
		.asciz \"Aomori\"
	string15_2:
		.asciz \"I will fight as if my pants are on fire!\"
	string15_3:
		.asciz \"Are those boobs real?\"
	string15_4:
		.asciz \"Shut your Pie-Hole!\"
");
#elif defined(FRENCH)

#elif defined(GERMAN)

#endif

// Very handy little thingymabopper to retrieve the address of a string stored in the above optimized 
// assembly format (storing strings like above can shave off hundreds of bytes!!!)
/*unsigned char * __attribute__((__regparm__)) Char_String_Addr(unsigned short Index asm("%d0"));
asm("
.data
.globl Char_String_Addr
.even

| Returns the address of strings[Index] in %a0
Char_String_Addr:
	add.w %d0,%d0
	lea strings(%pc),%a1
	move.w 0(%a1,%d0.w),%a0
	adda.l %a1,%a0
	
	rts
");*/

// This function may be called just like any regular C function
// For each assembly array you need to access, declared them in C like:
// extern void *strings
// Otherwise, the C compiler will not acknowledge the strings

// To call the function, you must use &strings as the array// Example call:  STRADDR_MASTER(0, &strings)
// Input:  %d0-%a1  Output:  %a0
unsigned char * __attribute__((__regparm__)) STRADDR_MASTER(unsigned short Index asm("%d0"), 
void *array asm("%a1"));
asm("
.text | Should be better than .data thanks to section reordering
.globl STRADDR_MASTER
.even

| Returns the address of array[Index] in %a0 (not an inline version)
STRADDR_MASTER:
add.w %d0,%d0
movea.w 0(%a1,%d0.w),%a0
adda.l %a1,%a0

rts
");

#if 0  // For Reference
typedef struct {
	char Index;  // if (Index > External_Slot) External Wrestler
	char Strength;
	char Health;  // 41 (min) + Amount of starting Health
	char Stamina;
	char Speed;
	unsigned char Age;  // if (Age == FALSE) disp "?"
	unsigned short Weight; // if (Weight == FALSE) disp "?"
} Character;
#endif

Character Default_Characters[NO_DEFAULT_WRESTLERS] = {
	{
		0, 
		24, 
		29, 
		18, 
		28, 
		20, 
		322, // (146 kg)
	}, {
		1, 
		30, 
		15, 
		26, 
		31, 
		10, 
		351, // (159 kg)
	}, {
		2, 
		28, 
		31, 
		26, 
		31, 
		24, 
		309, // (140 kg)
	}, {
		3, 
		23, 
		20, 
		26, 
		29, 
		28, 
		340, // (154 kg)
	}, {
		4, 
		26, 
		24, 
		13, 
		25, 
		32, 
		386, // (175 kg)
	}, {
		5, 
		28, 
		25, 
		29, 
		13, 
		32, 
		385, // (175 kg)
	}, {
		6, 
		25, 
		16, 
		20, 
		24, 
		29, 
		337, // (153 kg)
	}, {
		7, 
		17, 
		12, 
		14, 
		16, 
		27, 
		340, // (154 kg)
	},
	
	
	
// Sumo Wrestling Sumo Wrestling Sumo Wrestling Sumo Wrestling Sumo Wrestling Sumo Wrestling Sumo Wrestling //
// Sumo Wrestling Sumo Wrestling Sumo Wrestling Sumo Wrestling Sumo Wrestling Sumo Wrestling Sumo Wrestling //
	
	
	
	 {
		8, 
		29, 
		18, 
		24, 
		11, 
		27, 
		334, // (151.5 kg)
	}, {
		9, 
		23, 
		21, 
		20, 
		26, 
		27, 
		401, // (182 kg)
	}, {
		10, 
		14, 
		24, 
		27, 
		22, 
		29, 
		343, // (155.5 kg)
	}, {
		11, 
		28, 
		23, 
		15, 
		28, 
		28, 
		341, // (154.5 kg)
	}, {
		12, 
		23, 
		25, 
		22, 
		16, 
		32, 
		273, // (124 kg)
	}, {
		13, 
		30, 
		31, 
		18, 
		4, 
		31, 
		347, // (157.5 kg)
	}, {
		14, 
		23, 
		30, 
		8, 
		22, 
		31, 
		351, // (159 kg)
	}, {
		15, 
		24, 
		31, 
		16, 
		28, 
		27, 
		292, // (132.5 kg)
	}, 
};

#ifdef ENGLISH
const Finish_Text finish_text[8] = {
	{ PummelThrowLeft, "Thrown" }, //Turned?
	{ BackwardsSlamLeft, "Slammed" }, 
	{ ShovedRightEdge1, "Dropped Out" }, 
	{ Tumble3, "Tossed" }, 
	{ TossBackLeft, "Flipped" }, 
	{ PummelLift1, "Lifted" }, 
	{ PummelLift2, "Squished" }, 
	{ Spinn1, "Ousted" }, 
};
#elif defined(FRENCH)

#elif defined(GERMAN)

#endif


#if NoTeS
// 2nd line works well, right up to ~55 characters :)
// \x0A for Line Break



4 unused:
"After meeting you, I'm now in favor of abortion in cases of incest", 
"My 100 year old grandmother fights better than you!", 
"You couldn't find log base two of 65536 without a calculator", 
"Your age is higher than your IQ!", 


---


""Hooligans

~58 insults total

Smack Talk one-liners insult match
#endif

#endif