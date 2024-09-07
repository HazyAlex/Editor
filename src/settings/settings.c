#include "settings/settings.h"


Settings initialize_settings()
{
    Settings settings;

    settings.starting_window_width = 800;
    settings.starting_window_height = 600;
    settings.window_swap_interval_active = 0;
    settings.window_swap_interval_inactive = 60;
    settings.window_name = string_from_c("Editor");

    return settings;
}

void free_settings(Settings *settings)
{
    string_dealloc(&settings->window_name);

    memset(settings, 0, sizeof(Settings));
}


