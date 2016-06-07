#include "City.h"

City::City()
{
	textures.push_back(new Texture("bricks.ppm"));
	textures.push_back(new Texture("city7.ppm"));
	textures.push_back(new Texture("city5.ppm"));
	textures.push_back(new Texture("brick1.ppm"));
	textures.push_back(new Texture("road.ppm"));

	generateCity(8, 0, 12, 12, false);
}

City::~City()
{
}

void City::regenerate()
{
	generateCity(8, 0, 12, 12, true);
}

void City::generateCity(int numBuildings, int roadFactor, int cityWidth, int cityDepth, bool regenerate)
{
	float savedX = 0.0f;

	glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
	float radius = 10.0f;
	
	srand(time(NULL));
	if (regenerate) {

		/*
		for (int i = 0; i < numBuildings; ++i) {
			delete(this->buildings[i]);
			this->buildings[i] = new Building(
				3, 5, origin + glm::vec3(i * 5 * 1.25f, 0.0f, 0.0f), rand() % 3 + 1);
			savedX += i;
		}
		*/

		for (int i = 0; i < cityDepth; ++i) {
			savedX = 0.0f;
			float angle = 0.0f;
			for (int j = 0; j < cityWidth; ++j, angle += 360.0f / cityWidth) {
				this->buildings[cityDepth * i + j] = new Building(
					3, 4, origin + glm::vec3(radius * cos(glm::radians(angle)), 0.0f, radius * sin(glm::radians(angle))), rand() % 3 + 1);
				savedX += i;
			}
			radius += 5.0f;
		}
	}
	else {
	
		for (int i = 0; i < cityDepth; ++i) {
			savedX = 0.0f;
			float angle = 0.0f;
			for (int j = 0; j < cityWidth; ++j, angle += 360.0f / cityWidth) {
				this->buildings.push_back(new Building(
					3, 4, origin + glm::vec3(radius * cos(glm::radians(angle)), 0.0f, radius * sin(glm::radians(angle))), rand() % 3 + 1));
				savedX += i;
			}
			radius += 5.0f;
		}

		this->roads.push_back(new Road(2 * radius * cos(glm::radians(45.0f)), 2 * radius * sin(glm::radians(45.0f))));

	}
}

void City::draw(GLuint shaderProgram)
{
	glUseProgram(shaderProgram);
	for (int i = 0; i < buildings.size(); ++i) {
		buildings[i]->draw(shaderProgram, textures);
	}
	for (int i = 0; i < roads.size(); ++i) {
		roads[i]->draw(shaderProgram, textures[4]->textureID);
	}
}