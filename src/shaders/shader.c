#include "shaders/shader.h"
#include "log/log.h"
#include "string/string.h"
#include "file/file.h"
#include "opengl/wrapper.h"
#include <stdio.h>

void initialize_shaders(Graphics *graphics)
{
    const char *vertex_shader_path = "src/shaders/vertex.glsl";
    const char *fragment_shader_path = "src/shaders/fragment.glsl";
    const char *geometry_shader_path = NULL;

    Shader shader = {0};

    u32 vertex_shader_id = 0;
    u32 fragment_shader_id = 0;
    u32 geometry_shader_id = 0;

    if (vertex_shader_path != NULL) {
        String vertex_shader_source = read_contents(vertex_shader_path);

        vertex_shader_id = load_shader(vertex_shader_source, VertexShader);

        string_dealloc(&vertex_shader_source);
        DEBUG("Vertex shader loaded (%d).\n", vertex_shader_id);
    }

    if (fragment_shader_path != NULL) {
        String fragment_shader_source = read_contents(fragment_shader_path);

        fragment_shader_id = load_shader(fragment_shader_source, FragmentShader);

        string_dealloc(&fragment_shader_source);
        DEBUG("Fragment shader loaded (%d).\n", vertex_shader_id);
    }

    if (geometry_shader_path != NULL) {
        String geometry_shader_source = read_contents(geometry_shader_path);

        geometry_shader_id = load_shader(geometry_shader_source, GeometryShader);

        string_dealloc(&geometry_shader_source);
        DEBUG("Geometry shader loaded (%d).\n", vertex_shader_id);
    }

    shader.id = create_shader_program(vertex_shader_id, fragment_shader_id, geometry_shader_id);
    DEBUG("Shader program created (%d).\n", shader.id);

    graphics->shader = shader;
}
