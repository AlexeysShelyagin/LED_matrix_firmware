#include "led_control.h"

double clamp(double val, double min, double max){
    if(val < min)
        return min;
    if(val > max)
        return max;

    return val;
}

CRGB temp_to_rgb(int temp_in_kelvin){
    double temp = temp_in_kelvin / 100.0;
    double red, green, blue;

    if(temp <= 66){
        red = 255;

        green = temp;
        green = 99.4708025861 * log(green) - 161.1195681661;

        if(temp <= 19)
            blue = 0;
        else{
            blue = temp - 10;
            blue = 138.5177312231 * log(blue) - 305.0447927307;
        }
    }
    else{
        red = temp - 60;
        red = 329.698727446 * pow(red, -0.1332047592);

        green = temp - 60;
        green = 288.1221695283 * pow(green, -0.0755148492 );

        blue = 255;
    }

    return CRGB(
        clamp(red, 0, 255),
        clamp(green, 0, 255),
        clamp(blue, 0, 255)
    );
}

void LED_matrix::init_prepare(uint16_t w_, uint16_t h_){
    w = w_;
    h = h_;

    leds = new CRGB[w * h];
}

void LED_matrix::update(){
    if(enabled)
        FastLED.setBrightness(values -> brightness);
    else
        FastLED.setBrightness(0);
    if(paused){
        FastLED.show();
        return;
    }

    if(values -> current_mode == 1){
        for(int i = 0; i < w * h; i++)
            leds[i] = CHSV(values -> hue, values -> saturation, values -> visibility);
    }
    if(values -> current_mode == 2){
        for(int i = 0; i < w * h; i++)
            leds[i] = CRGB(values -> red, values -> green, values -> blue);
    }
    if(values -> current_mode == 3){
        for(int i = 0; i < w * h; i++)
            leds[i] = temp_to_rgb(values -> temperature);
    }


    FastLED.show();
}

void LED_matrix::disable(){
    enabled = false;
    paused = true;
}

void LED_matrix::enable(){
    enabled = true;
    paused = false;
}

void LED_matrix::pause(){
    paused = true;
}

void LED_matrix::unpause(){
    paused = false;
}