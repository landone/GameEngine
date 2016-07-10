#pragma once

#include <SDL2/SDL.h>
#undef main
#include <string>

class Display {
public:
	Display() {}
	Display(int width, int height, const std::string& title);
	void Init(int width, int height, const std::string& title);
	~Display();

	void Clear(double r, double g, double b, double a);
	void Update();
	void Fullscreen(bool on);
	bool IsOpen() { return !isClosed; }

	static Display* Global();
	static Display* MakeGlobal(int width, int height, const std::string& title) { Global()->Init(width, height, title); return Global(); }
private:
	bool isClosed, fullscr;
	SDL_GLContext glContext;
	SDL_Window* window;
};