#ifndef BUILDING_H
#define BUILDING_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "BuildingComponent.h"

class BuildingComponent;

class Building {
public:
	enum buildingTypes {
		SKYSCRAPER = 1,
		ARCH = 2,
		TOWER = 3
	};

	Building(int numComponents, int width, glm::vec3 origin, int type);
	void draw(GLint shaderProgram);
	GLuint loadTexture();
	unsigned char* loadPPM(const char* filename, int& width, int& height);
private:
	std::vector<BuildingComponent *> components;
	GLuint buildingTexture;
};

#endif