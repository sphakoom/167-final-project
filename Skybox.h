#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <vector>

#include "Window.h"

class Skybox
{
private:
	GLuint VAO, VBO;
	GLuint cubemapTexture;
	std::vector<const GLchar*> faces;
	glm::mat4 toWorld;
public:
	Skybox();
	~Skybox();
	unsigned char* loadPPM(const char* filename, int& width, int& height);
	GLuint loadCubemap();
	void draw(GLuint shaderProgram, GLuint globalShaderProgram);
};

#endif
