#pragma once

#include "Entity.h"
#include <SDL2\SDL_mixer.h>

/*
				SOUND
Plays sound files
Optional varying volume based on position
*/

class Sound : public Entity {
public:
	Sound(double volume = 1.0, double range = 25, int channel = -1);
	void Load(std::string path);
	void Unload();
	void Play();
	static void UnloadAll();
	bool IsPhysical() { return physical; }
	void SetPhysical(bool set) { physical = set; }
	virtual void Update();

	double GetVolume() { return volume; }
	void SetVolume(double vol) { volume = (int)round(MIX_MAX_VOLUME * vol); }
	double GetRange() { return range; }
	void SetRange(double rang) { range = rang; }
	int GetChannel() { return channel; }
	void SetChannel(int chan) { channel = chan; }
private:
	bool physical = true;
	std::string path;
	int channel, volume;
	double range = 25;
	char* m_sound = NULL;
};