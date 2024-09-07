#ifndef SETTINGS_INCLUDE_GUARD
#define SETTINGS_INCLUDE_GUARD

#include "string/string.h"
#include "structures/types.h"


typedef struct {
    i32 starting_window_width;
    i32 starting_window_height;

    i32 window_swap_interval_active;
    i32 window_swap_interval_inactive;
    String window_name;
} Settings;

Settings initialize_settings();
void free_settings(Settings *settings);


#endif
