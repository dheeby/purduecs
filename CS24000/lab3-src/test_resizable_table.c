
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "resizable_table.h"

void test1() {
	RESIZABLE_TABLE *rt;
	char * address;
	int result;

	rt = rtable_create();

	rtable_print_str(rt);

	printf("Add George\n");
	result = rtable_add_str(rt, "George", "23 Oak St, West Lafayette, 47906");

	rtable_print_str(rt);

	printf("Add Peter\n");
	result = rtable_add_str(rt, "Peter", "27 Oak St, West Lafayette, 47906");
	assert(result==0);

	rtable_print_str(rt);

	address = rtable_lookup(rt, "Peter");
	printf("Peter's address is: %s\n", address);

	address = rtable_lookup(rt, "George");
	printf("George's address is: %s\n", address);
}

void test2() {
	RESIZABLE_TABLE *rt;
	char * address;
	int result;

	rt = rtable_create();

	rtable_print_str(rt);

	printf("Add George\n");
	result = rtable_add_str(rt, "George", "23 Oak St, West Lafayette, 47906");

	rtable_print_str(rt);

	printf("Add Peter\n");
	result = rtable_add_str(rt, "Peter", "27 Oak St, West Lafayette, 47906");
	assert(result==0);

	rtable_print_str(rt);

	printf("Add George again\n");
	result = rtable_add_str(rt, "George", "289 Pine St, West Lafayette, 47906");

	rtable_print_str(rt);

	address = rtable_lookup(rt, "Peter");
	printf("Peter's address is: %s\n", address);

	address = rtable_lookup(rt, "George");
	printf("George's address is: %s\n", address);
}

void test3() {
	char name[20];
	char address[20];
	int i = 0;
	int result;
	RESIZABLE_TABLE *rt;

	rt = rtable_create();
	rtable_print_str(rt);

	for (i=0; i < 30; i++) {
		printf("Adding %d\n", i);
		sprintf(name,"name%d", i);
		sprintf(address, "address%d", i);
		result = rtable_add_str(rt, name, address);
	}
	rtable_print_str(rt);

}

void test4() {
	char name[20];
	char address[20];
	int i = 0;
	int result;
	RESIZABLE_TABLE *rt;

	rt = rtable_create();
	rtable_print_str(rt);

	for (i=0; i < 30; i++) {
		printf("Adding %d\n", i);
		sprintf(name,"name%d", i);
		sprintf(address, "address%d", i);
		result = rtable_add_str(rt, name, address );
	}
	rtable_print_str(rt);

	printf("remove name5\n");
	result = rtable_remove(rt, "name5");
	printf("result1=%d\n", result);

	rtable_print_str(rt);

	printf("remove name18\n");
	result = rtable_remove(rt, "name18");
	printf("result2=%d\n", result);

	rtable_print_str(rt);

	printf("remove name64\n");
	result = rtable_remove(rt, "name64");
	printf("result3=%d\n", result);

	rtable_print_str(rt);
}

void test5() {
	char name[20];
	char address[20];
	int i = 0;
	int result;
	RESIZABLE_TABLE *rt;
	char * name2, * value2;

	rt = rtable_create();
	rtable_print_str(rt);

	for (i=0; i < 30; i++) {
		printf("Adding %d\n", i);
		sprintf(name,"name%d", i);
		sprintf(address, "address%d", i);
		result = rtable_add_str(rt, name, address );
	}
	rtable_print_str(rt);

	// Get 11th value
	printf("Lookup 11th entry\n");
	result = rtable_get_ith(rt, 11, &name2, (void**) &value2);
	printf("For 11th value name=%s value=%s\n", name2, value2);
	printf("result1=%d\n", result);

	// Get 23th value
	printf("Lookup 23th entry\n");
	result = rtable_get_ith(rt, 23, &name2, (void **) &value2);
	printf("For 23th value name=%s value=%s\n", name2, value2);
	printf("result2=%d\n", result);

	// Get 64th value
	printf("Lookup 64th entry\n");
	result = rtable_get_ith(rt, 64, &name2, (void**) &value2);
	printf("result3=%d\n", result);
}

void test6() {
	char name[20];
	char address[20];
	int i = 0;
	int result;
	RESIZABLE_TABLE *rt;

	rt = rtable_create();
	rtable_print_str(rt);

	for (i=0; i < 30; i++) {
		printf("Adding %d\n", i);
		sprintf(name,"name%d", i);
		sprintf(address, "address%d", i);
		result = rtable_add_str(rt, name, address );
	}
	rtable_print_str(rt);

	printf("remove name in position 5\n");
	result = rtable_remove_ith(rt, 5);
	printf("result1=%d\n", result);

	rtable_print_str(rt);

	printf("remove name in position 23\n");
	result = rtable_remove_ith(rt, 23);
	printf("result2=%d\n", result);

	rtable_print_str(rt);

	printf("remove name in position 67\n");
	result = rtable_remove_ith(rt, 67);
	printf("result3=%d\n", result);

	rtable_print_str(rt);
}


void test7() {
	char name[20];
	char address[20];
	int i = 0;
	int result;
	RESIZABLE_TABLE *rt;

	rt = rtable_create();
	rtable_print_str(rt);

	for (i=0; i < 30; i++) {
		printf("Adding %d\n", i);
		sprintf(name,"name%d", i);
		sprintf(address, "address%d", i);
		result = rtable_add_str(rt, name, address );
	}
	rtable_print_str(rt);

	printf("number in table: %d, max: %d\n",
	       rtable_number_elements(rt), rtable_max_elements(rt));
}

void test8() {
	char name[20];
	char address[20];
	int i = 0;
	int result;
	RESIZABLE_TABLE *rt;

	rt = rtable_create();
	rtable_print_str(rt);

	for (i=0; i < 30; i++) {
		printf("Adding %d\n", i);
		sprintf(name,"name%d", i);
		sprintf(address, "address%d", i);
		result = rtable_add_str(rt, name, address );
	}
	rtable_print_str(rt);

	printf("Saving table /notallowed\n");
	result = rtable_save_str(rt, "/notallowed");
	printf("result1=%d\n", result);

	printf("Saving table mytable1.rt\n");
	result = rtable_save_str(rt, "mytable1.rt");
	printf("result2=%d\n", result);

	printf("----- cat mytable1.rt -----\n");
	system("cat mytable1.rt");
}

void test9() {
	int result;
	RESIZABLE_TABLE *rt;

	rt = rtable_create();
	rtable_print_str(rt);

	printf("Read non existing table\n");
	result = rtable_read_str(rt, "nonexsting.rt");
	printf("result1=%d\n", result);

	printf("Read friends.rt\n");
	result = rtable_read_str(rt, "friends.rt");
	printf("result2=%d\n", result);

	rtable_print_str(rt);
}

void test10() {
	int result;
	RESIZABLE_TABLE *rt;

	rt = rtable_create();
	rtable_print_str(rt);

	printf("Read non existing table\n");
	result = rtable_read_str(rt, "nonexsting.rt");
	printf("result1=%d\n", result);

	printf("Read friends.rt\n");
	result = rtable_read_str(rt, "friends.rt");
	printf("result2=%d\n", result);

	rtable_print_str(rt);

	printf("\nSort ascending\n");
	rtable_sort(rt,1);
	rtable_print_str(rt);

	printf("\nSort descending\n");
	rtable_sort(rt,0);
	rtable_print_str(rt);
}

void test11() {
	char name[20];
	char address[20];
	int i = 0;
	int result;
	RESIZABLE_TABLE *rt;

	rt = rtable_create();
	rtable_print_str(rt);

	result = rtable_remove_first(rt);
	printf("rtable_remove_first result=%d\n", result);

	result = rtable_remove_last(rt);
	printf("rtable_remove_last result=%d\n", result);
	
	for (i=0; i < 30; i++) {
		printf("Adding %d\n", i);
		sprintf(name,"name%d", i);
		sprintf(address, "address%d", i);
		result = rtable_add_str(rt, name, address);
	}
	rtable_print_str(rt);

	// Remove from one side or the other
	for (i=0; i < 10; i++) {
		result = rtable_remove_first(rt);
		printf("rtable_remove_first result=%d\n", result);
	}

	rtable_print_str(rt);

	for (i=0; i < 10; i++) {
		result = rtable_remove_last(rt);
		printf("rtable_remove_last result=%d\n", result);
	}

	rtable_print_str(rt);
	
}

void test12() {
	char name[20];
	char address[20];
	int i = 0;
	int result;
	RESIZABLE_TABLE *rt;

	rt = rtable_create();
	rtable_print_str(rt);

	for (i=0; i < 30; i++) {
		printf("Adding %d\n", i);
		sprintf(name,"name%d", i);
		sprintf(address, "address%d", i);
		result = rtable_add_str(rt, name, address);
	}
	rtable_print_str(rt);

	for (i=40; i < 50; i++) {
		printf("Adding first %d\n", i);
		sprintf(name,"name%d", i);
		sprintf(address, "address%d", i);
		result = rtable_insert_first(rt, name, (void*) strdup(address) );
	}
	rtable_print_str(rt);

	for (i=50; i < 60; i++) {
		printf("Adding %d\n", i);
		sprintf(name,"name%d", i);
		sprintf(address, "address%d", i);
		result = rtable_insert_last(rt, name, (void *) strdup(address) );
	}
	rtable_print_str(rt);

}

void test13() {
	RESIZABLE_TABLE *rt;
	long grade;
	int result;

	rt = rtable_create();

	rtable_print_int(rt);

	printf("Add George\n");
	result = rtable_add_int(rt, "George", 98 );

	rtable_print_int(rt);

	printf("Add Peter\n");
	result = rtable_add_int(rt, "Peter", 87 );
	assert(result==0);

	rtable_print_int(rt);

	grade = (long) rtable_lookup(rt, "Peter");
	printf("Peter's grade is: %ld\n", grade );

	grade = (long) rtable_lookup(rt, "George");
	printf("George's grade is: %ld\n", grade );
}

void test14() {
	RESIZABLE_TABLE *rt;
	long grade;
	int result;

	rt = rtable_create();

	rtable_print_int(rt);

	printf("Add George\n");
	result = rtable_add_int(rt, "George", 89);

	rtable_print_int(rt);

	printf("Add Peter\n");
	result = rtable_add_int(rt, "Peter", 97);
	assert(result==0);

	rtable_print_int(rt);

	printf("Add George again\n");
	result = rtable_add_int(rt, "George", 91);

	rtable_print_int(rt);

	grade =(long)  rtable_lookup(rt, "Peter");
	printf("Peter's grade is: %ld\n", grade);

	grade = (long) rtable_lookup(rt, "George");
	printf("George's grade is: %ld\n", grade);
}

void test15() {
	char name[20];
	long grade;
	int i = 0;
	int result;
	RESIZABLE_TABLE *rt;
	RESIZABLE_TABLE *rt2;

	rt = rtable_create();
	rtable_print_int(rt);

	for (i=0; i < 30; i++) {
		printf("Adding %d\n", i);
		sprintf(name,"name%d", i);
		grade = 70 + i;
		result = rtable_add_int(rt, name, grade );
	}
	rtable_print_int(rt);

	printf("Saving table /notallowed\n");
	result = rtable_save_int(rt, "/notallowed");
	printf("result1=%d\n", result);

	printf("Saving table grades1.rt\n");
	result = rtable_save_int(rt, "grades1.rt");
	printf("result2=%d\n", result);
	
	rt2 = rtable_create();
	result = rtable_read_int(rt2, "grades1.rt");

	rtable_print_int(rt2);
}

void test16() {
	int result;
	RESIZABLE_TABLE *rt;

	rt = rtable_create();
	rtable_print_int(rt);

	printf("Read non existing table\n");
	result = rtable_read_int(rt, "nonexsting.rt");
	printf("result1=%d\n", result);

	printf("Read friends.rt\n");
	result = rtable_read_int(rt, "friends_grades.rt");
	printf("result2=%d\n", result);

	rtable_print_int(rt);
}

int main(int argc, char ** argv) {

	char * test;
	
	if (argc <2) {
		printf("Usage: test_resizable_table test1|test2|...test16\n");
		exit(1);
	}

	test = argv[1];
	printf("Running %s\n", test);
	if (strcmp(test, "test1")==0) {
		test1();
	}
	else if (strcmp(test, "test2")==0) {
		test2();
	}
	else if (strcmp(test, "test3")==0) {
		test3();
	}
	else if (strcmp(test, "test4")==0) {
		test4();
	}
	else if (strcmp(test, "test5")==0) {
		test5();
	}
	else if (strcmp(test, "test6")==0) {
		test6();
	}
	else if (strcmp(test, "test7")==0) {
		test7();
	}
	else if (strcmp(test, "test8")==0) {
		test8();
	}
	else if (strcmp(test, "test9")==0) {
		test9();
	}
	else if (strcmp(test, "test10")==0) {
		test10();
	}
	else if (strcmp(test, "test11")==0) {
		test11();
	}
	else if (strcmp(test, "test12")==0) {
		test12();
	}
	else if (strcmp(test, "test13")==0) {
		test13();
	}
	else if (strcmp(test, "test14")==0) {
		test14();
	}
	else if (strcmp(test, "test15")==0) {
		test15();
	}
	else if (strcmp(test, "test16")==0) {
		test16();
	}
	else {
		printf("Test not found!!n");
		exit(1);
	}

	return 0;

}


