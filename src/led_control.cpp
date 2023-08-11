#include "led_control.h"

void LED_matrix::init_prepare(uint16_t w_, uint16_t h_){
    w = w_;
    h = h_;

    leds = new CRGB[w * h];
    FastLED.setBrightness(values -> brightness);
}

void LED_matrix::update(){
    for(int i = 0; i < w * h; i++)
        leds[i] = CHSV(values -> hue, values -> saturation, values -> visibility);
    FastLED.show();
}