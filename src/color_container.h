#ifndef COLOR_CONTAINER_H
#define COLOR_CONTAINER_H

#include <Arduino.h>

class Color_container{
public:
    struct RGB_color{
        int8_t r = 0, g = 0, b = 0;
    };

private:
    RGB_color container[26];
    uint32_t defined_mask = 0;
    
    bool valid(char id);

public:
    Color_container() = default;

    void load(char id = 0);
    void save(char id = 0);

    RGB_color get_by_id(char id);
    bool defined(char id = 0);
    void set_to_id(char id, RGB_color color);
};

#endif