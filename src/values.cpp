#include "values.h"

void Values::load(){
    color_container.load_init();

    EEPROM.get(EEPROM_SETTINGS_ADDR, max_brightness);
    EEPROM.get(EEPROM_SETTINGS_ADDR + 2, color_memory_preview);

    if(max_brightness == 0)
        max_brightness = 255;
}

void Values::save(){
    Serial.println("aa");
    EEPROM.put(EEPROM_SETTINGS_ADDR, max_brightness);
    EEPROM.put(EEPROM_SETTINGS_ADDR + 2, color_memory_preview);

    EEPROM.commit();
}

void Values::clear(){
    for (int i = 0; i < EEPROM_SIZE; i++)
        EEPROM.write(i, 0);

    EEPROM.commit();
}