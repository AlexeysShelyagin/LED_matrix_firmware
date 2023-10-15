#include <Arduino.h>
#include <EEPROM.h>

#include "config.h"
#include "display.h"
#include "encoder.h"
#include "ui.h"
#include "menu_data.h"
#include "voltage_handler.h"
#include "led_control.h"
#include "values.h"

Display_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
Encoder encoder(SA_PIN, SB_PIN, SW_PIN);
UI ui;
LED_matrix matrix;
Voltage_handler voltage(VOLTAGE_PIN, VOLTAGE_ADJUST_FACTOR, VOLTAGE_ADJUST_OFFSET);
Values *values = new Values();
uint64_t update_timer = 0;

bool ready_to_read = true;
bool turned = false;
bool dir;

IRAM_ATTR void encoder_interrupt(){
    encoder.tick();
}

void update_status_bar(){
    bool need_to_disable = false;

    float now_v = voltage.get_voltage();
    if(now_v >= MIN_OPERATING_VOLTAGE){
        ui.remove_icon_from_status(3);
    }
    else{
        need_to_disable = true;
        ui.add_icon_to_status(3);
    }

    if(!values -> enabled){
        need_to_disable = true;
        ui.add_icon_to_status(1);
    }
    else{
        ui.remove_icon_from_status(1);
    }

    if(need_to_disable)
        matrix.disable();
    else
        matrix.enable();

    if(values -> paused){
        matrix.pause();
        ui.add_icon_to_status(0);
    }
    else{
        matrix.unpause();
        ui.remove_icon_from_status(0);
    }

    ui.set_status_right(String(now_v, 1) + "v");

    String status = "";
    switch (values -> current_mode){
        case 1:
            status = "HSV";
            break;
        case 2:
            status = "RGB";
            break;
        case 3:
            status = "temp";
            break;
        default:
            break;
    }
    ui.set_status(status);
}

void setup() {
    pinMode(MATRIX_PIN, OUTPUT);

    Serial.begin(115200);
    Serial.println("\nstarted... ");
    
    while(!display.init(DISPLAY_ADDRESS)){
        Serial.println("Display allocation failed, trying to allocate");
        delay(10);
    }
    display.init_font();
    display.clear();
    display.show();

    attachInterrupt(digitalPinToInterrupt(SA_PIN), encoder_interrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(SB_PIN), encoder_interrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(SW_PIN), encoder_interrupt, CHANGE);
    
    matrix.init < MATRIX_PIN > (16, 16);

    voltage.enable_filtering(0.1);

    ui.init(
        load_menu(menu::menu_list, menu::menu_sizes, menu::menu_linking, menu::menu_types, menu::menu_n),
        &display, FONT_HEIGHT, ROW_HEIGHT, true
    );
    ui.tree();

    //ui.current_menu = ui.default_menu -> items[2].link_menu;
    ui.current_menu = ui.default_menu;

    ui.add_icon_bitmap(bitmap_pause, 6, 8);
    ui.add_icon_bitmap(bitmap_stop, 6, 8);
    ui.add_icon_bitmap(bitmap_wifi, 8, 8);
    ui.add_icon_bitmap(bitmap_battery, 8, 8);

    EEPROM.begin(EEPROM_SIZE);
    values -> color_container.load_init();

    Serial.println("initialized");
}

void loop() {
    update_status_bar();
    matrix.update();

    Encoder_data enc_data = encoder.get_updates();
    if(enc_data.turns != 0){
        ui.move(enc_data.turns);
    }
    if(enc_data.clicks != 0){
        ui.select();
    }

    if(ui.event_done || ui.function_is_running || update_timer + DISP_UPDATE_INTERVAL >= millis()){
      ui.render();
      update_timer = millis();
    }
}