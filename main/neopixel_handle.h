#pragma once

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#include "sck_key_datas.h"

#define NEO_PIN  9  // neopixel data pin
#define NEO_KEY  74 // number of neopixels (key)
#define NEO_SIDE 16 // number of neopixels (side)
#define NEO_NUM  NEO_KEY + NEO_SIDE // number of neopixels
#define NEO_BMAX 15 // LED max bright (default)

#define NEO_MODE_KEY_MAX  4 // neopixel modes (key)
#define NEO_MODE_SIDE_MAX 4 // neopixel modes (side)

Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(NEO_NUM, NEO_PIN, NEO_GRB + NEO_KHZ800);

unsigned short Neo_colors[NEO_KEY+NEO_SIDE] = {0,}; // 0xRGBW

unsigned short Neo_colors_custom[NEO_KEY+NEO_SIDE] = {
  0xFFF0,         0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0,     0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0,     0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0,

  0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0,    0xFFF0, 

    0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0,   0xFFF0,

      0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0,     0xFFF0, 

        0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0,       0xFFF0,

  0xFFF0,   0xFFF0,   0xFFF0,                   0xFFF0,                             0xFFF0,   0xFFF0,   0xFFF0,   0xFFF0, 

  0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0,
  0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0,
  0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0
}; // 0xRGBW

struct _neo_value {
  byte count;
  byte mode;
  byte bright;
};

struct _neo {
  byte timer;
  struct _neo_value key;
  struct _neo_value side;
} Neo;

void Neo_init(void);
void Neo_loop(void);
void Neo_all_off(void);
void Neo_boot(void);

void Neo_key_change(void);
void Neo_key_off(void);
void Neo_key_lighter(void);
void Neo_key_darker(void);
void Neo_key_rainbow(void);
void Neo_key_fixed_color(void);
void Neo_key_fixed_custom(void);

void Neo_side_change(void);
void Neo_side_off(void);
void Neo_side_lighter(void);
void Neo_side_darker(void);
void Neo_side_rainbow(void);
void Neo_side_fixed(void);

//////////////////////////////// functions ////////////////////////////////

/**
 * @brief SCK LED initalize
 * 
 */
void Neo_init(void) {
  neopixel.begin();
  Neo.key.bright = NEO_BMAX;
  Neo.side.bright = NEO_BMAX;
}

/**
 * @brief SCK LED loop function
 * 
 */
void Neo_loop(void) {
  switch(Neo.key.mode) {
    case 0:
      Neo_key_off();
    break;
    case 1:
      Neo_key_rainbow();
    break;
    case 2:
      Neo_key_fixed_color();
    break;
    case 3:
      Neo_key_fixed_custom();
    break;
    default:
      Neo_key_off();
    break;
  }
/*
  switch(Neo.side.mode) {
    case 0:
      Neo_side_off();
    break;
    case 1:
      Neo_side_rainbow();
    break;
    case 2:
      Neo_side_fixed();
    break;
    default:
      Neo_side_off();
    break;
  }
*/
  neopixel.show();
}

/**
 * @brief turn off all LEDs
 * 
 */
void Neo_all_off(void) {
  neopixel.clear();
  neopixel.show();
}

/**
 * @brief SCK LED boot animation
 * 
 */
void Neo_boot(void) {
  for(byte j=0; j<NEO_KEY+NEO_SIDE; j++) {
    neopixel.setPixelColor(j, 15, 15, 15);
    neopixel.show();
    delay(10);
  }

  Neo.key.mode = 1;
  Neo.side.mode = 1;
}

/////////////// neopixel (key) ///////////////

/**
 * @brief change LED mode (key)
 * 
 */
void Neo_key_change(void) {
  Neo.key.mode++;
  if(Neo.key.mode == NEO_MODE_KEY_MAX) Neo.key.mode = 0;
  Neo.key.count= 0;
}

/**
 * @brief turn off LEDs (key)
 * 
 */
void Neo_key_off(void) {
  neopixel.fill(neopixel.Color(0,0,0,0),0,NEO_KEY);
}

/**
 * @brief change LED bright lighter (key)
 * 
 */
void Neo_key_lighter(void) {
  Neo.key.bright++;
}

/**
 * @brief change LED bright darker (key)
 * 
 */
void Neo_key_darker(void) {
  Neo.key.bright--;
}

/**
 * @brief LED rainbow mode (key)
 * 
 */
void Neo_key_rainbow(void) {
  Neo.key.count -= 1;
  uint16_t first_hue = Neo.key.count * 256;

  for (uint16_t i=0; i<NEO_KEY; i++) {
    uint16_t hue = first_hue + (i * 1 * 65536) / NEO_KEY;
    uint32_t color = neopixel.ColorHSV(hue, 127, Neo.key.bright * 8);
    color = neopixel.gamma32(color);
    neopixel.setPixelColor(i, color);
  }
}

/**
 * @brief LED fixed color mode (key)
 * 
 */
void Neo_key_fixed_color(void) {
  unsigned short colordata = Neo_colors_custom[0]; // 0xRGBW
  for(unsigned char i=0; i<NEO_KEY; i++) {
    neopixel.setPixelColor(i, 15, 8, 0);
  }
}

/**
 * @brief LED fixed color mode (key)
 * 
 */
void Neo_key_fixed_custom(void) {
  for(unsigned char i=0; i<NEO_KEY; i++) {
    unsigned short colordata = Neo_colors_custom[i]; // 0xRGBW
    neopixel.setPixelColor(i, (colordata & 0xF000 >> 12), (colordata & 0x0F00 >> 8), (colordata & 0x00F0 >> 4));
  }
}

/////////////// neopixel (side) ///////////////

/**
 * @brief change LED mode (side)
 * 
 */
void Neo_side_change(void) {
  Neo.side.mode++;
  if(Neo.side.mode == NEO_MODE_SIDE_MAX) Neo.side.mode = 0;
  Neo.side.count = 0;
}

/**
 * @brief turn off LEDs (side)
 * 
 */
void Neo_side_off(void) {
  neopixel.fill(neopixel.Color(0,0,0,0),NEO_KEY,0);
}

/**
 * @brief change LED bright lighter (side)
 * 
 */
void Neo_side_lighter(void) {
  Neo.side.bright++;
}

/**
 * @brief change LED bright darker (side)
 * 
 */
void Neo_side_darker(void) {
  Neo.side.bright--;
}

/**
 * @brief LED rainbow mode (side)
 * 
 */
void Neo_side_rainbow(void) {
  Neo.side.count += 1;
  uint16_t first_hue = Neo.side.count << 8;

  for (uint16_t i=NEO_KEY; i<NEO_NUM; i++) {
    uint16_t hue = first_hue + (i * 1 * 65536) / NEO_SIDE;
    uint32_t color = neopixel.ColorHSV(hue, 127, Neo.side.bright * 8);
    color = neopixel.gamma32(color);
    neopixel.setPixelColor(i, color);
  }
}

/**
 * @brief LED fixed color mode (side)
 * 
 */
void Neo_side_fixed(void) {
  for(unsigned char i=0; i<NEO_SIDE; i++) {
    unsigned short colordata = Neo_colors_custom[i+NEO_KEY]; // 0xRGBW
    neopixel.setPixelColor(i+NEO_KEY, (colordata & 0xF000 >> 12), (colordata & 0x0F00 >> 8), (colordata & 0x00F0 >> 4));
  }
}
