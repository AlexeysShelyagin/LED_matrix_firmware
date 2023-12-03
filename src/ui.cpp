#include <ui.h>

Menu* load_menu(String menu_list[], uint8_t menu_sizes[], uint8_t menu_linking[], uint8_t menu_types[], uint8_t menu_n){
    uint8_t items_n = 0;
    uint8_t menu_list_i[menu_n];
    for(uint8_t i = 0; i < menu_n; i++){
        menu_list_i[i] = items_n;
        items_n += menu_sizes[i];
    }

    Menu* menus[menu_n];
    for(uint8_t i = 0; i < menu_n; i++){
        String items[menu_sizes[i] + 1];

        for(uint8_t j = 0; j < menu_sizes[i]; j++){
            items[j + 1] = menu_list[menu_list_i[i] + j];
        }

        menus[i] = new Menu(items, menu_sizes[i] + 1);
    }

    for(uint8_t i = 0; i < menu_n; i++){
        for(uint8_t j = 0; j < menu_sizes[i]; j++){
            uint8_t cur_i = menu_list_i[i] + j;

            menus[i] -> items[j + 1].type = menu_types[cur_i];

            if(menu_types[cur_i] == MENU){
                menus[i] -> items[j + 1].link_menu = menus[menu_linking[cur_i]];
                menus[menu_linking[cur_i]] -> items[0] = Menu_item(BACK_NAME, menus[i], true);
            }
            if(menu_types[cur_i] == FUNC || menu_types[cur_i] == PARAM){
                menus[i] -> items[j + 1].link_func = new Menu_func(menu_list[cur_i], menu_linking[cur_i]);
                menus[i] -> items[j + 1].link_func -> root = new Menu_item(BACK_NAME, menus[i], true);
            }
        }
    }

    menus[0] -> items[0].type = BACK;
    menus[0] -> items[0].name = MAIN_PAGE_NAME;

    return menus[0];
}

UI::UI(Menu* default_menu_, Display_SSD1306 *display_, int font_h_, int row_h_, bool status_line_, uint8_t home_func_id_){
    init(default_menu_, display_, font_h_, row_h_, status_line_);
}

void UI::init(Menu* default_menu_, Display_SSD1306 *display_, int font_h_, int row_h_, bool status_line_, uint8_t home_func_id_){
    default_menu = default_menu_;
    display = display_;

    home_func_id = home_func_id_;

    status_line = status_line_;
    if(status_line){
        status_window = Window(display, 0, 0, display -> width, row_h_, row_h_);
        menu_window = Window(display, 0, row_h_, display -> width, display -> height - row_h_, row_h_);
    }
    else{
        menu_window = Window(display, 0, 0, display -> width, display -> height, row_h_);
    }

    function_window = Window(
        display,
        display -> width, menu_window.y, 
        display -> width - HIDEN_MENU_WIDTH, menu_window.h, 
        row_h_
    );
    set_function_containter();
}

void UI::set_home_function_id(uint8_t home_func_id_){
    home_func_id = home_func_id_;
}

void UI::set_function_containter(){
    functions = new Function_container();
    functions -> event = &func_event;
}

void UI::render(){
    //uint64_t timeout = millis() - last_render;
    last_render = millis();

    render_status_line();

    if(current_menu == nullptr)
        render_main_screen();
    else{
        render_menu(current_menu);

        if(function_is_running)
            render_function();
    }
    
    if(functions -> quit){
        function_is_running = false;
        delete functions;
        set_function_containter();
        menu_window.w = display -> width;
        render();
    }

    event_done = false;
    display -> show();
}

void UI::render_menu(Menu* menu){
    menu_window.clear();
    for(uint8_t i = 0; i < min((int) menu -> size - scroll, menu_window.max_rows); i++){
        uint8_t i_absolute = i + scroll;
        String name = menu -> items[i_absolute].name;

        int x_offset = 0;
        if(i_absolute == current_item){
             x_offset = SELECT_X_OFFSET;
             menu_window.draw_bitmap(1, menu_window.row_h * i, bitmap_select, 8, 8);
        }

        menu_window.print(name, x_offset, menu_window.row_h * i);

        switch (menu -> items[i_absolute].type){
        case MENU:
            menu_window.draw_bitmap(max(menu_window.w - 10, MIN_ICON_X), menu_window.row_h * i + 1, bitmap_folder, 8, 8);
            break;
        case FUNC:
            menu_window.draw_bitmap(max(menu_window.w - 10, MIN_ICON_X), menu_window.row_h * i + 1, bitmap_control, 8, 8);
            break;
        case BACK:
            menu_window.draw_bitmap(max(menu_window.w - 10, MIN_ICON_X), menu_window.row_h * i + 1, bitmap_back, 8, 8);
            break;
        case PARAM:
            menu_window.draw_bitmap(max(menu_window.w - 10, MIN_ICON_X), menu_window.row_h * i + 1, bitmap_gear, 8, 8);
            break;
        default: break;
        };
    }

    if(function_is_running) menu_window.draw_right_border();
}

void UI::render_main_screen(){
    display -> clear();
    
    render_status_line();

    function_is_running = true;
    functions -> window = &menu_window;

    functions -> execute(home_func_id);
    func_event.moved = 0;
    func_event.selected = 0;

    if(functions -> quit){
        current_menu = default_menu;
        current_item = 1;
        scroll = 1;
    }

}

void UI::render_status_line(){
    if(!status_line) return;
    status_window.clear();

    status_window.print(status, 0, 0);
    status_window.print_right(status_right, status_window.w, 0);

    int icons_offset = status_window.get_text_width(status_right) + STATUS_ICONS_OFFSET;
    for(uint8_t i = 0; i < icons_on_display.size(); i++){
        status_window.draw_bitmap(
            display -> width - icons_offset - icon_sizes[icons_on_display[i]].first, 0, 
            status_icons[icons_on_display[i]],
            icon_sizes[icons_on_display[i]].first, icon_sizes[icons_on_display[i]].second   
        );
        icons_offset += icon_sizes[icons_on_display[i]].first + STATUS_ICONS_OFFSET;
    }
    
    status_window.draw_down_border();
}

void UI::render_function(){
    function_window.clear();

    functions -> execute(current_func -> index);

    func_event.moved = 0;
    func_event.selected = 0;
}

void UI::move(int steps){
    event_done = true;
    if(filter_scrolling) 
        steps = steps / abs(steps);

    if(function_is_running){
        func_event.moved += steps;
        return;
    }
    if(current_menu == nullptr) return;

    if(steps < 0)
        current_item = max(current_item + steps, 0);
    else
        current_item = min(current_item + steps, current_menu -> size - 1);

    if(current_item >= scroll + menu_window.max_rows)
        scroll = current_item - menu_window.max_rows + 1;
    else
        scroll = min(current_item, scroll);
}

void UI::select(){
    if(filter_clicks && millis() - last_click_time < MIN_CLICK_INTERVAL)
        return;
    last_click_time = millis();

    event_done = true;

    if(function_is_running){
        func_event.selected++;
        return;
    }
    if(current_menu == nullptr)
        return;
    

    uint8_t type = current_menu -> items[current_item].type;
    if(type == MENU || type == BACK){
        current_menu = current_menu -> items[current_item].link_menu;

        current_item = 1;
        scroll = 1;
    }
    if(type == FUNC || type == PARAM){
        function_is_running = true;
        current_func = current_menu -> items[current_item].link_func;
        functions -> window = &function_window;

        menu_window.w = HIDEN_MENU_WIDTH;
        function_window.x = HIDEN_MENU_WIDTH;
    }
}

uint8_t UI::add_icon_bitmap(const uint8_t bitmap[], int w, int h){
    status_icons.push_back(bitmap);
    icon_sizes.push_back(std::make_pair(w, h));
    return status_icons.size() - 1;
}

void UI::set_status(String status_){
    status = status_;
}

void UI::set_status_right(String status_){
    status_right = status_;
}

bool UI::add_icon_to_status(uint8_t ico_id){
    if(find_icon(ico_id) != icons_on_display.size())
        return false;
    
    icons_on_display.push_back(ico_id);

    return true;
}

bool UI::remove_icon_from_status(uint8_t ico_id){
    uint8_t ico_i = find_icon(ico_id);
    if(ico_i == icons_on_display.size() || icons_on_display.size() == 0)
        return false;
    
    icons_on_display.erase(icons_on_display.begin() + ico_i);

    return true;
}

uint8_t UI::find_icon(uint8_t ico_id){
    uint8_t i = 0;
    while(i < icons_on_display.size() && icons_on_display[i] != ico_id) i++;
    return i;
}

void UI::tree(){
    Serial.println("root:");
    print_tree(default_menu, 1);
}

void UI::print_tree(Menu* menu, uint8_t deep){
    for(uint8_t i = 1; i < menu -> size; i++){
        for(uint8_t j = 0; j < deep; j++) Serial.print("    ");
            Serial.print(menu -> items[i].name + ":");

        if(menu -> items[i].type == FUNC || menu -> items[i].type == PARAM){
            Serial.print(menu -> items[i].link_func -> index);
        }
        Serial.println();
        
        if(menu -> items[i].type == MENU || menu -> items[i].type == BACK){
            print_tree(menu -> items[i].link_menu, deep + 1);
        }
        
    }
}

Menu::Menu(String items_[], uint8_t size_){
    size = size_;
    for(uint8_t i = 0; i < size; i++){
        items[i].name = items_[i];
    }
}

Menu_func::Menu_func(String name_, uint8_t index_){
    name = name_;
    index = index_;
}

Menu_item::Menu_item(String name_, Menu* link, bool back){
    name = name_;
    link_menu = link;
    type = (back) ? BACK : MENU;
}

Menu_item::Menu_item(String name_, Menu_func* link){
    name = name_;
    link_func = link;
    type = FUNC;
}

Window::Window(Display_SSD1306 *display_, int x_, int y_, int w_, int h_, int row_h_){
    display = display_;
    x = x_;
    y = y_;
    w = w_;
    h = h_;

    row_h = row_h_;
    max_rows = h / row_h;
}

void Window::clear(){
    display -> draw_rect(x, y, w, h, true, true);
}

int Window::get_text_width(String text){
    return display -> get_text_width(text);
}

void Window::print(String text, int cur_x, int cur_y){
    display -> print(text, cur_x + x, cur_y + y);
}

void Window::print_right(String text, int cur_x, int cur_y){
    display -> print(text, cur_x + x - display -> get_text_width(text), cur_y + y);
}

void Window::print_fit(String text, int cur_x, int cur_y){
    int text_w = display -> get_text_width(text);
    if(cur_x + text_w > w) cur_x = w - text_w;
    if(cur_y + row_h > h) cur_y = h - row_h;
    display -> print(text, cur_x + x, cur_y + y);
}

void Window::print_centered(String text, bool center_h, int cur_y){
    int text_x = w / 2 - display -> get_text_width(text) / 2;
    int text_y = cur_y;
    if(center_h)
        text_y = h / 2 - display -> get_text_height(text) / 2;
    
    display -> print(text, text_x + x, text_y + y);
}

void Window::draw_border(bool down, bool left, bool up, bool right){
    if(down)
        draw_down_border();
    if(left)
        draw_left_border();
    if(up)
        draw_up_border();
    if(right)
        draw_right_border();
}

void Window::draw_down_border(){
    display -> draw_line(x, y + h - 1, x + w - 1, y + h - 1);
}

void Window::draw_left_border(){
    display -> draw_line(x, y, x, y + h - 1);
}

void Window::draw_up_border(){
    display -> draw_line(x, y, x + w - 1, y);
}

void Window::draw_right_border(){
    display -> draw_line(x + w - 1, y, x + w - 1, y + h - 1);
}

void Window::draw_line(int x0, int y0, int x1, int y1, bool inverted){
    display -> draw_line(x + x0, y + y0, x + x1, y + y1, inverted);
}

void Window::draw_rect(int x_, int y_, int w_, int h_, bool inverted, bool filled){
    display -> draw_rect(x + x_, y + y_, w_, h_, inverted, filled);
}

void Window::draw_bitmap(int x_, int y_, const uint8_t bitmap[], int w_, int h_, bool inverted){
    display -> draw_bitmap(x + x_, y + y_, bitmap, w_, h_, inverted);
}

void Window::draw_centered_bitmap(const uint8_t bitmap[], int w_, int h_, bool inverted){
    display -> draw_bitmap(x + w / 2 - w_ / 2, y + h / 2 - h_ / 2, bitmap, w_, h_, inverted);
}