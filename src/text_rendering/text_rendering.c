#include "text_rendering/text_rendering.h"
#include "log/log.h"
#include "opengl/wrapper.h"


void initialize_text_rendering(Settings settings, Character *characters)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        ERROR("Freetype: could not initialize the FreeType Library.\n");
        exit(FREETYPE_FAILED_TO_LOAD);
    }

    FT_Face face;
    if (FT_New_Face(ft, string_to_c(settings.font), 0, &face))
    {
        ERROR("Freetype: failed to load font.\n");
        exit(FREETYPE_FAILED_TO_LOAD);
    }

    if (FT_Set_Pixel_Sizes(face, 0, settings.font_size))
    {
        ERROR("Freetype: failed to set the font size.\n");
        exit(FREETYPE_FAILED_TO_LOAD);
    }

    prepare_load_font_texture();

    for (u8 c = 0; c < 128; ++c)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            ERROR("Freetype: Failed to load Glyph (%d).\n", c);
            continue;
        }

        u32 texture = load_font_texture(face);

        Character character = {
            .textureId = texture,
            .size = vec2_from_i32(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            .bearing = vec2_from_i32(face->glyph->bitmap_left, face->glyph->bitmap_top),
            .advance = face->glyph->advance.x,
        };
        characters[c] = character;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
