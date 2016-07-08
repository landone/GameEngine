#include "mesh.h"
#include <map>

struct ModelInfo {
	GLuint VA;
	GLuint indexSize;
};

static std::map<std::string, ModelInfo> globalMesh;

static std::string ToLowerCase(std::string temp) {
	for (unsigned int i = 0; i < temp.size(); i++) {
		temp[i] = tolower(temp[i]);
	}
	return temp;
}

void Mesh::Init(std::vector<Vertex>vertices, std::vector<GLuint>indices) {
	this->vertices = vertices;
	this->indices = indices;
	indexSize = indices.size();
	this->setupMesh();
}

bool FileExists(const std::string& path) {
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}

void Mesh::Load(const std::string& fileName) {
	std::string path = ToLowerCase(fileName);
	if (globalMesh.find(path) != globalMesh.end()) {
		VAO = globalMesh[path].VA;
		indexSize = globalMesh[path].indexSize;
	}
	else {
		if (!FileExists(path)) {
			std::cout << "ERROR: Mesh: File \"" << path << "\" does not exist. Mesh load cancelled" << std::endl;
			return;
		}
		IndexedModel m_model = OBJModel(path).ToIndexedModel();

		indices = m_model.indices;
		indexSize = indices.size();
		for (GLuint i = 0; i < m_model.positions.size(); i++) {
			Vertex v;
			v.position = m_model.positions[i];
			v.normal = m_model.normals[i];
			v.texCoord = m_model.texCoords[i];
			vertices.push_back(v);
		}
		setupMesh();

		globalMesh[path] = ModelInfo{ VAO, indexSize };
	}
}

void Mesh::Unload(const std::string& fileName) {
	std::string path = ToLowerCase(fileName);
	if (globalMesh.find(path) != globalMesh.end()) {
		glDeleteVertexArrays(1, &globalMesh[path].VA);
		globalMesh.erase(path);
	}
	else {
		Console::Warning("Attempted to unload non-existant mesh!");
	}
}

void Mesh::UnloadAll() {
	for (const auto& pair : globalMesh) {
		glDeleteVertexArrays(1, &pair.second.VA);
	}
	globalMesh.clear();
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	glBindVertexArray(0);
}

void Mesh::UpdateVertices(std::vector<Vertex>verts) {
	vertices = verts;
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);
	glBindVertexArray(0);
}

void Mesh::Draw() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}