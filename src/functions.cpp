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
void Spin_value < T >::change(T n){
    if(!active) return;

    if(!limited){
        *value += n * step;
        return;
    }

    if(n > 0) *value = min(static_cast < T > (*value + n * step), limit_max);
    else *value = max(static_cast < T > (*value + n * step), limit_min);
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
        T step = ceil((max_lims[i] - min_lims[i]) * SPIN_SPEED_FACTOR / timeout);

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
    case 11:
        func11();
        break;
    case 12:
        func12();
        break;
    case 13:
        func13();
        break;
    default:
        Serial.println("Function with index " + String(index) + " not found");
        quit = true;
        break;
    }
}

void Function_container::execute_page(int index){
    switch (index){
    case 0:
        page0();
        break;
    default:
        Serial.println("Page with index " + String(index) + " not found");
        quit = true;
        break;
    }
} 

void Function_container::RGB_template(int16_t *vars[3]){
    String names[3] = {"R", "G", "B"};
    int16_t min_lims[3] = {0, 0, 0};
    int16_t max_lims[3] = {255, 255, 255};
    vertical_sliders_template < int16_t > (3, names, vars, min_lims, max_lims, 3, 40);
}

void Function_container::HSV_template(int16_t *vars[3]){
    String names[3] = {"H", "S", "V"};
    int16_t min_lims[3] = {0, 0, 0};
    int16_t max_lims[3] = {255, 255, 255};
    vertical_sliders_template < int16_t > (3, names, vars, min_lims, max_lims, 3, 40);
}

void Function_container::func0(){
    single_spin_template < int > ("Bright",&(values -> brightness), 0, 255, window -> w / 2 - 20, 1, 1);
}

void Function_container::func1(){
    single_spin_template < int > ("Temp",&(values -> temperature), 0, 60000, 1, 1, 0, 100);

    values -> current_mode = 3;
}

void Function_container::func2(){
    int16_t *vars[3] = {&(values -> hue), &(values -> saturation), &(values -> visibility)};
    HSV_template(vars);

    values -> current_mode = 1;
}

void Function_container::func3(){
    int16_t *vars[3] = {&(values -> red), &(values -> green), &(values -> blue)};
    RGB_template(vars);

    values -> current_mode = 2;
}

void Function_container::func11(){
    values -> enabled = !(values -> enabled);
    quit = true;
}

void Function_container::func12(){
    values -> paused = !(values -> paused);
    quit = true;
}

void Function_container::func13(){
    if(page != nullptr){
        window -> clear();
        page -> execute_page(page_index);

        if(page -> quit){
            delete page;
            page = nullptr;
        }
        return;
    }

    if(event -> selected){
        if(selected == 26)
            quit = true;
        else{
            values -> current_color_id = selected + 'A';
            page = new Function_container();
            page -> window = window;
            page -> event = event;
            page_index = 0;
        }
    }

    if(event -> moved){
        int select_tmp = selected + event -> moved;
        if(select_tmp < 0)
            selected = 26;
        else if(select_tmp > 26)
            selected = 0;
        else
            selected = select_tmp;

        if(selected >= scroll + window -> max_rows)
            scroll = selected - window -> max_rows + 1;
        else
            scroll = min(selected, scroll);
    }

    for(int i = 0; i < window -> max_rows; i++){
        int x_offset = 0, y = i * window -> row_h;
        if(selected == scroll + i){
            x_offset = SELECT_X_OFFSET;
            window -> draw_bitmap(1, window -> row_h * i, bitmap_select, 8, 8);
        }

        if(i + scroll > 25){
            window -> print("Back", 1 + x_offset, y);
            window -> draw_bitmap(window -> w - 10, y, bitmap_back, 8, 8);
        }
        else{
            char color_id = i + scroll + 'A';
            if(values -> color_container.defined(color_id)){
                if(values -> color_container.type(color_id)){
                    Color_container::HSV_color col = values -> color_container.get_hsv_by_id(color_id);
                    window -> print(String(color_id) + " HSV(" + String(col.h) + ", " + String(col.s) + ", " + String(col.v) + ")", 1 + x_offset, y);
                }
                else{
                    Color_container::RGB_color col = values -> color_container.get_rgb_by_id(color_id);
                    window -> print(String(color_id) + " RGB(" + String(col.r) + ", " + String(col.g) + ", " + String(col.b) + ")", 1 + x_offset, y);
                }
            }
            else
                window -> print(String(color_id) + " -", 1 + x_offset, y);
        }
    }
}

void Function_container::page0(){
    if(page_index == 1){
        Color_container::RGB_color col = values -> color_container.get_rgb_by_id(values -> current_color_id);
        int16_t *vars[3] = {&col.r, &col.g, &col.b};
        RGB_template(vars);
        
        values -> color_container.set_to_id(values -> current_color_id, col);
        return;
    }
    if(page_index == 2){
        Color_container::HSV_color col = values -> color_container.get_hsv_by_id(values -> current_color_id);
        int16_t *vars[3] = {&col.h, &col.s, &col.v};
        HSV_template(vars);
        
        values -> color_container.set_to_id(values -> current_color_id, col);
        return;
    }

    if(event -> selected){
        if(selected == 2)
            quit = true;
        else
            page_index = selected + 1;
    }

    if(event -> moved)
        selected = min(max(selected + event -> moved, 0), 2);

    int x = window -> w / 2, y = window -> h / 2 - window -> row_h / 2;
    int underline_y = window -> h / 2 + window -> row_h / 2;

    window -> print_right("RGB", x - 12, y);
    window -> print("HSV", x + 12, y);

    if(selected == 0)
        window -> draw_line(x - 32, underline_y, x - 12, underline_y);
    if(selected == 1)
        window -> draw_line(x + 30, underline_y, x + 10, underline_y);
    if(selected == 2){
        Back_button back(window -> w / 2 - BACK_BUTTON_WIDTH / 2 , window -> h / 2 - BACK_BUTTON_HEIGHT / 2);
        back.render(window);
    }
}