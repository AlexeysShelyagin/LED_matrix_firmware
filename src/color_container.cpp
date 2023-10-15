#include "color_container.h"

bool Color_container::valid(char id){
    return !(id < 'A' || id > 'Z');
}

void Color_container::load(char id){

}

void Color_container::save(char id){

}

Color_container::RGB_color Color_container::get_by_id(char id){
    if(!valid(id))
        return RGB_color();
    uint8_t i = id - 'A';
    
    return container[i];
}

bool Color_container::defined(char id){
    if(!valid(id))
        return false;
    uint8_t i = id - 'A';

    return (bool) (defined_mask & ((uint32_t) 1 << i));
}

void Color_container::set_to_id(char id, RGB_color color){
    if(!valid(id))
        return;
    uint8_t i = id - 'A';
    
    defined_mask |= ((uint32_t) 1 << i);
    
    container[i] = color;
}