
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 80

// Generate Cellular Automaton strings based on 
// http://mathworld.wolfram.com/ElementaryCellularAutomaton.html

void printIndexToBits(unsigned int i) {
	printf("%d", (i << 29) >> 31);
	printf("%d", (i << 30) >> 31);
	printf("%d", (i << 31) >> 31);
}

main(int argc, char ** argv)
{
	if (argc < 2) {
		printf("Usage: life rule-number\n");
		exit(1);
	}
	

	// get rule number from argument
	unsigned int rule = atoi(argv[1]);

	printf("Rule number:%d\n", rule);

	if (rule > 255) {
		printf("Rule out of bounds. 0 <= rule-number < 256\n");
		exit(1);
	}

	// Convert rule number to array 
	int rules[8];
	int i;
	for (i = 0; i < 8; i++) {
		rules[i] = ((rule << 31 - i) >> 31);
	}

	// print rules
	
	printf("Rules:\n");

	for (i = 0; i < 8; i++) {
		printIndexToBits(i);
		printf("=>%d\n", rules[i]);
	}


	// Initialize initial string. A string of 80 spaces with a * in the middle.
	int * currentLine = malloc(sizeof(int) * MAXLINE);
	int * nextLine = malloc(sizeof(int) * MAXLINE);
	
	for (i = 0; i < MAXLINE; i++) {
		currentLine[i] = 0;
	}

	currentLine[40] = 1;
	nextLine[0] = 0;
	nextLine[79] = 0;

	// Generate 40 subsequent strings based on rules
	int m, n, p;
	for (m = 0; m <= 40; m++) {
		for (p = 0; p < MAXLINE; p++) {
			if (currentLine[p] == 1) {
				printf("*");
			}
			else {
				printf(" ");
			}
		}

		for (n = 1; n < 79; n++) {
			int index = (currentLine[n - 1] * 4) + (currentLine[n] * 2) + (currentLine[n + 1] * 1);
			nextLine[n] = rules[index];
		}

		for (i = 0; i < MAXLINE; i++) {
			currentLine[i] = nextLine[i];
		}

		printf("\n");
	}
	exit(0);
}
