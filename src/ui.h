#ifndef UI_H
#define UI_H

#define MAX_MENU_ITEMS 30

#include <Arduino.h>

#include "display.h"
#include "functions.h"
#include "bitmaps.h"

#define SELECT_X_OFFSET 10
#define MAIN_PAGE_NAME "Home"
#define BACK_NAME "Back"
#define HIDEN_MENU_WIDTH 12
#define LIMIT_SCROLL_TO_1 true      // filter multiple triggering
#define FILTER_CLICKS true          // filter multiple triggering
#define MIN_CLICK_INTERVAL 200
#define STATUS_ICONS_OFFSET 4

#define MENU 0
#define BACK 1
#define FUNC 2
#define PARAM 3

class Menu;
class Menu_item;
class Menu_func;
class Function_container;

class Window{
public:
    int x = 0, y = 0;
    int w = 0, h = 0;
    int row_h, max_rows;
    Display_SSD1306 *display;

    Window() = default;
    Window(Display_SSD1306 *display_, int x_, int y_, int w_ = 0, int h_ = 0, int row_h_ = 10);

    void clear();
    void draw_border(bool down = true, bool left = true, bool up = true, bool right = true);
    void draw_down_border();
    void draw_left_border();
    void draw_up_border();
    void draw_right_border();

    int get_text_width(String text);

    void print(String text, int cur_x = 0, int cur_y = 0);
    void print_right(String text, int cur_x = 0, int cur_y = 0);
    void print_fit(String text, int cur_x = 0, int cur_y = 0);
    void draw_line(int x0, int y0, int x1, int y1, bool inverted = false);
    void draw_rect(int x_, int y_, int w_, int h_, bool inverted = false, bool filled = false);
    void draw_bitmap(int x_, int y_, const uint8_t bitmap[], int w_, int h_, bool inverted = false);
};

class Function_event{
public:
    int moved = 0, selected = 0;

    Function_event() = default;
};

class UI{
    Display_SSD1306 *display;
    uint8_t current_item = 1, scroll = 1;

    Window menu_window, status_window, function_window;
    bool status_line;
    uint64_t last_render = 0;

    String status = "", status_right = "";
    std::vector < const uint8_t* > status_icons;
    std::vector < std::pair < int, int > > icon_sizes;
    std::vector < uint8_t > icons_on_display;

    Function_event func_event;
    Function_container* functions;
    Menu_func* current_func = nullptr;

    bool filter_scrolling = LIMIT_SCROLL_TO_1;
    bool filter_clicks = FILTER_CLICKS;
    uint64_t last_click_time = 0;

    void set_function_containter();

    void render_menu(Menu* menu);
    void render_main_screen();
    void render_status_line();
    void render_function();

    uint8_t find_icon(uint8_t ico_id);

    void print_tree(Menu* menu, uint8_t deep);
public:
    Menu* current_menu = nullptr;
    Menu* default_menu;

    bool event_done = true;
    bool function_is_running = false;

    UI() = default;
    UI(Menu* default_menu_, Display_SSD1306 *display_, int font_h_, int row_h_, bool status_line_ = false);
    void init(Menu* default_menu_, Display_SSD1306 *display_, int font_h_, int row_h_, bool status_line_ = false);

    void render();

    void move(int steps);
    void select();

    uint8_t add_icon_bitmap(const uint8_t bitmap[], int w, int h);
    void set_status(String status_);
    void set_status_right(String status_);
    bool add_icon_to_status(uint8_t ico_id);
    bool remove_icon_from_status(uint8_t ico_id);

    void tree();
};

class Menu_func{
    int moved = 0, selected = 0;
public:
    String name;
    uint8_t index;

    Menu_item* root;

    Menu_func() = default;
    Menu_func(String name_, uint8_t index_);
};

class Menu_item{
public:
    String name = "";
    
    Menu* link_menu = nullptr;
    Menu_func* link_func = nullptr;

    uint8_t type;

    Menu_item() = default;
    Menu_item(String name_, Menu* link, bool back = false);
    Menu_item(String name_, Menu_func* link);
};

class Menu{
public:
    Menu_item items[MAX_MENU_ITEMS];
    uint8_t size = 0;

    Menu() = default;
    Menu(String items_[], uint8_t size_);
};

Menu* load_menu(String menu_list[], uint8_t menu_sizes[], uint8_t menu_linking[], uint8_t menu_types[], uint8_t menu_n);

#endif