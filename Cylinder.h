#ifndef CYLINDER_H
#define CYLINDER_H

#include "Geode.h"
#include "Window.h"

class Cylinder : public OBJObject
{
private:
public:
	Cylinder(const char * filename);
	void draw();
	void update(glm::mat4 C);
};

#endif