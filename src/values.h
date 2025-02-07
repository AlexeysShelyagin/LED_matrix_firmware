#ifndef VALUES_H
#define VALUES_H

#include <EEPROM.h>
#include "color_container.h"

class Values{
public:
    uint16_t brightness = 100;
    bool enabled = true, paused = false;

    uint8_t current_mode = 0;

    int temperature = 4000;
    int16_t hue = 0, saturation = 150, visibility = 255;
    int16_t red = 0, green = 0, blue = 0;

    Color_container color_container;
    char current_color_id;

    uint16_t max_brightness = 255;
    bool color_memory_preview = false;

    Values() = default;

    void load();
    void save();

    void clear();
};

extern Values *values;

#endif