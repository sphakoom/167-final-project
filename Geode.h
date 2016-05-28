#ifndef GEODE_H
#define GEODE_H

#include <limits>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Node.h"
#include "OBJObject.h"

class OBJObject;

class Geode : public Node
{
public:
	glm::vec3 position;
	glm::mat3 rotation;
protected:
	OBJObject* toDraw;
	glm::mat4 M;

public:
	Geode(OBJObject * modelToDraw);
	void printMat();
	void draw();
	void update(glm::mat4 C);
};

#endif