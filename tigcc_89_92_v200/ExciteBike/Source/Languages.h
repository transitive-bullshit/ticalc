// Header File
// Created 5/7/2004; 5:16:59 PM

/*  Compile English Version  */
/* ------------------------- */
				#define ENGLISH
/* ------------------------- */

/*  Compile French Version   */
/* ------------------------- */
//				#define FRENCH
/* ------------------------- */

/*  Compile German Version   */
/* ------------------------- */
//				#define GERMAN
/* ------------------------- */


// Can only compile one language version at a time
#if (((defined(ENGLISH) && defined(FRENCH)) || (defined(ENGLISH) && defined(GERMAN)) || \
(defined FRENCH) && defined(GERMAN)))
	#error "Multiple languages defined (Languages.h)"
#elif defined(ENGLISH) // Default is English language
	#define EFG(English, French, German) (English)  // Very handy Macro!
#elif defined(FRENCH)
	#define EFG(English, French, German) (French)
#elif defined(GERMAN)
	#define EFG(English, French, German) (German)
#else
	#error "Must define a Language (Languages.h)"
#endif

// http://gilou82.free.fr/Vrac/ebsource.zip (French version by 240-185)