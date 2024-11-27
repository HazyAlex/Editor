#include <math.h>
#include "editor/editor.h"
#include "log/log.h"
#include "shaders/shader.h"
#include "string/string.h"
#include "file/file.h"
#include "settings/settings.h"
#include "opengl/wrapper.h"
#include "text_rendering/text_rendering.h"


void execute_main_loop(EditorState *editor, Graphics graphics, Character *characters)
{
    initialize_view(editor->settings, graphics);

    double line_padding = (editor->settings.font_size / 2.0) + 14;
    double line_height = 4;

    float starting_width = 0;
    float starting_height = editor->settings.starting_window_height - 22;

    color background = { .r = 34, .g = 36, .b = 53 };

    while(! should_close_window(graphics))
    {
        set_background(background);

        for (u32 i = 0; i < editor->buffer.line_count; ++i) {
            u32 width = starting_width;
            u32 height = starting_height - (line_height * i) - (line_padding * i);

            render_text(graphics, characters, string_to_c(editor->buffer.lines[i]), color_from(225, 0, 0), width, height, 1);

            if (editor->buffer.current_line == i) {
                cursor_blink(&editor->cursor);
                render_cursor(*editor, graphics, characters, width, height);
            }
        }

        render_frame(graphics);
    }
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        ERROR("No arguments provided..\n");
        exit(EXIT_CODE_OPENING_FILE);
    }

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
    EditorState editor = initialize_editor(settings, argv[1]);
    set_editor_state(&graphics, &editor);

    DEBUG("Starting the main editor loop..\n");
    execute_main_loop(&editor, graphics, characters);

    dealloc_editor(&editor);
    dealloc_graphics_api(&graphics);
    dealloc_settings(&settings);

    return 0;
}
