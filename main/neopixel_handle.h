#pragma once

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#include "sck_key_datas.h"

#define KEY_PIN  9  // neopixel data pin
#define KEYS_NUM 74 // number of neopixels
#define SIDE_PIN 8  // neopixel data pin
#define SIDE_NUM 16 // number of neopixels
#define P_BMAX   15 // PIXEL_BRIGHTMAX

Adafruit_NeoPixel pixel_key  = Adafruit_NeoPixel(KEYS_NUM, KEY_PIN,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixel_side = Adafruit_NeoPixel(SIDE_NUM, SIDE_PIN, NEO_GRB + NEO_KHZ800);

void Neo_init(void);
void Neo_loop(void);
void Neo_off(void);

void Neo_init(void) {
  pixel_key.begin();
  pixel_side.begin();
}

void Neo_loop(void) {


  pixel_key.show();
  pixel_side.show();
}

void Neo_off(void) {
  pixel_key.fill(pixel_side.Color(0,0,0,0),0,0);
  pixel_side.fill(pixel_side.Color(0,0,0,0),0,0);

  pixel_key.show();
  pixel_side.show();
}