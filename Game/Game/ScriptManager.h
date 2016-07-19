#pragma once

#include "Console.h"
#include "Prism.h"
#include "Image.h"
#include "Prop.h"
#include "Ambient.h"
#include "Spotlight.h"
#include "Sprite.h"
#include "display.h"
#include "Player.h"
#include "EZServer.h"
#include "EZClient.h"
#include "World.h"
#include "Sound.h"
#include "ComFunc.h"

/*
			SCRIPT MANAGER
Allows creation and deletion of scripts
Scripts allow high-level, game focused coding
*/

class Script : public Listener {
public:
	virtual void Close() {}
	void SetID(int id) { my_id = id; }
	int GetID() { return my_id; }
private:
	int my_id = 0;
};

class ScriptManager {
public:
	ScriptManager() {}
	static Script* Start(int id);
	static void Stop(Script* script);
	static void UnloadAll();
};