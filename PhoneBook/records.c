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

#include "records.h"

void
get_records(char* file_buff, int fsize, int* records_size, PHONEBOOK_ENTRY** records)
{
    int i, strings_size=0;
    char** strings = (char**) malloc(strings_size * sizeof(char*));

    for (i=0; i<fsize; i++)
    {
        if (file_buff[i] == ',' || file_buff[i] == '\n')
        {
            file_buff[i] = '\0';
            if (strings_size == 0 && i != 0)
            {
                strings = (char**)realloc(strings, ++strings_size * sizeof(char*));
                strings[strings_size-1] = file_buff;

                if (i+1 < fsize)
                {
                    strings = (char**)realloc(strings, ++strings_size * sizeof(char*));
                    strings[strings_size-1] = &file_buff[i+1];
                }
            }
            else if (i+1 < fsize)
            {
                strings = (char**)realloc(strings, ++strings_size * sizeof(char*));
                strings[strings_size-1] = &file_buff[i+1];
            }
        }
    }

    *records_size = strings_size / 5;
    (*records) = (PHONEBOOK_ENTRY*)realloc((*records), *records_size * sizeof(PHONEBOOK_ENTRY));

    for (i=0; i<*records_size; i++)
    {
        (*records)[i].lname   = strings[ i * 5 + 0 ];
        f_capital((*records)[i].lname);
        (*records)[i].fname   = strings[ i * 5 + 1 ];
        f_capital((*records)[i].fname);
        (*records)[i].address = strings[ i * 5 + 2 ];
        f_capital((*records)[i].address);
        (*records)[i].city    = strings[ i * 5 + 3 ];
        f_capital((*records)[i].city);
        (*records)[i].phone   = strings[ i * 5 + 4 ];
        f_capital((*records)[i].phone);
    }

    free(strings);
}

void
load_record_file(char* filename, int* entries_size, PHONEBOOK_ENTRY** entries)
{
    if (strlen(filename) == 0) return;
    file_buff = load_file(filename, &file_size);
    if (file_buff > 0 && file_size > 0)
    {
        unload_record_file();
        strcpy(file_name, filename);
        get_records(file_buff, file_size, entries_size, entries);
    }
}

void
unload_record_file()
{
//    if (current_loaded_buf != 0) {
    //printf("%s\n", current_loaded_buf );
//        unload_file(current_loaded_buf);
    //current_loaded_buf = 0;
    //memset(current_loaded_file, '\0', 1);
    //  }
}

void
search_records(PHONEBOOK_ENTRY* entries, int entries_size, char* str, int* results_size, PHONEBOOK_ENTRY*** results)
{
    int i;
    *results_size = 0;
    *results = (PHONEBOOK_ENTRY**)realloc(*results, (*results_size) * sizeof(PHONEBOOK_ENTRY*));
    for (i=0; i<entries_size; i++)
    {
        if (sub_string(entries[i].fname, str) ||
                sub_string(entries[i].lname, str) ||
                sub_string(entries[i].city, str)  ||
                sub_string(entries[i].phone, str) ||
                sub_string(entries[i].address, str))
        {
            *results = (PHONEBOOK_ENTRY**)realloc(*results, ++(*results_size) * sizeof(PHONEBOOK_ENTRY*));
            (*results)[*results_size - 1] = &entries[i];
            //printf("\n%s\n", entries[i].fname);
            //printf("%s\n", entries[i].lname);
            //printf("%s\n", entries[i].phone);
            //printf("%s\n", entries[i].address);
            //printf("%s\n", entries[i].city);
        }
    }
}

void
delete_record(PHONEBOOK_ENTRY** entries, int* entries_size, int index)
{
    if (index < (*entries_size)-1)
    {
        memcpy(&((*entries)[index]), &((*entries)[index + 1]), (*entries_size - index - 1)* sizeof(PHONEBOOK_ENTRY));
        (*entries) = (PHONEBOOK_ENTRY*)realloc((*entries), --(*entries_size) * sizeof(PHONEBOOK_ENTRY));
    }
    else
        (*entries) = (PHONEBOOK_ENTRY*)realloc((*entries), --(*entries_size) * sizeof(PHONEBOOK_ENTRY));
}

int
save_records(char* filename, PHONEBOOK_ENTRY* entries, int size)
{
    int i;
    FILE* fp;
    fp = fopen (filename, "w+");

    if (fp == NULL) return 0;

    for (i=0; i<size; i++)
        fprintf(fp, "%s,%s,%s,%s,%s\n",
                entries[i].lname,
                entries[i].fname,
                entries[i].address,
                entries[i].city,
                entries[i].phone);

    fclose(fp);
    return 1;
}

void
swap_record(PHONEBOOK_ENTRY *p1,PHONEBOOK_ENTRY *p2)
{
    tmp_entry = *p1;
    *p1 = *p2;
    *p2 = tmp_entry;
}

void
sort_records(PHONEBOOK_ENTRY* items, int size)
{
    int i, j, status;
    for(i=0; i<size; i++)
    {
        for(j=i+1; j<size; j++)
        {
            status = strcmp(items[i].lname,items[j].lname);
            if(status > 0)
                swap_record(&items[i], &items[j]);
            else if (status == 0)
            {
                status = strcmp(items[i].fname,items[j].fname);
                if(status > 0)
                    swap_record(&items[i], &items[j]);
                else if(status == 0)
                {
                    status = strcmp(items[i].phone,items[j].phone);
                    if(status > 0)
                        swap_record(&items[i], &items[j]);
                }
            }
        }
    }

    for (i=0; i<size; i++) items[i].index = i;
}
