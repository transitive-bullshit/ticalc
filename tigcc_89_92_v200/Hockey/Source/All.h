// C Header File
// Created 9/7/2003; 12:07:45 PM

/*#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization
#define MIN_AMS 101           // Compile for AMS 1.01 or higher
#define ENABLE_ERROR_RETURN
#define COMPRESSED_FORMAT_RELOCS  // There was a spelling mistake in this for v0.62!!
// This was pointed out by Kevin Kofler.  Thanks :)
#define MERGE_BSS*/  // All defined in the Project Options now
#include "Structures.h"
register Game *g asm("a4");

#include <tigcclib.h>         // Include All Header Files
#include "extgraph.h"
#include "Definitions.h"
#include "Globals.h"