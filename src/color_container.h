#ifndef COLOR_CONTAINER_H
#define COLOR_CONTAINER_H

#include <Arduino.h>
#include <EEPROM.h>

#include "config.h"

class Color_container{
public:
    struct RGB_color{
        int16_t r = 0, g = 0, b = 0;
    };
    struct HSV_color{
        int16_t h = 0, s = 0, v = 0;
    };

private:
    RGB_color container_rgb[26];
    HSV_color container_hsv[26];
    uint32_t defined_mask = 0;
    uint32_t type_mask = 0;
    
    bool valid(char id);

    RGB_color HSV_to_RGB(HSV_color color);

public:
    Color_container() = default;

    void load_init();
    void load(char id = 0);
    void save(char id = 0, bool commit = false);
    void del(char id = 0);
    void commit();

    RGB_color get_rgb_by_id(char id);
    HSV_color get_hsv_by_id(char id);
    bool defined(char id = 0);
    bool type(char id = 0);
    void set_to_id(char id, RGB_color color);
    void set_to_id(char id, HSV_color color);
};

#endif