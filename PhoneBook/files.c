#include "files.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
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
