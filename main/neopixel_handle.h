#pragma once

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#include "sck_key_datas.h"

#define NEO_PIN 9     // neopixel data pin
#define NEO_NUM 74+16 // number of neopixels
#define P_BMAX  15    // PIXEL_BRIGHTMAX

Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(NEO_NUM, NEO_PIN,  NEO_GRB + NEO_KHZ800);

void Neo_init(void);
void Neo_loop(void);
void Neo_off(void);
void Neo_side_off(void);

void Neo_init(void) {
  neopixel.begin();
}

void Neo_loop(void) {


  neopixel.show();
}

void Neo_off(void) {
  neopixel.fill(neopixel.Color(0,0,0,0),0,0);

  neopixel.show();
}

void Neo_side_off(void) {
  neopixel.fill(neopixel.Color(0,0,0,0),74,0);

  neopixel.show();
}