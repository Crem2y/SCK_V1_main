// for arduino Leonardo board
#include <EEPROM.h> // ATmega32U4 has 1024 bytes
//#include <HID-Project.h>
//#include <Adafruit_NeoPixel.h>
//#include <avr/power.h>

//#include "customcode.h"
//#include "keydatas.h"
//#include "constants.h"
#include "customi2c.h"

#define TIM_DISABLE TIMSK0&=(0<<OCIE0A)
#define TIM_ENABLE TIMSK0|=(1<<OCIE0A)

//byte lock_key;

String uartString = "";

void setup(void) {
  byte i;

  pinMode(LED_BUILTIN, OUTPUT);

  // other funtion set
  //BootKeyboard.begin();
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("[ver] SCK V1");
  Serial.println("[sys] waiting 5 seconds...");
  delay(5000);
  Serial.println("[sys] done!");

  /*
  // timer set (1kHz = 1ms)
  TCCR0A = 0; //TCCR0A initialize
  TCCR0B = 0; //TCCR0B initialize
  TCNT0 = 0;  //TCNT0 initialize
  OCR0A= 250; 
  TCCR0B |= (1<<WGM02);
  TCCR0B |= (1<<CS01) | (1<<CS00);
  TIMSK0 |= (1<<OCIE0A);
  //TIM_DISABLE;
  // timer set end
  */

  // I2C set
  if(!I2C_init()) {
    Serial.println("[I2C] init error!");
    while(1);
  }
  Serial.println("[I2C] init!");

  sei();

  for(i=0x20; i<0x30; i++) {
    Serial.print("[I2C] slave 0x");
    Serial.print(i, HEX);
    Serial.print(" checking...");
    if(I2C_check(i)) {
      Serial.println(" ACK!");
    } else {
      Serial.println(" ERROR!");
    }
    delay(100);
  }
  Serial.println("[I2C] all slaves checked!\n");
  // I2C set end
  Serial.println("[sys] waiting 5 seconds...");
  delay(5000);
}

//////////////////////////////// main loop start ////////////////////////////////
void loop(void) {
  digitalWrite(LED_BUILTIN, HIGH);
  while(Serial.available()) { //데이터가 오면
    uartString = Serial.readStringUntil('\n');
    Serial.println(uartString);
  }

  I2C_write_byte_falling(0x20);

  I2C_reading_data[0] = 0xff;
  if(!I2C_read_byte(0x20)) {
    Serial.println("[I2C] error!");
  }
  while(I2C_is_communicating);
  Serial.print("[I2C] data = 0x");
  Serial.println(I2C_reading_data[0], HEX);

  delay(1000);
}
//////////////////////////////// main loop end ////////////////////////////////
