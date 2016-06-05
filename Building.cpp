#include "Building.h"

const int SKYSCRAPER = 1;
const int ARCH = 2;
const int TOWER = 3;

Building::Building(int numComponents, int width, glm::vec3 origin, int type)
{
	float height;
	float savedHeight = 0.0f;
	float widthF = width;

	printf("Building origin: %lf, %lf, %lf\n", origin.x, origin.y, origin.z);

	switch(type)
	{
		case TOWER:
			for (int i = 0; i < numComponents; ++i) {
				//int type = rand() % 3 + 1;
				// Defines a max height
				height = ((float)rand() / RAND_MAX) * 3.0f + 1.5f;
				height += rand() % 2 + (-1.4f);
				int type = (i == numComponents - 1) ? 3 : 1;
				if (i == 0) {
					components.push_back(new BuildingComponent(height, widthF, glm::vec3(origin.x, i, 0.0f), type));
				}
				else {
					float widthFactor = (float)rand() / RAND_MAX;
					widthFactor = (widthFactor < 0.5f) ? 0.65f : widthFactor;
					components.push_back(new BuildingComponent(height, widthF * widthFactor, glm::vec3(origin.x, savedHeight, 0.0f), type));
				}
				savedHeight += height;
			}
			break;
		default:
			break;
	}

	
}

void Building::draw(GLint shaderProgram)
{
	for (int i = 0; i < components.size(); ++i) {
		glUniform1i(glGetUniformLocation(shaderProgram, "buildingPart"), i % components.size());

		components[i]->draw(shaderProgram);
	}
}