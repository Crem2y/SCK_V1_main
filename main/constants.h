#pragma once
//constants or defines
#define KEY_PIN  9  // neopixel data pin
#define KEYS_NUM 74 // number of neopixels
#define SIDE_PIN 8  // neopixel data pin
#define SIDE_NUM 16 // number of neopixels
#define P_BMAX  15  // PIXEL_BRIGHTMAX

// limit values
#define MS_MAX 120 // mouseSpeed max value (~ 127)
#define MS_MIN 1   // mouseSpeed min value (0 ~)
#define MS_CNG 1   // mouseSpeed change value (1 ~ MS_MIN)
#define MS_DEF 1   // mouseSpeed default value (MS_MAX ~ MS_MIN)

#define WS_MAX 20  // wheelSpeed max value (~ 127)
#define WS_MIN 1   // wheelSpeed min value (0 ~)
#define WS_CNG 1   // wheelSpeed change value (1 ~ WS_MIN)
#define WS_DEF 2   // wheelSpeed default value (WS_MAX ~ WS_MIN)

#define RS_MAX 500 // repeatSpeed max value (~ 65535)
#define RS_MIN 10  // repeatSpeed min value (1 ~)
#define RS_CNG 1   // repeatSpeed change value (1 ~ RS_MIN)
#define RS_DEF 10  // repeatSpeed default value (RS_MAX ~ RS_MIN)