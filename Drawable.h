#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <limits>

class Drawable 
{
public:
	virtual void draw(GLuint shaderProgram,int) = 0;
	virtual void update() = 0;
	virtual void manipulateXYZ(glm::vec3) = 0;
	virtual void scale(float scaleFactor) = 0;
	virtual void manipulatePointSize(bool decrease = false) = 0;
	virtual void manipulateRasterizedPointSize(bool decrease = false) = 0;
	virtual void orbit(bool counter = false) = 0;
	virtual void spin(float deg) = 0;
	virtual void reset() = 0;
	virtual void loadData() = 0;
	virtual std::vector<glm::vec3> getNormals() = 0;
	virtual std::vector<glm::vec3> getVertices() = 0;
	virtual float getPointSize() = 0;
	virtual void doTrackballRotation(glm::tvec3<double>lastPoint, glm::tvec3<double>currPoint, float velocity) = 0;
	virtual void doDirectionalLight(glm::tvec3<double>direction,int) = 0;
	virtual void changeLightDistance(double, int) = 0;
	virtual void scroll(double amt) = 0;
	float cutOff;
};

#endif