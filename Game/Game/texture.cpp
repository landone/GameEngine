#include "texture.h"
#include "stb_image.h"
#include "Console.h"
#include <cassert>
#include <iostream>
#include <map>

static std::map<std::string, GLuint> globalTex;

static std::string ToLowerCase(std::string temp) {
	for (unsigned int i = 0; i < temp.size(); i++) {
		temp[i] = tolower(temp[i]);
	}
	return temp;
}

GLuint CreateTexture(const std::string &fileName) {
	int width, height, numComp;
	GLuint result;
	unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &numComp, 4);
	if (imageData == NULL) {
		Console::Error("Texture file doesn't exist:");
		std::cout << fileName << std::endl;
		return 0;
	}

	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//Repeat texture width
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//Repeat texture height

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//Minify texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//Magnify texture

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
	return result;
}

void Texture::Load(const std::string &fileName) {
	pathway = ToLowerCase(fileName);
	if (globalTex.find(pathway) != globalTex.end()) {
		m_texture = globalTex[pathway];
	}
	else {
		m_texture = CreateTexture(pathway);
		globalTex[pathway] = m_texture;
	}
}

void Texture::Unload(const std::string& fileName) {
	std::string temp = ToLowerCase(fileName);
	if (globalTex.find(temp) != globalTex.end()) {
		glDeleteTextures(1, &globalTex[temp]);
		globalTex.erase(temp);
	}
}

void Texture::UnloadAll() {
	for (const auto& pair : globalTex) {
		glDeleteTextures(1, &pair.second);
	}
	globalTex.clear();
}

Texture::~Texture() {
	
}

void Texture::Bind() {
	//assert(unit >= 0 && unit <= 31);
	//glActiveTexture(GL_TEXTURE0 + unit);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}