#ifndef FILE_PROCESSING_H
#define FILE_PROCESSING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "../enum.h"

/**
 * @brief Text - the structure contains variables related to the original text file and buffer.
 * 
 */
struct Text {
    char* file_buffer;
    size_t size_buffer;
    struct Line* line_info;
    int number_line_text;
};

/**
 * @brief FileInfo - a structure containing the name of the original file and its size
 * 
 */
struct FileInfo {
    const char* file_name;
    size_t size_file;
};

/**
 * @brief Line - a structure containing a pointer to a string and its length.
 * 
 */
struct Line {
    char* pointer_line;
    size_t length;
};

/**
 * @brief SizeFile - finds the file size in bytes.
 * 
 * @param text - text file.
 * @param text_info - pointer to the structure that contains a variable to which the file size is passed.
 * @return the result of the function execution (0 - successful execution, more than 0 - error)
 */
int SizeFile (FILE* text, struct Text* text_info);

/**
 * @brief NumberLineText - counts the number of rows and replaces '\n' with '\0'
 * 
 * @param text_info - pointer to the structure containing the file size and a pointer to the buffer of this file.
 * @return number of rows
 */
int NumberLineText (struct Text* text_info);

/**
 * @brief BufferCreater - creates a file buffer.
 * 
 * @param file_info - pointer to the structure containing basic data about a file.
 * @param text_info - pointer to the structure containing the file size and a pointer to the buffer of this file.
 * @return the result of the function execution (0 - successful execution, more than 0 - error).
 */

int BufferCreater (struct FileInfo* file_info, struct Text* text_info);
/**
 * @brief TextCtor - creates a text file buffer.
 * 
 * @param file_info - pointer to the structure containing basic data about a file.
 * @param text_info - pointer to the structure containing the file size and a pointer to the buffer of this file.
 * @return the result of the function execution (0 - successful execution, more than 0 - error).
 */
int TextCtor (struct FileInfo* file_info, struct Text* text_info);

/**
 * @brief BufferTransferPointer - passes the data on the line from the file buffer to the pointer array.
 * 
 * @param text_info - pointer to the structure containing data about a text file buffer and an array of pointers.
 */
void BufferTransferPointer (struct Text* text_info);

/**
 * @brief TextDtor - clears the buffer after using dynamic memory.
 * 
 * @param text_info - a pointer to a structure containing data about the file buffer and an array of pointers to its lines.
 */
void TextDtor (struct Text* text_info);

#endif //FILE_PROCESSING_H
