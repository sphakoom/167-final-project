#include "LSystem.h"

LSystem::LSystem(string start) {
	buffer = new LNode();
	buffer->data = start;
	this->start = start;
	lst = LinkedList();
	rules = map<string, string>();
}

void LSystem::generate(int n) {
	// Should probably delete the list that is buffer
	buffer = new LNode();
	buffer->data = start;
	for (int i = 0; i < n; i++) {
		iterate();
	}
	//printbuffer();
}

void LSystem::printbuffer() {
	LNode * curr = buffer;
	while (curr != NULL) {
		fprintf(stderr, "%s", curr->data.c_str());
		curr = curr->next;
	}
}

void LSystem::addrule(string key, string value) {
	rules.insert(make_pair(key, value));
}

void LSystem::iterate() {
	// Loop through each item in buffer
	LNode * curr = buffer;
	string rule;
	while (curr != NULL) {
		// Take the data and check if there is a rule for it
		string v = curr->data;
		//fprintf(stderr, "Checking rules for: %s\n", v.c_str());
		if (rules.find(v) != rules.end()) {
			rule = rules.find(v)->second;
			//fprintf(stderr, "Found rule: %s\n", rule.c_str());
			// Replace the curr LNode with a new list that represents the rule
			curr = lst.replace(curr, lst.string2list(rule)); // returns the head of list so we have to forward rule.size()
			//fprintf(stderr, "New head is: %s\n", curr->data.c_str());
			curr = lst.fastforward(curr, rule.size() - 1);
			if (curr->next == NULL) {
				LNode * temp = curr;
				while (temp->last != NULL) {
					temp = temp->last;
				}
				buffer = temp;
			}
			curr = curr->next;
		} else { // didnt recognize key
			//fprintf(stderr, "Unrecognized key: %s\n", v.c_str());
			curr = curr->next;
		}
	}
}