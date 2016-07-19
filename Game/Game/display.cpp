#include "display.h"
#include <iostream>
#include "GL/glew.h"
#include "Events.h"
#include "Console.h"

static Display global_disp;
Display* Display::Global() {
	return &global_disp;
}

Display::Display(int width, int height, const std::string& title) {
	Init(width, height, title);
}

void Display::Init(int width, int height, const std::string& title) {
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_DisplayMode dmode;
	SDL_GetCurrentDisplayMode(0, &dmode);
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	glContext = SDL_GL_CreateContext(window);

	GLenum status = glewInit();
	if (status != GLEW_OK) { Console::Error("GLEW failed to initialize!"); }

	isClosed = false;
	fullscr = false;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	SDL_ShowCursor(0);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	Console::Log("Display created");
}

Display::~Display() {
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Display::Clear(double r, double g, double b, double a) {
	glClearColor((GLclampf)r, (GLclampf)g, (GLclampf)b, (GLclampf)a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::Fullscreen(bool on) {
	if (on) {
		fullscr = true;
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else {
		fullscr = false;
		SDL_SetWindowFullscreen(window, 0);
	}
}

void Display::Update() {
	SDL_GL_SwapWindow(window);
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_KEYDOWN) {
			SDL_Keycode key = e.key.keysym.sym;
			if (key == SDLK_ESCAPE) {
				if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
					SDL_ShowCursor(1);
					SDL_SetRelativeMouseMode(SDL_FALSE);
				}
				else {
					SDL_ShowCursor(0);
					SDL_SetRelativeMouseMode(SDL_TRUE);
				}
			}
			Events::Send_KeyEvent(key, true);
		}
		if (e.type == SDL_KEYUP) {
			SDL_Keycode key = e.key.keysym.sym;
			Events::Send_KeyEvent(key, false);
			//if (key == SDLK_F11 && button_fscreen) { button_fscreen = false; }
		}
		if (e.type == SDL_MOUSEMOTION) {
			Events::Send_MouseMotion(e.motion.xrel, e.motion.yrel);
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			Events::Send_MouseClick(e.button.x, e.button.y, (int)e.button.button);
		}
		if (e.type == SDL_MOUSEBUTTONUP) {
			Events::Send_MouseRelease((int)e.button.button);
		}
		if (e.type == SDL_QUIT) {
			isClosed = true;
			(*this).~Display();//Close faster
		}
	}
}