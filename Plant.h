#pragma once
#ifndef _Plant_H_
#define _Plant_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <string>
#include "LSystem.h"
#include "Turtle.h"
//#include "LinkedList.h"
#include "OBJObject.h"

class Plant
{
public:
	Plant();
	//~Plant();

	glm::mat4 toWorld;
	GLuint VBO, VAO;

	vector<glm::vec3> parse(string ops);
	void draw(GLuint shader);
	void update();
};

#endif

