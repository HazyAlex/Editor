#ifndef EDITOR_INCLUDE_GUARD
#define EDITOR_INCLUDE_GUARD

#include "settings/settings.h"
#include "string/string.h"
#include <time.h>


typedef struct {
    color text_color;
} Cursor;

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
    Cursor cursor;
} EditorState;

EditorState initialize_editor(const char *path);
Cursor initialize_cursor();
void cursor_blink(Cursor *cursor, time_t *timer);


#endif
