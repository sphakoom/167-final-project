#include "Road.h"

Road::Road(int width, int depth)
{
	srand(time(NULL));

	int maxWidth = width / 2;
	int maxDepth = depth / 2;

	glm::vec3 roadNode = glm::vec3();

	roadNode.x = ((float)rand() / RAND_MAX) * width - maxWidth;
	roadNode.z = ((float)rand() / RAND_MAX) * depth - maxDepth;
}

void Road::draw(GLint shaderProgram)
{

}