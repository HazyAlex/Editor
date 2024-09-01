#include <stdio.h>
#include <stdlib.h>
#include "log/log.h"
#include "string/string.h"


String read_contents(const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        ERROR("Could not find the file to read from!\n");

        return string_empty();
    }

    fseek(file, 0, SEEK_END);
    u64 filesize = (u64)ftell(file);
    rewind(file);

    String contents = string_alloc(filesize + 1);

    size_t readbytes = fread(contents.data, 1, filesize, file);
    if (readbytes < filesize) {
        ERROR("Could read the file: %s", filename);
        exit(EXIT_CODE_OUT_OF_MEMORY);
    }

    if (fclose(file) != 0) {
        WARNING("Could not close file: %s", filename);
    }

    contents.data[filesize] = '\0';

    return contents;
}
