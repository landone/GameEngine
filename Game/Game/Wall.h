#pragma once

#include "Entity.h"
#include "mesh.h"
#include "texture.h"

class Wall : public Entity {
public:
	Wall(double width = 1, double height = 1, double texX = 1, double texY = 1);

	virtual void Draw();
	virtual Mesh& GetMesh() { return mesh; }
	virtual void SetTex(const std::string& path); virtual void SetTex(Texture& tex);
	virtual Texture& GetTex() { return texture; }

	virtual glm::vec3 GetNormal() { return normal; }
	virtual double GetWidth() { return width; }
	virtual double GetHeight() { return height; }
	virtual double GetTexWidth() { return width / texX; }
	virtual double GetTexHeight() { return height / texY; }

	virtual void SetSize(double width, double height);
	virtual void SetWidth(double width);
	virtual void SetHeight(double height);
	virtual void SetScale(double amount);
	virtual void SetTexSize(double width, double height);
protected:
	Texture texture;
	Mesh mesh;
	double width = 1, height = 1;
	double texX = 1, texY = 1;
	glm::vec3 normal;
	std::vector<Vertex> v;

	virtual void UpdateTexture();
};