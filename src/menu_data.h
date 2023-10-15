#ifndef MENU_DATA_H
#define MENU_DATA_H

#include <Arduino.h>

#define MENU 0
#define BACK 1
#define FUNC 2
#define PARAM 3

namespace menu{

    static uint8_t menu_n = 6;

    static String menu_list[] = {
        "Control",
        "ON/OFF",
        "Pause/Run",
        "Settings",
        "Wi-Fi",

        "Static",
        "Dynamic",

        "Memory",
        "Current limit",
        "Max brightness",
        "Color calibration",
        "Preview color ID",

        "Brightness",
        "Temperature",
        "Color HSV",
        "Color RGB",
        "Color by ID",

        "One-color strobe",
        "Multi-color strobe",
        "Fade strobe",

        "Timings",
        "Colors"
    };

    static uint8_t menu_sizes[] = {5, 2, 5, 5, 3, 2};

    static uint8_t menu_linking[] = {
        1, 11, 12, 2, 9,
        3, 4,
        5, 4, 5, 6, 7,
        0, 1, 2, 3, 16,
        10, 14, 15,
        8, 13
    };

    static uint8_t menu_types[] = {
        MENU, FUNC, FUNC, MENU, FUNC,
        MENU, MENU,
        MENU, PARAM, PARAM, PARAM, PARAM,
        FUNC, FUNC, FUNC, FUNC, FUNC,
        FUNC, FUNC, FUNC,
        FUNC, FUNC
    };

}

/*
root:                               MENU_0
    Control:                        MENU_1
        Static:                     MENU_3
            Brightness:                 FUNC_0
            Temperature:                FUNC_1
            Color HSV:                  FUNC_2
            Color RGB:                  FUNC_3
        Dynamic:                    MENU_4
            Strobe:                     FUNC_10
    ON/OFF:                             FUNC_11
    Run/Stop:                           FUNC_12
    Settings:                       MENU_2
        Memory:                     MENU_5
            Timings                     FUNC_8
            Colors                      FUNC_13
        Current limit:                  FUNC_4
        Max brightness:                 FUNC_5
        Color calibration:              FUNC_6
        About:                          FUNC_7
    Wi-Fi:                              FUNC_9
*/

#endif