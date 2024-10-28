#include "settings/settings.h"
#include <string.h>


Settings initialize_settings(void)
{
    Settings settings;

    settings.starting_window_width = 2560;
    settings.starting_window_height = 1440;
    settings.window_swap_interval_active = 0;
    settings.window_swap_interval_inactive = 60;

    settings.window_name = string_from_c("Editor");
    settings.window_fullscreen = TRUE;
    settings.window_monitor_index = 0;

    settings.font = string_from_c("assets/fonts/clacon2.ttf");
    settings.font_size = 32;

    return settings;
}

void free_settings(Settings *settings)
{
    string_dealloc(&settings->font);
    string_dealloc(&settings->window_name);

    memset(settings, 0, sizeof(Settings));
}
