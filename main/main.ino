// for arduino Leonardo board
#include <EEPROM.h> // ATmega32U4 has 1024 bytes
#include <HID-Project.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#include "custom_key_code.h"
#include "key_datas.h"
#include "constants.h"
#include "i2c_master_interrupt.h"

#define TIM_DISABLE TIMSK0&=(0<<OCIE0A)
#define TIM_ENABLE TIMSK0|=(1<<OCIE0A)

byte lock_key;
String uartString = "";

void setup(void) {
  byte i;

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  delay(3000);
  
  Serial.println("[ver] --SCK V1--");
  Serial.print("[sys] waiting 5 seconds");
  for(i=0; i<5; i++) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("done!");

  // I2C set
  if(!I2C_init()) {
    Serial.println("[I2C] init error!");
    while(1);
  }

  sei();

  Serial.println("[I2C] slave scanning...");
  digitalWrite(LED_BUILTIN, HIGH);
  for(i=0x20; i<0x70; i++) {
    //Serial.print("[I2C] 0x");
    //Serial.print(i, HEX);
    if(I2C_check(i)) {
      Serial.print("[I2C] 0x");
      Serial.print(i, HEX);
      Serial.print(" ACK! data : 0x");
      if(I2C_reading_data[0] < 0x10) Serial.print('0');
      Serial.println(I2C_reading_data[0], HEX);
    } else {
      //Serial.println(" ERROR!");
    }
    delay(1);
  }
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("[I2C] all slaves checked!\n");
  delay(1000);
  // I2C set end
  Serial.print("[sys] waiting 3 seconds");
  for(i=0; i<3; i++) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("done!");
}

//////////////////////////////// main loop start ////////////////////////////////
void loop(void) {
  digitalWrite(LED_BUILTIN, HIGH);
  
  while(Serial.available()) { //데이터가 오면
    uartString = Serial.readStringUntil('\n');
    Serial.println(uartString);
  }

  if(!I2C_read_byte(0x21)) {
    Serial.println("[I2C] error!");
  }
  while(I2C_is_communicating);
  Serial.print("[I2C] data = 0x");
  if(I2C_reading_data[0] < 0x10) Serial.print('0');
  Serial.println(I2C_reading_data[0], HEX);
  
  delay(1);
}
//////////////////////////////// main loop end ////////////////////////////////
