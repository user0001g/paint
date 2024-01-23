#include "window.hpp"
#include "console_input.hpp"
#include <ctime>
#include <fstream>
#include "colors.hpp"

#define saveDirectory "save_files/"

#define CENTER_Y yMax/2 - (rows/2)
#define CENTER_X xMax/2 - (cols/2)

Window::Window(int rows, int cols, int posY, int posX, bool boxFlag, int borderFlag, int fillFlag, std::string textFlag) : Input(ConsoleInput::getInput()), Text(textFlag) {

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);

	if (posY == 0 && posX == 0) {
		ncurses_win = newwin(rows, cols, CENTER_Y, CENTER_X);
	} else {
		ncurses_win = newwin(rows, cols, posY, posX);
	}

	if (boxFlag == true) {
		this->drawBox();
	}

	if (borderFlag != 0) {
		this->drawBorder(borderFlag);
	}

	if (fillFlag != 0) {
		this->fill(fillFlag);
	}

	if (textFlag != "") {
		this->addStringAt(Text.c_str(), 1, 1, CP_WHITE_ON_BLACK);
	}

	update();
}

bool Window::isDrawing = false;

void Window::highlight(int color_pair) {
	this->addStringAt(Text.c_str(), 1, 1, color_pair);
}

void Window::drawFlag() {

	if (isDrawing == false)  {
		isDrawing = true;
	} 
	else {
		isDrawing = false;
	}
}

void Window::type(std::string todo) {

	Window* typing_field = new Window(3, 12, 0, 0);
	Window* typing_field_bkgd = new Window(10, 35, 0, 0);

	char buffer[256];
	
	int bufferIndex = 0;
	int cursorPosition = 0;
	int ch;

	std::string input = "";
	std::string result = (todo == "save") ? "Save as" : "Load";


	typing_field_bkgd->fill(BLACK);
	typing_field_bkgd->drawBorder((todo == "save") ? MAGENTA : CYAN);
	typing_field_bkgd->addStringAt(result.c_str(), 2, 13, CP_WHITE_ON_BLACK);

	curs_set(1);

	while (1) {
		typing_field->addCh(input);
		move(14, 54 + cursorPosition);

		ch = getch();
		if (ch == '\n') {

			if (todo == "save") {
				this->Save(saveDirectory + input + ".txt");
			}

			if (todo == "load") {
				this->loadSave(saveDirectory + input + ".txt");
			}

			typing_field->clear();
			typing_field_bkgd->clear();
			this->bringForward();
			curs_set(0);
			delete typing_field;
			delete typing_field_bkgd;
			break;

		}



		if (ch == KEY_BACKSPACE || ch == 127) {
			if (cursorPosition > 0) {
				input.erase(cursorPosition - 1, 1);
				cursorPosition--;
			}
		} else if (ch == KEY_LEFT) {
			if (cursorPosition > 0) {
				cursorPosition--;
			}
		} else if (ch == KEY_RIGHT) {
			if (cursorPosition < static_cast<int>(input.length())) {
				cursorPosition++;
			}
		} else if (isprint(ch) && input.length() < 8) {
			input.insert(cursorPosition, 1, ch);
			cursorPosition++;
		}

	}
}


void Window::bringForward() {
	redrawwin(ncurses_win);
	update();
}


void Window::update() {
	wrefresh(ncurses_win);
}


bool Window::isEnclosed() {

	COORD mousePos = Input.getMousePosition();
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

void Window::drawBox() {
	wattron(ncurses_win, COLOR_PAIR(CP_WHITE_ON_BLACK));
	box(ncurses_win, 0, 0);
	wattroff(ncurses_win, COLOR_PAIR(CP_WHITE_ON_BLACK));
	update();
}
void Window::drawBorder(int pair_value) {
	wattron(ncurses_win, COLOR_PAIR(pair_value));
	wborder(ncurses_win, '|', '|', '-', '-', '+', '+', '+', '+');
	update();
	wattroff(ncurses_win, COLOR_PAIR(pair_value));
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

void Window::Draw(int color) {



	if (Input.mouseMoved() && Input.mouseLeftClickHeld() || Input.mouseLeftClickHeld()) {
		COORD mousePos = Input.getMousePosition();
		wattron(ncurses_win, COLOR_PAIR(color));
		draw_pixel(ncurses_win, &mousePos.X, &mousePos.Y, ' ', 2);
		wattroff(ncurses_win, COLOR_PAIR(color));
	}

}
