#include "wrapper.h"


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

void *initialize_graphics_api(Settings *settings)
{
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

    GLFWwindow* window = glfwCreateWindow(settings->starting_window_width, settings->starting_window_height, "Editor", NULL, NULL);
    if (! window)
    {
        ERROR("Failed to create GLFW window.\n");
        glfwTerminate();

        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowSizeLimits(window, 600, 400, GLFW_DONT_CARE, GLFW_DONT_CARE); // Min/max window size
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

    return (void *)window;
}

void clean_graphics_api(void *data)
{
    GLFWwindow *window = (GLFWwindow *)data;
    (void) window;

    glfwTerminate();
}

void color(void *data)
{
    GLFWwindow *window = (GLFWwindow *)data;

    while(! glfwWindowShouldClose(window))
    {
        glClearColor(34 / 255.f, 36 / 225.f, 53 / 255.f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
