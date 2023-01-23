// for arduino Leonardo board
#include <EEPROM.h> // ATmega32U4 has 1024 bytes
#include <HID-Project.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#include "sck_module_handle.h"

//Adafruit_NeoPixel pixel_key  = Adafruit_NeoPixel(KEYS_NUM, KEY_PIN,  NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel pixel_side = Adafruit_NeoPixel(SIDE_NUM, SIDE_PIN, NEO_GRB + NEO_KHZ800);


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

  //pixel_key.begin();
  //pixel_side.begin();

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

  //Neo_loop(); // neopixel display
  delay(1);
}
//////////////////////////////// main loop end ////////////////////////////////

/////////////// serial function ///////////////
/**
 * @brief check if String is command
 * 
 * @param str String
 */
void commandCheck(String str) {
  Serial.print("[com] command : ");
  Serial.println(str);
  if(str == "SAVE") {
    Serial.println("[com] Saving to EEPROM...");
    eepromSave();
  } else if(str == "LOAD") {
    Serial.println("[com] Loading from EEPROM...");
    eepromLoad();
  } else if(str == "PRINT") {
    Serial.println("[com] Printing data...");
    printData();
  } else if(str == "SETMODE") {
    Serial.println("[com] Setting mode...");
    setKey();
  } else {
    Serial.println("[com] No command!");
  }
}

/**
 * @brief save 'keySets' data to EEPROM
 */
void eepromSave(void) {
  unsigned int address = 0;
  byte data = 0;

  Serial.println("Save complete!");
  Serial.println(String(address)+" of 1024 bytes used");
}

/**
 * @brief load 'keySets' data from EEPROM
 */
void eepromLoad(void) {

}

/**
 * @brief print 'keySets' data to serial
 */
void printData(void) {

}

/**
 * @brief get data from serial and save to 'keysets'
 */
void setKey(void) {

}

/////////////// user function ///////////////

void user_func_set(void) {
  user_func[0] = myfunc;
}

void myfunc(void) {
  Serial.println("[wow] ?!");
}