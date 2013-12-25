#include <stdio.h>
#include <stdlib.h>

#ifndef FILES_H_INCLUDED
#define FILES_H_INCLUDED

char* load_file(const char* filename, int* file_size);
unsigned int get_file_size (const char * file_name);
void unload_file(char* file_buff);
#endif // FILES_H_INCLUDED
