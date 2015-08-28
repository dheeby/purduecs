
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mymemdump(FILE * fd, char * p , int len) {
    // Add your code here.
    // You may see p as an array.
    // p[0] will return the element 0 
    // p[1] will return the element 1 and so on

    int i, j;
    for (i = 0; i < len; i+=16) {
        fprintf(fd, "0x%016lX: ", (unsigned long) &p[i]);
        for (j = i; j < i + 16; j++) {
            int c = p[j]&0xFF;
            if (j < len) {
                fprintf(fd, "%02X ", c);
            } else {
                fprintf(fd, "   ");
            }
        }
        fprintf(fd, " ");
        for (j = i; j < i + 16; j++) {
            int c = p[j]&0xFF;
            if (j < len) {
                fprintf(fd, "%c", (c>=32)?c:'.');
            }
        }
        fprintf(fd, "\n");
    }
}

