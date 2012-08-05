// C Header File
// Created 6/26/2003; 2:22:17 PM

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization
#define MERGE_BSS
#define MIN_AMS 101           // Compile for AMS 1.01 or higher
//#define MIN_AMS 200           // Compile for AMS 2.00 or higher
#define NO_EXIT_SUPPORT				// The program has no need for exit support

enum Status {RUNNING, EXIT, DROP};
enum Colors {LIGHT, DARK, BLACK};

typedef struct {
	char name[15];
	int score;
} SCORE;

#define pxltst(x, y)  (( !((x-3)%14) && !((y-4)%20) )||( !((x-10)%14) && !((y-14)%20) ))
#define Waitkey() { while(!_rowread(0)); while(_keytest (RR_2ND)||_keytest (RR_DOWN)); }
#define MAX_PRODUCTS  25
#define HIGH_MAX  5
#define QUIT -1

// Beginning of Function Prototypes
unsigned char wait_for_keypress(void);
void DRAWALL(unsigned char, unsigned char);
static inline signed char INTRO(void);
void FADE_DARKER(void);
void FADE_LIGHTER(void);
void DELAY(short);
void GRAPHIC(short, char, unsigned char, int);
void DRAW_BOX(short, char, unsigned char, unsigned char);
void LINE(short, char, short, char, char, short);
static inline unsigned int DROPPING(unsigned char *); 
static inline char main_game(unsigned char);
static inline void new_chip(unsigned char, unsigned char);
static inline void MOVE_LEFT(unsigned char *);
static inline void MOVE_RIGHT(unsigned char *);
static inline void SHUFFLE(unsigned char);
static inline void PRINT_TOTAL(unsigned int);
static inline void GET_PRODUCT(unsigned char*, unsigned char*, char*);
static inline int PRODUCTS(void);
static inline void UPDATE_SCREEN(unsigned char, char*, char*);
void SHOW_PLINKOS(char, signed char, char*, char*);
void DrawString(short, char*);
static inline int MENU(void);
static inline void DRAW_MENU(int choice);
static inline void info(void);

static inline int Getname(char*);			// Gets a new high score user's name
void Display_name(unsigned char y, char *name);
static inline int CHECK_FOR_HIGH(int);  // Checks for a new high score
static inline int HIGH_WRITE(SCORE *);  // Writes 5 SCORE structures into the "PLINKSAVE" file
int HIGH_SCORES(SCORE *); // Reads 5 SORES structures and returns the data via pointer
static inline int makefile(void);				// Makes a "PLINKSAVE" file if it doesn't already exit
int DISPLAY_SCORES(void);

static inline int Gray_prep(void);
static inline void Cleanup(void);