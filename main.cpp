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
	ConsoleInput& Input = ConsoleInput::getInput();

	Window *save = new Window(3, 6, 0, 10, true, 0, 0, "Save");
	Window *load = new Window(3, 6, 0, 16, true, 0, 0, "Load");
	Window *brush = new Window(3, 7, 5, 111, false, CP_WHITE_ON_BLACK, 0, "Brush");
	Window *eraseAll = new Window(3, 7, 15, 111, true, 0, 0, "Erase");
	Window *chosenColor = new Window(3, 6, 0, 104, true, 0, WHITE);


	Window *colorWins[8];
	int colorToDraw = 0;
	for (int i = BLACK; i <= WHITE; i++) {
		static int buffer = 30;
		colorWins[i] = new Window(3, 6, 0, buffer+=6, true, 0, i);
	}

	Window *mainWins[4] = {save, load, brush, eraseAll};

	Window *Canvas = new Window(25, 100, 0, 0, false, 0, WHITE);
	Canvas->bringForward();

	while (Input.keyPressed() != 'q') {

		Input.readInput();

		if (!Canvas->isDrawing)
			for (auto &window : mainWins) {
				window->highlight(window->isEnclosed() ? CP_YELLOW_ON_BLACK : CP_WHITE_ON_BLACK);
			}

		if (Input.keyPressed() == 'b') {
			Canvas->drawFlag();
			brush->drawBorder((Canvas->isDrawing) ? CP_YELLOW_ON_BLACK : CP_WHITE_ON_BLACK);
		}

		if (Input.mouseLeftClickHeld()) {

			if (!Canvas->isDrawing) {
				if (eraseAll->isEnclosed()) {
					Canvas->clear();
				}
				if (save->isEnclosed())
					Canvas->type("save");
				if (load->isEnclosed())
					Canvas->type("load");
			}



			if (brush->isEnclosed()) {
				Canvas->drawFlag();
				brush->drawBorder((Canvas->isDrawing) ? CP_YELLOW_ON_BLACK : CP_WHITE_ON_BLACK);
			}

			for (int i = BLACK; i <= WHITE; i++) {
				if (colorWins[i]->isEnclosed()) {
					colorToDraw = i;
					chosenColor->fill(i);
					break;
				}
			}

		}

		if (Canvas->isDrawing) {
			Canvas->Draw(colorToDraw);
		}

		refresh();
	}

	delete Canvas;
	delete save;
	delete load;
	delete brush;
	delete eraseAll;

	endwin();
	return 0;
}
