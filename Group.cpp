#include "Group.h"

Group::Group()
{
}

void Group::draw()
{
	for (unsigned int i = 0; i < this->children.size(); ++i)
		this->children[i]->draw();
}

void Group::update(glm::mat4 C)
{
	for (unsigned int i = 0; i < this->children.size(); ++i) {
		this->children[i]->update(C);
	}
}

void Group::addChild(Node * child)
{
	this->children.push_back(child);
}

void Group::removeChild()
{
	children.pop_back();
}