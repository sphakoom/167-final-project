#ifndef BUILDING_H
#define BUILDING_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "BuildingComponent.h"
#include "Texture.h"

class BuildingComponent;

class Building {
public:
	enum buildingTypes {
		SKYSCRAPER = 1,
		ARCH = 2,
		TOWER = 3
	};

	Building(int numComponents, int width, glm::vec3 origin, int type);
	int buildingType;
	void draw(GLint shaderProgram, std::vector<Texture *> & texture);
private:
	std::vector<BuildingComponent *> components;
};

#endif