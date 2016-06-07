#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#include "Drawable.h"
#include "Window.h"


class OBJObject : public Drawable
{
protected:
std::vector<unsigned int> indices;
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
glm::vec3 lightSource;
float pointSize;
GLuint VBO, VAO, EBO, NBO;

public:
	OBJObject();
	OBJObject(const char* filepath, std::string material);
	~OBJObject();
	glm::mat4 toWorld;

	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec3> getNormals();
	std::vector<unsigned int> getIndices();
	glm::vec4 getPosition();
	std::string material;
	float cutOff;

	void parse(const char* filepath);
	void draw();
	void draw(GLuint shaderProgram,  int);
	void draw(GLuint shaderProgram, int, glm::mat4);
	void OBJObject::draw(GLuint shaderProgram, glm::vec3 camera_position);
	void update();
	void manipulateXYZ(glm::vec3 direction);
	void scale(float scaleFactor);
	void manipulatePointSize(bool decrease = false);
	void manipulateRasterizedPointSize(bool decrease = false);
	void orbit(bool counter = false);
	void spin(float deg);
	void reset();
	void loadData();
	float getPointSize();
	void doTrackballRotation(glm::tvec3<double>lastPoint, glm::tvec3<double>currPoint, float velocity);
	void doDirectionalLight(glm::tvec3<double>direction,int);
	void changeLightDistance(double, int);
	void scroll(double amt);
	void setInitial(glm::vec3 position);
	void OBJObject::update(glm::vec3 newPosition);
};

#endif