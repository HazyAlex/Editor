#ifndef OPENGL_WRAPPER_TYPES
#define OPENGL_WRAPPER_TYPES

#include "structures/types.h"

#define ERROR_FAILED_TO_COMPILE_SHADERS 3


enum ShaderType {
    VertexShader = 1,
    FragmentShader = 2,
    GeometryShader = 3,
};

typedef struct {
    u32 id;
} Shader;

typedef struct {
    void *window;
    void *frame;
    Shader shader;
} Graphics;


#endif
