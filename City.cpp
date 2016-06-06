#include "City.h"

City::City()
{
	generateCity(8, 0, 10, 10);
}

City::~City()
{

}

void City::generateCity(int numBuildings, int roadFactor, int cityWidth, int cityDepth)
{
	/*
	// City width = x axis, city depth = z axis
	for (int i = 0; i < roadFactor; ++i) {
		roads.push_back(new Road(cityWidth, cityDepth));
	}
	*/
	srand(time(NULL));
	float savedX = 0.0f;
	// Generate building vertices
	for (int i = 0; i < numBuildings; ++i) {
		this->buildings.push_back(new Building(3, 2, glm::vec3(i*2 *1.25f, 0.0f, 0.0f), rand() % 3 + 1));
		savedX += i;
	}
}

void City::draw(GLuint shaderProgram)
{
	glUseProgram(shaderProgram);
	for (int i = 0; i < buildings.size(); ++i) {
		buildings[i]->draw(shaderProgram);
	}
	for (int i = 0; i < roads.size(); ++i) {
		roads[i]->draw(shaderProgram);
	}
}