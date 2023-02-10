// for arduino Leonardo board
#include <HID-Project.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#include "sck_module_handle.h"
#include "command.h"
#include "neopixel_handle.h"

#define P_NL 19 // num lock led pin
#define P_CL 20 // caps lock led pin
#define P_SL 21 // scroll lock led pin

const char version_string[20] = "0.6.230210.A";
String uartString = "";

void setup(void) {
  byte i;

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(P_NL, OUTPUT);
  pinMode(P_CL, OUTPUT);
  pinMode(P_SL, OUTPUT);

  Serial.begin(115200);
  delay(3000);
  
  Serial.println(F("[sys] --SCK V1--"));
  Serial.print(F("[sys] firmware ver. "));
  Serial.println(version_string);

  BootKeyboard.begin();
  Mouse.begin();
  SurfaceDial.begin();

  Neo_init();
  Neo_boot();

  Neo.key.mode = 1;
  SCK_led_power = true;

  SCK_init();

  led_func_set();
  user_func_set();
}

//////////////////////////////// main loop //////////////////////////////// 
void loop(void) {
  //digitalWrite(LED_BUILTIN, HIGH);
  //while(1);

  while(Serial.available()) { //데이터가 오면
    TIM_DISABLE;
    uartString = Serial.readStringUntil('\n');
    commandCheck(uartString);
    TIM_ENABLE;
  }

  SCK_loop();

  // lock led
  digitalWrite(P_NL, !(SCK_lock_key & LED_NUM_LOCK));
  digitalWrite(P_CL, !(SCK_lock_key & LED_CAPS_LOCK));
  digitalWrite(P_SL, !(SCK_lock_key & LED_SCROLL_LOCK));

  TIM_DISABLE;
  //Neo_loop();
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