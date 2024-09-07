#include <stdio.h>
#include <stdlib.h>

#include "structures/types.h"
#include "log/log.h"
#include "string/string.h"
#include "file/file.h"
#include "settings/settings.h"
#include "opengl/wrapper.h"


void execute_main_loop(void *graphics)
{
    color(graphics);
}

int main() {
    DEBUG("Initializing settings..\n");
    Settings settings = initialize_settings();

    DEBUG("Initializing graphics API..\n");
    void *graphics = initialize_graphics_api(&settings);

    DEBUG("Reading the file..\n");
    String contents = read_contents("fixtures/test.txt");
    DEBUG("Data:\n%s", contents.data);

    execute_main_loop(graphics);

    string_dealloc(&contents);
    clean_graphics_api(graphics);

    return 0;
}