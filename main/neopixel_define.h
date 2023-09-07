#pragma once

#define NEO_PIN  9  // neopixel data pin
#define NEO_KEY  74 // number of neopixels (key)
#define NEO_SIDE 16 // number of neopixels (side)
#define NEO_NUM  NEO_KEY + NEO_SIDE // number of neopixels
#define NEO_BMAX 15 // LED max bright (default)

enum neo_mode {
  NEO_MODE_NONE = 0,  // neopixel off
  NEO_MODE_RAINBOW_1, // neopixel rainbow mode (1)
  NEO_MODE_RAINBOW_2, // neopixel rainbow mode (2)
  NEO_MODE_FIXED_COL, // neopixel fixed color mode (one color)
  NEO_MODE_FIXED_CUS, // neopixel fixed color mode (custom)
  NEO_MODE_WHITE,     // neopixel white color mode (fixed)
  NEO_MODE_REACTION,  // neopixel reaction mode
  NEO_MODE_MAX,       //
};

struct _neo_value {
  byte count;
  byte count2;
  byte mode;
  byte bright;
};

struct _neo {
  byte timer;
  struct _neo_value key;
  struct _neo_value side;
} Neo;