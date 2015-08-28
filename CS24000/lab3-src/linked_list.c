
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"

//
// It returns a new LINKED_LIST. It allocates it dynamically,
// and initializaes the values. The initial list is empty.
//
LINKED_LIST * llist_create() {

	LINKED_LIST * list = (LINKED_LIST *) malloc(sizeof(LINKED_LIST));
	if (list == NULL) {
		return NULL;
	}

	// Create Sentinel node. This node does not store any data
	// but simplifies the list implementation.
	list->head = (LINKED_LIST_ENTRY *) malloc(sizeof(LINKED_LIST_ENTRY));
	if (list->head == NULL) {
		return NULL;
	}
	
	
	list->nElements = 0;
	list->head->next = list->head;
	list->head->previous = list->head;
	
	return list;
}

//
// It prints the elements in the alist in the form:
//
//===== List =====
//currentElements=2 maxElements=10
//0: Name: "George"       "23 Oak St, West Lafayette, 47906"
//1: Name: "Peter"        "27 Oak St, West Lafayette, 47906"
//======== End List =======
//
void llist_print(LINKED_LIST * list) {
	
	LINKED_LIST_ENTRY * e;

	printf("===== List =====\n");
	printf("nElements=%d\n", list->nElements);

	e = list->head->next;
	while (e != list->head) {
		printf("name=\"%s\" value=\"%s\"\n", e->name, e->value);
		e = e->next;
	}
	printf("======== End List =======\n");
}

//
// Adds one pair name/value to the list. If the name already exists it will
// Substitute its value. Otherwise, it will store name/value in a new entry.
// The name/vale strings are duplicated with strdup() before adding them to the
// list.
//
int llist_add(LINKED_LIST * list, char * name, char * value) {
	LINKED_LIST_ENTRY * e;
	e = list->head->next;
	while (e != list->head) {
		if (strcmp(e->name, name) == 0) {
			free(e->value);
			e->value = strdup(value);
			return 1;
		}
		e = e->next;
	}
	LINKED_LIST_ENTRY * ent = (LINKED_LIST_ENTRY *)malloc(sizeof(LINKED_LIST_ENTRY));
	ent->name = strdup(name);
	ent->value = strdup(value);
	ent->previous = e->previous;
	ent->next = e;
	e->previous->next = ent;
	e->previous = ent;
	list->nElements++;
	return 0;
}

//
// Returns the value that correspond to the name or NULL if the
// name does not exist in the list.
//
char * llist_lookup(LINKED_LIST * list, char * name) {
	LINKED_LIST_ENTRY * e;
	e = list->head->next;
	while (e != list->head) {
		if (strcmp(e->name, name) == 0) {
			return e->value;
		}
		e = e->next;
	}
	return NULL;
}

//
// It removes the entry with that name from the list.
// Also the name and value strings will be freed.
//
int llist_remove(LINKED_LIST * list, char * name) {
	LINKED_LIST_ENTRY * e;
	e = list->head->next;
	while (e != list->head) {
		if (strcmp(e->name, name) == 0) {
			free(e->name);
			free(e->value);
			list->nElements--;
			e->previous->next = e->next;
			e->next->previous = e->previous;
			free(e);
			return 1;
		}
		e = e->next;
	}
	return 0;
}
//
// It returns in *name and *value the name and value that correspond to
// the ith entry. It will return 1 if successful, or 0 otherwise.
//
int llist_get_ith(LINKED_LIST * list, int ith, char ** name, char ** value) {
	if (list->nElements <= ith) {
		return 0;
	}
	int i;
	LINKED_LIST_ENTRY * e;
	e = list->head->next;
	for (i = 0; i < ith; i++) {
		e = e->next;
	}
	*name = e->name;
	*value = e->value;
	return 1;
}

//
// It removes the ith entry from the list.
// Also the name/value strings are freed.
//
int llist_remove_ith(LINKED_LIST * list, int ith) {
	if (list->nElements <= ith) {
		return 0;
	}
	int i;
	LINKED_LIST_ENTRY * e;
	e = list->head->next;
	for (i = 0; i < ith; i++) {
		e = e->next;
	}
	free(e->name);
	free(e->value);
	list->nElements--;
	e->previous->next = e->next;
	e->next->previous = e->previous;
	free(e);
	return 1;	
}

//
// It returns the number of elements in the list.
//
int llist_number_elements(LINKED_LIST * list) {
	return list->nElements;
}


//
// It saves the list in a file called file_name. The format of the
// file is as follows:
//
// name1\n
// value1\n
// \n
// name2\n
// value2\n
// ...
//
// Notice that there is an empty line between each name/value pair.
//
int llist_save(LINKED_LIST * list, char * file_name) {
	FILE *f;
	f = fopen(file_name, "w");
	if (f == NULL) {
		return 0;
	}
	LINKED_LIST_ENTRY * e;
	e = list->head->next;
	while (e != list->head) {
		fprintf(f, "%s\n%s\n\n", e->name, e->value);
		e = e->next;
	}
	fclose(f);
	return 1;
}

//
// It reads the list from the file_name indicated. If the list already has entries, it will
// clear the entries.
//
int llist_read(LINKED_LIST * list, char * file_name) {
	FILE *f;
	f = fopen(file_name, "r");
	if (f == NULL) {
		return 0;
	}
	char buf1[512];
	char buf2[512];
	char buf3[512];
	while (fgets(buf1, 512, f)) {
		char * p;
		if ((p = strchr(buf1, '\n')) != NULL) *p = '\0';
		fgets(buf2, 512, f);
		if ((p = strchr(buf2, '\n')) != NULL) *p = '\0';
		fgets(buf3, 512, f);
		llist_add(list, buf1, buf2);
	}
	fclose(f);
	return 1;
}

//
// It sorts the list according to the name. The parameter ascending determines if the
// order si ascending (1) or descending(0).
//
void llist_sort(LINKED_LIST * list, int ascending) {
	int i, j;
	char * temp1;
	char * temp2;
	LINKED_LIST_ENTRY * t;
	LINKED_LIST_ENTRY * d;
	t = list->head->next;
	d = t->next;
	if (ascending) {
		while (t != list->head->previous) {
			while (d != list->head) {
				if (strcmp(t->name, d->name) > 0) {
					temp1 = d->name;
					temp2 = d->value;
					d->name = t->name;
					d->value = t->value;
					t->name = temp1;
					t->value = temp2;
				}
				d = d->next;
			}
			t = t->next;
			d = t->next;
		}
	} else {
		while (t != list->head->previous) {
			while (d != list->head) {
				if (strcmp(t->name, d->name) < 0) {
					temp1 = d->name;
					temp2 = d->value;
					d->name = t->name;
					d->value = t->value;
					t->name = temp1;
					t->value = temp2;
				}
				d = d->next;
			}
			t = t->next;
			d = t->next;
		}
	}
}

//
// It removes the first entry in the list.
// All entries are moved down one position.
// It also frees memory allocated for name and value.
//
int llist_remove_first(LINKED_LIST * list) {
	if (list->nElements == 0) {
		return 0;
	}
	LINKED_LIST_ENTRY * e;
	e = list->head->next;
	free(e->name);
	free(e->value);
	list->head->next = list->head->next->next;
	list->head->next->previous = list->head;
	free(e);
	list->nElements--;
	return 1;
}

//
// It removes the last entry in the list
// It also frees memory allocated for name and value.
//
int llist_remove_last(LINKED_LIST * list) {
	if (list->nElements == 0) {
		return 0;
	}
	LINKED_LIST_ENTRY * e;
	e = list->head->previous;
	free(list->head->previous->name);
	free(list->head->previous->value);
	list->head->previous = list->head->previous->previous;
	list->head->previous->next = list->head;
	free(e);
	list->nElements--;
	return 1;
}

//
// Insert a name/value pair at the beginning of the list.
// There is no check if the name already exists. The entry is added
// at the beginning of the list.
//
int llist_insert_first(LINKED_LIST * list, char *name, char * value) {
	LINKED_LIST_ENTRY * e = (LINKED_LIST_ENTRY *)malloc(sizeof(LINKED_LIST_ENTRY));
	e->name = strdup(name);
	e->value = strdup(value);
	e->previous = list->head;
	e->next = list->head->next;
	list->head->next->previous = e;
	list->head->next = e;
	list->nElements++;
	return 1;
}

//
// Insert a name/value pair at the end of the list.
// There is no check if the name already exists. The entry is added
// at the end of the list.
//
int llist_insert_last(LINKED_LIST * list, char *name, char * value) {
	LINKED_LIST_ENTRY * e = (LINKED_LIST_ENTRY *)malloc(sizeof(LINKED_LIST_ENTRY));
	e->name = strdup(name);
	e->value = strdup(value);
	e->previous = list->head->previous;
	e->next = list->head;
	list->head->previous->next = e;
	list->head->previous = e;
	list->nElements++;
	return 1;
}

