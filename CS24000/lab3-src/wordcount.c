
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resizable_table.h"

#define MAXLINE 512

void printUsage() {
	printf("Usage: wordcount [-w|-s] file\n");
}

void printWordsOnly(FILE * f) {
	char c;
	while ((c = fgetc(f)) != EOF) {
		if ((c > 64) && (c < 91)) {
			printf("%c", c + 32);
		}
		if ((c > 96) && (c < 123)) {
			printf("%c", c);
		}
		if ((c == 32) || (c == 10)) printf("\n");
	}
}

void addWordsTable(FILE * f, RESIZABLE_TABLE * rt) {
	char c;
	char * charBuf = (char *)malloc(sizeof(char) * 2);
	char * buf = (char *)malloc(MAXLINE);
	void * val;
	long num;
	while ((c = fgetc(f)) != EOF) {
		//charBuf[0] = c;
		charBuf[1] = '\0';
		if ((c > 64) && (c < 91)) {
			charBuf[0] = c + 32;
			strcat(buf, charBuf);
		}
		else if ((c > 96) && (c < 123)) {
			charBuf[0] = c;
			strcat(buf, charBuf);
		}
		else {// ((c == 32) || (c == 10)) {
			if (strcmp(buf, "") == 0) {
				continue;
			}
			else {
				val = rtable_lookup(rt, buf);
				if (val == NULL) {
					rtable_add_int(rt, buf, 1);
				} else {
					num = (long)val;
					num++;
					rtable_add_int(rt, buf, num);
				}
			}
			memset(buf, 0, strlen(buf));
		}
	}
}

int main(int argc, char **argv) {	
	if (argc < 2) {
		printUsage();
		exit(1);
	}
	
	FILE * f;
	RESIZABLE_TABLE * rt;
	rt = rtable_create();

	if ( argc == 2) {
		f = fopen(argv[1], "r");
		if (f == NULL) exit(0);

		addWordsTable(f, rt);
		rtable_sort(rt, 1);
		rtable_print_int(rt);
		exit(0);
	}

	if ( argc == 3) {
		f = fopen(argv[2], "r");
		if (f == NULL) exit(0);
		
		if (strcmp(argv[1], "-w") == 0) {
			printWordsOnly(f);
		}

		if (strcmp(argv[1], "-s") == 0) {
			addWordsTable(f, rt);
			rtable_sort_by_intval(rt, 0);
		}

		rtable_print_int(rt);
		exit(0);
	}

	exit(1);
}


