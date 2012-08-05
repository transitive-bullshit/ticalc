// Header File
// Created 12/14/2003; 12:03:09 PM
#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization
#define MIN_AMS 101           // Compile for AMS 1.01 or higher
// Size Optimizations
#define COMPRESSED_FORMAT_RELOCS  // Was buggy with Tigcc v0.95 Beta 4!
// If you are using v0.95 Beta 4 (and below), the compressed format relocs 
// has been known to produce bad code.  It you really don't want to update 
// to a newer version of TIGCC (or are unable to), then I suggest you 
// comment out the above #define.
#define MERGE_BSS

#include <tigcclib.h>         // Include All Header Files
#include "extgraph.h"					// TICT's Graphical Library v2.0 (Beta)
#include "Languages.h"				// English and French Definitions for Compiling separate Language versions
#include "Structures.h"				// struct typedef's and accompanying Macros
#include "Globals.h"					// extern declarations of Global Variables
#include "Definitions.h"			// All #defines, enums, and prototypes