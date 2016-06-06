#pragma once
#ifndef _LSystem_H_
#define _LSystem_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <map>
#include <stdio.h>
#include <iostream>
#include <string>
#include "LinkedList.h"
using namespace std;
class LSystem
{
public:
	LSystem(string start);
	//~LSystem();
	string start;
	map <string, string> rules;
	LinkedList lst;
	LNode *buffer;
	void generate(int n);
	void iterate();
	void addrule(string key, string value);
	void printbuffer();
};

#endif

