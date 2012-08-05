// C Source File
// Created 3/4/2004; 3:43:43 PM
#include <tigcclib.h>
#include "Tracks.h"


/* ---- !!!!!! ---- !!!!!! ---- !!!!!! ---- */
// WARNING:  If you send this prog to your 
// calc, make sure it is UNarchived before 
// running the program, or it will NOT work 
// and will probably cause your calc to 
// crash.
/* ---- !!!!!! ---- !!!!!! ---- !!!!!! ---- */


// Function Prototypes to make the Compiler happy :)
short CompressRLE(unsigned char*, unsigned short);
unsigned short *Get_Data_Ptr(const char*, short);
static inline void Archive(const char *);
static inline void UnArchive(const char*);
void __attribute__ ((noreturn)) Error(FILE*);

#define Getshort(fp)	((getc((fp)) << 8) + getc((fp)))
#define Putshort(a, fp) putc((unsigned char)((a) >> 8), (fp)); putc((unsigned char)((a)), (fp))
//#define CHECK_FILE

// Main Function
void _main(void)
{
	unsigned short a, Sizes[5], Offsets[5] = { 10, 0, 0, 0, 0 };
	// Prepare and open the 'excitrck' File
	ST_helpMsg("Please wait... Saving external tracks");
	UnArchive("excitrck");
	
	#ifndef CHECK_FILE
	FILE *fp = fopen("excitrck", "wb");
	if (fp == NULL)
		Error(NULL);
	
	#else  // Check (skim through) the previously created File's contents
	
	FILE *fp = fopen("excitrck", "rb");
	if (fp == NULL)
		Error(NULL);
	
	clrscr();
	for(a = 5; a--;)
		printf("%d ", Getshort(fp));
	ngetchx();
	
	while(1) {
		clrscr();
		for(a = 0; a < 10; a++)
			printf("%d ", getc(fp));
		ngetchx();
	}
	#endif
	
	
	/* RLE Compress each Track and Write the corresponding offsets of each Track to the File */
	Sizes[0] = CompressRLE(Track1, sizeof(Track1));
	Sizes[1] = CompressRLE(Track2, sizeof(Track2));
	Sizes[2] = CompressRLE(Track3, sizeof(Track3));
	Sizes[3] = CompressRLE(Track4, sizeof(Track4));
	Sizes[4] = CompressRLE(Track5, sizeof(Track5));
	for(a = 1; a < 5; a++)
		Offsets[a] = Sizes[a - 1] + Offsets[a - 1] + 2;
	if (fwrite(Offsets, sizeof(Offsets), 1, fp) != 1)
		Error(fp);
	
	/* Write all compressed Track Data into the 'excitrck' File */
	Putshort((short)sizeof(Track1), fp);
	if (fwrite(Track1, Sizes[0], 1, fp) != 1)
		Error(fp);
	Putshort((short)sizeof(Track2), fp);
	if (fwrite(Track2, Sizes[1], 1, fp) != 1)
		Error(fp);
	Putshort((short)sizeof(Track3), fp);
	if (fwrite(Track3, Sizes[2], 1, fp) != 1)
		Error(fp);
	Putshort((short)sizeof(Track4), fp);
	if (fwrite(Track4, Sizes[3], 1, fp) != 1)
		Error(fp);
	Putshort((short)sizeof(Track5), fp);
	if (fwrite(Track5, Sizes[4], 1, fp) != 1)
		Error(fp);
	
	// Put the extension "trck" onto the File and close it
	fputc(0, fp);
	fputs("trck", fp);
	fputc(0, fp);
	fputc(OTH_TAG, fp);
	fclose(fp);
	Archive("excitrck");  // Archive the File
	
	GKeyFlush();
	ST_helpMsg("Default Tracks saved successfully");
}

short CompressRLE(unsigned char *Buf, unsigned short Size) {
	register unsigned char *Ptr   = Buf;
	const unsigned char *Original = Buf;
	register unsigned short a;
	unsigned char Val;
	
	while (Size > 0) {
		a = 1;
		Val = *Ptr;
		while(Size - a > 0 && Val == *(Ptr + a)) a++;
		if (a > 3) {  // We have a run to compress (this run looks like { x, x, x, x... })
			*Buf++ = 0x91;  // Flag
			*Buf++ = Val;   // Value being repeated
			*Buf++ = a;     // Number of Repititions
			Ptr   += a;
			Size  -= a;
		} else {
			a = 1;
			while(Size - a > 0 && Val + a == *(Ptr + a)) a++;
			if (a > 3) {  // We have a secondary run to compress (this run looks like { x, x+1, x+2, x+3... })
				*Buf++ = 0x92;  // Second Flag
				*Buf++ = Val;   // Value being repeated
				*Buf++ = a;     // Number of Repititions
				Ptr   += a;
				Size  -= a;
			} else {
				*Buf++ = Val;
				Ptr   += 1;
				Size  -= 1;
			}
		}
	}
	
	a = (Buf - Original);
	return a + (a & 1);  // New size of Compressed Buffer (make sure it is an Even number)
}

unsigned short *Get_Data_Ptr(const char *Filename, short Offset) {
	unsigned short *Location;
	SYM_ENTRY *SymPtr;
	
	if ((SymPtr = SymFindPtr(SYMSTR(Filename), 0)) == NULL)
		return NULL;  // File not found
	
	if ((Location = (unsigned short*) HeapDeref(SymPtr->handle)) == NULL)
		return NULL;
	return (Location + 2 + Offset);
}

static inline void Archive(const char *Filename) {
	SYM_ENTRY *SymPtr = NULL;
	HSym hsym = SymFind(SYMSTR(Filename));
	
	SymPtr = DerefSym (hsym);
	
	if (EM_findEmptySlot(*Get_Data_Ptr(Filename, -2)) == NULL)
		return;  // Make sure Garbage Collection will not occur
	if (!SymPtr->flags.bits.archived)
		EM_moveSymToExtMem(SYMSTR(Filename), HS_NULL);
}

static inline void UnArchive(const char *Filename) {
	SYM_ENTRY *SymPtr = NULL;
	HSym hsym = SymFind(SYMSTR(Filename));
	
	SymPtr = DerefSym (hsym);
	
	if (SymPtr->flags.bits.archived)
		EM_moveSymFromExtMem(SYMSTR(Filename), HS_NULL);
}

void __attribute__ ((noreturn)) Error(FILE *fp) {
	if (fp != NULL)
		fclose(fp);
	ST_helpMsg("Error creating 'excitrck' File");
	exit(1);
}