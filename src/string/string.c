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

    memcpy(copy, data, length * sizeof(u8));
    copy[length] = '\0';

    String string;
    string.data = copy;
    string.size = length;
    string.capacity = length;

    return string;
}

String string_copy_c(const char *data, u64 size)
{
    String copy = string_alloc(size);

    memcpy(copy.data, data, sizeof(u8) * size);
    copy.data[size] = '\0';
    copy.size = size;

    return copy;
}

String string_copy(String data)
{
    String copy = string_alloc(data.size);

    memcpy(copy.data, data.data, sizeof(u8) * data.size);
    copy.data[data.size] = '\0';
    copy.size = data.size;

    return copy;
}

String string_copy_slice(String data, u64 start, u64 end)
{
    u64 size = end - start + 1;

    String copy = string_alloc(size);

    memcpy(copy.data, data.data + start, sizeof(u8) * size);
    copy.data[size] = '\0';
    copy.size = size;

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

String string_alloc(u64 capacity)
{
    String string;

    u8 *data = (u8 *) calloc(capacity + 1, sizeof(u8));
    if (data == NULL) {
        ERROR("OUT OF MEMORY!");
        exit(EXIT_CODE_OUT_OF_MEMORY);
    }

    string.data = data;
    string.size = 0;
    string.capacity = capacity;

    return string;
}

void string_grow(String *string)
{
    u64 grow_by = (string->capacity * 1.5) - string->capacity;

    string_grow_by(
        string,
        grow_by >= 15 ? grow_by : 15
    );
}

void string_grow_by(String *string, u64 by)
{
    u64 new_capacity = string->capacity + by;

    u8 *data = (u8 *) realloc(string->data, sizeof(u8) * (new_capacity + 1));
    if (data == NULL) {
        ERROR("OUT OF MEMORY!");
        exit(EXIT_CODE_OUT_OF_MEMORY);
    }

    data[new_capacity] = '\0';

    string->data = data;
    string->capacity = new_capacity;
}

void string_concat(String *destination, String source)
{
    if (destination->size + source.size >= destination->capacity) {
        string_grow_by(destination, source.size);
    }

    memmove(destination->data + destination->size, source.data, sizeof(u8) * source.size);
    destination->size += source.size;
}

void string_concat_from_c(String *destination, const char *source)
{
    u64 source_size = strlen(source);
    if (destination->size + source_size > destination->capacity) {
        string_grow_by(destination, source_size);
    }

    memmove(destination->data + destination->size, source, sizeof(u8) * source_size);
    destination->size += source_size;
}

void string_move_over(String *string, u64 index)
{
    if (string->size == 0) {
        return;
    }

    u64 leftover = string->size - index;
    if (leftover == 0) {
        return;
    }

    memmove(string->data + index + 1, string->data + index, sizeof(u8) * (leftover + 1));

    string->size += 1;
}

void string_move_before(String *string, u64 index)
{
    if (string->size == 0) {
        return;
    }

    if (string->size == 1) {
        string->data[0] = '\0';
        string->size -= 1;
        return;
    }

    u64 leftover = string->size - index;
    if (leftover == 0) {
        return;
    }

    memmove(string->data + index, string->data + index + 1, sizeof(u8) * leftover);

    string->size -= 1;
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
