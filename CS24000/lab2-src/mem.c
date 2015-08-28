
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mymemdump(FILE *fd, char * p , int len);

struct X{
  char a;
  int i;
  char b;
  int *p;
};

struct List {
  char * str;
  struct List * next;
};

int
main() {
  char str[20];
  int a = 5;
  int b = -5;
  double y = 12;
  struct X x;
  struct List * head;

  x.a = 'A';
  x.i = 9;
  x.b = '0';
  x.p = &x.i;
  strcpy(str, "Hello world\n");
  printf("str = %s , &str=0x%lX\n", str, (unsigned long)&str);
  printf("a = %d, &a=0x%lX\n", a, (unsigned long)&a);
  printf("b = %d, &b=0x%lX\n", b, (unsigned long)&b);
  printf("y = %f, &y=0x%lX\n", y, (unsigned long)&y);
  printf("x.a = %c &x.a=0x%lX\n", x.a, (unsigned long)&x.a);
  printf("x.i = %d &x.i=0x%lX\n", x.i, (unsigned long)&x.i);
  printf("x.b = %c &x.b=0x%lX\n", x.b, (unsigned long)&x.b);
  printf("x.p = 0x%lX\n", (unsigned long)&x.p);

  mymemdump(stdout, (char *) &x.a, 64);
  head = (struct List *) malloc(sizeof(struct List));
  head->str=strdup("Welcome ");
  head->next = (struct List *) malloc(sizeof(struct List));
  head->next->str = strdup("to ");
  head->next->next = (struct List *) malloc(sizeof(struct List));
  head->next->next->str = strdup("cs250");
  head->next->next->next = NULL;
  printf("head=0x%lX\n", (unsigned long) head);
  mymemdump(stdout, (char*) head, 256);

  printf("&head = 0x%lX\n", (unsigned long) head);
  printf("&head->str = 0x%lX\n", (unsigned long) head->str);
  printf("&head->next  = 0x%lX\n", (unsigned long) head->next);
  printf("&head->next->str  = 0x%lX\n", (unsigned long) head->next->str);
  printf("&head->next->next  = 0x%lX\n", (unsigned long) head->next->next);
  printf("&head->next->next->str = 0x%lX\n", (unsigned long) head->next->next->str);
  printf("&head->next->next->next = 0x%lX\n", (unsigned long) head->next->next->next);
  printf("head = 0x%lX\n", (unsigned long) &head);
  printf("head->str = 0x%lX\n", (unsigned long) &head->str);
  printf("head->next  = 0x%lX\n", (unsigned long) &head->next);
  printf("head->next->str  = 0x%lX\n", (unsigned long) &head->next->str);
  printf("head->next->next  = 0x%lX\n", (unsigned long) &head->next->next);
  printf("head->next->next->str = 0x%lX\n", (unsigned long) &head->next->next->str);
  printf("head->next->next->next = 0x%lX\n", (unsigned long) &head->next->next->next);
}

