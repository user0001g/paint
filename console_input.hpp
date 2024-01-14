#include <windows.h>
#include <iostream>
#pragma once

extern HANDLE hInput;
extern INPUT_RECORD inputBuffer;
extern DWORD eventsRead;

bool mouseMoved();
bool mouseLeftClickHeld();
bool mouseRightClickHeld();
COORD getMousePosition();

char keyPressed();
bool ctrlHeld();
bool consoleResized();

void readInput();