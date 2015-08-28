#include <stdio.h>

int average(int l, int h) {
	return (l + h) / 2;
}

main(int argc, char **argv) {
	int l, m, h;
  printf("Welcome to the High Low game...\n");
  // Write your implementation here...
  while (1) {
    l = 1;
    h = 100;
  	printf("Think of a number between 1 and 100 and press press <enter>");
  	getchar();
  	while (1) {
      m = average(l, h);
  		if (h < l) {
  			printf("\n>>>>>> The number is %d\n\n", m + 1);
  			break;
  		}
  		printf("Is it higher than %d? (y/n)\n", m);
        char answer = getchar();
  		if (answer == 'y') {
  			l = m + 1;
  		} else if (answer == 'n') {
  			h = m - 1;
  		} else {
        printf("Type y or n\n");
      }
      getchar();
  	}
  	printf("Do you want to continue playing (y/n)?");
  	if (getchar() == 'n') {
  		break;
  	}
    getchar();
 	}
 	printf("Thanks for playing!!!\n");
}

