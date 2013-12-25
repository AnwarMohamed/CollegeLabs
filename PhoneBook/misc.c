#include "misc.h"


int sub_string(char* s1, char* s2) {
    int len1 = strlen(s1),
    len2 = strlen(s2), i, j;
    if (len2 > len1) return 0;

    for (i=0; i<len1; i++)
        for (j=0; j<len2; j++)
        {
            //printf("%c %c\n", tolower(s1[i+j]), tolower(s2[j]));
            if (tolower(s1[i+j]) == tolower(s2[j]) && j+1==len2)
                return 1;

            if (tolower(s1[i+j]) != tolower(s2[j]))
                break;
        }

    //printf("wrong\n\n");
    return 0;
}

char *trim(char *str)
{
    size_t len = 0;
    char *frontp = str - 1;
    char *endp = NULL;

    if( str == NULL )
            return NULL;

    if( str[0] == '\0' )
            return str;

    len = strlen(str);
    endp = str + len;


    while( isspace(*(++frontp)) );
    while( isspace(*(--endp)) && endp != frontp );

    if( str + len - 1 != endp )
            *(endp + 1) = '\0';
    else if( frontp != str &&  endp == frontp )
            *str = '\0';

    endp = str;
    if( frontp != str )
    {
            while( *frontp ) *endp++ = *frontp++;
            *endp = '\0';
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
    return str;
}
