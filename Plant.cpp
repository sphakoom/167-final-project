#include "Plant.h"
TurtleSystem ts = TurtleSystem();
Plant::Plant(){
	string start = "B";
	LSystem ls = LSystem(start);
	ls.addrule("A","A<A");
	ls.addrule("B", "AA[B]B");
	//fprintf(stderr, "Generating buffer\n");
	ls.generate(7);
	//ls.printbuffer();
	parse(ls.lst.list2string( ls.buffer));

}

vector<glm::vec3> Plant::parse(string ops) {
	vector<glm::vec3> res = vector<glm::vec3>();
	char op;
	// Loop through each of the ops
	for (int i = 0; i < ops.size(); i++) {
		op = ops[i];
		//fprintf(stderr, "Operation: %c\n", op);
		switch (op) {
			case 'A': // Draw Cylinder at 
				ts.drawLeaf();
				ts.move(0.5f);
				//ts.turnRight(45.0f);
				//ts.thicken(1.0f);
				break;
			case 'B': // move to the next position w/o drawing
				ts.drawLeaf();
				ts.move(0.5f);
				break;
			case '+': // Rotate clk wise
				ts.turnRight(1.0f);
				break;
			case '-': // Rotate counter-clk wise
				ts.turnLeft(1.0f);
				break;
			case '|': // rotate 180 degrees
				ts.turn180(1.0f);
				break;
			case '[': // Push state
				ts.save();
				ts.turnLeft(45);
				break;
			case ']': // Pop state
				ts.restore();
				ts.turnRight(45);
				break;
			case '!': // Reverse rotation angle
				break;
			case '(': // Increase rotation angle by 1.1
				break;
			case ')': // Reduce rotation angle by .9
				break;
			case '<': // Scale length by 1.1
				ts.thicken(1.1f);
				break;
			case '>': // Scale length by .9
				ts.thicken(.9f);
				break;
			default:
				fprintf(stderr, "Unrecognized operation: %c", op);
				break;
		}
	}

	return res;
}


void Plant::draw(GLuint shader) {
	for (Drawable *o : ts.objs) {
		((OBJObject*)o)->draw(shader,0);
	}
}