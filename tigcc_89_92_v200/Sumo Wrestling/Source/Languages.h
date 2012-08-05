// Header File
// Created 2/6/2005; 1:02:21 PM
/* ---------------------------------------------------------- */
/* Defines what language version of Sumo Wrestling to compile */
/* ---------------------------------------------------------- */
#define NUMBER_LANGUAGES_AVAILABLE	(1)

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

#if NUMBER_LANGUAGES_AVAILABLE == 1
	// Currently, only English is available, but I want all of the macros and #ifdef's in place to 
	// make the job easier for potential translators
	#define EFG(English) (English)
#else
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
#endif