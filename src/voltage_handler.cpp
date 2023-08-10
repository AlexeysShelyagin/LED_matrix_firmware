#include "voltage_handler.h"

Voltage_handler::Voltage_handler(int pin_, float adjust_factor, float adjust_offset){
    pin = pin_;
    adj_a = adjust_factor;
    adj_b = adjust_offset;
}

void Voltage_handler::enable_filtering(float range){
    filtering = true;
    filter_range = range;
}

void Voltage_handler::disable_filtering(){
    filtering = false;
}

float Voltage_handler::get_voltage(){
    int analog_val = analogRead(pin);
    
    float voltage = analog_val / 1024.0 * 33.0 * adj_a + adj_b;

    if(filtering && abs(voltage - last_val) <= filter_range && last_change + MAX_VOLTAGE_UPDATE_TIME <= millis())
        voltage = last_val;
    
    last_val = voltage;

    return max(voltage, (float) 0.0);
}