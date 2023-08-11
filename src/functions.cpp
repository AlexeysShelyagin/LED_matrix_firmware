#include "functions.h"

template < class T >
Spin_value < T >::Spin_value(T &value_, T step_, int x_, int y_, String name_){
    x = x_;
    y = y_;
    name = name_;
    value = &value_;
    step = step_;
}

template < class T >
void Spin_value < T >::set_size(int w_, int h_){
    w = w_;
    h = h_;
}

template < class T >
void Spin_value < T >::set_limits(T min, T max){
    limit_min = min;
    limit_max = max;
    limited = true;
}

template < class T >
void Spin_value < T >::change(int n){
    if(!active) return;

    if(!limited){
        *value += n * step;
        return;
    }

    if(n > 0) *value = min(*value + n * step, limit_max);
    else *value = max(*value + n * step, limit_min);
}

template < class T >
void Spin_value < T >::render(Window *window){
    String bracket_left = ":< ", bracket_right = " >";
    bool animate_state = ((millis() / BLINK_TIMEOUT) % 2 == 0);
    if(!active || animate_state){
        bracket_left = ":  ";
        bracket_right = "  ";
    }

    window -> print(name + bracket_left + String(*value) + bracket_right, x, y);
}

template < class T >
bool Spin_value < T >::slider_setup(Window *window){
    if(!limited){
        Serial.println("Unable to draw slider <" + name + ">");
        return false;
    }
    if(w == -1 || h == -1){
        w = window -> w;
        h = window -> h;
    }

    return true;
}

template < class T >
void Spin_value < T >::render_hor_slider(Window *window, bool show_value){
    if(!slider_setup(window)) return;
    int row_h = y + window -> row_h;

    window -> print(String(limit_min), x, y);
    window -> print_right(String(limit_max), x + w, y);

    int w_filled = (double) (*value - limit_min) / (limit_max - limit_min) * w;
    window -> draw_rect(x, row_h, w, SLIDER_H);
    window -> draw_line(x, row_h - 1, x, row_h + SLIDER_H);
    window -> draw_line(x + w, row_h - 1, x + w, row_h + SLIDER_H);
    window -> draw_line(x + w_filled, row_h - 1, x + w_filled, row_h + SLIDER_H);

    bool animate_state = ((millis() / BLINK_TIMEOUT) % 2 == 0);
    if(!active || animate_state)
        window -> draw_rect(x, row_h, w_filled, SLIDER_H, false, true);

    if(show_value)
        window -> print_fit(String(*value), x + w_filled, row_h + SLIDER_H + 1);
}

template < class T >
void Spin_value < T >::render_vert_slider(Window *window, bool show_value){
    if(!slider_setup(window)) return;
    int row_h = y + window -> row_h - 3;
    int offset = max(
        window -> get_text_width(String(limit_max)), 
        window -> get_text_width(String(limit_min))
    );

    window -> print_right(String(limit_max), x + offset, y);
    window -> print_right(String(limit_min), x + offset, y + h - row_h);

    int h_filled = h - (double) (*value - limit_min) / (limit_max - limit_min) * h;
    window -> draw_rect(x + offset + 1, y, SLIDER_W, h);
    window -> draw_line(x + offset, y, x + offset + SLIDER_W + 2, y);
    window -> draw_line(x + offset, y + h, x + offset + SLIDER_W + 2, y + h);
    window -> draw_line(x + offset, y + h_filled, x + offset + SLIDER_W + 2, y + h_filled);

    bool animate_state = ((millis() / BLINK_TIMEOUT) % 2 == 0);
    if(!active || animate_state)
        window -> draw_rect(x + offset + 1, y + h_filled, SLIDER_W, h - h_filled, false, true);

    if(show_value)
        window -> print_fit(String(*value), x + offset + SLIDER_W + 3, y + h_filled);
}

Back_button::Back_button(int x_, int y_, int w_, int h_){
    x = x_;
    y = y_;
    w = w_;
    h = h_;
}

Back_button::Back_button(int x_, int y_){
    x = x_;
    y = y_;
    w = BACK_BUTTON_WIDTH;
    h = BACK_BUTTON_HEIGHT;
    with_text = true;
}

void Back_button::render(Window *window){
    window -> draw_rect(x, y, w, h, true, true);
    window -> draw_rect(x, y, w, h);

    if(with_text){
        window -> print(BACK_BUTTON_TEXT, x + 8, y + 2);
    }
}

template < class T >
void Function_container::vertical_sliders_template(uint8_t n, String name_list[], T *variables[], T min_lims[], T max_lims[], int st_x, int offset_x){
    uint64_t timeout = millis() - last_call;
    if(event -> moved != 0) last_call = millis();

    Spin_value < T > *value_list[n];
    for(uint8_t i = 0; i < n; i++){
        int step = ceil((max_lims[i] - min_lims[i]) * SPIN_SPEED_FACTOR / timeout);

        value_list[i] = new Spin_value(*variables[i], step, st_x + offset_x * i, window -> row_h, name_list[i]);
        value_list[i] -> set_size(offset_x, window -> h - 3 - window -> row_h);
        value_list[i] -> set_limits(min_lims[i], max_lims[i]);
    }

    Back_button back(window -> w / 2 - BACK_BUTTON_WIDTH / 2 , window -> h / 2 - BACK_BUTTON_HEIGHT / 2);

    if(event -> selected)
        element_active = !element_active;

    for(uint8_t i = 0; i < n; i++){
        value_list[i] -> active = false;
    }

    if(element_active){
        if(0 <= selected && selected < n){
            value_list[selected] -> active = true;
            value_list[selected] -> change(event -> moved);
        }
        
        if(selected == n)
            quit = true;
    }
    else{
        selected += event -> moved;
        selected %= n + 1;
    }
    
    for(uint8_t i = 0; i < n; i++){
        if(element_active || i != selected || (millis() / BLINK_TIMEOUT) % 2 != 0)
            window -> print(name_list[i] + ":" + String(*variables[i]), offset_x * i + st_x - 2, 0);
        value_list[i] -> render_vert_slider(window, false);
    }
    if(selected == n)
        back.render(window);

    for(uint8_t i = 0; i < n; i++)
        delete value_list[i];
}

template < class T >
void Function_container::single_spin_template(String name, T* variable, T min_lim, T max_lim, int st_x, int st_y, uint8_t type, T fixed_step){
    uint64_t timeout = millis() - last_call;
    if(event -> moved != 0) last_call = millis();
    int step;
    if(fixed_step == 0) 
        step = ceil((max_lim - min_lim) * SPIN_SPEED_FACTOR / timeout);
    else 
        step = fixed_step;

    Spin_value < T > spin_val(*variable, step, st_x, st_y, name);
    spin_val.set_limits(min_lim, max_lim);
    spin_val.active = true;

    if(event -> selected)
        quit = true;

    spin_val.change(event -> moved);
    
    switch (type){
    case 0:
        spin_val.render(window);
        break;
    case 1:
        spin_val.set_size(0, window -> h - 3 - st_y);
        spin_val.render_vert_slider(window);
        break;
    default: break;
    }
        
}

void Function_container::execute(int index){
    switch (index){
    case 0:
        func0();
        break;
    case 1:
        func1();
        break;
    case 2:
        func2();
        break;
    case 3:
        func3();
        break;
    default:
        Serial.println("Function with index " + String(index) + " not found");
        quit = true;
        break;
    }
}

void Function_container::func0(){
    single_spin_template < int > ("Bright",&(values -> brightness), 0, 255, window -> w / 2 - 20, 1, 1);
}

void Function_container::func1(){
    single_spin_template < int > ("Temp",&(values -> temperature), 0, 10000, 1, 1, 0, 100);
}

void Function_container::func2(){
    int *vars[3] = {&(values -> hue), &(values -> saturation), &(values -> visibility)};
    String names[3] = {"H", "S", "V"};
    int min_lims[3] = {0, 0, 0};
    int max_lims[3] = {255, 255, 255};
    vertical_sliders_template < int > (3, names, vars, min_lims, max_lims, 3, 40);
}

void Function_container::func3(){
    int *vars[3] = {&(values -> red), &(values -> green), &(values -> blue)};
    String names[3] = {"R", "G", "B"};
    int min_lims[3] = {0, 0, 0};
    int max_lims[3] = {255, 255, 255};
    vertical_sliders_template < int > (3, names, vars, min_lims, max_lims, 3, 40);
}