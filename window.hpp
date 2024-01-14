#pragma once

#include <iostream>
#include <ncursesw/curses.h>
#include <unistd.h>
#include <windows.h>

class Window {
	public:
		Window(int rows, int cols);

		bool isDrawing;

		void setSize(int rows, int cols);
		void setPosition(int rows, int cols);
		
		void drawBox();
		void drawBorder(int pair_value);
		void fill(int pair_value);
		
		void update();
		void clear();
		void bringForward();
		void erase();
		
		void addCh(std::string);
		void removeCh(char);
		void addStringAt(const char*, int y, int x, int pair_value);
		void returnInput();

		void loadSave(std::string saveFile);
		void Save(std::string saveTo);

		bool isEnclosed();

		void enableDrawing();
		void Draw(int color);
		void disableDrawing();

	private:
		WINDOW* ncurses_win;

		void draw_pixel(WINDOW* win, short int* x, short int* y, char brush, int brushSize);
};

extern int isDrawing;
