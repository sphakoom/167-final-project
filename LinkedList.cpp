#include "LinkedList.h"
LinkedList::LinkedList() {}

LNode * LinkedList::replace(LNode *head1, LNode *head2) {
	
	
	if (head1->last != NULL) {
		LNode * last = head1->last;
		last->next = head2;
		head2->last = last;
	}
	if (head1->next != NULL) {
		LNode * curr = head2;
		while (curr->next != NULL) {
			curr = curr->next;
		}
		LNode * next = head1->next;
		curr->next = next;
		next->last = curr;
	}
	
	return head2;
}
LNode * LinkedList::insert(LNode *head, string data) {
	if (head == NULL) {
		//fprintf(stderr, "Head is null for: %s", data.c_str());
		head = new LNode();
		head->data = data;
	} else {
		LNode * curr = head;
		while (curr->next != NULL) {
			curr = curr->next;
		}
		//fprintf(stderr, "Tail node: %s", curr->data.c_str());
		curr->next = new LNode();
		curr->next->last = curr;
		curr->next->data = data;
	}
	return head;
}

LNode * LinkedList::string2list(string data) {
	LNode * head = NULL;
	for (int i = 0; i < data.size(); i++) {
		head = insert(head, string(1,data[i]));
	}
	return head;
}

string LinkedList::list2string(LNode * head) {
	LNode * curr = head;
	string res = string();
	while (curr != NULL) {
		// Might have to change if there is more than 1 c in a data
		res.push_back(curr->data[0]);
		curr = curr->next;
	}
	return res;
}

LNode * LinkedList::fastforward(LNode * head, int len) {
	LNode * curr = head;
	while (curr != NULL && len > 0) {
		curr = curr->next;
		len--;
	}
	return curr;
}

void LinkedList::print(LNode * head) {
	LNode * curr = head;
	while (curr != NULL) {
		fprintf(stderr, "Value: %s\n", curr->data.c_str());
		curr = curr->next;
	}
}


/*
LinkedList lst = LinkedList();
LNode * head = NULL;
head = lst.insert(head, "p");
head = lst.insert(head, "i");
head = lst.insert(head, "z");
head = lst.insert(head, "z");
head = lst.insert(head, "a");
LNode * beef = lst.string2list("beef");
beef = lst.replace(head->next->next, beef);
beef = lst.fastforward(beef, 4);
lst.print(beef);
 */