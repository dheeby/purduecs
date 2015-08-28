
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

typedef struct NODE {
	double val;
	struct NODE * prev;
} NODE;

typedef struct STACK {
	struct NODE * top;
} STACK;

STACK * stack_create();
void stack_push(STACK * stack, double val);
void stack_pop(STACK * stack);
int stack_is_empty(STACK * stack);
void stack_print(STACK * stack);

 the stack
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

int is_operand(char * c) {
	if ((strcmp(c, "+") == 0) || (strcmp(c, "-") == 0) || (strcmp(c, "x") == 0) || (strcmp(c, "/") == 0) 
		|| (strcmp(c, "sin") == 0) || (strcmp(c, "cos") == 0) || (strcmp(c, "pow") == 0) 
		|| (strcmp(c, "log") == 0) || (strcmp(c, "exp") == 0)) {
		return 1;
	}
	return 0;
}

void compute_operation(STACK * stack, char * op) {
	double a = stack->top->val;
	stack_pop(stack);
	if (strcmp(op, "sin") == 0) {
		stack_push(stack, sin(a));
		return;
	}
	if (strcmp(op, "cos") == 0) {
		stack_push(stack, cos(a));
		return;
	}
	if (strcmp(op, "log") == 0) {
		stack_push(stack, log(a));
		return;
	}
	if (strcmp(op, "exp") == 0) {
		stack_push(stack, exp(a));
		return;
	}
	
	if (stack_is_empty(stack)) {
		printf("Elements remain in the stack.\n");
		exit(1);
	}
	double b = stack->top->val;
	stack_pop(stack);
	if (strcmp(op, "+") == 0) {
		stack_push(stack, (b + a));
		return;
	}
	if (strcmp(op, "-") == 0) {
		stack_push(stack, (b - a));
		return;
	}
	if (strcmp(op, "x") == 0) {
		stack_push(stack, (b * a));
		return;
	}
	if (strcmp(op, "/") == 0) {
		stack_push(stack, (b / a));
		return;
	}
	if (strcmp(op, "pow") == 0) {
		stack_push(stack, pow(b, a));
		return;
	}
}

int main(int argc, char ** argv)
{
	if (argc < 2) {
		printf("Usage: rpncalc op1 op2 ...\n");
		exit(1);
	}
	
	STACK * s = stack_create();

	int i;
	char * c;
	for (i = 1; i < argc; i++) {
		if (is_operand(c = argv[i])) {
			if (stack_is_empty(s)) {
				printf("Elements remain in the stack.\n");
				exit(1);
			} else {
				compute_operation(s, c);
			}
		} else {
			stack_push(s, atof(c));
		}
	}
	
	double result = s->top->val;
	stack_pop(s);
	if(stack_is_empty(s)) {
		printf("%f\n", result);
	} else {
		printf("Elements remain in the stack.\n");
	}

	exit(0);
}
