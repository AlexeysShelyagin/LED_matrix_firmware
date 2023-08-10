#ifndef VALUES_H
#define VALUES_H

#include <EEPROM.h>

class Values{
public:
    int brightness = 100;

    int temperature = 4000;
    int hue = 0, saturation = 0, visibility = 0;
    int red = 0, green = 0, blue = 0;
    
    Values();
};

#endif