#include "string.h"


String string_empty()
{
    String string;

    string.data = NULL;
    string.size = 0;
    string.capacity = 0;

    return string;
}

String string_alloc(i64 capacity)
{
    String string;

    u8 *data = (u8*) calloc(capacity, sizeof(u8));
    if (data == NULL) {
        ERROR("OUT OF MEMORY!");
        exit(EXIT_CODE_OUT_OF_MEMORY);
    }

    string.data = data;
    string.size = 0;
    string.capacity = capacity;

    return string;
}

void string_dealloc(String *string)
{
    free(string->data);
    string->capacity = 0;
    string->size = 0;
    string->data = NULL;
}
