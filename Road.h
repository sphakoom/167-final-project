#ifndef ROAD_H
#define ROAD_H

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <time.h>

#include "Window.h"

class Road {
public:
	Road(int width, int depth);
	void draw(GLint shaderProgram);
private:
	GLuint VAO, VBO, EBO;
	std::vector<glm::vec3> vertices;
	glm::mat4 toWorld;
};

#endif