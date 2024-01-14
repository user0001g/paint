#ifdef _WIN32
#include <windows.h>
#endif

#include "initialize.hpp"
#include "colors.hpp"

void ncurses_config() {
#ifdef _WIN32
	{


		HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
		DWORD mode;
		GetConsoleMode(hStdin, &mode);
		SetConsoleMode(hStdin, mode & ~(ENABLE_QUICK_EDIT_MODE | ENABLE_INSERT_MODE));

	}
#endif
}


void initialize_colors() {


	init_pair(BLACK, COLOR_BLACK, COLOR_BLACK);
	init_pair(RED,	COLOR_RED, COLOR_RED);
	init_pair(CYAN, COLOR_CYAN, COLOR_CYAN);
	init_pair(MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(YELLOW, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(GREEN, COLOR_GREEN, COLOR_GREEN);
	init_pair(WHITE, COLOR_WHITE, COLOR_WHITE);
	init_pair(BLUE, COLOR_BLUE, COLOR_BLUE);

	init_pair(CP_WHITE_ON_BLACK, COLOR_WHITE, COLOR_BLACK);
	init_pair(CP_BLACK_ON_RED, COLOR_BLACK, COLOR_RED);
	init_pair(CP_YELLOW_ON_BLACK, COLOR_YELLOW, COLOR_BLACK);
	
}


void initialize_ncurses() {
	initscr();
	refresh();
	cbreak();
	noecho();
	mousemask(ALL_MOUSE_EVENTS, NULL);

	keypad(stdscr, TRUE);
	start_color();
	curs_set(0);

	initialize_colors();
	ncurses_config();
}