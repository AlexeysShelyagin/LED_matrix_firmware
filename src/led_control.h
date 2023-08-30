#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <Arduino.h>
#include <FastLED.h>

#include "values.h"

class LED_matrix{
    CRGB *leds;
    uint16_t w, h;
    bool enabled = true, paused = false;

    void init_prepare(uint16_t w_, uint16_t h_);
public:
    LED_matrix() = default;

    template < size_t PIN >
    void init(uint16_t w_, uint16_t h_){
        init_prepare(w_, h_);
        FastLED.addLeds < WS2812B, PIN, GRB > (leds, w * h).setCorrection(TypicalLEDStrip);
        disable();
    }

    void update();
    
    void disable();
    void enable();
    void pause();
    void unpause();
};

#endif