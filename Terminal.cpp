#include "Terminal.h"

void enableVT() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	dwMode &= ~ENABLE_QUICK_EDIT_MODE;
	dwMode |= ENABLE_EXTENDED_FLAGS;
	SetConsoleMode(hOut, dwMode);
}

void hideCursor() { std::cout << "\033[?25l"; }
void restoreCursor() { std::cout << "\033[?25h"; }
void clearScreen() { std::cout << "\033[H"; }