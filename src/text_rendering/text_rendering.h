#ifndef TEXT_RENDERING_INCLUDE_GUARD
#define TEXT_RENDERING_INCLUDE_GUARD

#include "structures/types.h"
#include "settings/settings.h"
#include <freetype/freetype.h>


#define FREETYPE_FAILED_TO_LOAD 2

typedef struct {
    u32 textureId;  // ID handle of the glyph texture
    u32 advance;    // Offset to advance to next glyph
    ivec2 size;     // Size of glyph
    ivec2 bearing;  // Offset from baseline to left/top of glyph
} Character;

void initialize_text_rendering(Settings settings, Character *characters);


#endif
