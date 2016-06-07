#pragma once
#ifndef _Plant_H_
#define _Plant_H_
#include "TurtleSystem.h"
#include <string>
#include "LSystem.h"


class Plant
{
public:
	Plant(string start, int generations, float moveLength, float turnAngle, float thickness);
	//~Plant();
	LSystem *ls;
	TurtleSystem *ts;
	float turnAngle;
	float moveLength;
	float thickness;

	int generations = 0;
	string start;

	
	void generate();
	void parse(string ops);
	void draw(GLuint shader);
	void update();
};

#endif

