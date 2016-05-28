#ifndef MATRIX_TRANSFORM_H
#define MATRIX_TRANSFORM_H

#include <glm/gtc/matrix_transform.hpp>

#include "Group.h"

class MatrixTransform : public Group
{
private:
public:
	glm::mat4 M;

	MatrixTransform();
	MatrixTransform(glm::mat4);
	void update(glm::mat4 C);	
};

#endif