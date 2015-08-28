
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "resizable_table.h"

//
//
// It returns a new RESIZABLE_TABLE. It allocates it dynamically,
// and initializaes the values. The initial maximum size of the array is 10.
//
RESIZABLE_TABLE * rtable_create() {

	// Allocate a RESIZABLE_TABLE
	RESIZABLE_TABLE * table = (RESIZABLE_TABLE *)
	  malloc(sizeof(RESIZABLE_TABLE));
	if (table == NULL) {
		return NULL;
	}
	
	// Initialze the members of RESIZABLE_TABLE
	table->maxElements = INITIAL_SIZE_RESIZABLE_TABLE;
	table->currentElements = 0;
	table->array = (struct RESIZABLE_TABLE_ENTRY *)
		malloc(table->maxElements*sizeof(RESIZABLE_TABLE_ENTRY));
	if (table->array==NULL) {
		return NULL;
	}
	
	return table;
}

//
// It prints the elements in the array assuming the value is a string in the form:
//
//======== Table =======
//currentElements=2 maxElements=10
//0: "George"       "23 Oak St, West Lafayette, 47906"
//1: "Peter"        "27 Oak St, West Lafayette, 47906"
//======== End Table =======
//
void rtable_print_str(RESIZABLE_TABLE * table)
{
	int i = 0;
	printf("\n======== Table =======\n");
	printf("currentElements=%d maxElements=%d\n", table->currentElements,
	       table->maxElements);
	for (i=0; i<table->currentElements; i++) {
		printf("%d: \"%s\" \"%s\"\n", i, table->array[i].name,
		       (char*) table->array[i].value);
	}
	printf("======== End Table =======\n");
	return;
}

//
// It prints the elements in the array assuming the value is an int:
//
//======== Table =======
//currentElements=2 maxElements=10
//0: "George"               101
//1: "Peter"                245
//======== End Table =======
//
void rtable_print_int(RESIZABLE_TABLE * table)
{
	int i = 0;
	printf("\n======== Table =======\n");
	printf("currentElements=%d maxElements=%d\n", table->currentElements,
	       table->maxElements);
	for (i=0; i<table->currentElements; i++) {
		printf("%d: \"%s\" %ld\n", i, table->array[i].name,
		       (long) table->array[i].value);
	}
	printf("======== End Table =======\n");
	return;
}

//
// Adds one pair name/value to the table. If the name already exists it will
// Substitute its value. Otherwise, it will store name/value in a new entry.
// If the new entry does not fit, it will double the size of the array.
// The name string is duplicated with strdup() before assigning it to the
// table.
//
int rtable_add(RESIZABLE_TABLE * table, char * name, void * value) {
	// Find if it is already there and substitute value
	int i;
	for (i = 0; i < table->currentElements; i++) {
		if (strcmp(table->array[i].name, name) == 0) {
			table->array[i].value = value;
			return 0;
		}
	}
 
	// If we are here is because the entry was not found.
	// Make sure that there is enough space
	if (table->currentElements == table->maxElements) {
		size_t n = table->maxElements * sizeof(RESIZABLE_TABLE_ENTRY);
		struct RESIZABLE_TABLE_ENTRY * rte = (struct RESIZABLE_TABLE_ENTRY *)
			malloc(n * 2);
		memcpy(rte, table->array, n);
		free(table->array);
		table->array = rte;
		table->maxElements = table->maxElements * 2;
	}
	//
	// Add name and value to a new entry.
	// We need to use strdup to create a copy of the name but not value.
	//
	table->array[table->currentElements].name = strdup(name);
	table->array[table->currentElements].value = value; 
	table->currentElements++;

	return 0;
}

//
// Add name and value into table where value is a string (char *)
// Implement on top of rtable_add.
//
int rtable_add_str(RESIZABLE_TABLE * table, char * name, char * str_value) {
	return rtable_add(table, name, (void *) strdup(str_value));
}

//
// Add name and value into table where value is a long
// Implement on top of rtable_add.
//
int rtable_add_int(RESIZABLE_TABLE * table, char * name, long int_value) {  
	return rtable_add(table, name, (void *) int_value );
}

//
// Returns the value that correspond to the name or NULL if the
// name does not exist in the table.
//
void * rtable_lookup(RESIZABLE_TABLE * table, char * name) {
	int i;
	for (i = 0; i < table->currentElements; i++) {
		if (strcmp(table->array[i].name, name) == 0)
			return table->array[i].value;
	}
	return NULL;
}

//
// It removes the entry with that name from the table. The entries after the entry
// removed will shift downwards. Also the name and value strings will be freed.
//
int rtable_remove(RESIZABLE_TABLE * table, char * name) {
	int i, j;
	for (i = 0; i < table->currentElements; i++) {
		if (strcmp(table->array[i].name, name) == 0) {
			free(table->array[i].name);
			free(table->array[i].value);
			table->currentElements--;
			for (j = i; j < table->currentElements; j++) {
				table->array[j] = table->array[j + 1];
			}
			return 1;
		}
	}
	return 0;
}

//
// It returns in *name and *value the name and value that correspond to
// the ith entry. It will return 1 if successful, or 0 otherwise.
//
int rtable_get_ith(RESIZABLE_TABLE * table, int ith, char ** name, void ** value) {
	if (table->array[ith].name != NULL) {
		*name = table->array[ith].name;
		*value = table->array[ith].value;
		return 1;
	}
	return 0;
}

//
// It removes the ith entry from the table. The entries after the entry removed are
// moved downwards to use the empty space. Also the name/value strings are freed.
//
int rtable_remove_ith(RESIZABLE_TABLE * table, int ith) {
	if (table->array[ith].name != NULL) {
		free(table->array[ith].name);
		free(table->array[ith].value);
		int j;
		for (j = ith; j < table->currentElements; j++) {
			table->array[j] = table->array[j + 1];
		}
		table->currentElements--;
		return 1;
	}
	return 0;
}

//
// It returns the number of elements in the table.
//
int rtable_number_elements(RESIZABLE_TABLE * table) {
	return table->currentElements;
}

//
// It returns the maximum number of elements in the table
//
int rtable_max_elements(RESIZABLE_TABLE * table) {
	return table->maxElements;
}

#define MAXLINE 512

//
// It saves the table in a file called file_name with values as char *. The format of the
// file is as follows:
//
// name1\n
// str1\n
// \n
// name2\n
// str2\n
// ...
//
// Notice that there is an empty line between each name/value pair.
//
int rtable_save_str(RESIZABLE_TABLE * table, char * file_name) {
	int i;
	FILE *f;
	f = fopen(file_name, "w");
	if (f == NULL) return 0;
	for (i = 0; i < table->currentElements; i++) {
		fputs(table->array[i].name, f);
		fputc('\n', f);
		fputs(table->array[i].value, f);
		fputc('\n', f);
		fputc('\n', f);
	}
	fclose(f);
	return 1;
}

//
// It reads the table from the file_name indicated asumiing that the values are
// char *. If the table already has entries, it will clear the entries.
//
int rtable_read_str(RESIZABLE_TABLE * table, char * file_name) {
	int i;
	FILE *f;
	f = fopen(file_name, "r");
	if (f == NULL) return 0;
	if (table->currentElements > 0) {
		for (i = 0; i < table->currentElements; i++) {
			free(table->array[i].name);
			free(table->array[i].value);
		}
		table->currentElements = 0;
	}
	char buf1[MAXLINE];
	char buf2[MAXLINE];
	char buf3[MAXLINE];
	while (fgets(buf1, MAXLINE, f)){
		char * p;
		if ((p = strchr(buf1, '\n')) != NULL) *p = '\0';
		fgets(buf2, MAXLINE, f);
		if ((p = strchr(buf2, '\n')) != NULL) *p = '\0';
		fgets(buf3, MAXLINE, f);
		rtable_add_str(table, buf1, buf2);
	}	
	fclose(f);
	return 1;
}

//
// It saves the table in a file called file_name with values as int. The format of the
// file is as follows:
//
// name1\n
// int1\n
// \n
// name2\n
// int2\n
// ...
//
// Notice that there is an empty line between each name/value pair.
//
int rtable_save_int(RESIZABLE_TABLE * table, char * file_name) {
	int i;
	FILE *f;
	f = fopen(file_name, "w");
	if (f == NULL) return 0;
	
	for (i = 0; i < table->currentElements; i++) {
		fprintf(f, "%s\n%ld\n\n", table->array[i].name, (long)table->array[i].value);
	}
	
	fclose(f);
	return 1;
}

//
// It reads the table from the file_name indicated asumiing that the values are
// char *. If the table already has entries, it will clear the entries.
//
int rtable_read_int(RESIZABLE_TABLE * table, char * file_name) {
	int i;
	FILE *f;
	f = fopen(file_name, "r");
	if (f == NULL) return 0;

	if (table->currentElements > 0) {
		for (i = 0; i < table->currentElements; i++) {
			free(table->array[i].name);
			free(table->array[i].value);
		}
		table->currentElements = 0;
	}

	char newName[MAXLINE];
	long newValue;

	while(fscanf(f, "%s\n%ld", newName, &newValue) > 0 ){
		rtable_add_int(table, newName, newValue);

	}	

	fclose(f);
	return 1;
}

//
// It sorts the table according to the name. The parameter ascending determines if the
// order si ascending (1) or descending(0).
//
void rtable_sort(RESIZABLE_TABLE * table, int ascending) {
	int i, j;
	if (ascending) {
		for (i = 0; i < table->currentElements - 1; i++) {
			for (j = i + 1; j < table->currentElements; j++) {
				if (strcmp(table->array[j].name,table->array[i].name) < 0) {
					struct RESIZABLE_TABLE_ENTRY temp = table->array[i];
					table->array[i] = table->array[j];
					table->array[j] = temp;
				}
			}
		}	
	} else {
		for (i = 0; i < table->currentElements - 1; i++) {
			for (j = i + 1; j < table->currentElements; j++) {
				if (strcmp(table->array[j].name, table->array[i].name) > 0) {
					struct RESIZABLE_TABLE_ENTRY temp = table->array[i];
					table->array[i] = table->array[j];
					table->array[j] = temp;
				}
			}
		}
	}
}

//
// It sorts the table according to the name. The parameter ascending determines if the
// order si ascending (1) or descending(0).
//
void rtable_sort_by_intval(RESIZABLE_TABLE * table, int ascending) {
	int i, j, k;
	long a, b;
	struct RESIZABLE_TABLE_ENTRY tmp;
	if (ascending) {
		i = 0;
		while (i < table->currentElements) {
			for (j = i + 1; j < table->currentElements; j++) {
				a = (long)(table->array[i].value);
				b = (long)(table->array[j].value);
				if (b < a) {
					tmp = table->array[j];
					for (k = j; k > i; k--) {
						table->array[k] = table->array[k - 1];
					}
					table->array[i] = tmp;
				}
			}
			i++;
		}
		/*
		for (i = 0; i < table->currentElements -1; i++) {
			for (j = i + 1; j < table->currentElements; j++) {
				a = (long)(table->array[j].value);
				b = (long)(table->array[i].value);
				if (a < b) {
					struct RESIZABLE_TABLE_ENTRY temp = table->array[i];
					table->array[i] = table->array[j];
					table->array[j] = temp;
				}
			}
		}*/
	} else {
		i = 0;
		while (i < table->currentElements) {
			for (j = i + 1; j < table->currentElements - 1; j++) {
				a = (long)(table->array[i].value);
				b = (long)(table->array[j].value);
				if (b > a) {
					tmp = table->array[j];
					for (k = j; k > i; k--) {
						table->array[k] = table->array[k - 1];
					}
					table->array[i] = tmp;
				}
			}
			i++;
		}
	}
}

//
// It removes the first entry in the table.
// All entries are moved down one position.
// It also frees memory allocated for name and value.
//
int rtable_remove_first(RESIZABLE_TABLE * table) {
	if (table->currentElements > 0) {
		free(table->array[0].name);
		free(table->array[0].value);
		int j;
		for (j = 0; j < table->currentElements - 1; j++) {
			table->array[j] = table->array[j + 1];
		}
		table->currentElements--;
		return 1;
	}
	return 0;
}

//
// It removes the last entry in the table.
// It also frees memory allocated for name and value.
//
int rtable_remove_last(RESIZABLE_TABLE * table) {
	if (table->currentElements > 0) {
		free(table->array[table->currentElements - 1].name);
		free(table->array[table->currentElements - 1].value);
		table->currentElements--;
		return 1;
	}
	return 0;
}


//
// Insert a name/value pair at the beginning of the table.
// The entries are moved one position upwards.
// There is no check if the name already exists. The entry is added
// at the beginning of the table.
//
int rtable_insert_first(RESIZABLE_TABLE * table, char *name, void * value) {
	int j;
	if (table->currentElements == table->maxElements) {
		size_t n = table->maxElements * sizeof(RESIZABLE_TABLE_ENTRY);
		struct RESIZABLE_TABLE_ENTRY * rte = (struct RESIZABLE_TABLE_ENTRY *)
			malloc(n * 2);
		memcpy(rte, table->array, n);
		free(table->array);
		table->array = rte;
		table->maxElements = table->maxElements * 2;
	}
	for (j = table->currentElements; j > 0; j--) {
		table->array[j].name = table->array[j - 1].name;
		table->array[j].value = table->array[j - 1].value;
	}
	table->array[0].name = strdup(name);
	table->array[0].value = value;
	table->currentElements++;
	return 1;
}

//
// Insert a name/value pair at the end of the table.
// The entries are moved one position upwards.
// There is no check if the name already exists. The entry is added
// at the end of the table.
//
int rtable_insert_last(RESIZABLE_TABLE * table, char *name, void * value) {
	if (table->currentElements == table->maxElements) {
		size_t n = table->maxElements * sizeof(RESIZABLE_TABLE_ENTRY);
		struct RESIZABLE_TABLE_ENTRY * rte = (struct RESIZABLE_TABLE_ENTRY *)
			malloc(n * 2);
		memcpy(rte, table->array, n);
		free(table->array);
		table->array = rte;
		table->maxElements = table->maxElements * 2;
	}
	table->array[table->currentElements].name = strdup(name);
	table->array[table->currentElements].value = value;
	table->currentElements++;
	return 1;
}

