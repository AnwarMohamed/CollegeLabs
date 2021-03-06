/*
 *
 *  Copyright (C) 2013  Anwar Mohamed <anwarelmakrahy[at]gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to Anwar Mohamed
 *  anwarelmakrahy[at]gmail.com
 *
 */

#include "misc.h"
#include "files.h"

#ifndef RECORDS_H_INCLUDED
#define RECORDS_H_INCLUDED

void
delete_record(PHONEBOOK_ENTRY** entries, int* entries_size, int index);

void
get_records(char* file_buff, int fsize, int* entries_size, PHONEBOOK_ENTRY** records);

void
load_record_file(char* filename, int* entries_size, PHONEBOOK_ENTRY** entries);

void
search_records(PHONEBOOK_ENTRY* entries, int entries_size, char* str, int* results_size, PHONEBOOK_ENTRY*** results);

void
unload_record_file();

void
swap_record(PHONEBOOK_ENTRY *p1,PHONEBOOK_ENTRY *p2);

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
