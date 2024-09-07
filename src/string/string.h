#ifndef STRING_INCLUDE_GUARD
#define STRING_INCLUDE_GUARD

#include <stdlib.h>
#include <string.h>
#include "structures/types.h"
#include "log/log.h"

#define EXIT_CODE_OUT_OF_MEMORY 1


typedef struct {
    u8* data;
    i32 capacity;
    i32 size;
} String;

String string_empty();
String string_from_c(char *data);
String string_alloc(i64 capacity);
void string_dealloc(String *string);


#endif
