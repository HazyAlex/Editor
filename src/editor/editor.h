#ifndef EDITOR_INCLUDE_GUARD
#define EDITOR_INCLUDE_GUARD

#include "string/string.h"


typedef struct {
    String *lines;
    u64 cursor_index;
    u64 current_line;
    u64 current_column;
    u64 line_count;
    u64 line_capacity;
} Buffer;

typedef struct {
    Buffer buffer;
} EditorState;

EditorState initialize_editor(const char *path);


#endif
