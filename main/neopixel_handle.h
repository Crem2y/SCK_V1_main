#pragma once

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#include "sck_key_datas.h"

#define NEO_PIN  9  // neopixel data pin
#define NEO_KEY  74 // number of neopixels (key)
#define NEO_SIDE 16 // number of neopixels (side)
#define NEO_NUM  NEO_KEY + NEO_SIDE // number of neopixels
#define NEO_BMAX 15 // PIXEL_BRIGHTMAX

#define NEO_MODE_KEY_MAX 4 // neopixel
#define NEO_MODE_SIDE_MAX 4 // neopixel

Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(NEO_NUM, NEO_PIN,  NEO_GRB + NEO_KHZ800);

unsigned short Neo_colors[74+16] = {0,}; // 0xRGBW

byte Neo_count = 0;
byte Neo_count_key = 0;
byte Neo_count_side = 0;

byte Neo_mode_key = 0;
byte Neo_mode_side = 0;

void Neo_init(void);
void Neo_loop(void);
void Neo_all_off(void);

void Neo_change_key(void);
void Neo_key_off(void);
void Neo_key_rainbow(void);

void Neo_change_side(void);
void Neo_side_off(void);
void Neo_side_rainbow(void);


void Neo_init(void) {
  neopixel.begin();
}

void Neo_loop(void) {
  switch(Neo_mode_key) {
    case 0:
      Neo_key_off();
    break;
    case 1:
      Neo_key_rainbow();
    break;
    default:
      Neo_key_off();
    break;
  }

  switch(Neo_mode_side) {
    case 0:
      Neo_side_off();
    break;
    case 1:
      Neo_key_rainbow();
    break;
    default:
      Neo_side_off();
    break;
  }

  neopixel.show();
}

void Neo_all_off(void) {
  neopixel.clear();
  neopixel.show();
}

/////////////// neopixel key ///////////////
void Neo_change_key(void) {
  Neo_mode_key++;
  if(Neo_mode_key == NEO_MODE_KEY_MAX) Neo_mode_key = 0;
  Neo_count_key = 0;
}

void Neo_key_off(void) {
  neopixel.fill(neopixel.Color(0,0,0,0),0,NEO_KEY);
}

void Neo_key_rainbow(void) {
  Neo_count_key += 1;
  uint16_t first_hue = Neo_count_key << 8;

  for (uint16_t i=0; i<NEO_KEY; i++) {
    uint16_t hue = first_hue + (i * 65536) / NEO_KEY;
    uint32_t color = neopixel.ColorHSV(hue, 0, NEO_BMAX);
    color = neopixel.gamma32(color);
    neopixel.setPixelColor(i, color);
  }
}

/////////////// neopixel side ///////////////
void Neo_change_side(void) {
  Neo_mode_side++;
  if(Neo_mode_side == NEO_MODE_SIDE_MAX) Neo_mode_side = 0;
  Neo_count_side = 0;
}

void Neo_side_off(void) {
  neopixel.fill(neopixel.Color(0,0,0,0),NEO_KEY,0);
}

void Neo_side_rainbow(void) {
  Neo_count_side += 1;
  uint16_t first_hue = Neo_count_side << 8;

  for (uint16_t i=NEO_KEY; i<NEO_NUM; i++) {
    uint16_t hue = first_hue + (i * 65536) / NEO_SIDE;
    uint32_t color = neopixel.ColorHSV(hue, 0, NEO_BMAX);
    color = neopixel.gamma32(color);
    neopixel.setPixelColor(i, color);
  }
}