#ifndef OPENGL_WRAPPER
#define OPENGL_WRAPPER

#include "structures/types.h"
#include "settings/settings.h"
#include "text_rendering/text_rendering.h"
#include "graphics/types.h"
#include "editor/editor.h"


Graphics initialize_graphics_api(Settings *settings);
void initialize_view(Settings settings, Graphics graphics);
bool should_close_window(Graphics graphics);
void dealloc_graphics_api(Graphics *graphics);

void prepare_load_font_texture(void);
u32 load_font_texture(FT_Face face);

u32 load_shader(String source, enum ShaderType type);
u32 create_shader_program(u32 vertex_shader_id, u32 fragment_shader_id, u32 geometry_shader_id);

void set_editor_state(Graphics *graphics, EditorState *editor);
void set_background(color background);
void render_text(Graphics graphics, Character *characters, const char *text, color text_color, float x, float y, float scale);
void render_cursor(EditorState editor, Graphics graphics, Character *characters, float x, float y);
void render_frame(Graphics graphics);


#endif
