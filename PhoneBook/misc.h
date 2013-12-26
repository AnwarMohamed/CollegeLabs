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

int sub_string(char* s1, char* s2);
char *trim(char *str);
void f_capital(char* str);
int is_numeric(char* str);
#endif // MISC_H_INCLUDED
