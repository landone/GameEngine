#pragma once

#include "Console.h"
#include <fstream>
#include "GL/glew.h"
#include "transform.h"
#include "camera.h"

/*
				SHADER
Loads programs that control final graphic changes
Globally used
Tracks ambients & spotlights
*/

class Shader {
public:
	void Init(const std::string& fileName);
	Shader() {
		m_program = NULL;
		effect = 0;
	}

	void Bind();
	void SetEffect(int set);
	void Update(const Transform& transform);//FOR 2D DRAWING
	void Update(const Transform& transform, const Camera& camera);
	~Shader();
	GLuint effect;

	static Shader* Global();

	static void SetAmbient(char* amb, glm::vec3 color, double intensity, double range);
	static void RemoveAmbient(char* amb);
	static void ClearAmbients();

	static void SetSpotlight(char* temp, glm::vec3 color, glm::vec3 dir, double intensity, double range, double radius);
	static void RemoveSpotlight(char* temp);
	static void ClearSpotlights();
private:
	static const unsigned int NUM_SHADERS = 2;

	GLuint m_program, m_shaders[NUM_SHADERS];
};