#include "Road.h"

Road::Road(int width, int depth)
{
	srand(time(NULL));

	int maxWidth = cityWidth / 2;
	int maxDepth = cityDepth / 2;

	roadNode.x = ((float)rand() / RAND_MAX) * width - maxWidth;
	roadNode.z = ((float)rand() / RAND_MAX) * depth - maxDepth;
}

void Road::draw(GLint shaderProgram)
{

}