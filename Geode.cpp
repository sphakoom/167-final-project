#include "Geode.h"

class OBJObject;

Geode::Geode(OBJObject * modelToDraw)
{
	this->toDraw = modelToDraw;
	M = modelToDraw->toWorld;
}

void Geode::update(glm::mat4 C)
{
	//toDraw->toWorld = C;
	M = C;

	// Position = the last column
	this->position = glm::vec3(C[3]);
	// Rotation = the upper left 3x3 matrix
	this->rotation = glm::mat3(glm::vec3(C[0]), glm::vec3(C[1]), glm::vec3(C[2]));
}

void Geode::draw()
{
	// Call the actual objects' draw
	toDraw->draw(Window::shaderProgram, 1, M);
}

void Geode::printMat()
{
	printf("%lf %lf %lf %lf\n %lf %lf %lf %lf\n %lf %lf %lf %lf\n %lf %lf %lf %lf\n",
		M[0][0], M[1][0], M[2][0], M[3][0],
		M[0][1], M[1][1], M[2][1], M[3][1],
		M[0][2], M[1][2], M[2][2], M[3][2],
		M[0][3], M[1][3], M[2][3], M[3][3]);
}