#include "initialize.hpp"
#include "window.hpp"
#include "colors.hpp"
#include "console_input.hpp"
#include <direct.h>

#define saveDirectory "save_files/"

using namespace std;

int main() {

	if (!_access(saveDirectory, 0) == 0) {
		if (_mkdir(saveDirectory) == 0) {
			_mkdir(saveDirectory);
		}
	}


	initialize_ncurses();

	Window *save = new Window(3, 6);
	Window *load = new Window(3, 6);
	Window *chosenColor = new Window(3, 6);
	Window *save_as=nullptr;
	Window *save_as_input=nullptr;
	Window *brush = new Window(3, 7);
	Window *eraseAll = new Window(3, 7);

	save->setPosition(0, 10);
	save->drawBox();
	load->setPosition(0, 16);
	load->drawBox();
	chosenColor->setPosition(0, 104);
	chosenColor->drawBox();
	chosenColor->fill(WHITE);
	eraseAll->setPosition(15, 111);
	eraseAll->drawBox();
	brush->setPosition(5, 111);
	brush->drawBorder(CP_WHITE_ON_BLACK);


	int ch;
	int buffer = 30;
	int colorToDraw = 0;

	Window *Canvas = new Window(25, 100);
	Canvas->fill(WHITE);

	Window *windows[8];

	for (int i = BLACK; i <= WHITE; i++) {
		windows[i] = new Window(3, 6);
		windows[i]->setPosition(0, buffer+=6);
		windows[i]->fill(i);
		windows[i]->drawBox();
		if (i == BLACK) {
			windows[i]->drawBox();
		}
	}

	std::string Text1 = "Save";
	std::string Text2 = "Load";
	std::string Text3 = "Erase";
	std::string Text4 = "Brush";
	std::string Text5 = "Save as";
	std::string Text6 = "Load";


	save->addStringAt(Text1.c_str(), 1, 1, CP_WHITE_ON_BLACK);
	load->addStringAt(Text2.c_str(), 1, 1, CP_WHITE_ON_BLACK);
	eraseAll->addStringAt(Text3.c_str(), 1, 1, CP_WHITE_ON_BLACK);
	brush->addStringAt(Text4.c_str(), 1, 1, CP_WHITE_ON_BLACK);

	Canvas->bringForward();

	while (keyPressed() != 'q') {

		readInput();
		refresh();


		save->addStringAt(Text1.c_str(), 1, 1, CP_WHITE_ON_BLACK);
		load->addStringAt(Text2.c_str(), 1, 1, CP_WHITE_ON_BLACK);
		eraseAll->addStringAt(Text3.c_str(), 1, 1, CP_WHITE_ON_BLACK);
		brush->addStringAt(Text4.c_str(), 1, 1, CP_WHITE_ON_BLACK);

		//key to enable brush/drawing
		if (keyPressed() == 'b' && Canvas->isDrawing == false) {
			Canvas->isDrawing = true;
			brush->drawBorder(CP_YELLOW_ON_BLACK);
		} else if(keyPressed() == 'b' && Canvas->isDrawing == true) {
			Canvas->isDrawing = false;
		}

		//click to enable brush/drawing
		if (brush->isEnclosed() && mouseLeftClickHeld() && !Canvas->isDrawing ) {
			Canvas->isDrawing = true;
			brush->drawBorder(CP_YELLOW_ON_BLACK);
		} else if (brush->isEnclosed() && mouseLeftClickHeld() && Canvas->isDrawing ) {
			Canvas->isDrawing = false;
		}

		if (eraseAll->isEnclosed() && mouseLeftClickHeld() && !Canvas->isDrawing) {
			Canvas->clear();
		}

		//text light up
		if (save->isEnclosed() && !Canvas->isDrawing) {
			save->addStringAt(Text1.c_str(), 1, 1, CP_YELLOW_ON_BLACK);
		} else if (load->isEnclosed() && !Canvas->isDrawing) {
			load->addStringAt(Text2.c_str(), 1, 1, CP_YELLOW_ON_BLACK);
		} else if (eraseAll->isEnclosed() && !Canvas->isDrawing) {
			eraseAll->addStringAt(Text3.c_str(), 1, 1, CP_YELLOW_ON_BLACK);
		} else if (brush->isEnclosed()) {
			brush->drawBorder(CP_YELLOW_ON_BLACK);
			brush->addStringAt(Text4.c_str(), 1, 1, CP_YELLOW_ON_BLACK);
		};



		if (save->isEnclosed() && !Canvas->isDrawing && mouseLeftClickHeld() ) {


			char buffer[256];
			int bufferIndex = 0;
			int ch;
			std::string input = "";
			int cursorPosition = 0;

			save_as = new Window(10, 35);
			save_as->fill(BLACK);
			save_as->drawBorder(MAGENTA);
			save_as->addStringAt(Text5.c_str(), 2, 13, CP_WHITE_ON_BLACK);

			save_as_input = new Window(3, 12);

			curs_set(1);

			while (1) {
				save_as_input->addCh(input);

				move(14, 54 + cursorPosition);

				ch = getch();
				if (ch == '\n') {
					Canvas->Save(saveDirectory + input + ".txt");
					Canvas->bringForward();
					delete save_as;
					delete save_as_input;
					curs_set(0);
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



		if (load->isEnclosed() && !Canvas->isDrawing && mouseLeftClickHeld()) {
			char buffer[256];
			int bufferIndex = 0;
			int ch;
			std::string input = "";
			int cursorPosition = 0;

			save_as = new Window(10, 35);
			save_as->fill(BLACK);
			save_as->drawBorder(CYAN);
			save_as->addStringAt(Text6.c_str(), 2, 13, CP_WHITE_ON_BLACK);

			save_as_input = new Window(3, 12);

			curs_set(1);


			while (1) {
				save_as_input->addCh(input);

				move(14, 54 + cursorPosition);

				ch = getch();

				if (ch == '\n') {
					Canvas->bringForward();
					Canvas->loadSave(saveDirectory + input + ".txt");

					delete save_as;
					delete save_as_input;
					curs_set(0);

					refresh();
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

		for (int i = BLACK; i <= WHITE; i++) {
			if (windows[i]->isEnclosed() && mouseLeftClickHeld()) {
				colorToDraw = i;
				chosenColor->fill(i);
				break;
			}
		}

		if (Canvas->isDrawing) {
			Canvas->Draw(colorToDraw);
		} else {
			brush->drawBorder(CP_WHITE_ON_BLACK);
		}

	}

	delete Canvas;
	delete save;
	delete load;
	delete brush;
	delete eraseAll;

	endwin();
	return 0;
}