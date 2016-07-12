#pragma once

#include "Entity.h"
#include "mesh.h"
#include "texture.h"

/*
		WALL
Basic rectangular plane
*/

class Wall : public Entity {
public:
	Wall(double width = 1, double height = 1, double texX = 1, double texY = 1);
	~Wall();

	virtual void Draw();
	virtual Mesh& GetMesh() { return mesh; }
	virtual void SetTex(const std::string& path); virtual void SetTex(Texture& tex);
	virtual Texture& GetTex() { return texture; }

	virtual glm::vec3 GetNormal();
	virtual double GetWidth() { return width; }
	virtual double GetHeight() { return height; }
	virtual double GetTexWidth() { return width / texX; }
	virtual double GetTexHeight() { return height / texY; }

	virtual void SetSize(double width, double height);
	virtual void SetWidth(double width);
	virtual void SetHeight(double height);
	virtual void SetScale(double amount);
	virtual void SetTexSize(double width, double height);
	virtual void SetRotation(glm::vec3 rot); virtual void SetRotation(double x, double y, double z) { SetRotation(glm::vec3(x, y, z)); }
	virtual void Rotate(glm::vec3 amount); virtual void Rotate(double x, double y, double z) { Rotate(glm::vec3(x, y, z)); }

	virtual glm::vec3 GetVert(int index);
protected:
	Texture texture;
	Mesh mesh;
	double width = 1, height = 1;
	double texX = 1, texY = 1;
	glm::vec3 normal;
	std::vector<Vertex> v;

	virtual void UpdateTexture();
	virtual void CalcNormal();
};