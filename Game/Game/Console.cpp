#include "Console.h"
#include <windows.h>
#include <vector>

static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void Console::SetColor(int value) {
	SetConsoleTextAttribute(hConsole, value);
}

void Console::ResetColor() {
	SetColor(15);
}

void Console::Print(std::string msg) {
	std::cout << msg;
}

void Console::PrintLn(std::string msg) {
	std::cout << msg << std::endl;
}

void Console::Warning(std::string msg) {
	SetColor(14);
	Print("WARNING: ");
	ResetColor();
	PrintLn(msg);
}

void Console::Error(std::string msg) {
	SetColor(12);
	Print("ERROR: ");
	ResetColor();
	PrintLn(msg);
}