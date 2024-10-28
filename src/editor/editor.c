#include "editor/editor.h"
#include "file/file.h"
#include "log/log.h"
#include "string/string.h"
#include <stdlib.h>
#include <string.h>


void initialize_state(EditorState *state)
{
    Buffer buffer;
    buffer.lines = (String *)calloc(10, sizeof(String));
    buffer.line_capacity = 10;
    buffer.line_count = 0;
    buffer.current_line = 0;
    buffer.current_column = 0;
    buffer.cursor_index = 0;

    state->buffer = buffer;
}

void add_line(Buffer *buffer, String contents, u64 line, u64 start, u64 end)
{
    String copy = string_copy_slice(contents, start, end);

    if (line == buffer->line_capacity) {
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

    buffer->lines[line] = copy;
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

        end += 1;

        if (isEndOfFile || isNewline) {
            add_line(&state->buffer, contents, line, start, end);
            line += 1;

            start = end;
        }
        if (isEndOfFile) {
            break;
        }
    }
}

EditorState initialize_editor(const char *path)
{
    EditorState state = {0};
    initialize_state(&state);

    String contents = read_contents(path);

    file_to_buffer(&state, contents);

    string_dealloc(&contents);

    return state;
}
