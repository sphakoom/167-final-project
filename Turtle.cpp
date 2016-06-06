#include <iostream>
#include "Turtle.h"
Turtle::Turtle(){
	position = glm::vec3(0.0f);
	direction = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::vec3(1.0f, 0.0f, 0.0f);
	thickness = 1;
	reduction = 0.95f;

}
void Turtle::setReduction(float param)
{
	reduction = param / 100;
}
void Turtle::setThickness(float param)
{
	thickness = param / 100;
}
void Turtle::thicken(float param)
{
	thickness += thickness * param / 100;
}
void Turtle::narrow(float param)
{
	thicken(-param);
}
void Turtle::turnRight(float angle)
{
	// cout <<"Turning "<<angle<<" right"<<endl;
	//angle = angle + glm::pi<float>() / 180;
	glm::vec3 axis = direction;
	axis = glm::cross(axis, right);
	direction = glm::rotate(direction, angle, axis);
	right = glm::rotate(right, angle, axis);
	direction = glm::normalize(direction);
	right = glm::normalize(right);
}
void Turtle::turnLeft(float angle)
{
	turnRight(-angle);
}
void Turtle::pitchUp(float angle)
{
	// cout <<"Pitching "<<angle<<" up"<<endl;
	angle = angle* glm::pi<float>() / 180;
	glm::rotate(direction, angle, right);
	glm::normalize(direction);
}
void Turtle::pitchDown(float angle)
{
	pitchUp(-angle);
}
void Turtle::rollRight(float angle)
{
	// cout <<"Rolling "<<angle<<" right"<<endl;
	angle = angle* glm::pi<float>() / 180;
	glm::rotate(right, angle, direction);
	glm::normalize(right);
}
void Turtle::rollLeft(float angle)
{
	rollRight(-angle);
}
void Turtle::move(float distance)
{
	glm::vec3 t = direction;
	glm::normalize(t);
	position += distance*t;
}
void Turtle::turn180(float temp)
{
	turnRight(glm::pi<float>());
}
TurtleSystem::TurtleSystem()
{
	objs = vector<Drawable*>();
}
void TurtleSystem::save()
{
	Turtle t = (Turtle)*this;
	// for (int i=0;i<state.size();++i) cout <<"--";
	// cout <<"Saving state ("<<t.position.X()<<","<<t.position.Y()<<","<<t.position.Z()<<")-("
	//     <<t.direction.X()<<","<<t.direction.Y()<<","<<t.direction.Z()<<")"<<endl;
	state.push(t);
}
void TurtleSystem::restore()
{
	Turtle t = state.top();

	state.pop();
	// for (int i=0;i<state.size();++i) cout <<"--";
	// cout <<"Restoring state ("<<t.position.X()<<","<<t.position.Y()<<","<<t.position.Z()<<")-("
	// <<t.direction.X()<<","<<t.direction.Y()<<","<<t.direction.Z()<<")"<<endl;
	position = t.position;
	direction = t.direction;
	right = t.right;
	thickness = t.thickness;
	reduction = t.reduction;
	// (Turtle)*this=t; //FIXME: doesn't work, figure out why!
}
void TurtleSystem::drawLeaf()
{
	OBJObject * obj = new OBJObject("Cylinder.obj", "jade");
	obj->toWorld[3] = glm::vec4(position,1.0f);
	obj->toWorld = glm::scale(obj->toWorld, glm::vec3(thickness / 2.0f, 0.5f, thickness / 2.0f));
	//fprintf(stderr, "THE ANGLE IS: %f\n", glm::angle(glm::vec3(0.0f, 1.0f, 0.0f), direction));
	obj->toWorld = glm::rotate(obj->toWorld, glm::angle(glm::vec3(0.0f, 1.0f, 0.0f), direction), glm::vec3(0.0f, 1.0f, 0.0f));
	objs.push_back(obj);
}
void TurtleSystem::draw()
{

}