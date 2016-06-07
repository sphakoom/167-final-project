#include "Plant.h"
#include "OBJObject.h"
Plant::Plant(string start, int generations, float moveLength, float turnAngle, float thickness){
	//string start = "F";
	ls = new LSystem(start);
	ts = new TurtleSystem();
	//ls.addrule("X", "F[+X][-X]FX");
	//ls.addrule("F", "FF");
	//ls->addrule("F","F[+F]F[-F]F");
	//ls.addrule("B", "A[B]B");
	//fprintf(stderr, "Generating buffer\n");
	this->start = start;
	this->generations = generations;
	this->moveLength = moveLength;
	this->turnAngle = turnAngle;
	this->thickness = thickness;
}

void Plant::generate() {
	ls->generate(5);
	parse(ls->lst.list2string(ls->buffer));
}

void Plant::parse(string ops) {
	char op;
	// Loop through each of the ops
	ts->setThickness(thickness);
	for (int i = 0; i < ops.size(); i++) {
		op = ops[i];
		//fprintf(stderr, "Operation: %c\n", op);
		switch (op) {
			case 'F': // Draw Cylinder at 
				ts->drawLeaf();
				ts->move(moveLength);
				//ts.turnRight(45.0f);
				//ts.thicken(1.0f);
				break;
			case 'f': // move to the next position w/o drawing
				ts->move(moveLength);
				break;
			case '+': // turn left
				ts->turnLeft(turnAngle);
				break;
			case '-': // Rotate counter-clk wise
				ts->turnRight(turnAngle);
				break;
			case '|': // rotate 180 degrees
				ts->turn180(1.0f);
				break;
			case '[': // Push state
				ts->save();
				//ts.move(-0.5f);
				//ts.turnLeft(45.0f);
				//ts.move(.09f);
				break;
			case ']': // Pop state
				ts->restore();
				//ts.move(-.6f);
				//ts.turnRight(45.0f);
				 //ts.move(0.09f);
				break;
			case '&':
				ts->pitchDown(turnAngle);
				break;
			case '^':
				ts->pitchUp(turnAngle);
				break;
			case '\\':
				ts->rollLeft(turnAngle);
				break;
			case '/':
				ts->rollRight(turnAngle);
				break;
			case '!': // Decrement the diameter of the leaf
				break;
			case '(': // Increase rotation angle by 1.1
				break;
			case ')': // Reduce rotation angle by .9
				break;
			case '<': // Scale length by 1.1
				ts->thicken(1.1f);
				break;
			case '>': // Scale length by .9
				ts->thicken(.9f);
				break;
			default:
				fprintf(stderr, "Unrecognized operation: %c", op);
				break;
		}
	}
}


void Plant::draw(GLuint shader) {
	for (Drawable *o : ts->objs) {
		OBJObject * obj = ((OBJObject*)o);
		glUniform3f(glGetUniformLocation(shader, "plantPos"),obj->toWorld[3].x, obj->toWorld[3].y, obj->toWorld[3].z);
		obj->draw(shader,-1);
	}
}