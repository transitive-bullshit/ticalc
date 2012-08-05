/******************************************************************************
*
* file name:       c_strings.c
* version:         1.0
* initial date:    8/12/2004
* authors:         phishfud@elknet.net (Fisch2)
* contributors:    TICT (Thomas Nussbaumer/Lionel Debroux)
* description:     Converts an array of strings into assembly format
*
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
static inline void CleanUp(void);

unsigned char **strings;
unsigned char *arrayname;
unsigned char str_macro[255];
unsigned short MAX_STRINGS;
FILE *in, *out;

void PrintUsage() {
    printf("\nUsage: c_strings [-i|-f|-f2] <infile> <outfile> (array_name)\n"\
           "       -i         ... include inline version of function to access strings\n"\
           "       -f         ... include regular function to access the strings\n"\
           "       -f2        ... include master function to access the strings, which\n"\
           "                      takes the string to access as an argument\n"\
           "       infile     ... file including C strings to convert\n"\
           "       outfile    ... output filename (use - as name for output to stdout)\n"\
           "       array_name ... optional name of array to generate\n\n"\
           "       Converts a C string array into its optimized assembly\n"\
           "       form including the necessary functions to access them.\n\n"\
           "NOTE:  If you only need to access the strings a few times, using an\n"\
           "       inline version of the function to access them will be faster\n"\
           "       and possibly smaller, but will also be harder to use.\n"\
           "NOTE:  Each string Must be surrounded by quotes.\n\n"\
           "Ex.    c_strings -f in.txt out.txt\n\n");
}

//=============================================================================
// converts given string into uppercase
//=============================================================================
static inline void ToUppercase(char* str) {
    while (*str) {
        *str = (char)toupper(*str);
        str++;
    }
}

//=============================================================================
// where it all starts...
//=============================================================================
int main(int argc, char* argv[])
{
  short Inline;
  
  // Process paramaters
  if (argc == 5) arrayname = argv[4];
  else if (argc == 4) arrayname = "strings";
  else {
  	if (argc != 1)
  		printf("ERROR: invalid number of parameters\n");
  	PrintUsage();
  	return 1;
  }
  
  //-------------------------------------------------------------------------
  // parse inline flag
  //-------------------------------------------------------------------------
  if (!strcmp(argv[1], "-i"))           Inline = 1;
  else if (!strcmp(argv[1], "-f"))      Inline = 0;
  else if (!strcmp(argv[1], "-f2"))     Inline = 2;
  else {
  	fprintf(stderr, "ERROR: neither -i nor -f specified\n");
  	PrintUsage();
    return 1;
  }
  
  if ((in = fopen(argv[2], "r")) == NULL) {
    printf("Error opening file %s", argv[2]);
    PrintUsage();
    return 1;
  }
	if (!strcmp(argv[3],"-")) {
		out = stdout;
	} else if ((out = fopen(argv[3], "w")) == NULL) {
    fclose(in);
    printf("Error opening file %s", argv[3]);
    PrintUsage();
    return 1;
  }
  
  short n = 0, a, c, No_strings = 0, str = 1;
  MAX_STRINGS = 0;
  while((c = fgetc(in)) != EOF) {
  	if (c == '"' && (str = !str))
  		MAX_STRINGS++;
  }
  // reset input file's pointer to the beginning of the file
  rewind(in);
  
  if ((strings = (unsigned char**)malloc(MAX_STRINGS * sizeof(unsigned char*))) == NULL) {
  	CleanUp();
		
		printf("Error memory");
		PrintUsage();
		return 1;
  }
  for(a = MAX_STRINGS; a--;)
  	strings[a] = NULL;
  
  // Move through input file, recording all strings found
  do {
		while((c = fgetc(in)) != '"') {
			if (c == EOF)
				goto done;
		}
		
		a = 0;
		long Original_pos = ftell(in);
		while ((c = fgetc(in)) != '"') {
			if (c == EOF)
				goto done;
			
			a++;
		}
		
		if ((strings[n] = malloc(a + 1)) == NULL) {
			CleanUp();
			
			printf("Error memory");
			PrintUsage();
			return 1;
		}
		if (fseek(in, Original_pos, SEEK_SET) != 0) {
			CleanUp();
			
			printf("Error reading file %s", argv[2]);
			PrintUsage();
			return 1;
		}
		
		a = 0;
		while ((c = fgetc(in)) != '"') {
			if (c == EOF)
				goto done;
			
			strings[n][a++] = c;
		}
		
		strings[n][a] = '\0';
		n++;
  } while(1);
  
  done:
  
  // Print header information at top of output file
  fprintf(out, "// Number of strings in array\n#define NO_STRINGS  (%d)\n\n", n);
  strcpy(str_macro, "STRADDR_");
  
  if (Inline == 2)
  	strcat(str_macro, "master");
  else strcat(str_macro, arrayname);
  //sprintf(str_macro,"STRADDR_%s", (Inline == 2 ? "master" : arrayname));
  ToUppercase(str_macro);
  //fprintf(out, "// Returns address of a %s[Index]", arrayname);
  //fprintf(out, "#define %s ()\n\n", str_macro);
  static const char quote[] = "\x22";
  if (Inline == 0) {
	  //---------------------------------------------------------------------------
	  // print non-inline function to access strings
	  //---------------------------------------------------------------------------
	  fprintf(out, 
	  "// This function may be called just like any regular C function\n"\
	  "// Input:  %%d0  Output:  %%a0\n"\
	  "unsigned char * __attribute__((__regparm__)) %s(unsigned short Index asm(%s%%d0%s));\n", 
	  	str_macro, quote, quote);
	  fprintf(out, "asm(%s\n"\
	  ".data\n"\
		".globl %s""\n"\
		".even\n"\
		"\n"\
		"| Returns the address of %s[Index] in %%a0 (not an inline version)\n"\
	  "%s"":\n"\
			"add.w %%d0,%%d0\n"\
			"lea %s(%%pc),%%a1\n"\
			"move.w 0(%%a1,%%d0.w),%%a0\n"\
			"adda.l %%a1,%%a0\n"\
			"\n"\
			"rts\n"\
		"%s);", quote, str_macro, arrayname, str_macro, arrayname, quote);
  } else if (Inline == 1) {
  	//---------------------------------------------------------------------------
  	// print inline version of function to access strings
  	//---------------------------------------------------------------------------
  	static const char dpercent[] = "\x25\x25";
		
		fprintf(out, 
		"// Inline version of function to access strings:\n"\
		"// str_address is the (char*) address the accessed string will be stored to\n"\
		"// str_address may then be used to manipulate/draw the string just like regular C strings\n"\
		"// Index is the number of the string you want to access (%s[Index])\n", arrayname);
		fprintf(out, 
		"asm(%s\n"\
		"move.w %%1,%sd0\n"\
		"add.w %sd0,%sd0\n"\
		"lea %s(%spc),%sa0\n"\
		"move.w 0(%sa0,%sd0.w),%%0\n"\
		"adda.l %sa0,%%0%s\n"\
		": %s=a%s(str_address) // output\n"\
		": %sg%s(Index)        // input\n"\
		": %sd0%s,%sa0%s         // clobbered registers\n"\
		");", quote, dpercent, dpercent, dpercent, arrayname, dpercent, dpercent, dpercent, 
          dpercent, dpercent, quote, quote, quote, quote, quote, quote, quote, 
          quote, quote);
  } else if (Inline == 2) {
  	//---------------------------------------------------------------------------
	  // print non-inline master function to access multiple assembly string arrays
	  //---------------------------------------------------------------------------
	  fprintf(out, 
	  "// This function may be called just like any regular C function\n"\
	  "// For each assembly array you need to access, declared them in C like:\n"\
	  "// extern void *%s\n"\
	  "// Otherwise, the C compiler will not acknowledge the strings\n\n"\
	  "// To call the function, you must use &%s as the array\n"\
	  "// Example call:  %s(0, &%s)\n"\
	  "// Input:  %%d0-%%a1  Output:  %%a0\n"\
	  "unsigned char * __attribute__((__regparm__)) %s(unsigned short Index asm(%s%%d0%s), \n"\
		"void *array asm(%s%%a1%s));\n", arrayname, arrayname, str_macro, arrayname, str_macro, quote, 
			quote, quote, quote);
	  
	  fprintf(out, "asm(%s\n"\
	  ".text | Should be better than .data thanks to section reordering\n"\
		".globl %s""\n"\
		".even\n"\
		"\n"\
		"| Returns the address of array[Index] in %%a0 (not an inline version)\n"\
	  "%s"":\n"\
			"add.w %%d0,%%d0\n"\
			"movea.w 0(%%a1,%%d0.w),%%a0\n"\
			"adda.l %%a1,%%a0\n"\
			"\n"\
			"rts\n"\
		"%s);", quote, str_macro, str_macro, quote);
  }
  
  fprintf(out, "\n\n");
  fprintf(out, "asm(\x22\n");
  fprintf(out, "%s:\n", arrayname);
  
  No_strings = 0;
  // Print offsets in assembly of each string from the start of the string array
  for(a = 0; a < n; a++) {
  	short str_no = a;
  	for(c = 0; c < a; c++) {
  		if (!strcmp(strings[a], strings[c])) {
  			str_no = c;
  			break;
  		}
  	}
  	if (str_no == a)
  		str_no = No_strings++;
  	
  	fprintf(out, ".word %s%-4d- %s\n", arrayname, str_no, arrayname);
  }
  
  fprintf(out, "\n");
  
  No_strings = 0;
  // Print each string in assembly format
  for(a = 0; a < n; a++) {
  	short str_no = a;
  	for(c = 0; c < a; c++) {
  		if (!strcmp(strings[a], strings[c])) {
  			str_no = c;
  			break;
  		}
  	}
  	
  	const char format[] = "\x5C\x22";
  	// Write one string to the file in asm format
  	if (str_no == a)
			fprintf(out, "%s%d:\n.asciz %s%s%s\n", arrayname, No_strings++, format, strings[str_no], format);
  }
  
  fprintf(out, "%s);\n\n", quote);
  
  fprintf(out, 
  "/*       c_strings v1.0       */\n"\
  "// By:  Travis Fischer (Fisch)\n"\
  "/*       c_strings v1.0       */");
  
  CleanUp();
  printf("\n\nConversion successful!\n"\
  			 "Number of C strings   = %d\n"\
  			 "Number of asm strings = %d\n\n"\
  			 "By:  Travis Fischer (Fisch2)\n", n, No_strings);
  
  return 0;
}

//=============================================================================
// Close files and free allocated memory
//=============================================================================
static inline void CleanUp(void) {
	fclose(in);
	fclose(out);
	
	unsigned short a;
	if (strings != NULL) {
		for(a = 0; a < MAX_STRINGS; a++) {
			if (strings[a] == NULL)
				break;
			free(strings[a]);
		}
		
		free(strings);
	}
}
