#ifndef VOLTAGE_HANDLER_H
#define VOLTAGE_HANDLER_H

#include <Arduino.h>

#define MAX_VOLTAGE_UPDATE_TIME 2000

class Voltage_handler{
    int pin = 0;
    float adj_a = 1.0, adj_b = 0.0;
    bool filtering = false;
    float last_val = 0.0, filter_range;
    uint64_t last_change = 0;
public:
    Voltage_handler() = default;
    Voltage_handler(int pin_, float adjust_factor = 1.0, float adjust_offset = 0.0);

    void enable_filtering(float range = 0.1);
    void disable_filtering();

    float get_voltage();
};

#endif