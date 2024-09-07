#ifndef OPENGL_WRAPPER
#define OPENGL_WRAPPER

#include <stdlib.h>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include "settings/settings.h"


void *initialize_graphics_api(Settings *settings);
void clean_graphics_api(void *data);
void color(void *data);

#endif
