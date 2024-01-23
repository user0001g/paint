#include <windows.h>
#include <iostream>
#pragma once

class ConsoleInput {

	private:
		ConsoleInput();
		HANDLE hInput;
		INPUT_RECORD inputBuffer;
		DWORD eventsRead;


	public:
		static ConsoleInput& getInput();

		bool mouseMoved();
		bool mouseLeftClickHeld();
		bool mouseRightClickHeld();
		COORD getMousePosition();

		char keyPressed();
		bool ctrlHeld();
		bool consoleResized();

		void readInput();

		ConsoleInput(const ConsoleInput&) = delete;
		ConsoleInput& operator=(const ConsoleInput&) = delete;
};

