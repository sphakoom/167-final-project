#include "City.h"

City::City()
{
	generateCity(10, 5, 10, 10);
}

City::~City()
{

}

void City::generateCity(int numBuildings, int roadFactor, int cityWidth, int cityDepth)
{
	// City width = x axis, city depth = z axis
	for (int i = 0; i < roadFactor; ++i) {
		roads.push_back(new Road(cityWidth, cityDepth));
	}

	// Generate building vertices
	for (int i = 0; i < numBuildings; ++i) {
		// Random height
		float height = ((float)rand() / RAND_MAX) * 5.0f + 1.5f;
		this->buildings.push_back(new Building(height, (float)i));
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