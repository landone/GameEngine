#include "Events.h"

static std::map<Listener*, unsigned int> listeners;

void Events::Listen(EventType type, Listener* listen) {
	if (listeners.find(listen) == listeners.end()) {
		listeners[listen] = 0;
	}
	if (listeners[listen] & type) {//Already listening
		return;
	}
	listeners[listen] |= type;
}

void Events::StopListen(EventType type, Listener* listen) {
	if (listeners.find(listen) == listeners.end()) {
		return;
	}
	if (listeners[listen] & type) {
		listeners[listen] -= type;
	}
}

void Events::Send_MouseMotion(double x, double y) {
	for (std::pair<Listener*, unsigned int>pir : listeners) {
		if (pir.second & Evt_MouseMotion) {
			pir.first->MouseMotion(x, y);
		}
	}
}

void Events::Send_KeyEvent(SDL_Keycode key, bool press) {
	for (std::pair<Listener*, unsigned int>pir : listeners) {
		if (pir.second & Evt_KeyEvent) {
			pir.first->KeyEvent(key, press);
		}
	}
}

void Events::Send_MouseClick(double x, double y, int type) {
	for (std::pair<Listener*, unsigned int>pir : listeners) {
		if (pir.second & Evt_MouseClick) {
			pir.first->MouseClick(x, y, type);
		}
	}
}

void Events::Send_MouseRelease(int type) {
	for (std::pair<Listener*, unsigned int>pir : listeners) {
		if (pir.second & Evt_MouseClick) {
			pir.first->MouseRelease(type);
		}
	}
}

void Events::RemoveListener(Listener* listen) {
	if (listeners.find(listen) != listeners.end()) {
		listeners.erase(listen);
	}
}

Listener::~Listener() {
	Events::RemoveListener(this);
}