#pragma once

#include <SDL2\SDL_keycode.h>

/*
		EVENTS & LISTENERS
Allow any class to listen to events or send events.
*/

enum EventType {
	Evt_MouseMotion = 1,
	Evt_KeyEvent = 2,
	Evt_MouseClick = 4,
	Evt_MouseRelease = 8,
	Evt_OnTick = 16,
	Evt_OnPlayerJump = 32
};

class Listener {
public:
	~Listener();
	virtual void MouseMotion(double x, double y) {}
	virtual void KeyEvent(SDL_Keycode key, bool press) {}
	virtual void MouseClick(double x, double y, int type) {}
	virtual void MouseRelease(int type) {}
	virtual void OnTick() {}
	virtual void OnPlayerJump() {}
};

class Events {
public:
	static void Listen(EventType type, Listener* listen);
	static void StopListen(EventType type, Listener* listen);
	static void RemoveListener(Listener* listen);

	static void Send_MouseMotion(double x, double y);
	static void Send_KeyEvent(SDL_Keycode key, bool press);
	static void Send_MouseClick(double x, double y, int type);
	static void Send_MouseRelease(int type);
	static void Send_OnTick();
	static void Send_OnPlayerJump();
};