// Stack Implementation

// Used for cout << "string"
#include <iostream>

using namespace std;

#include "Stack.h"

// Constructor
Stack::Stack(int maxStack) {
	this->maxStack = maxStack;
	stack = new double[maxStack];
	top = 0;
}

bool
Stack::push(double value) {
	if (top == maxStack) {
		return false;
	}
	stack[top++] = value;
	return true;
}

bool
Stack::pop(double & value) {
	if (top == 0) {
		return false;
	}
	top--;
	value = stack[top];
	return true;
}

int
Stack::getTop() const {
	return top;
}

int
Stack::getMaxStack() const {
	return maxStack;
}

void
Stack::print() const {
	cout << "Stack:" << endl;
	for (int i = 0; i < top; i++) {
		cout << i << ":" << stack[i] << endl;
	}
	cout << "------" << endl;
}

// Destructor
Stack::~Stack() {
	delete [] stack;
}

