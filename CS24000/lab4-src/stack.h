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

