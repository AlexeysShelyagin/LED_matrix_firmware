#include "display.h"

Display_SSD1306::Display_SSD1306(int width_, int height_){
    display = Adafruit_SSD1306(width_, height_, &Wire, -1);
    width = width_;
    height = height_;
}

bool Display_SSD1306::init(int adress){
    return display.begin(SSD1306_SWITCHCAPVCC, adress);
}

void Display_SSD1306::init_font(int size){
    display.setTextSize(size);
    display.setTextColor(SSD1306_WHITE);
    display.cp437(true);
    display.setTextWrap(false);
}

void Display_SSD1306::clear(){
    display.clearDisplay();
}

void Display_SSD1306::show(){
    display.display();
}

int Display_SSD1306::get_text_width(String text){
    int16_t  x1, y1;
    uint16_t w, h;
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    return w;
}

void Display_SSD1306::print(String text, int cur_x, int cur_y){
    display.setCursor(cur_x, cur_y);
    display.print(text);
}

void Display_SSD1306::draw_line(int x0, int y0, int x1, int y1, bool inverted){
    uint16_t color = (inverted) ? 0 : 1;
    display.drawLine(x0, y0, x1, y1, color);
}

void Display_SSD1306::draw_rect(int x, int y, int w, int h, bool inverted, bool filled){
    uint16_t color = (inverted) ? 0 : 1;
    if(filled) display.fillRect(x, y, w, h, color);
    else display.drawRect(x, y, w, h, color);
}

void Display_SSD1306::draw_bitmap(int x, int y, const uint8_t bitmap[], int w, int h, bool inverted){
    display.drawBitmap(x, y, bitmap, w, h, !inverted);
}