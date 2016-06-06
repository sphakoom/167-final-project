#pragma once
#include <string>
using namespace std;

struct LNode {
	LNode *next;
	LNode *last;
	string data;
};

class LinkedList
{
public:
	LinkedList();
	//~LinkedList();
	LNode * replace(LNode *head1, LNode *head2);
	LNode * insert(LNode *head, string data);
	LNode *string2list(string data);
	string list2string(LNode * head);
	LNode *fastforward(LNode * head, int len);
	void print(LNode * head);
};
