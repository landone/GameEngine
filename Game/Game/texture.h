#pragma once

#include <string>
#include "GL/glew.h"

class Texture {
public:
	void Load(const std::string& fileName);
	Texture() {}
	~Texture();

	static void Unload(const std::string& fileName);
	static void UnloadAll();

	void Bind();
	
	std::string pathway;
private:
	GLuint m_texture;
};