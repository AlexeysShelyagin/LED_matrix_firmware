#ifndef DISPLAY_H
#define DISPLAY_H

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#include "config.h"

class Display_SSD1306{
public:
    Adafruit_SSD1306 display;
    int width, height;

    Display_SSD1306(int width_, int height_);

    bool init(int adress);
    void init_font(int size = 1);

    void clear();
    void show();

    int get_text_width(String text);

    void print(String text, int cur_x = 0, int cur_y = 0);
    void draw_line(int x0, int y0, int x1, int y1, bool inverted = false);
    void draw_rect(int x, int y, int w, int h, bool inverted = false, bool filled = false);
    void draw_bitmap(int x, int y, const uint8_t bitmap[], int w, int h, bool inverted = false);
};

#endif