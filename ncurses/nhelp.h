/*
    
    A_NORMAL        Normal display (no highlight)
    A_STANDOUT      Best highlighting mode of the terminal.
    A_UNDERLINE     Underlining
    A_REVERSE       Reverse video
    A_BLINK         Blinking
    A_DIM           Half bright
    A_BOLD          Extra bright or bold
    A_PROTECT       Protected mode
    A_INVIS         Invisible or blank mode
    A_ALTCHARSET    Alternate character set
    A_CHARTEXT      Bit-mask to extract a character
    COLOR_PAIR(n)   Color-pair number n 
    

	init_pair(n, COLOR_fore, COLOR_back);
	COLOR_BLACK   0
        COLOR_RED     1
        COLOR_GREEN   2
        COLOR_YELLOW  3
        COLOR_BLUE    4
        COLOR_MAGENTA 5
        COLOR_CYAN    6
        COLOR_WHITE   7
	
*/

#ifndef __NHELP_H
#define __NHELP_H

#include <ncurses.h>
#include <stdlib.h>

#define colorPairId(f, b) (1+(8*f)+b)
//#define colorText colorPairId(COLOR_WHITE, COLOR_BLACK)
#define colorText 50

int defaultColorPairId(int b) {
	switch(b) {
		default:
		case 0:
			return colorPairId(COLOR_WHITE, b);
		case 1:
			return colorPairId(COLOR_BLACK, b);
		case 2:
			return colorPairId(COLOR_BLACK, b);
		case 3:
			return colorPairId(COLOR_BLACK, b);
		case 4:
			return colorPairId(COLOR_WHITE, b);
		case 5:
			return colorPairId(COLOR_BLACK, b);
		case 6:
			return colorPairId(COLOR_WHITE, b);
		case 7:
			return colorPairId(COLOR_BLACK, b);
	}
}

void n_engage() {
	initscr();
	//resizeterm(30, 80);
	
	if(has_colors() == FALSE)
	{	
		endwin();
		printf("Your terminal does not support color. Please do better.\n");
		exit(1);
	}
	
	if (can_change_color() == FALSE) {
		//endwin();
		//printf("Your terminal does not support changing color. Please do better.\n");
		//exit(1);
	}
	
	start_color();
	int f,b;
	for (f = 0; f < 8; f++)
	for (b = 0; b < 8; b++)
		init_pair(colorPairId(f, b), f, b);

	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	mmask_t old;
	mousemask(BUTTON1_CLICKED, &old); 
}

WINDOW* n_window(int height, int width, int starty, int startx) {
	WINDOW* nwin = newwin(height, width, starty, startx);
	keypad(nwin, TRUE);
	return nwin;
}

void n_disengage() {
	endwin();
}

void n_pause() {
	def_prog_mode();
	endwin();
}

void n_resume() {
	reset_prog_mode();
}

#endif
