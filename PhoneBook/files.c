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

#include "files.h"
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

FILE * file = NULL;

unsigned int
get_file_size (const char * file_name)
{
    struct stat sb;
    if (stat (file_name, & sb) != 0)
    {
        return 0;
    }
    return sb.st_size;
}

char*
load_file(const char* file_name, int* file_size)
{
    *file_size =0;
    unsigned int s;
    char * contents;
    size_t bytes_read;
    int status;

    s = get_file_size (file_name);
    contents = malloc (s * sizeof(char));
    if (! contents)
    {
        return (char*)-1;
    }

    file = fopen (file_name, "rw");
    if (! file)
    {
        return (char*)-1;
    }

    bytes_read = fread (contents, sizeof (char), s, file);
    if (bytes_read != s)
    {
        return (char*)-1;
    }
    status = fclose (file);
    if (status != 0)
    {
        return (char*)-1;
    }

    *file_size = bytes_read;
    return contents;
}

void
unload_file(char* file)
{
    free(file);
}
