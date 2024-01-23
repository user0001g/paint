#include "console_input.hpp"

ConsoleInput::ConsoleInput() {
	hInput = GetStdHandle(STD_INPUT_HANDLE);
}

ConsoleInput& ConsoleInput::getInput() {
    static ConsoleInput instance;
    return instance;
}

void ConsoleInput::readInput() {
	ReadConsoleInput(hInput, &inputBuffer, 1, &eventsRead);
}

bool ConsoleInput::mouseLeftClickHeld() {
	return (inputBuffer.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED);
}

bool ConsoleInput::mouseMoved() {
	return (inputBuffer.Event.MouseEvent.dwEventFlags == MOUSE_MOVED);
}

COORD ConsoleInput::getMousePosition() {
	return (inputBuffer.Event.MouseEvent.dwMousePosition);
}

char ConsoleInput::keyPressed() {
	wchar_t ch = inputBuffer.Event.KeyEvent.uChar.UnicodeChar;
	if (inputBuffer.Event.KeyEvent.bKeyDown) {
		ch = inputBuffer.Event.KeyEvent.uChar.UnicodeChar;
		return (char(ch));
	}

	return 'N';

}

bool ConsoleInput::ctrlHeld() {
	return (inputBuffer.Event.KeyEvent.dwControlKeyState == LEFT_CTRL_PRESSED);
}

bool ConsoleInput::consoleResized() {
	return (inputBuffer.EventType == WINDOW_BUFFER_SIZE_EVENT);
}