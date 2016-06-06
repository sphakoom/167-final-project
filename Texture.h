#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Texture {
public:
	Texture(const char * filename);
	GLuint textureID;
private:
	GLuint loadTexture(const char * filename);
	unsigned char* loadPPM(const char* filename, int& width, int& height);
};

#endif