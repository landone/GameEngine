#include "shader.h"
#include "Entity.h"
#include <map>

enum {
	TRANSFORM_U,
	POSIFORM_U,
	EFFECT_U,
	AMBIENT_U,
	AMBIENTC_U,
	SPLIGHT_U,
	SPLIGHTC_U,
	SPLIGHTDIR_U,
	NUM_UNIFORMS
};
static GLuint m_uniforms[NUM_UNIFORMS];

static Shader global_shader;
Shader* Shader::Global() {
	return &global_shader;
}

static std::vector<Entity*> ambientID;
static std::vector<glm::vec4> ambients, ambientc;

static std::vector<Entity*> spotID;
static std::vector<glm::vec4> spots, spotc, spotd;

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static std::string LoadShader(const std::string& fileName);
static GLuint CreateShader(const std::string& text, GLenum shaderType);

void Shader::SetAmbient(char* temp, glm::vec3 color, double intensity, double range) {
	Entity* amb = (Entity*)temp;
	for (unsigned int i = 0; i < ambientID.size(); i++) {
		if (ambientID[i] == amb) {
			ambients[i] = glm::vec4(amb->GetPos(), intensity);
			ambientc[i] = glm::vec4(color, range);
			return;
		}
	}

	ambientID.push_back(amb);
	ambients.push_back(glm::vec4(amb->GetPos(), intensity));
	ambientc.push_back(glm::vec4(color, range));
}

void Shader::RemoveAmbient(char* temp) {
	Entity* amb = (Entity*)temp;
	unsigned int id = -1;
	for (unsigned int i = 0; i < ambientID.size(); i++) {
		if (ambientID[i] == amb) {
			id = i;
			break;
		}
	}
	if (id == -1) {
		Console::Warning("Attempted to remove non-existant ambient!");
		return;
	}
	int size = ambients.size();
	for (int i = id; i < size - 1; i++) {
		ambientID[i] = ambientID[i + 1];
		ambients[i] = ambients[i + 1];
		ambientc[i] = ambientc[i + 1];
	}
	ambientID[size-1] = 0;
	ambients[size - 1] = glm::vec4(0, 0, 0, 0);
	ambientc[size - 1] = glm::vec4(0, 0, 0, 0);
	glUniform4fv(m_uniforms[AMBIENT_U], ambients.size(), &ambients[0][0]);
	glUniform4fv(m_uniforms[AMBIENTC_U], ambientc.size(), &ambientc[0][0]);
	ambientID.pop_back();
	ambients.pop_back();
	ambientc.pop_back();
}

void Shader::ClearAmbients() {
	ambientID.clear();
	ambients.clear();
	ambientc.clear();
}

void Shader::SetSpotlight(char* temp, glm::vec3 color, glm::vec3 dir, double intensity, double range, double radius) {
	Entity* spot = (Entity*)temp;
	for (unsigned int i = 0; i < spotID.size(); i++) {
		if (spotID[i] == spot) {
			spots[i] = glm::vec4(spot->GetPos(), intensity);
			spotc[i] = glm::vec4(color, range);
			spotd[i] = glm::vec4(dir, radius);
			return;
		}
	}

	spotID.push_back(spot);
	spots.push_back(glm::vec4(spot->GetPos(), intensity));
	spotc.push_back(glm::vec4(color, range));
	spotd.push_back(glm::vec4(dir, radius));
}

void Shader::RemoveSpotlight(char* temp) {
	Entity* spot = (Entity*)temp;
	unsigned int id = -1;
	for (unsigned int i = 0; i < spotID.size(); i++) {
		if (spotID[i] == spot) {
			id = i;
			break;
		}
	}
	if (id == -1) {
		Console::Warning("Attempted to remove non-existant spotlight!");
		return;
	}
	int size = spots.size();
	for (int i = id; i < size - 1; i++) {
		spotID[i] = spotID[i + 1];
		spots[i] = spots[i + 1];
		spotc[i] = spotc[i + 1];
		spotd[i] = spotd[i + 1];
	}
	spotID[size - 1] = 0;
	spots[size - 1] = glm::vec4(0, 0, 0, 0);
	spotc[size - 1] = glm::vec4(0, 0, 0, 0);
	spotd[size - 1] = glm::vec4(0, 0, 0, 0);
	glUniform4fv(m_uniforms[SPLIGHT_U], spots.size(), &spots[0][0]);
	glUniform4fv(m_uniforms[SPLIGHTC_U], spotc.size(), &spotc[0][0]);
	glUniform4fv(m_uniforms[SPLIGHTDIR_U], spotd.size(), &spotd[0][0]);
	spotID.pop_back();
	spots.pop_back();
	spotc.pop_back();
	spotd.pop_back();
}

void Shader::ClearSpotlights() {
	spotID.clear();
	spots.clear();
	spotc.clear();
	spotd.clear();
}

void Shader::Init(const std::string& fileName) {
	if (m_program != NULL) {//remove previous shaders
		for (unsigned int i = 0; i < NUM_SHADERS; i++) {
			glDetachShader(m_program, m_shaders[i]);
			glDeleteShader(m_shaders[i]);
		}

		glDeleteProgram(m_program);
	}
	m_program = glCreateProgram();
	m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glAttachShader(m_program, m_shaders[i]);
	}

	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "texCoord");
	glBindAttribLocation(m_program, 2, "normal");

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "ERROR: Program linking failed: ");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "ERROR: Program is invalid: ");

	m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");
	m_uniforms[POSIFORM_U] = glGetUniformLocation(m_program, "posiform");
	m_uniforms[AMBIENT_U] = glGetUniformLocation(m_program, "ambients");//Positions & Intensity
	m_uniforms[AMBIENTC_U] = glGetUniformLocation(m_program, "ambientc");//Colors & Range
	m_uniforms[EFFECT_U] = glGetUniformLocation(m_program, "effect");
	m_uniforms[SPLIGHT_U] = glGetUniformLocation(m_program, "splights");//Positions & Intensity
	m_uniforms[SPLIGHTC_U] = glGetUniformLocation(m_program, "splightc");//Colors & Range
	m_uniforms[SPLIGHTDIR_U] = glGetUniformLocation(m_program, "splightdir");//Direction Vector & Radius
}

Shader::~Shader() {
	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}

	glDeleteProgram(m_program);
}

void Shader::Bind() {
	glUseProgram(m_program);
}

void Shader::SetEffect(int set) {
	glUniform1i(m_uniforms[EFFECT_U], set);
	effect = set;
}

void Shader::Update(const Transform& transform, const Camera& camera) {
	glm::mat4 model = camera.GetViewProjection() * transform.GetModel();
	glm::mat4 posi = transform.GetModel();
	//UpdateAmbients();

	glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m_uniforms[POSIFORM_U], 1, GL_FALSE, &posi[0][0]);
	glUniform4fv(m_uniforms[AMBIENT_U], ambients.size(), &ambients[0][0]);
	glUniform4fv(m_uniforms[AMBIENTC_U], ambientc.size(), &ambientc[0][0]);
	glUniform4fv(m_uniforms[SPLIGHT_U], spots.size(), &spots[0][0]);
	glUniform4fv(m_uniforms[SPLIGHTC_U], spotc.size(), &spotc[0][0]);
	glUniform4fv(m_uniforms[SPLIGHTDIR_U], spotd.size(), &spotd[0][0]);
}

void Shader::Update(const Transform& transform) {
	glm::mat4 model = transform.GetModel();

	glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
}

static GLuint CreateShader(const std::string& text, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0) {
		Console::Error("Shader creation failed!");
	}

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLengths[0] = text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "ERROR: Shader compilation failed: ");

	return shader;
}

static std::string LoadShader(const std::string& fileName) {
	std::ifstream file;
	file.open((fileName).c_str());
	std::string output, line;
	if (file.is_open()) {
		while (file.good()) {
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}
	return output;
}

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage) {
	GLint success = 0;
	GLchar error[1024] = { 0 };
	if (isProgram) {
		glGetProgramiv(shader, flag, &success);
	}
	else {
		glGetShaderiv(shader, flag, &success);
	}
	if (success == GL_FALSE) {
		if (isProgram) {
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		}
		else {
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}
		std::cerr << errorMessage << ":  '" << error << "'" << std::endl;
	}
}