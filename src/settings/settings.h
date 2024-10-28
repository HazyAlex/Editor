#ifndef SETTINGS_INCLUDE_GUARD
#define SETTINGS_INCLUDE_GUARD

#include "structures/types.h"
#include "string/string.h"


typedef struct {
    i32 starting_window_width;
    i32 starting_window_height;

    String window_name;
    bool window_fullscreen; // 0 - windowed, 1 - fullscreen
    i8 window_monitor_index; // monitor index (fullscreen only)
    i32 window_swap_interval_active;
    i32 window_swap_interval_inactive;

    String font;
    u32 font_size;
} Settings;

Settings initialize_settings(void);
void free_settings(Settings *settings);


#endif
