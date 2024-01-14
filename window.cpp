#include "window.hpp"
#include "console_input.hpp"
#include <ctime>
#include <fstream>
#include "colors.hpp"

Window::Window(int rows, int cols) : isDrawing(false) {
	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);

	ncurses_win = newwin(rows, cols, yMax/2 - (rows/2), xMax/2 - (cols/2));
	update();
}

void Window::bringForward() {
	redrawwin(ncurses_win);
	update();
}


void Window::update() {
	wrefresh(ncurses_win);
}


bool Window::isEnclosed() {
	COORD mousePos = getMousePosition();
	return wenclose(ncurses_win, mousePos.Y, mousePos.X);
}

void Window::setSize(int rows, int cols) {
	clear();
	wresize(ncurses_win, rows, cols);
	update();
}

void Window::setPosition(int rows, int cols) {
	mvwin(ncurses_win, rows, cols);
	wnoutrefresh(ncurses_win);
	refresh();
}

void Window::fill(int pair_value) {
	wbkgd(ncurses_win, COLOR_PAIR(pair_value));
	update();
}

void Window::drawBox() {
	wattron(ncurses_win, COLOR_PAIR(CP_WHITE_ON_BLACK));
	box(ncurses_win, 0, 0);
	wattroff(ncurses_win, COLOR_PAIR(CP_WHITE_ON_BLACK));
	update();
}

void Window::clear() {
	wclear(ncurses_win);
	update();
}

void Window::addStringAt(const char* B, int y, int x, int pair_value) {
	wattron(ncurses_win, COLOR_PAIR(pair_value));
	mvwaddstr(ncurses_win, y, x, B);
	wattroff(ncurses_win, COLOR_PAIR(pair_value));
	update();
}

void Window::addCh(std::string input) {
	clear();
	mvwprintw(ncurses_win, 0, 0, "%s.txt", input.c_str());
	update();
}

void Window::drawBorder(int pair_value) {
	wattron(ncurses_win, COLOR_PAIR(pair_value));
	wborder(ncurses_win, '|', '|', '-', '-', '+', '+', '+', '+');
	update();
	wattroff(ncurses_win, COLOR_PAIR(pair_value));
}

void Window::draw_pixel(WINDOW* win, short int *x, short int *y, char brush, int brushSize) {
	int Y = int(*y);
	int X = int(*x);
	int a = getbegx(ncurses_win);
	int b = getbegy(ncurses_win);

	if (!wenclose(ncurses_win, Y, X))
		return;

	wmouse_trafo(ncurses_win, &Y, &X, FALSE);

	mvwaddch(ncurses_win, Y, X, brush);

	update();

}

bool fileExists(const std::string& filePath) {
#ifdef _WIN32
	return _access(filePath.c_str(), 0) == 0;
#else
	return access(filePath.c_str(), F_OK) == 0;
#endif
}

void Window::loadSave(std::string saveFile) {

	const char* result;

	result = saveFile.c_str();

	if (fileExists(saveFile)) {
		FILE *file = fopen(result, "r");
		ncurses_win = getwin(file);
		fclose(file);
		update();
	}


}


void Window::Save(std::string saveTo) {
	std::string randomFilename = saveTo;

	const char* result;
	result = randomFilename.c_str();

	FILE *file = fopen(result, "w");
	putwin(ncurses_win, file);
	fclose(file);
}


void Window::Draw(int color) {



	if (mouseMoved() && mouseLeftClickHeld() || mouseLeftClickHeld()) {
		COORD mousePos = getMousePosition();
		wattron(ncurses_win, COLOR_PAIR(color));
		draw_pixel(ncurses_win, &mousePos.X, &mousePos.Y, ' ', 2);
		wattroff(ncurses_win, COLOR_PAIR(color));
	}

}
