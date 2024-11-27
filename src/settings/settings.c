#include "settings/settings.h"
#include <string.h>


Settings initialize_settings(void)
{
    Settings settings;

    settings.starting_window_width = 1280;
    settings.starting_window_height = 768;
    settings.window_swap_interval_active = 1;
    settings.window_swap_interval_inactive = 60;

    settings.window_name = string_from_c("Editor");
    settings.window_fullscreen = TRUE;
    settings.window_monitor_index = 1;

    settings.font = string_from_c("assets/fonts/clacon2.ttf");
    settings.font_size = 32;

    return settings;
}

void dealloc_settings(Settings *settings)
{
    string_dealloc(&settings->font);
    string_dealloc(&settings->window_name);

    memset(settings, 0, sizeof(Settings));
}
