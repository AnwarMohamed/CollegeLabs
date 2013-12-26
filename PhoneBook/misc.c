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
#include <ctype.h>

char* remove_comma(char* str)
{
    int i,len=strlen(str);
    for (i=0; i<len; i++) {
        if (str[i]==',') {
            str[i] = ' ';
        }
    }
    return str;
}

int sub_string(char* s1, char* s2) {
    int len1 = strlen(s1),
    len2 = strlen(s2), i;
    if (len2 > len1) return 0;
    for (i=0; i<len2; i++)
        if (tolower(s1[i]) != tolower(s2[i]))
            return 0;
    return 1;
}

char *trim(char *str)
{
    size_t len = 0;
    char *start = str - 1;
    char *end = NULL;

    if(str == NULL) return NULL;
    if(str[0] == '\0') return str;

    len = strlen(str);
    end = str + len;

    while(isspace(*(++start)));
    while(isspace(*(--end)) && end != start);

    if(str + len - 1 != end) *(end + 1) = '\0';
    else if( start != str &&  end == start ) *str = '\0';

    end = str;
    if(start != str)
    {
            while(*start) *end++ = *start++;
            *end = '\0';
    }

    return str;
}

int is_numeric(char* str)
{
    int i, len = strlen(str);
    if ( len > 0 )
    {
        for(i = 0; i < len; i++)
            if (!isdigit(str[i]))
                return 0;
    }
    return 1;
}

void f_capital(char* str)
{
    int i, len = strlen(str);
    if ( len > 0 )
    {
        str[0] = toupper(str[0]);
        for(i = 1; i < len; i++)
            str[i] = tolower(str[i]);
    }
}
