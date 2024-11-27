#ifndef EDITOR_INCLUDE_GUARD
#define EDITOR_INCLUDE_GUARD

#include "settings/settings.h"
#include "string/string.h"
#include <time.h>


typedef struct {
    color text_color;
    clock_t blink_timer;
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
    Settings settings;
    const char *filepath;
} EditorState;

EditorState initialize_editor(Settings settings, const char *path);
void dealloc_editor(EditorState *state);
Cursor initialize_cursor(void);
void cursor_blink(Cursor *cursor);
void cursor_blink_reset(Cursor *cursor);
void editor_save_file(EditorState *state);

//
// Events
//
void go_down(EditorState *state);
void go_up(EditorState *state);
void go_right(EditorState *state);
void go_left(EditorState *state);
void replace_character(EditorState *state, char character);
bool add_character(EditorState *state, char character);
bool delete_character(EditorState *state, u64 index);
void add_line(EditorState *state);


#endif
