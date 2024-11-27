#include "editor/editor.h"
#include "file/file.h"
#include "log/log.h"
#include "string/string.h"
#include "structures/types.h"
#include <stdlib.h>
#include <string.h>


void initialize_state(EditorState *state, const char *filepath, Settings settings)
{
    Buffer buffer;
    buffer.lines = (String *)calloc(10, sizeof(String));
    buffer.line_capacity = 10;
    buffer.line_count = 0;
    buffer.current_line = 0;
    buffer.current_column = 0;
    buffer.cursor_index = 0;

    state->buffer = buffer;
    state->cursor = initialize_cursor();
    state->settings = settings;
    state->filepath = filepath;
}

void dealloc_editor(EditorState *state)
{
    for (u32 i = 0; i < state->buffer.line_count; ++i) {
        String *line = (state->buffer.lines + i);
        string_dealloc(line);
    }

    free(state->buffer.lines);

    memset(state, 0, sizeof(EditorState));

    dealloc_settings(&state->settings);
}

void editor_add_line(Buffer *buffer, String line, u64 index)
{
    if (buffer->line_capacity == index) {
        u64 new_capacity = buffer->line_capacity * 1.5;
        u64 new_block_size = sizeof(String) * (new_capacity - buffer->line_capacity);

        buffer->lines = (String *)realloc(buffer->lines, sizeof(String) * new_capacity);
        if (buffer->lines == NULL) {
            ERROR("OUT OF MEMORY!");
            exit(EXIT_CODE_OUT_OF_MEMORY);
        }

        memset(buffer->lines + buffer->line_capacity, 0, new_block_size);

        buffer->line_capacity = new_capacity;
    }

    buffer->lines[index] = line;
    buffer->line_count += 1;
}

void file_to_buffer(EditorState *state, String contents)
{
    u64 start = 0;
    u64 end = 0;
    u64 line = 0;

    while (TRUE) {
        char character = contents.data[end];
        bool isEndOfFile = character == '\0';
        bool isNewline = character == '\n';

        if (isEndOfFile || isNewline) {
            String copy = contents.data[start] == '\0'
                ? string_alloc(0)
                : string_copy_slice(contents, start, end);

            editor_add_line(&state->buffer, copy, line);
            line += 1;

            start = end + 1;
        }
        if (isEndOfFile) {
            break;
        }

        end += 1;
    }
}

EditorState initialize_editor(Settings settings, const char *path)
{
    EditorState state = {0};
    initialize_state(&state, path, settings);

    String contents = read_contents(path);

    file_to_buffer(&state, contents);

    string_dealloc(&contents);

    return state;
}

Cursor initialize_cursor(void)
{
    Cursor cursor = {
        .text_color = (color){.r = 225, .g = 225, .b = 225}
    };

    return cursor;
}

void cursor_blink(Cursor *cursor)
{
    double elapsed = (double)(clock() - cursor->blink_timer) / CLOCKS_PER_SEC;
    if (elapsed > 1) {
        cursor->text_color.b = cursor->text_color.b == 225 ? 0 : 225;
        cursor->blink_timer = clock();
    }
}

void cursor_blink_reset(Cursor *cursor)
{
    cursor->blink_timer = clock();
    cursor->text_color.b = 0;
}

void go_down(EditorState *state)
{
    if (state->buffer.current_line < (state->buffer.line_count - 1)) {
        state->buffer.current_line += 1;

        String line = state->buffer.lines[state->buffer.current_line];

        if (line.size <= (OS_NEWLINE_SIZE + 1)) {
            state->buffer.current_column = 0;
            return;
        }

        bool hasNewline = line.data[line.size - 1] == '\n';
        u64 limit = hasNewline
            ? line.size - 1 - OS_NEWLINE_SIZE
            : line.size - 1;

        if (state->buffer.current_column > limit) {
            state->buffer.current_column = limit;
        }
    }
}

void go_up(EditorState *state)
{
    if (state->buffer.current_line > 0) {
        state->buffer.current_line -= 1;

        String line = state->buffer.lines[state->buffer.current_line];
        u64 limit = line.size - 1 - OS_NEWLINE_SIZE;

        if (state->buffer.current_column > limit) {
            state->buffer.current_column = limit;
        }
    }
}

void go_right(EditorState *state)
{
    String line = state->buffer.lines[state->buffer.current_line];
    bool hasNewline = line.size > 0
       ? line.data[line.size - 1] == '\n'
       : FALSE;

    u64 limit = hasNewline
        ? line.size - 1 - OS_NEWLINE_SIZE
        : line.size - 1;

    if (state->buffer.current_column < limit) {
        state->buffer.current_column += 1;
    }
}

void go_left(EditorState *state)
{
    if (state->buffer.current_column > 0) {
        state->buffer.current_column -= 1;
    }
}

void replace_character(EditorState *state, char character)
{
    u64 current_line = state->buffer.current_line;
    u64 current_column = state->buffer.current_column;

    String line = state->buffer.lines[current_line];

    line.data[current_column] = character;
}

bool add_character(EditorState *state, char character)
{
    u64 current_line = state->buffer.current_line;
    u64 current_column = state->buffer.current_column;

    String *line = &state->buffer.lines[current_line];

    if (line->size == line->capacity) {
        string_grow(line);
    }

    string_move_over(line, current_column);

    if (line->size <= OS_NEWLINE_SIZE + 1) {
        replace_character(state, character);
        return FALSE;
    }

    line->data[current_column + 1] = (u8) character;
    return TRUE;
}

bool delete_character(EditorState *state, u64 index)
{
    u64 current_line = state->buffer.current_line;
    u64 column_to_remove = index;

    String *line = &state->buffer.lines[current_line];

    if (line->size <= OS_NEWLINE_SIZE) {
        return FALSE;
    }

    string_move_before(line, column_to_remove);

    return TRUE;
}

void add_line(EditorState *state)
{
    u64 current_line = state->buffer.current_line;
    bool isLastLine = current_line == state->buffer.line_count;

    // Append a newline to the current line
    String *line = &state->buffer.lines[current_line];

    if (memcmp(line->data + line->size - OS_NEWLINE_SIZE, OS_NEWLINE, OS_NEWLINE_SIZE) != 0) {
        string_concat_from_c(line, OS_NEWLINE);
    }

    // If it isn't the last line, append a newline to the end
    String new_line = string_empty();
    if (isLastLine) {
        string_alloc(0);
    } else {
        string_alloc(OS_NEWLINE_SIZE);
        string_concat_from_c(&new_line, OS_NEWLINE);
    }

    // Add the line at the end, then move it to the right place.
    editor_add_line(&state->buffer, new_line, state->buffer.line_count);

    for (u64 i = state->buffer.line_count - 1; i > current_line; i--) {
        String previous_line = state->buffer.lines[i - 1];

        state->buffer.lines[i] = previous_line;
    }

    state->buffer.lines[current_line] = new_line;
}

void editor_save_file(EditorState *state)
{
    FILE* file = open_file(state->filepath, "wb");

    for (u64 i = 0; i < state->buffer.line_count; ++i) {
        String line = state->buffer.lines[i];

        write_contents(file, line);
    }

    close_file(file);
}
