#include "City.h"

City::City()
{
	textures.push_back(new Texture("bricks.ppm"));
	textures.push_back(new Texture("city7.ppm"));

	generateCity(8, 0, 10, 10, false);
}

City::~City()
{
}

void City::regenerate()
{
	generateCity(8, 0, 10, 10, true);
}

void City::generateCity(int numBuildings, int roadFactor, int cityWidth, int cityDepth, bool regenerate)
{
	float savedX = 0.0f;

	srand(time(NULL));
	if (regenerate) {
		// Generate building vertices
		for (int i = 0; i < numBuildings; ++i) {
			delete(this->buildings[i]);
			this->buildings[i] = new Building(3, 3, glm::vec3(i * 3 * 1.25f, 0.0f, 0.0f), rand() % 3 + 1);
			savedX += i;
		}
	}
	else {
		// Generate building vertices
		for (int i = 0; i < numBuildings; ++i) {
			this->buildings.push_back(new Building(3, 3, glm::vec3(i * 3 * 1.25f, 0.0f, 0.0f), rand() % 3 + 1));
			savedX += i;
		}
	}
}

void City::draw(GLuint shaderProgram)
{
	glUseProgram(shaderProgram);
	for (int i = 0; i < buildings.size(); ++i) {
		buildings[i]->draw(shaderProgram, textures);
	}
	for (int i = 0; i < roads.size(); ++i) {
		roads[i]->draw(shaderProgram);
	}
}