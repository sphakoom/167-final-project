#pragma once
#include <stack>
#include <vector>
#include "Drawable.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtx/rotate_vector.hpp>

#ifndef _TurtleSystem_H_
#define _TurtleSystem_H_

using namespace std;
struct Turtle
{
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 right;
	float thickness;
	float reduction;
	Turtle();
	void turnRight(float angle);
	void turnLeft(float angle);
	void pitchDown(float angle);
	void pitchUp(float angle);
	void rollLeft(float angle);
	void rollRight(float angle);
	void move(float distance);
	void turn180(float temp);
	void thicken(float param);
	void narrow(float param);
	void setThickness(float param);
	void setReduction(float param);


};
class TurtleSystem : public Turtle
{
public:
	vector<Drawable*> objs;
	stack<Turtle> state;
public:
	TurtleSystem();
	void save();
	void restore();
	void draw();
	void drawLeaf();

};
#endif