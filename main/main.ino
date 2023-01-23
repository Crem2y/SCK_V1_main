// for arduino Leonardo board
#include <HID-Project.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#include "sck_module_handle.h"
#include "command.h"
//#include "neopixel_handle.h"

#define P_NL 15 // num lock led pin
#define P_CL 16 // caps lock led pin
#define P_SL 17 // scroll lock led pin

String uartString = "";

void setup(void) {
  byte i;

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(P_NL, OUTPUT);
  pinMode(P_CL, OUTPUT);
  pinMode(P_SL, OUTPUT);

  Serial.begin(115200);
  delay(3000);
  
  Serial.println("[sys] --SCK V1--");
  Serial.println("[sys] firmware ver. 0.2.230123");

  BootKeyboard.begin();
  Mouse.begin();
  SurfaceDial.begin();

  //Neo_init();

  Serial.print("[sys] waiting 3 seconds");
  for(i=0; i<3; i++) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("done!");

  SCK_init();

  user_func_set();
}

//////////////////////////////// main loop start ////////////////////////////////
void loop(void) {
  byte i, j;
  digitalWrite(LED_BUILTIN, HIGH);
  
  while(Serial.available()) { //데이터가 오면
    TIM_DISABLE;
    uartString = Serial.readStringUntil('\n');
    commandCheck(uartString);
    TIM_ENABLE;
  }

  SCK_loop();

  digitalWrite(P_NL, SCK_lock_key & LED_NUM_LOCK);
  digitalWrite(P_CL, SCK_lock_key & LED_CAPS_LOCK);
  digitalWrite(P_SL, SCK_lock_key & LED_SCROLL_LOCK);

  //lock_key
  //Neo_loop();
  delay(1);
}
//////////////////////////////// main loop end ////////////////////////////////

/////////////// user function ///////////////

void user_func_set(void) {
  user_func[0] = myfunc;
}

void myfunc(void) {
  Serial.println("[wow] ?!");
}