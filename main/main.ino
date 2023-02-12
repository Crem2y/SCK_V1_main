// for arduino Leonardo board
#include <HID-Project.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <avr/wdt.h>

#include "sck_module_handle.h"
#include "command.h"
#include "neopixel_handle.h"

#define P_NL 19 // num lock led pin
#define P_CL 20 // caps lock led pin
#define P_SL 21 // scroll lock led pin

const char version_string[20] = "0.7.230212.B";
String uart_string = "";
unsigned short sleep_count = 0;

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(P_NL, OUTPUT);
  pinMode(P_CL, OUTPUT);
  pinMode(P_SL, OUTPUT);

  Serial.begin(115200);
  delay(1000); // power stabilization time

  Neo_init();
  Neo_boot();
  
  Serial.println(F("[sys] --SCK V1--"));
  Serial.print(F("[sys] firmware ver. "));
  Serial.println(version_string);

  BootKeyboard.begin();
  Mouse.begin();
  SurfaceDial.begin();

  Neo.key.mode = 1;
  SCK_led_power = true;

  SCK_init();

  led_func_set();
  user_func_set();
  debug_func_set();
}

//////////////////////////////// main loop //////////////////////////////// 
void loop(void) {
  while(Serial.available()) {
    TIM_DISABLE;
    uart_string = Serial.readStringUntil('\n');
    commandCheck(uart_string);
    TIM_ENABLE;
  }

  SCK_loop();

  // lock led
  digitalWrite(P_NL, !(SCK_lock_key & LED_NUM_LOCK));
  digitalWrite(P_CL, !(SCK_lock_key & LED_CAPS_LOCK));
  digitalWrite(P_SL, !(SCK_lock_key & LED_SCROLL_LOCK));

  TIM_DISABLE;
  Neo_loop();
  TIM_ENABLE;

  delay(1);
}

/////////////// user function ///////////////
void user_func_set(void) {
  user_func[0] = uf_undo;
  user_func[1] = uf_redo;
}

void uf_undo(void) {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press('z');
  Keyboard.release('z');
  Keyboard.release(KEY_LEFT_CTRL);
}

void uf_redo(void) {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press('y');
  Keyboard.release('y');
  Keyboard.release(KEY_LEFT_CTRL);
}

/////////////// led function ///////////////
void led_func_set(void) {
  led_func[0] = Neo_key_change;
  led_func[1] = Neo_side_change;
  led_func[2] = Neo_key_lighter;
  led_func[3] = Neo_key_darker;
  led_func[4] = Neo_side_lighter;
  led_func[5] = Neo_side_darker;
}

/////////////// debug function ///////////////
void debug_func_set(void) {
  debug_func[0] = debug_reset;
  debug_func[1] = debug_program;
  //debug_func[2] = SCK_func_none;
  //debug_func[3] = SCK_func_none;
}

void debug_reset(void) {
  wdt_enable(WDTO_15MS);
  while(1);
}

void debug_program(void) {
  cli();
  I2C_writing_data[0] = 0x00;
  I2C_write_byte(I2C_GCA);
  Neo_all_off();

  for(byte i=0; i<10; i++) { // 10s wait
    digitalWrite(P_NL, 0);
    digitalWrite(P_CL, 1);
    digitalWrite(P_SL, 1);
    delay(333);
    digitalWrite(P_NL, 1);
    digitalWrite(P_CL, 0);
    digitalWrite(P_SL, 1);
    delay(333);
    digitalWrite(P_NL, 1);
    digitalWrite(P_CL, 1);
    digitalWrite(P_SL, 0);
    delay(333);
  }
  sei();
}
