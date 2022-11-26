#include "differentiator.h"

int main ()
{
    setlocale (LC_ALL, "ru_RU.cp1251");
    char name_file_function[MAX_LEN_STR] = "function.txt";

    struct Text buffer_function = {};
    struct FileInfo file_function = {.file_name = name_file_function};
    struct Tree tree = {.file_function = &file_function, .buffer_function = &buffer_function};

    start_programm (&tree);

    return 0;
}