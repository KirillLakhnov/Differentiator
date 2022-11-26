#ifndef ENUM_H
#define ENUM_H

enum ERRORS {
    GOOD_WORKING                    = 0,
    ERROR_MEMMORY                   = 1,
    ERROR_READING_FILE              = 2,
    ERROR_FILE_OPEN                 = 3,
    ERROR_FILE_CLOSE                = 4,
    ERROR_FSEEK                     = 5,
    ERROR_FTELL                     = 6,
    ERROR_FPRINTF                   = 7,
    ERROR_FPUTS                     = 8,
    ERROR_BUFFER_CREATER            = 9,
    ERROR_GETS                      = 10,
    ERROR_SIZE_FILE                 = 11,
    ERROR_CALLOC                    = 12,
    ERROR_REALLOC                   = 13,
    ERROR_SSCANF                    = 14,
    ERROR_COMMAND_CMP               = 15,
    ERROR_STACK_CTOR                = 16,
    ERROR_FWRITE                    = 17,
};

#endif // ENUMS_H
