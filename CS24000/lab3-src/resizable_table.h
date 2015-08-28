
#if !defined RESIZABLE_ARRAY_H
#define RESIZABLE_ARRAY_H

#define INITIAL_SIZE_RESIZABLE_TABLE 10

typedef struct RESIZABLE_TABLE_ENTRY {
	char * name;
	void * value;
} RESIZABLE_TABLE_ENTRY;

typedef struct RESIZABLE_TABLE {
	int maxElements;
	int currentElements;
	struct RESIZABLE_TABLE_ENTRY * array;
} RESIZABLE_TABLE;

RESIZABLE_TABLE * rtable_create();
int rtable_add(RESIZABLE_TABLE * table, char * name, void * value);
int rtable_add_str(RESIZABLE_TABLE * table, char * name, char * str_value);
int rtable_add_int(RESIZABLE_TABLE * table, char * name, long int_value);
void * rtable_lookup(RESIZABLE_TABLE * table, char * name);
int rtable_remove(RESIZABLE_TABLE * table, char * name);
int rtable_get_ith(RESIZABLE_TABLE * table, int ith, char ** name, void ** value);
int rtable_remove_ith(RESIZABLE_TABLE * table, int ith);
int rtable_number_elements(RESIZABLE_TABLE * table);
int rtable_max_elements(RESIZABLE_TABLE * table);
void rtable_sort(RESIZABLE_TABLE * table, int ascending);
void rtable_sort_by_intval(RESIZABLE_TABLE * table, int ascending);

int rtable_remove_first(RESIZABLE_TABLE * table );
int rtable_remove_last(RESIZABLE_TABLE * table );
int rtable_insert_first(RESIZABLE_TABLE * table, char *name, void * value);
int rtable_insert_last(RESIZABLE_TABLE * table, char *name, void * value);

void rtable_print_str(RESIZABLE_TABLE * table);
void rtable_print_int(RESIZABLE_TABLE * table);

int rtable_save_str(RESIZABLE_TABLE * table, char * file_name);
int rtable_read_str(RESIZABLE_TABLE * table, char * file_name);

int rtable_save_int(RESIZABLE_TABLE * table, char * file_name);
int rtable_read_int(RESIZABLE_TABLE * table, char * file_name);


#endif

