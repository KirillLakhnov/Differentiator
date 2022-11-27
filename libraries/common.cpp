#include "common.h"

void screen_clear ()
{
    system ("clear");
}

void clear_input ()
{
    while (getchar () != '\n');
}

int stricmp (const char* str1, const char* str2)
{
    assert (str1);
    assert (str2);
    
    int i = 0;
    while (tolower (str1[i]) == tolower (str2[i]) && (str1[i] != '\0' && str2[i++] != '\0'));
    
    return tolower (str1[i]) - tolower (str2[i]);
}

char* bracket_find (char* current_position)
{
    int i = 0;
    while (*(current_position + i) != '(' || *(current_position + i) != ')')
    {
        i++;
    }

    return current_position + i;
}

int factorial (int value)
{
    int factorial_value = 1;

    if (value == 0)
    {
        return factorial_value;
    }

    for (int i = 1; i <= value; i++)
    {
        factorial_value *= i;
    }

    return factorial_value;
}