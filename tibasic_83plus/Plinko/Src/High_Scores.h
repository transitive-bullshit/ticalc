// C Header File
// Created 7/11/2003; 9:24:08 PM

//----------------------------------------
// High scores Functions
//----------------------------------------

static inline int CHECK_FOR_HIGH(int Total) {
	int loop, NEW_HIGH = -1;
	char new_name[15];
	SCORE board[HIGH_MAX];
	
	if (HIGH_SCORES(&board[0]) == QUIT)
		return QUIT;
	
	for(loop = 0; loop < HIGH_MAX ; loop++)  // Check if new Total is greater than any of the high scores
		if (board[loop].score <= Total && NEW_HIGH == -1)
			NEW_HIGH = loop;  // We have a new high score!!
	
	if (NEW_HIGH == -1)  // No new high score
		return 0;
	
	for(loop = HIGH_MAX - 1; loop > NEW_HIGH; loop --) {
		
		board[loop].score = board[loop - 1].score;
		board[loop].name[0] = 0;
		strcpy(board[loop].name, board[loop - 1].name);
		
	}
	
	board[NEW_HIGH].score = Total;
	board[NEW_HIGH].name[0] = 0;  // Terminates the string
	
	if (Getname(new_name) == QUIT)  // Ask the user for their name (max 14 characters)
		return QUIT;
	
	///////////////////////////////////////////////  See me in a week :)
	strcpy(board[NEW_HIGH].name, new_name);  // :)
	///////////////////////////////////////////////  Make it a nice Function
	
	if (HIGH_WRITE(&board[0]) == QUIT)  // Writes the new High score data into the "PLINKSAVE" file
		return QUIT;  // You should always check for errors!!
	
	DISPLAY_SCORES();  // displays the high score table
	
	return 0;
}

static inline int HIGH_WRITE(SCORE *s) {
	FILE *f = NULL;
	
	if ((f = fopen("PLNKSAVE","wb")) == NULL)
		return -1;
	if (fwrite(s, sizeof(SCORE), HIGH_MAX, f) != HIGH_MAX)
		return -1;
	
	fputc(0, f);
	fputs("BAH", f);
	fputc(0, f);
	fputc(OTH_TAG, f);
	
	// close the file
	fclose(f);
	
	return 0;
}

int HIGH_SCORES(SCORE *s) {
	FILE *f = NULL;
	
	if ((f = fopen("PLNKSAVE","rb")) == NULL)
		return -1;
	
	if (fread(s, sizeof(SCORE), HIGH_MAX, f) != HIGH_MAX)
		return -1;
	
	// close the file
	fclose(f);
	
	return 0;
}

static inline int makefile(void) {
	FILE *f = NULL;
	unsigned char loop;
	const char * const custom_names[HIGH_MAX] = {"Roger Moore", "Sean Connery", "Pierce Brosnan", 
	"Timothy Dalton", "George Lazenby"};
	const int scores[5] = {20000, 15000, 10000, 8000, 5000};
	SCORE s;  // Temporary structure that will hold one of my pre-made highscore names and accompanying score
	
	// if we cannot open the file, we must need to create it
	if ((f = fopen("PLNKSAVE","rb")) == NULL) {
		
		if ((f = fopen("PLNKSAVE","wb")) == NULL)  // open the file for writing
			return -1;
		
		for(loop = 0; loop < HIGH_MAX; loop++) {
			
			s.name[0] = 0;
			sprintf(s.name, custom_names[loop]);
			s.score = scores[loop];
			
			if (fwrite(&s, sizeof(SCORE), 1, f) != 1)
				return -1;
		}
		
		fputc(0, f);
		fputs("BAH", f);
		fputc(0, f);
		fputc(OTH_TAG, f);
	}
	
	fclose(f);
	return 0;  // no errors, so we return 0
}

static inline int Getname(char *new_name) {
	
	void *kbq = kbd_queue();
	short input, pos = 0, STATUS = RUNNING;
	memcpy(new_name, 0, sizeof(char) * 15);  // Clears the new_name string, (optional?)
	
	GraySetInt1Handler(interrupt1);
	FastCopyScreen(GrayDBufGetActivePlane (DARK_PLANE), virtual_light);
	
	Display_name(42, new_name);
	DrawGrayStrExt2B(0, 33, (char*)"Press Enter when done", A_NORMAL|A_SHADOWED|A_CENTERED, F_6x8, 
		Hiddenl, Hiddend);
	GrayDBufToggleSync(); // switches two sets of planes
	
	while (STATUS == RUNNING) {  // Loop until the user presses 2nd or HOME
		
		while (OSdequeue(&input, kbq)) continue;
		input = input & 0xf7ff;
		
		if (input == KEY_BACKSPACE && pos > 0)
				new_name[--pos] = 0;
		else if (isprint(input) && pos < 14) {
			new_name[pos++] = input;
			new_name[pos] = 0;
			//for(wait = 0; wait < 90000; wait++);
			DELAY(18);
		}
		
		Display_name(42, new_name);  // Displays the name here.
		DrawGrayStrExt2B(0, 33, (char*)"Press Enter when done", A_NORMAL|A_SHADOWED|A_CENTERED, F_6x8, 
			Hiddenl, Hiddend);
		GrayDBufToggleSync(); // switches two sets of planes
		
		if (_keytest (RR_ESC)) STATUS = EXIT;
		if (_keytest (RR_ENTER) && pos > 0) STATUS = DROP;
	}
	
	GraySetInt1Handler(DUMMY_HANDLER);
	
	if (STATUS == EXIT)
		return QUIT;
	
	return 0;
}

void Display_name(unsigned char y, char *name) {
	unsigned char width;
	memset(Hiddend, 0, LCD_SIZE);
	FastCopyScreen(virtual_light, Hiddenl);
	
	width = DrawStrWidth(name, F_6x8);
	
	DRAW_BOX(35, y, 8, 83);
	
	if (!width) {// If there is no name(no width), the user hasn't typed anything yet
		DrawGrayStrExt2B(0, y + 4, (char*)"Enter Name", A_NORMAL|A_SHADOWED|A_CENTERED, F_6x8, 
		Hiddenl, Hiddend);
		return;
	}
	
	DrawGrayStrExt2B(0, y + 4, (char*)name, A_NORMAL|A_SHADOWED|A_CENTERED, F_6x8, Hiddenl, Hiddend);
}

int DISPLAY_SCORES(void) {
	SCORE board[HIGH_MAX];
	unsigned char name[15], a, x, y;
	unsigned int score;
	
	if (HIGH_SCORES(&board[0]) == QUIT)  // Reads the high score file and puts the data read into the 
	// board array, which is an array of 5 SCORE structures.
		return QUIT;
	
	ClearGrayScreen2B(Hiddenl, Hiddend);
	
	GrayDBufSetHiddenAMSPlane (LIGHT_PLANE);
		BitmapPut(32, 0, &HIGH, &(SCR_RECT){{0,0,159,99}}, A_NORMAL);
	GrayDBufSetHiddenAMSPlane (DARK_PLANE);
		BitmapPut(32, 0, &HIGH, &(SCR_RECT){{0,0,159,99}}, A_NORMAL);
	
	for(a = 0; a < HIGH_MAX; a++) {
		name[0] = 0;
		score = board[a].score;
		
		x = 113 + 6 * (score < 10000) + 6 * (score < 1000) + 6 * (score < 100)  + 6 * (score < 10);
		y = 20 + 16 * a;
		
		// Puts a box around each name/high score
		DRAW_BOX(15, y, 8, 124);
		
		// Displays the name here.
		DrawGrayStrExt2B(19, y + 4, (char*)board[a].name, A_NORMAL|A_SHADOWED, F_6x8, Hiddenl, Hiddend);
		// Copies the score into the temporary name string
		sprintf(name, "%d", score);
		// Puts the score string right aligned inside the box
		DrawGrayStrExt2B(x, y + 4, (char*)name, A_NORMAL|A_SHADOWED, F_6x8, Hiddenl, Hiddend);
	}
	GrayDBufToggleSync(); // switches two sets of planes
	
	if (wait_for_keypress() == EXIT)
		return QUIT;
	
	return 0;
}