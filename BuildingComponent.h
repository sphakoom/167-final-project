#ifndef BUILDING_COMP_H
#define BUILDING_COMP_H

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <time.h>

#include "Window.h"

class BuildingComponent {
public:
	enum Types
	{
		RECTANGLE = 1,
		ARCH = 2,
		PYRAMID = 3
	};
	BuildingComponent(glm::vec3 dim, glm::vec3 origin, int type);
	void draw(GLuint shaderProgram);
private:
	std::vector<glm::vec3> vertices;
	GLuint VAO, VBO, EBO;
	glm::mat4 toWorld;
	int componentType;
};

#endif