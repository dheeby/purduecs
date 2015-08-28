#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 512

int main(int argc, char ** argv) {
	printf("Program to separate text to words. Type a string and ctrl-d to exit\n");
	int i;
	int wordCount = 0;
	char buf[MAXLINE];
	char * buf2 = (char *)malloc(sizeof(char) * MAXLINE);
	while (fgets(buf, MAXLINE, stdin)) {
		if (buf[strlen(buf) - 1] == '\n' && buf[0] != '\n') {
			for (i = 0; i < strlen(buf); i++) {
				if ((buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= 'A' && buf[i] <= 'Z') ||
					(buf[i]>='0' && buf[i]<='9') || buf[i]==',' || buf[i]=='.' || buf[i]=='\'' || 
					buf[i]=='?' || buf[i]==':' || buf[i]==';' || buf[i]=='-') {
					buf2[strlen(buf2)] = buf[i];
				}
				else if ((buf[i] == 32 && buf[i - 1] != 32) || (buf[i] == '\n')) {
					buf2[strlen(buf2)] = '\0'; 
					if (strcmp(buf2, "") == 0) {
						break;
					} else {
						printf("Word %d: %s\n", wordCount, buf2);
						memset(buf2, 0, strlen(buf2));
						wordCount++;
					}
				}
			}
		}
	}
	printf("words total = %d\n", wordCount);
	
}
