#include "Building.h"

Building::Building(int numComponents, int width, glm::vec3 origin, int type)
{
	float height;
	float savedHeight = 0.0f;
	float widthF = width;

	switch(type) {
		case TOWER: {
			for (int i = 0; i < numComponents; ++i) {
				//int type = rand() % 3 + 1;
				// Defines a max height
				height = ((float)rand() / RAND_MAX) * 3.0f + 1.5f;
				height += rand() % 2 + (-1.0f);
				int type = (i == numComponents - 1) ? BuildingComponent::PYRAMID : BuildingComponent::RECTANGLE;
				if (i == 0) {
					components.push_back(new BuildingComponent(glm::vec3(widthF, height, widthF), glm::vec3(origin.x, i, 0.0f), type));
				}
				else {
					float widthFactor = (float)rand() / RAND_MAX;
					widthFactor = (widthFactor < 0.5f) ? 0.65f : widthFactor;
					components.push_back(new BuildingComponent(glm::vec3(widthF * widthFactor, height, widthF * widthFactor), glm::vec3(origin.x, savedHeight, 0.0f), type));
				}
				savedHeight += height;
			}
			break;
		}
		case ARCH: {
			float pillarWidth = widthF / 3.0f;
			// Build both pillars
			height = ((float)rand() / RAND_MAX) * 3.0f + 1.5f;
			height += rand() % 2 + (-1.0f);
			components.push_back(new BuildingComponent(glm::vec3(pillarWidth, height, widthF), glm::vec3(origin.x - pillarWidth, 0.0f, 0.0f), BuildingComponent::RECTANGLE));
			components.push_back(new BuildingComponent(glm::vec3(pillarWidth, height, widthF), glm::vec3(origin.x + pillarWidth, 0.0f, 0.0f), BuildingComponent::RECTANGLE));

			savedHeight += height;
			height = ((float)rand() / RAND_MAX);
			// Then rectangle on top
			components.push_back(new BuildingComponent(glm::vec3(widthF,height, widthF), glm::vec3(origin.x, savedHeight, 0.0f), BuildingComponent::RECTANGLE));
			savedHeight += height;
			height = ((float)rand() / RAND_MAX);
			height = (height < 0.5f) ? 0.5f : height;
			// Then trapezoid
			components.push_back(new BuildingComponent(glm::vec3(widthF, height, widthF), glm::vec3(origin.x, savedHeight, 0.0f), BuildingComponent::ARCH));

			break;
		}
		case SKYSCRAPER: {
			float currWidth = widthF;
			for (int i = 0; i < numComponents; ++i) {
				//int type = rand() % 3 + 1;
				// Defines a max height
				height = ((float)rand() / RAND_MAX) * 3.0f + 1.5f;
				height += rand() % 2 + (-1.0f);
				if (i == 0) {
					components.push_back(new BuildingComponent(glm::vec3(currWidth, height, currWidth), glm::vec3(origin.x, i, 0.0f), BuildingComponent::RECTANGLE));
				}
				else {
					float widthFactor = (float)rand() / RAND_MAX;
					widthFactor = (widthFactor < 0.5f) ? 0.65f : widthFactor;
					components.push_back(new BuildingComponent(glm::vec3(currWidth * widthFactor, height, currWidth * widthFactor), glm::vec3(origin.x, savedHeight, 0.0f), type));
					currWidth *= widthFactor;
				}
				savedHeight += height;
			}

			break;
		}
		default:
			break;
	}

	//buildingTextures.push_back(loadTexture());
}

void Building::draw(GLint shaderProgram, GLuint buildingTexture)
{
	for (int i = 0; i < components.size(); ++i) {
		glUniform1i(glGetUniformLocation(shaderProgram, "buildingPart"), components[i]->componentType);
		components[i]->bindTexture(buildingTexture);
		components[i]->draw(shaderProgram, buildingTexture);
	}
}