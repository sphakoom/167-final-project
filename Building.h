#ifndef BUILDING_H
#define BUILDING_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "BuildingComponent.h"

class BuildingComponent;

class Building {
public:
	Building(int numComponents, int width, glm::vec3 origin, int type);
	void draw(GLint shaderProgram);
private:
	std::vector<BuildingComponent *> components;
};

#endif