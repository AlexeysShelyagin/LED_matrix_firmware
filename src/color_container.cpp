#include "color_container.h"

bool Color_container::valid(char id){
    return !(id < 'A' || id > 'Z');
}

Color_container::RGB_color Color_container::HSV_to_RGB(HSV_color color){
    double s = color.s / 256;
    double v = color.v / 256;
    double H = color.h / 256 * 360;

    double C = s * v;
    double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    double m = v - C;

    double r, g, b;

    if(H >= 0 && H < 60)
        r = C, g = X, b = 0;
    else if(H >= 60 && H < 120)
        r = X, g = C, b = 0;
    else if(H >= 120 && H < 180)
        r = 0, g = C, b = X;
    else if(H >= 180 && H < 240)
        r = 0, g = X, b = C;
    else if(H >= 240 && H < 300)
        r = X, g = 0, b = C;
    else
        r = C, g = 0, b = X;
    
    RGB_color res;
    res.r = (r + m) * 255;
    res.g = (g + m) * 255;
    res.b = (b + m) * 255;

    return res;
}

void Color_container::load(char id){

}

void Color_container::save(char id){

}

Color_container::RGB_color Color_container::get_rgb_by_id(char id){
    if(!valid(id))
        return RGB_color();
    uint8_t i = id - 'A';
    
    return container_rgb[i];
}

Color_container::HSV_color Color_container::get_hsv_by_id(char id){
    if(!valid(id))
        return HSV_color();
    uint8_t i = id - 'A';
    
    return container_hsv[i];
}

bool Color_container::defined(char id){
    if(!valid(id))
        return false;
    uint8_t i = id - 'A';

    return (bool) (defined_mask & ((uint32_t) 1 << i));
}

bool Color_container::type(char id){
    if(!valid(id))
        return false;
    uint8_t i = id - 'A';

    return (bool) (type_mask & ((uint32_t) 1 << i));
}

void Color_container::set_to_id(char id, RGB_color color){
    if(!valid(id))
        return;
    uint8_t i = id - 'A';
    
    uint32_t mask_i = ((uint32_t) 1 << i);
    defined_mask |= mask_i;
    type_mask &= ~mask_i;
    
    container_rgb[i] = color;
}

void Color_container::set_to_id(char id, HSV_color color){
    if(!valid(id))
        return;
    uint8_t i = id - 'A';
    
    uint32_t mask_i = ((uint32_t) 1 << i);
    defined_mask |= mask_i;
    type_mask |= mask_i;
    
    container_hsv[i] = color;
}