#include "file/file.h"
#include <stdlib.h>
#include <string.h>
#include "structures/types.h"
#include "log/log.h"
#include "string/string.h"


String read_contents(const char* filename)
{
    FILE* file = NULL;

    errno_t error = fopen_s(&file, filename, "rb");
    if (error != 0 || file == NULL) {
        ERROR("Could not open the file '%s', error: '%s'\n", filename, strerror(error));
        exit(EXIT_CODE_OPENING_FILE);
    }

    fseek(file, 0, SEEK_END);
    u64 filesize = (u64)ftell(file);
    rewind(file);

    String contents = string_alloc(filesize + 1);

    size_t readbytes = fread(contents.data, 1, filesize, file);
    if (readbytes < filesize) {
        ERROR("Could read the file: %s", filename);
        exit(EXIT_CODE_READING_FILE);
    }

    if (fclose(file) != 0) {
        WARNING("Could not close file: %s", filename);
        exit(EXIT_CODE_CLOSING_FILE);
    }

    contents.size = filesize;
    contents.data[filesize] = '\0';

    return contents;
}

FILE *open_file(const char* filename, const char *mode)
{
    FILE* file = NULL;

    errno_t error = fopen_s(&file, filename, mode);
    if (error != 0 || file == NULL) {
        ERROR("Could not open the file '%s', error: '%s'\n", filename, strerror(error));
        exit(EXIT_CODE_OPENING_FILE);
    }

    return file;
}

void write_contents(FILE* file, String contents)
{
    if (fputs(string_to_c(contents), file) != 0) {
        ERROR("Could not write to the file.\n");
        exit(EXIT_CODE_WRITING_FILE);
    }
}

void close_file(FILE* file)
{
    if (fclose(file) != 0) {
        ERROR("Could not close the file.\n");
        exit(EXIT_CODE_CLOSING_FILE);
    }
}
