#include "Sound.h"
#include "glm\glm.hpp"
#include <map>
#include "Player.h"
#include "World.h"

static std::map<std::string, Mix_Chunk*> globalSound;

static std::string ToLowerCase(std::string temp) {
	for (unsigned int i = 0; i < temp.size(); i++) {
		temp[i] = tolower(temp[i]);
	}
	return temp;
}

Sound::Sound(double vol, double rang, int chan) {
	vol = (vol > 1.0 || vol < 0.0) ? 1.0 : vol;
	volume = (int)round(MIX_MAX_VOLUME * vol);
	channel = chan;
	range = range < 0.0 ? 0 : range;
	classname = "Sound";
}

void Sound::Update() {
	if (velocity.x != 0 || velocity.y != 0 || velocity.z != 0) {
		Move(velocity);
	}
	if (!physical) {
		Mix_VolumeChunk((Mix_Chunk*)m_sound, volume);
	}
	else {
		double dist = glm::distance(position, Player::Global()->GetPos());
		if (dist <= range) {
			Mix_VolumeChunk((Mix_Chunk*)m_sound, (int)round(volume * (1 - pow(dist, 2) / pow(range, 2))));
		}
		else {
			Mix_VolumeChunk((Mix_Chunk*)m_sound, 0);
		}
	}
}

void Sound::Load(std::string file) {
	path = ToLowerCase(file);
	if (globalSound.find(path) != globalSound.end()) {
		m_sound = (char*)globalSound[path];
	}
	else {
		m_sound = (char*)Mix_LoadWAV(path.c_str());
		if (m_sound == NULL) {
			Console::Error("Could not load a sound file: ");
			std::cout << path << std::endl;
			return;
		}
		globalSound[path] = (Mix_Chunk*)m_sound;
	}
	Mix_VolumeChunk((Mix_Chunk*)m_sound, volume);
}

void Sound::Unload() {
	if (m_sound == NULL) { return; }
	if (globalSound.find(path) != globalSound.end()) {
		Mix_FreeChunk((Mix_Chunk*)m_sound);
		m_sound = NULL;
		globalSound.erase(path);
	}
}

void Sound::Play() {
	if (m_sound == NULL || !enabled) { return; }
	Mix_VolumeChunk((Mix_Chunk*)m_sound, volume);
	Mix_PlayChannel(channel, (Mix_Chunk*)m_sound, 0);
}

void Sound::UnloadAll() {
	for (std::pair<std::string, Mix_Chunk*> pir : globalSound) {
		Mix_FreeChunk(pir.second);
	}
	globalSound.clear();
}