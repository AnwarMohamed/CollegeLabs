#include "misc.h"
#include "files.h"

#ifndef RECORDS_H_INCLUDED
#define RECORDS_H_INCLUDED

void
get_records(char* file_buff, int fsize, int* entries_size, PHONEBOOK_ENTRY** records);

void
load_record_file(char* filename, int* entries_size, PHONEBOOK_ENTRY** entries);

void
search_records(PHONEBOOK_ENTRY* entries, int entries_size, char* str, int* results_size, PHONEBOOK_ENTRY*** results);

void
unload_record_file();

void
sort_records(PHONEBOOK_ENTRY* items, int size);

char* file_buff;
int file_size;
char file_name[100];

char** strings_table;
int strings_table_size;
char* tmp_string;
PHONEBOOK_ENTRY tmp_entry;

int
save_records(char* filename, PHONEBOOK_ENTRY* entries, int size);
#endif // RECORDS_H_INCLUDED
