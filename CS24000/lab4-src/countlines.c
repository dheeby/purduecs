#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv) {
	char c;
	int count = 0;
	
	printf("Program to count lines. Type a string and ctrl-d to exit\n");

	while ((c = getchar()) != EOF) {
		if (c == '\n') count++;
	}

	printf("Total lines: %d\n", count);
	exit(0);
}
