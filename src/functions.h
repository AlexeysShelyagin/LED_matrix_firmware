#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>
#include "ui.h"
#include "values.h"

#define SPIN_SPEED_FACTOR 1.5
#define SLIDER_H 3
#define SLIDER_W 3
#define BLINK_TIMEOUT 500
#define BACK_BUTTON_TEXT "Back?"
#define BACK_BUTTON_WIDTH 44
#define BACK_BUTTON_HEIGHT 22

class Window;
class Function_event;

template < class T >
class Spin_value{
    String name;
    int x, y;

    int w = -1, h = -1;

    T *value;
    T step;
    T limit_min, limit_max;
    bool limited = false;

    bool slider_setup(Window *window);
public:
    bool active = false;

    Spin_value() = default;
    Spin_value(T &value_, T step_, int x_ = 0, int y_ = 0, String name_ = "");

    void set_size(int w_, int h_);
    void set_limits(T min, T max);
    void change(T n);

    void render(Window *window);
    void render_hor_slider(Window *window, bool show_value = true);
    void render_vert_slider(Window *window, bool show_value = true);
};

class Back_button{
    int x, y;
    int w = 16, h = 16;

    bool with_text = false;
public:
    Back_button() = default;
    Back_button(int x_, int y_);
    Back_button(int x_, int y_, int w_, int h_);

    void render(Window *window);
};

class Function_container{
    uint64_t last_call = 0;
    uint8_t selected = 0;
    uint8_t scroll = 0;

    Function_container *page = nullptr;
    uint8_t page_index = 0;

    bool element_active = false;

    template < class T >
    void vertical_sliders_template(uint8_t n, String name_list[], T *variables[], T min_lims[], T max_lims[], int st_x, int offset_x);
    
    template < class T >
    void single_spin_template(String name, T* variable, T min_lim, T max_lim, int st_x, int st_y, uint8_t type = 0, T fixed_step = 0);

    void list_spin_template(String list[], int options_n, String name = "", int x = 0, int y = 0);

    void RGB_template(int16_t *vars[3]);
    void HSV_template(int16_t *vars[3]);

    void color_ids_template();

    void func0();
    void func1();
    void func2();
    void func3();
    void func7();
    void func11();
    void func12();
    void func13();
    void func16();

    void page0();
public:
    Window* window;
    Function_event* event;

    bool quit = false;

    Function_container() = default;

    void execute(int index);
    void execute_page(int index);
};

#endif