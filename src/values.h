#ifndef VALUES_H
#define VALUES_H

#include <EEPROM.h>
#include "color_container.h"

class Values{
public:
    int brightness = 100;
    bool enabled = true, paused = false;

    uint8_t current_mode = 0;

    int temperature = 4000;
    int hue = 0, saturation = 150, visibility = 255;
    int red = 0, green = 0, blue = 0;

    Color_container color_container;

    Values() = default;
};

extern Values *values;

#endif