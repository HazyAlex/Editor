#ifndef FILE_INCLUDE_GUARD
#define FILE_INCLUDE_GUARD

#include <stdio.h>
#include "string/string.h"

#define EXIT_CODE_OPENING_FILE 2
#define EXIT_CODE_READING_FILE 3
#define EXIT_CODE_CLOSING_FILE 4
#define EXIT_CODE_WRITING_FILE 5


String read_contents(const char* filename);
FILE * open_file(const char* filename, const char *mode);
void write_contents(FILE* file, String contents);
void close_file(FILE* file);


#endif
