#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// Initializes a new stack data structure
STACK * stack_create() {
	STACK * stack = (STACK *)malloc(sizeof(STACK));
	stack->top = NULL;
}

// Adds an element to the stack
void stack_push(STACK * stack, double val) {
	NODE * temp = (NODE *)malloc(sizeof(NODE));
	temp->val = val;

	if (stack_is_empty(stack)) {
		stack->top = temp;
		stack->top->prev = NULL;
	} else {
		temp->prev = stack->top;
		stack->top = temp;
	}
}

// Removes the element at the top of the stack
void stack_pop(STACK * stack) {
	NODE * temp = (NODE *)malloc(sizeof(NODE));
	if (stack_is_empty(stack)) {
		printf("Cannot Pop Empty Stack\n");
	} else {
		temp = stack->top->prev;
		free(stack->top);
		stack->top = temp;
	}
}

// Checks if the stack is empty and returns 1
// Return 0 if stack is not empty
int stack_is_empty(STACK * stack) {
	if (stack->top == NULL) {
		return 1;
	}
	return 0;
}

// Prints all the elements in the stack starting from the top
void stack_print(STACK * stack) {
	printf("** PRINTING STACK **\n");
	NODE * temp = stack->top;
	while (temp != NULL) {
		printf("Value: %f\n", (double)temp->val);
		temp = temp->prev;
	}
	printf("\n");
}

/*
int main() {
	STACK * stack = stack_create();
	stack_print(stack);
	double d;
	d = 5.0;
	stack_push(stack, d);
	stack_print(stack);
	
	long i;
	for (i = 0; i < 10; i++) {
		printf("Pushing < %ld > on stack\n", i);
		stack_push(stack, (void *)i);
	}
	printf("\n");
	stack_print(stack);
	printf("Popping < %ld > off stack\n", (long)stack->top->val);
	stack_pop(stack);
	printf("Popping < %ld > off stack\n", (long)stack->top->val);
	stack_pop(stack);
	printf("\n");
	stack_print(stack);
	printf("Pushing < %c > on stack\n\n", '+');
	stack_push(stack, (void *)'+');
	stack_print(stack);
	
	while (stack->top != NULL) {
		printf("Popping < %ld > off stack\n", (long)stack->top->val);
		stack_pop(stack);
	}
	printf("\n");
	stack_print(stack);
	stack_pop(stack);
	printf("**** DONE TEST ****");
}*/

