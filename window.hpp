#pragma once

#include <iostream>
#include <ncursesw/curses.h>
#include <unistd.h>
#include <windows.h>
#include "console_input.hpp"

class Window {
	public:
		Window(int rows, int cols, int posY = 0, int posX = 0, bool BoxFlag = false, int BorderFlag = 0, int fillFlag = 0, std::string textFlag = "");

		static bool isDrawing;

		void setSize(int rows, int cols);
		void setPosition(int rows = 0, int cols = 0);
		
		void drawBox();
		void drawBorder(int pair_value);
		void fill(int pair_value);
		void highlight(int pair_value);
		void type(std::string todo);
		
		
		void update();
		void clear();
		void bringForward();
		
		void addCh(std::string);
		void addStringAt(const char*, int y, int x, int pair_value);

		void loadSave(std::string saveFile);
		void Save(std::string saveTo);

		bool isEnclosed();

		void drawFlag();
		void Draw(int color);

	private:
		WINDOW* ncurses_win;
		
		ConsoleInput& Input;
		std::string Text;

		void draw_pixel(WINDOW* win, short int* x, short int* y, char brush, int brushSize);
};

