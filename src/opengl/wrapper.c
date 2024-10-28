#include "wrapper.h"
#include "log/log.h"
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"
#include "string/string.h"
#include "text_rendering/text_rendering.h"


void error_callback(int error, const char* description)
{
    ERROR("GLFW error (%d): %s\n", error, description);
}

void window_iconify_callback(GLFWwindow* window, int minimized)
{
    Settings *settings = (Settings *)glfwGetWindowUserPointer(window);

    if (minimized) {
        glfwSwapInterval(settings->window_swap_interval_inactive);
    } else {
        glfwSwapInterval(settings->window_swap_interval_active);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;

    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void) scancode;
    (void) mods;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

GLFWmonitor *getMonitor(Settings *settings) {
    if (! settings->window_fullscreen) {
        return NULL;
    }

    i32 index = settings->window_monitor_index;
    i32 count = 0;
    GLFWmonitor **monitors = glfwGetMonitors(&count);

    if (index > count) {
        return NULL;
    }

    return monitors[index];
}

GLFWwindow *initialize_window(Settings *settings) {
    glfwSetErrorCallback(error_callback);

    if (! glfwInit()) {
        ERROR("Could not initialize GLFW.\n");

        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Do not use deprecated functionality, only 3.0+
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Hide the old GL stuff, only 3.2+

#ifdef GL_DEBUG
    glfwWindowHint(GLFW_CONTEXT_DEBUG, GL_TRUE); // Debug mode
#endif

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    glfwWindowHint(GLFW_DECORATED, GL_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
    glfwWindowHint(GLFW_CENTER_CURSOR, GL_TRUE);
    glfwWindowHint(GLFW_FLOATING, GL_TRUE); // Always on top
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE); // Window refresh -> highest value (GL_DONT_CARE)

    GLFWmonitor *monitor = getMonitor(settings);

    GLFWwindow* window = glfwCreateWindow(settings->starting_window_width, settings->starting_window_height, "Editor", monitor, NULL);
    if (! window)
    {
        ERROR("Failed to create GLFW window.\n");
        glfwTerminate();

        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowSizeLimits(window, 600, 400, GLFW_DONT_CARE, GLFW_DONT_CARE); // Set the min/max window size
    glfwSetWindowPos(window, 800, 600);
    glfwSwapInterval(settings->window_swap_interval_active);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowIconifyCallback(window, window_iconify_callback);
    glfwSetWindowUserPointer(window, settings);

    if (gladLoadGL() == 0)
    {
        ERROR("Failed to initialize GLAD.\n");

        exit(EXIT_FAILURE);
    }

    return window;
}

Graphics initialize_graphics_api(Settings *settings)
{
    GLFWwindow *window = initialize_window(settings);

    // Setup the OpenGL state.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Build the Graphics object.
    // The frame data will hold the VAO and VBO.
    uvec2 *frame = calloc(1, sizeof(uvec2));

    Graphics graphics = {
        .window = (void*)window,
        .frame = (void*)frame,
        .shader = {0}
    };

    return graphics;
}

bool should_close_window(Graphics graphics)
{
    GLFWwindow *window = (GLFWwindow *)graphics.window;

    return glfwWindowShouldClose(window);
}

void clean_graphics_api(Graphics *graphics)
{
    GLFWwindow *window = (GLFWwindow *)graphics->window;
    (void) window;

    uvec2 *frame = (uvec2 *)graphics->frame;
    frame->x = 0;
    frame->y = 0;
    free(frame);

    glfwTerminate();
}

void initialize_view(Settings settings, Graphics graphics)
{
    const double left = 0.0;
    const double right = (double) settings.starting_window_width;
    const double top = (double) settings.starting_window_height;
    const double bottom = 0.0;

    const double far = 1000;
    const double near = 0.01;

    GLfloat projection[4][4] = {0};
    projection[0][0] = 2.0 / (right - left);
    projection[1][1] = 2.0 / (top - bottom);
    projection[2][2] = 1.0 / (far - near);
    projection[3][0] = - (right + left) / (right - left);
    projection[3][1] = - (top + bottom) / (top - bottom);
    projection[3][2] = - near / (far - near);
    projection[3][3] = 1;

    glUseProgram(graphics.shader.id);
    glUniformMatrix4fv(glGetUniformLocation(graphics.shader.id, "projection"), 1, GL_FALSE, (const GLfloat *) &projection);

    u32 VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    uvec2 *frame = (uvec2 *)graphics.frame;
    frame->x = VAO;
    frame->y = VBO;
}

void prepare_load_font_texture(void)
{
    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

u32 load_font_texture(FT_Face face)
{
    u32 texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RED,
        face->glyph->bitmap.width, face->glyph->bitmap.rows,
        0, GL_RED, GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

void check_shader_compile_errors(u32 shader_id, const char *name)
{
    GLint success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLchar log[1024];
        glGetShaderInfoLog(shader_id, sizeof(log), NULL, log);

        ERROR("Shader (%s) compilation error: %s", name, log);
        exit(ERROR_FAILED_TO_COMPILE_SHADERS);
    }
}

u32 load_shader(String source, enum ShaderType type)
{
    u32 opengl_shader_id = 0;
    const char *shader_name = "";

    switch (type) {
        case VertexShader:
            opengl_shader_id = GL_VERTEX_SHADER;
            shader_name = "VERTEX";
            break;
        case FragmentShader:
            opengl_shader_id = GL_FRAGMENT_SHADER;
            shader_name = "FRAGMENT";
            break;
        case GeometryShader:
            opengl_shader_id = GL_GEOMETRY_SHADER;
            shader_name = "GEOMETRY";
            break;
    };

    u32 shader_id = glCreateShader(opengl_shader_id);

    glShaderSource(shader_id, 1, (const GLchar *const *)&source.data, NULL);
    glCompileShader(shader_id);
    check_shader_compile_errors(shader_id, shader_name);

    return shader_id;
}

u32 create_shader_program(u32 vertex_shader_id, u32 fragment_shader_id, u32 geometry_shader_id)
{
    u32 program_id = glCreateProgram();

    if (vertex_shader_id > 0)   glAttachShader(program_id, vertex_shader_id);
    if (fragment_shader_id > 0) glAttachShader(program_id, fragment_shader_id);
    if (geometry_shader_id > 0) glAttachShader(program_id, geometry_shader_id);

    glLinkProgram(program_id);
    check_shader_compile_errors(program_id, "PROGRAM");

    if (vertex_shader_id > 0)   glDeleteShader(vertex_shader_id);
    if (fragment_shader_id > 0) glDeleteShader(fragment_shader_id);
    if (geometry_shader_id > 0) glDeleteShader(geometry_shader_id);

    return program_id;
}

void set_background(color background)
{
    glClearColor(background.r / 255.0, background.g / 225.0, background.b / 255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void render_text(Graphics graphics, Character *characters, const char *text, color text_color, float x, float y, float scale)
{
    uvec2 *frame_data = (uvec2 *)graphics.frame;
    u32 VAO = frame_data->x;
    u32 VBO = frame_data->y;

    glUseProgram(graphics.shader.id);
    glUniform3f(glGetUniformLocation(graphics.shader.id, "textColor"), text_color.r, text_color.g, text_color.b);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (u32 i = 0; i < strlen(text); ++i) {
        u8 character_index = (u8) text[i];
        if (character_index < 32) continue; // Ignore ASCII terminal control characters.

        Character ch = characters[character_index];

        float xPos = x + ch.bearing.x * scale;
        float yPos = y - (ch.size.y - ch.bearing.y) * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        // Update the VBO for each character
        float vertices[6][4] = {
            { xPos,     yPos + h,   0.0f, 0.0f },
            { xPos,     yPos,       0.0f, 1.0f },
            { xPos + w, yPos,       1.0f, 1.0f },

            { xPos,     yPos + h,   0.0f, 0.0f },
            { xPos + w, yPos,       1.0f, 1.0f },
            { xPos + w, yPos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureId);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Advance the cursor for the next glyph (advance is number of 1/64 pixels, 2^6 = 64)
        x += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void render_frame(Graphics graphics)
{
    GLFWwindow *window = (GLFWwindow *)graphics.window;

    glfwSwapBuffers(window);
    glfwPollEvents();
}
