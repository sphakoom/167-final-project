#ifndef _CITY_H_
#define _CITY_H_

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <time.h>

#include "Building.h"
#include "Road.h"

class Building;
class Road;

class City {
public:
	City();
	~City();
	void draw(GLuint shaderProgram);
private:
	void generateCity(int numBuildings, int roadFactor, int cityWidth, int cityDepth);
	std::vector <Building *> buildings;
	std::vector <Road *> roads;

};

#endif