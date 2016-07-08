#pragma once

#include <glm/glm.hpp>
#include <gl/glew.h>
#include "obj_loader.h"
#include "Console.h"
#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

struct Tex {
	GLuint id;
	std::string type;
};

class Mesh {
public:
	Mesh() {}
	void Init(std::vector<Vertex> vertices, std::vector<GLuint> indices);
	void Load(const std::string& fileName);
	static void Unload(const std::string& fileName);
	static void UnloadAll();
	
	void Draw();
	void UpdateVertices(std::vector<Vertex>vertices);
private:
	GLuint VAO, VBO, EBO;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices; GLuint indexSize;
	void setupMesh();
};