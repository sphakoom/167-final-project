#ifndef GROUP_H
#define GROUP_H

#include <vector>
#include "Node.h"


class Group : public Node
{	
public:
	Group();
	void draw();
	std::vector<Node*> children; // a list of this group's children - can be other groups or geodes (leaf objects)
	virtual void update(glm::mat4 C);
	void addChild(Node * child);
	void removeChild();
};

#endif