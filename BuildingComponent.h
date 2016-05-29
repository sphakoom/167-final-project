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
	BuildingComponent(float height, float width, float offsetx, float offsety);
	void draw(GLuint shaderProgram);
private:
	std::vector<glm::vec3> vertices;
	GLuint VAO, VBO, EBO;
	glm::mat4 toWorld;
};

#endif