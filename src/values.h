#ifndef VALUES_H
#define VALUES_H

#include <EEPROM.h>

class Values{
public:
    int brightness = 255;

    int temperature = 4000;
    int hue = 0, saturation = 0, visibility = 50;
    int red = 0, green = 0, blue = 0;
    
    Values();
};

extern Values *values;

#endif