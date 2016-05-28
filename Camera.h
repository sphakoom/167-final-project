#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>
#include "OBJObject.h"

class Camera
{
private:

public:
	Camera::Camera();
	glm::mat4 M;
};

#endif