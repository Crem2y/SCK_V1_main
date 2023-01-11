//constants or defines
#define PIXELPIN  4  // neopixel data pin
#define NUMPIXELS 20 // number of neopixels
#define P_BMAX  15  // PIXEL_BRIGHTMAX

#define LCD_RS 3  // LCD RS
#define LCD_E  2  // LCD E
#define LCD_D4 15 // LCD D4
#define LCD_D5 14 // LCD D5
#define LCD_D6 16 // LCD D6
#define LCD_D7 10 // LCD D7
#define LCD_BLANK "                "

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