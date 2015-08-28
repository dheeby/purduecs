
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mymemdump(FILE * fd, char * p , int len) {
    // Add your code here.
    // You may see p as an array.
    // p[0] will return the element 0 
    // p[1] will return the element 1 and so on

    fprintf(fd, "0x%016lX: ", (unsigned long) p);
    int c = p[0]&0xFF;

    // Print first byte as hexadecimal
    fprintf(fd, "%02X ", c);

    // Print first byte as character
    fprintf(fd, "%c", (c>=32)?c:'.');
    fprintf(fd,"\n");
}

