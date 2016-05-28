#ifndef POINT_H
#define POINT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Point {
public:
	Point();
	Point(float, float, float);
	glm::vec3 coords;
private:
};

#endif