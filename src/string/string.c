#include <stdlib.h>
#include <string.h>
#include "string/string.h"
#include "log/log.h"
#include "structures/types.h"


String string_empty(void)
{
    String string;

    string.data = NULL;
    string.size = 0;
    string.capacity = 0;

    return string;
}

String string_from_c(char *data)
{
    size_t length = strlen(data);

    u8 *copy = (u8*) calloc(length + 1, sizeof(u8));
    if (copy == NULL) {
        ERROR("OUT OF MEMORY!");
        exit(EXIT_CODE_OUT_OF_MEMORY);
    }

    memcpy(copy, data, (length + 1) * sizeof(u8));

    String string;
    string.data = copy;
    string.size = length;
    string.capacity = length;

    return string;
}

String string_copy_c(const char *data, u64 size)
{
    String copy = string_alloc(size + 1);
    memcpy(copy.data, data, sizeof(u8) * size);
    copy.size = size;

    return copy;
}

String string_copy(String data)
{
    String copy = string_alloc(data.size + 1);
    memcpy(copy.data, data.data, sizeof(u8) * data.size);
    copy.size = data.size;

    return copy;
}

String string_copy_slice(String data, u64 start, u64 end)
{
    u64 size = end - start + 1;

    String copy = string_alloc(size);

    memcpy(copy.data, data.data + start, sizeof(u8) * size);
    copy.size = size;
    copy.data[size - 1] = '\0';

    return copy;
}

char *string_to_c(String string)
{
    return (char *)(string.data);
}

char *string_pointer_to_c(String *string)
{
    return (char *)(string->data);
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

bool string_is_empty(String string)
{
    return string.size > 0 && string.capacity > 0 && string.data != NULL;
}

bool string_is_not_empty(String string)
{
    return ! string_is_empty(string);
}
