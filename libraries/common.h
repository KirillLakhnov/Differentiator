#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <locale.h>

const size_t MAX_LEN_CMD = 350;

#define SPEECH_SYNTHESIZER(work_synthesizer, ...)   if (work_synthesizer)                                           \
                                                    {                                                               \
                                                        char command[MAX_LEN_CMD] = "";                             \
                                                        snprintf (command, MAX_LEN_CMD, "say " __VA_ARGS__);        \
                                                        system (command);                                           \
                                                    }                           

#ifndef NDEBUG
#define CHECK_ERROR(condition, error) (condition) ? error : 0
#define PRINT_ERROR_GRAPH(file, code_of_error_programm, error)  if (code_of_error_programm & error)                                                     \
                                                                {                                                                                       \
                                                                    fprintf (file,  "\"%s\"[shape = \"record\", fontname = \"Helvetica-Bold\","         \
                                                                                    "fillcolor = \"white\","                                            \
		                                                                            "\n\t\tlabel = \"%s\"]",                                            \
                                                                                    #error, #error);                                                    \
                                                                }    
#else
#define CHECK_ERROR(condition, error)
#endif

void screen_clear ();

void clear_input ();

int stricmp (const char* str1, const char* str2);

char* bracket_find (char* current_position);

#endif // COMMON_H