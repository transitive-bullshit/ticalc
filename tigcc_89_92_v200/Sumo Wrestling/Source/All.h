// Header File
// Created 5/22/2004; 12:28:31 PM
// Size Optimizations
#include "Structures.h"				// struct typedef's and accompanying Macros
register GlobalVars *g asm("a4");

#include <tigcclib.h>         // Include All Standard TIGCC Header Files
#include "extgraph.h"					// TICT's Graphical Library v2.0 (Beta 4)
#include "Languages.h"				// Definitions for Compiling separate Language versions of Sumo Wrestling
//#include "TileMap.h"					// Julien Richard-Foy's ExtGraph TileMap Engine
#include "Definitions.h"			// All #defines, enums, and prototypes
#include "Globals.h"					// extern declarations of Global Variables seen by all C source files