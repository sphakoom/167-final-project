#ifndef POD_H
#define POD_H

#include "OBJObject.h"

class Pod : public OBJObject
{
private:
	bool loaded = false;
public:
	Pod(const char * filename);
	void draw();
	void update(glm::mat4 C);
	void setInitial(glm::vec3);
};

#endif