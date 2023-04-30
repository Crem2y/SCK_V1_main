// for arduino Leonardo board
#include <HID-Project.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <avr/wdt.h>

#include "sck_module_handle.h"
#include "sck_command.h"
#include "neopixel_handle.h"
#include "user_functions.h"

#define KM_RS 18 // keyboard module reset pin

#define P_NL 19 // num lock led pin
#define P_CL 20 // caps lock led pin
#define P_SL 21 // scroll lock led pin

String uart_string = "";
unsigned short sleep_count = 0;
bool is_sleep_mode = false;

struct version_t firm_version = {0x01, 0x02, {0x23, 0x04, 0x30}, 0x0B};

void setup(void) {
  pinMode(KM_RS, OUTPUT); // keyboard module reset
  digitalWrite(KM_RS, HIGH);

  pinMode(P_NL, OUTPUT);
  pinMode(P_CL, OUTPUT);
  pinMode(P_SL, OUTPUT);

  digitalWrite(P_NL, HIGH);
  digitalWrite(P_CL, HIGH);
  digitalWrite(P_SL, HIGH);

  Serial.begin(115200);

  delay(200); // power stabilization time & led check
  digitalWrite(P_NL, LOW);
  delay(200);
  digitalWrite(P_CL, LOW);
  delay(200);
  digitalWrite(P_SL, LOW);
  delay(200);

  Neo_init();
  Neo_boot();
  
  Serial.println(F("[sys] --SCK V1--"));
  Serial.print(F("[sys] firmware ver. "));
  firm_ver = &firm_version;
  print_firm_ver();

  BootKeyboard.begin();
  Mouse.begin();
  SurfaceDial.begin();

  SCK_init();

  if(SCK_KM_count == 0) {
    delay(3000);
    debug_reset();
  }

  led_func_set();
  user_func_set();
  debug_func_set();

  wdt_enable(WDTO_120MS);
  wdt_reset();
}

//////////////////////////////// main loop //////////////////////////////// 
void loop(void) {
  normal_loop();
  if(is_sleep_mode)
    sleep_loop();
}

void normal_loop(void) {
  while(true) {
    while(Serial.available()) {
      wdt_disable();
      TIM_DISABLE;
      uart_string = Serial.readStringUntil('\n');
      check_command(uart_string);
      TIM_ENABLE;
      wdt_enable(WDTO_120MS);
    }
    SCK_loop();

    // lock led
    digitalWrite(P_NL, !(SCK_lock_key & LED_NUM_LOCK));
    digitalWrite(P_CL, !(SCK_lock_key & LED_CAPS_LOCK));
    digitalWrite(P_SL, !(SCK_lock_key & LED_SCROLL_LOCK));

    TIM_DISABLE;
    Neo_loop();
    TIM_ENABLE;

    // general call data (power, ---, ---, ---, ---, scroll_lock, caps_lock, num_lock)
    I2C_writing_data[0] = (SCK_led_power << 7) | (SCK_lock_key & 0x07);
    //I2C_write_byte(I2C_GCA);

    if(false) {
      I2C_writing_data[1] = 0x00;
      I2C_write_data(I2C_GCA, 2);
    } else {
      // 0xRG, 0xBW
      uint32_t col_temp[6];
      col_temp[0] = neopixel.getPixelColor(0);
      col_temp[1] = neopixel.getPixelColor(26);
      col_temp[2] = neopixel.getPixelColor(27);
      col_temp[3] = neopixel.getPixelColor(53);
      col_temp[4] = neopixel.getPixelColor(54);
      col_temp[5] = neopixel.getPixelColor(73);

      for(byte i=0; i<6; i++) {
        I2C_writing_data[i*3 +1]  = (col_temp[i] & 0x00FF0000) >> 16;
        I2C_writing_data[i*3 +2]  = (col_temp[i] & 0x0000FF00) >> 8;
        I2C_writing_data[i*3 +3]  = (col_temp[i] & 0x000000FF);
      }

      I2C_write_data(I2C_GCA, 1+18);
    }

    if(!(SCK_lock_key & 0x07)) { // if all leds are off
      byte i, j;
      byte k = 0;

      for(i=0; i<KM_V; i++) {
        for(j=0; j<KM_H; j++) {
          if(SCK_KM_pressed[i][j]) {
            k++;
            break;
          }
        }
      }
      if(k == 0) {
        sleep_count++;
        if(sleep_count > 6000) { // 200 = 1s
          sleep_count = 0;
          is_sleep_mode = true;
          return;
        }
      } else {
        sleep_count = 0;
      }
    }
    wdt_reset();
    delay(1);
  }
  return;
}

void sleep_loop(void) {
  TIM_DISABLE;
  Neo.key.mode  = NEO_MODE_NONE;
  Neo.side.mode = NEO_MODE_NONE;
  SCK_led_power = false;
  Neo_loop();
  SCK_loop();
  I2C_writing_data[1] = 0x00;
  I2C_write_data(I2C_GCA, 2);

  while(is_sleep_mode) {
    SCK_loop();

    if(SCK_lock_key & 0x07) {
      break;
    }
    
    byte i, j;
    byte k = 0;

    for(i=0; i<KM_V; i++) {
      for(j=0; j<KM_H; j++) {
        if(SCK_KM_pressed[i][j]) {
          k++;
          break;
        }
      }
    }
    for(i=0; i<FM_V; i++) {
      for(j=0; j<FM_H; j++) {
        if(SCK_FM_pressed[i][j]) {
          k++;
          break;
        }
      }
    }
    for(i=0; i<PM_V; i++) {
      for(j=0; j<PM_H; j++) {
        if(SCK_PM_pressed[i][j]) {
          k++;
          break;
        }
      }
    }
    for(i=0; i<MM_V; i++) {
      for(j=0; j<MM_H; j++) {
        if(SCK_MM_pressed[i][j]) {
          k++;
          break;
        }
      }
    }
    if(k) {
      break;
    }
    wdt_reset();
    delay(10);
  }
  SCK_loop();
  Keyboard.releaseAll();
  debug_reset();
}

//////////////////////////////// debug functions ////////////////////////////////

/**
 * @brief load debug functions to array
 * 
 */
void debug_func_set(void) {
  debug_func[0] = debug_reset;
  debug_func[1] = debug_program_mode;
  debug_func[2] = debug_led_on;
  debug_func[3] = debug_led_off;
}

void debug_reset(void) {
  wdt_enable(WDTO_15MS);
  while(1);
}

void debug_program_mode(void) {
  cli();
  wdt_disable();
  I2C_writing_data[0] = 0x00;
  I2C_write_byte(I2C_GCA);
  Neo_all_off();

  byte i, j;
  while(true) {
    SCK_loop();
    if(SCK_KM_pressed[0][0]) { // if ESC key is pressing
      debug_reset();
    }

    for(i=0; i<10; i++) {
      for(j=0; j<5; j++) {
        digitalWrite(P_NL, 1);
        digitalWrite(P_CL, 1);
        digitalWrite(P_SL, 1);
        delay(i);
        digitalWrite(P_NL, 0);
        digitalWrite(P_CL, 0);
        digitalWrite(P_SL, 0);
        delay(10-i);
      }
    }
    for(i=0; i<10; i++) {
      for(j=0; j<5; j++) {
        digitalWrite(P_NL, 1);
        digitalWrite(P_CL, 1);
        digitalWrite(P_SL, 1);
        delay(10-i);
        digitalWrite(P_NL, 0);
        digitalWrite(P_CL, 0);
        digitalWrite(P_SL, 0);
        delay(i);
      }
    }
  }
}

void debug_led_on(void) {
  Neo.key.mode = NEO_MODE_RAINBOW_1;
  SCK_led_power = true;
}

void debug_led_off(void) {
  Neo.key.mode = NEO_MODE_NONE;
  SCK_led_power = false;
}