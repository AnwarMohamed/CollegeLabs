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

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#ifndef MISC_H_INCLUDED
#define MISC_H_INCLUDED

typedef struct {
    char* fname;
    char* lname;
    char* city;
    char* address;
    char* phone;
    int index;
} PHONEBOOK_ENTRY;

char* remove_comma(char* str);
int sub_string(char* s1, char* s2);
char *trim(char *str);
void f_capital(char* str);
int is_numeric(char* str);
#endif // MISC_H_INCLUDED
