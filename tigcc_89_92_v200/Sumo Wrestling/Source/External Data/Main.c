// C Source File
// Created 6/16/2004; 3:27:13 PM
#include <tigcclib.h>

#define EXTERNAL_DATA
#define Putshort(a, fp) putc((unsigned char)((a) >> 8), (fp)); putc((unsigned char)((a)), (fp))

enum ring_index {
	Whit, // 0x00, Blank
	//Blac, // 0xFF
	Low1, // Bottom Left of Ring
	Low2, // Bottom Middle (repeated) along width of Ring
	Low3, // Bottom Right of Ring
	Cir1, Cir1M, // Bottom Left (M)
	Cir2, Cir2M, Cir3, Cir4, Cir5, Cir5M, Cir6, Cir7, Cir9, 
	Ci10, Ci10M, Ci11, Ci11M, Ci12, Ci12M, Ci13, Ci13M, 
	Ci14, Ci14M, Ci15, Ci15M, Ci16, Ci16M, Ci17, Ci17M, Ci18, Ci18M, 
	Ci19, Ci19M, Ci20, Ci20M, Ci21, Ci21M, Ci22, Ci22M, Ci23, Ci24, 
	
	Top1, Top2, Top3, Top4, Top5, 
	Pillar, PillarM, 
	Lin1, Lin1M, Lin2, Lin2M, 
	
	// Small Face Fillers which also include part of the Ring
	Eye1, Eye1M, Eye2, Eye2M, Eye3, Eye3M, Eye4, Eye4M, 
	// 'Outside' audience sprites (higher bar)
	Audience1, Audience1M, Audience2, Audience2M, // Black Hair
	Audience3, Audience3M, Audience4, Audience4M, // Light Hair
	Audience5, Audience5M, Audience6, Audience6M, // Bald, No Hair
	Audience7, Audience7M, Audience8, Audience8M, // Frizzy, Dark Gray Hair
	// 'Inside' audience sprites (lower bar)
	Aud1, Aud1M, Aud2, Aud2M, // Dark Gray Hair
	Aud3, Aud3M, Aud4, Aud4M, // Frizzy, Dark Gray Hair (low)
	Aud5, Aud5M, Aud6, Aud6M, // Male, Bald Announcer
	Aud7, Aud7M, Aud8, Aud8M, // Female Announcer
	Aud9, Aud10, // DogMan
	// Misc
	PicLeft, PicRight, // Girl with Camera (not animated)
	// Small Faces
	Face1, Face2, 
	Face3, Face4, 
	// In Middle, under Top of 'roof'-thingy
	Face5, Face6, 
	
	// Placeholders for audience sprites in the Ring_Matrix
	// Before every Match, these will be replaced by random/appropriate Audience 
	// sprites from above, creating a different audience each match
	Outs, Insi, Smal, Smal2, Camera, 
};

#include "Wrestlergfx.h"
#include "Ring2.h"
void __attribute__ ((noreturn)) Error(FILE*);

void Archive(const char *Filename) {
	SYM_ENTRY *SymPtr = NULL;
	HSym hsym = SymFind(SYMSTR(Filename));
	
	SymPtr = DerefSym (hsym);
	
	if (!SymPtr->flags.bits.archived)
		EM_moveSymToExtMem(SYMSTR(Filename), HS_NULL);
}

void UnArchive(const char *Filename) {
	SYM_ENTRY *SymPtr = NULL;
	HSym hsym = SymFind(SYMSTR(Filename));
	
	SymPtr = DerefSym (hsym);
	
	if (SymPtr->flags.bits.archived)
		EM_moveSymFromExtMem(SYMSTR(Filename), HS_NULL);
}

// Main Function
void _main(void)
{
	ST_helpMsg("Saving external data, please wait...");
	UnArchive("sumogfx");
	FILE *fp = fopen("sumogfx", "wb");
	if (!fp)
		Error(NULL);
	
	/* Write all Graphics Data into the 'sumodat' File */
	//#define Write(x)	printf("%lu\n", sizeof(x)); ngetchx();
	
	#define Write(x) ({ Putshort(sizeof((x)), fp); if (fwrite((x), sizeof((x)), 1, fp) != 1) {Error(fp);} })
	Write(stomp1);
	Write(stomp2);
	Write(stomp3);
	
	Write(salt1);
	Write(salt3);
	
	Write(kneel1);
	Write(kneel3);
	Write(kneel5);
	
	Write(tackle1);
	Write(tackle3);
	
	Write(walk1);
	Write(walk3);
	
	Write(charge1);
	Write(charge3);
	
	Write(tumble1);
	Write(tumble2);
	Write(tumble3);
	Write(tumble4);
	
	Write(spinn1);
	Write(spinn3);
	
	Write(tossbackleft);
	
	Write(pummel1);
	Write(pummelpush1);
	
	Write(pummellift1);
	Write(pummellift3);
	Write(pummellift5);
	
	Write(earthquake_throwleft);
	
	Write(shoveleft);
	Write(shovedright);
	Write(shovedleftedge1);
	Write(shovedleftedge2);
	
	Write(pummelthrowleft);
	Write(throwleft);
	Write(thrownright);
	Write(pummelthrowcounter1);
	
	Write(punchleft1);
	Write(punchleft2);
	Write(punchedleft1);
	Write(punchedleft2);
	
	Write(tauntleft1);
	Write(tauntleft2);
	
	Write(laughing1);
	Write(laughing2);
	
	Write(sumoeating1);
	Write(sumoeating2);
	
	Write(sweepingleft);
	Write(smushedleft);
	Write(bellyflopleft1);
	Write(bellyflopleft2);
	Write(backwardsslamleft1);
	Write(backwardsslamleft2);
	Write(slammed);
	Write(Ring_Matrix);
	Write(ring_sprites);
	
	// Put the extension "gfx" onto the Graphics File and close it
	fputc(0, fp);
	fputs("gfx", fp);
	fputc(0, fp);
	fputc(OTH_TAG, fp);
	fclose(fp);
	Archive("sumogfx");  // Archive the Configuration File
	ST_helpMsg("External Data File 'sumogfx' created");
}

void __attribute__ ((noreturn)) Error(FILE *fp) {
	if (fp != NULL)
		fclose(fp);
	
	ST_helpMsg("Error creating 'sumogfx' File");
	exit(1);
}