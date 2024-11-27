#ifndef STRING_INCLUDE_GUARD
#define STRING_INCLUDE_GUARD

#include "structures/types.h"

#define EXIT_CODE_OUT_OF_MEMORY 1


typedef struct {
    u8* data;
    u64 capacity;
    u64 size;
} String;

String string_empty(void);
String string_from_c(char *data);
String string_copy_c(const char *data, u64 size);
String string_copy(String data);
String string_copy_slice(String data, u64 start, u64 end);
char * string_to_c(String string);
char * string_pointer_to_c(String *string);
String string_alloc(u64 capacity);
void string_grow(String *string);
void string_grow_by(String *string, u64 by);
void string_concat(String *destination, String source);
void string_concat_from_c(String *destination, const char *source);
void string_move_over(String *string, u64 index);
void string_move_before(String *string, u64 index);
void string_dealloc(String *string);
bool string_is_empty(String string);
bool string_is_not_empty(String string);


#endif
