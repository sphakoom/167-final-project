#include "MatrixTransform.h"

MatrixTransform::MatrixTransform()
{
	M = glm::mat4(1.0f);
}

MatrixTransform::MatrixTransform(glm::mat4 mat)
{
	M = mat;
}

void MatrixTransform::update(glm::mat4 C) {
	Group::update(C*M);
}

