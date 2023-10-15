#ifndef CONFING_H
#define CONFIG_H

#define MATRIX_PIN 2 //D4 pin, FastLED takes only gpio number, not D<N>
#define SA_PIN D6
#define SB_PIN D7
#define SW_PIN D5
#define VOLTAGE_PIN A0

#define MATRIX_W 16
#define MATRIX_H 16

#define VOLTAGE_ADJUST_FACTOR 1.0271
#define VOLTAGE_ADJUST_OFFSET -0.1967

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#define DISPLAY_ADDRESS 0x3C //0x3D or 0x3C

#define FONT_HEIGHT 7
#define ROW_HEIGHT 10
#define MIN_ICON_X 100

#define DISP_UPDATE_INTERVAL 500

#define MIN_OPERATING_VOLTAGE 4.5

#define COLOR_ID_N 26
#define COLOR_DEFINED_MASK_ADDR 0
#define COLOR_TYPE_MASK_ADDR 4
#define COLOR_FIRST_VAL_ADDR 8
#define EEPROM_SIZE 164                 //COLOR_ID_N * 6 + 8

#endif