#include "console_input.hpp"


HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
INPUT_RECORD inputBuffer;
DWORD eventsRead;

void readInput(){
	ReadConsoleInput(hInput, &inputBuffer, 1, &eventsRead);
}

bool mouseLeftClickHeld() {
	return (inputBuffer.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED);
}

bool mouseMoved(){
	return (inputBuffer.Event.MouseEvent.dwEventFlags == MOUSE_MOVED);
}

COORD getMousePosition(){
	return (inputBuffer.Event.MouseEvent.dwMousePosition);
}

char keyPressed(){
	wchar_t ch = inputBuffer.Event.KeyEvent.uChar.UnicodeChar;
	if (inputBuffer.Event.KeyEvent.bKeyDown){
		ch = inputBuffer.Event.KeyEvent.uChar.UnicodeChar;
		return (char(ch));
	}
	
	return 'N';

}

bool ctrlHeld(){
	return (inputBuffer.Event.KeyEvent.dwControlKeyState == LEFT_CTRL_PRESSED);
}

bool consoleResized(){
	return (inputBuffer.EventType == WINDOW_BUFFER_SIZE_EVENT);
}