#include <math.h>
#include <time.h>
#include "editor/editor.h"
#include "log/log.h"
#include "shaders/shader.h"
#include "string/string.h"
#include "settings/settings.h"
#include "opengl/wrapper.h"
#include "text_rendering/text_rendering.h"


void execute_main_loop(EditorState editor, Settings settings, Graphics graphics, Character *characters)
{
    initialize_view(settings, graphics);

    double line_padding = (settings.font_size / 2.0) + 14;
    double line_height = 4;

    float starting_width = 0;
    float starting_height = settings.starting_window_height - 22;

    time_t cursor_timer = time(NULL);

    color background = { .r = 34, .g = 36, .b = 53 };

    while(! should_close_window(graphics))
    {
        set_background(background);

        for (u32 i = 0; i < editor.buffer.line_count; ++i) {
            u32 width = starting_width;
            u32 height = starting_height - (line_height * i) - (line_padding * i);

            render_text(graphics, characters, string_to_c(editor.buffer.lines[i]), color_from(225, 0, 0), width, height, 1);

            if (editor.buffer.current_column == i) {
                const char *line = string_to_c(editor.buffer.lines[i]);
                const char character = line[editor.buffer.current_column];

                cursor_blink(&editor.cursor, &cursor_timer);
                render_cursor(graphics, characters, character, editor.cursor, width, height);
            }
        }

        render_frame(graphics);
    }
}

int main(void) {
    DEBUG("Initializing settings..\n");
    Settings settings = initialize_settings();

    DEBUG("Initializing graphics API..\n");
    Graphics graphics = initialize_graphics_api(&settings);

    DEBUG("Initializing text rendering..\n");
    Character characters[128] = {0};
    initialize_text_rendering(settings, characters);

    DEBUG("Initializing the shaders..\n");
    initialize_shaders(&graphics);

    DEBUG("Initializing the editor..\n");
    EditorState editor = initialize_editor("fixtures/test.txt");

    DEBUG("Starting the main editor loop..\n");
    execute_main_loop(editor, settings, graphics, characters);

    clean_graphics_api(&graphics);

    return 0;
}
