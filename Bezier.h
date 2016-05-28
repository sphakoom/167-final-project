#pragma once
#ifndef BEZIER_H
#define BEZIER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <vector>

#include "Window.h"
#include "Point.h"

class Bezier {
protected:
	glm::mat4 toWorld;
	GLuint VBO, VAO;
	glm::vec3 lightSource;
public:
	std::vector<glm::vec3> points;
	Bezier(std::vector<glm::vec3>);
	void draw();
};

#endif // !BEZIER_H
