#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printUsage() {
	printf("Usage: convert <basefrom> <baseto> <number>\n");
}

// Converts the base as a string to an int
int baseToNum(char * base) {
	if (strlen(base) == 1) return (base[0] - 48);
	return (((base[0] - 48) * 10) + (base[1] - 48));
}

char * baseTenToTarget(char * num, int baseTo) {
	int i;
	char * result = (char *)malloc(sizeof(char) * 32);
	
}

char * toBaseTen(char * num) {
	int i;
	char * result = (char *)malloc(sizeof(char) * 32);
	for (i = 0; i < strlen(num); i++) {
		
	}
}

int main(int argc, char ** argv) {
	if (argc !=  4) {
		printUsage();
		exit(1);
	}
	char * baseFrom = argv[1];
	char * baseTo = argv[2];
	char * num = argv[3];

	printf("Base: %s = %d in base-10\n", baseTo, baseToNum(baseTo));
}
