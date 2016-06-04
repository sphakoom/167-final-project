#include "Building.h"

Building::Building(int numComponents, int offsetx)
{
	float height = ((float)rand() / RAND_MAX) * 5.0f + 1.5f;
	float width = 1.0f;
	for (int i = 0; i < numComponents; ++i) {
		components.push_back(new BuildingComponent(height, width, offsetx, height*i));
	}
}

void Building::draw(GLint shaderProgram)
{
	for (int i = 0; i < components.size(); ++i) {
		glUniform1i(glGetUniformLocation(shaderProgram, "buildingPart"), i % components.size());

		components[i]->draw(shaderProgram);
	}
}