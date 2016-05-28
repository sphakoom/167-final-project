#ifndef NODE_H
#define NODE_H

#include <glm/mat4x4.hpp>

class Node
{
private:

public:
	virtual void draw() = 0;
	virtual void update(glm::mat4 C) = 0;
};

#endif