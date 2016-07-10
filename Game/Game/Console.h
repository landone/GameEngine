#pragma once

#include <iostream>
#include <string>

class Console {
public:
	static void SetColor(int value);
	static void ResetColor();
	static void Print(std::string msg);
	static void PrintLn(std::string msg);
	static void Warning(std::string msg);
	static void Error(std::string msg);
};