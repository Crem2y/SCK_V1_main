#pragma once

#include <HID-Project.h>

#include "i2c_master_interrupt.h"
#include "constants.h"
#include "sck_key_handle.h"
#include "key_datas.h"
#include "macro_timer.h"

unsigned char SCK_KM_address = 0x10; // 0x10 ~ 0x17
unsigned char SCK_PM_address = 0x18; // 0x18 ~ 0x1B
unsigned char SCK_FM_address = 0x1C; // 0x1C ~ 0x1F
unsigned char SCK_MM_addresses[MM_H] = {0x20,0x21,0x22,0x23,0x24}; // 0x20 ~ 0x2F

unsigned char SCK_KM_count = 0;
unsigned char SCK_PM_count = 0;
unsigned char SCK_FM_count = 0;
unsigned char SCK_MM_count = 0;

unsigned char SCK_key_layer = 0;
bool SCK_power_status = false;
byte SCK_lock_key;

volatile unsigned int msCount = 0; // timer count

void SCK_init(void);
void SCK_loop(void);

void keyCheck_MM(byte key_state, byte keyposV, byte module_num, byte key_layer);
void keyRepeat_MM(byte key_state, byte keyposV, byte module_num, byte key_layer);
void keyToggle_MM(byte key_state, byte keyposV, byte module_num, byte key_layer);
void toggleRepeat_MM(byte key_state, byte keyposV, byte module_num, byte key_layer);

/**
 * @brief AVR Interrupt Service Routine (TIMER3_COMPA)
 * timer int (1kHz = 1ms)
 */
ISR(TIMER3_COMPA_vect) {
  msCount++;
  if (msCount < repeatSpeed) {
    return;
  }

  byte i,j;
  byte key_mask;

  for(i=0; i<MM_H; i++) { // key checking
    key_mask = 0x80;
    for(j=0; j<4; j++) {
      if (SCK_MM_keyset[SCK_key_layer][0][i] & (0x40>>(j*2))) { // if repeat mode
        if (SCK_MM_keyset[SCK_key_layer][0][i] & (0x80>>(j*2))) { // if toggle mode
          if (SCK_MM_toggled[j][i]) { // if toggle flag on
          SCK_keyHandle(SCK_MM_keyset[SCK_key_layer][2+j][i], true); // click a key
          SCK_keyHandle(SCK_MM_keyset[SCK_key_layer][2+j][i], false);
          }
        } else if (SCK_MM_pressed[j][i]) {
          SCK_keyHandle(SCK_MM_keyset[SCK_key_layer][2+j][i], true); // click a key
          SCK_keyHandle(SCK_MM_keyset[SCK_key_layer][2+j][i], false);
        }
      }
      key_mask >>= 1;
    } // key_mask = 0x08
    for(j=0; j<4; j++) {
      if (SCK_MM_keyset[SCK_key_layer][1][i] & (0x40>>(j*2))) { // if repeat mode
        if (SCK_MM_keyset[SCK_key_layer][1][i] & (0x80>>(j*2))) { // if toggle mode
          if (SCK_MM_toggled[4+j][i]) { // if toggle flag on
          SCK_keyHandle(SCK_MM_keyset[SCK_key_layer][6+j][i], true); // click a key
          SCK_keyHandle(SCK_MM_keyset[SCK_key_layer][6+j][i], false);
          }
        } else if (SCK_MM_pressed[4+j][i]) {
          SCK_keyHandle(SCK_MM_keyset[SCK_key_layer][6+j][i], true); // click a key
          SCK_keyHandle(SCK_MM_keyset[SCK_key_layer][6+j][i], false);
        }
      }
      key_mask >>= 1;
    }
  }

  msCount = 0;
}

/**
 * @brief SCK initalize
 * 
 */
void SCK_init(void) {
  byte i;

  // module set
  if(!I2C_init()) {
    Serial.println("[SCK] i2c init error!");
    while(1);
  }

  sei();

  Serial.println("[SCK] i2c slave scanning...");
  for(i=0x10; i<0x30; i++) {
    if(I2C_check(i)) {
      Serial.print("[SCK] 0x");
      Serial.print(i, HEX);
      Serial.print(" ACK! data : 0x");
      if(I2C_reading_data[0] < 0x10) Serial.print('0');
      Serial.println(I2C_reading_data[0], HEX);

      if(i > 0x0F && i < 0x18) {
        SCK_KM_address = i;
        SCK_KM_count++;
      } else if(i > 0x17 && i < 0x1C) {
        SCK_PM_address = i; 
        SCK_PM_count++;
      } else if(i > 0x1B && i < 0x20) {
        SCK_FM_address = i;
        SCK_FM_count++;
      } else if(i > 0x1F && i < 0x30) {
        //SCK_MM_addresses[SCK_MM_count] = i;
        SCK_MM_count++;
      }
    }
    delay(1);
  }
  Serial.println("[SCK] all slaves checked!\n");
  // module set end

  Serial.print("[SCK] keyboard modules : ");
  Serial.println(SCK_KM_count);
  Serial.print("[SCK] keypad modules : ");
  Serial.println(SCK_PM_count);
  Serial.print("[SCK] fnkey modules : ");
  Serial.println(SCK_FM_count);
  Serial.print("[SCK] macro modules : ");
  Serial.println(SCK_MM_count);

  if(SCK_KM_count == 0 && SCK_PM_count == 0 && SCK_FM_count == 0 && SCK_MM_count == 0) {
    Serial.println("[SCK] no module detected!");
    Serial.println("[SCK] Please disconnect the cable.");
    while(1);
  }

  Serial.print("\n[SCK] default mouseSpeed  : ");
  Serial.println(mouseSpeed);
  Serial.print("[SCK] default wheelSpeed  : ");
  Serial.println(wheelSpeed);
  Serial.print("[SCK] default repeatSpeed : ");
  Serial.println(repeatSpeed);

  SCK_user_func_init();

  Serial.println("[SCK] keyboard start!");
  timer_init();
}

/**
 * @brief SCK loop function
 * 
 */
void SCK_loop(void) {
  byte i, j;
  byte key_mask;
  byte key_state;

  // getting key
  if(SCK_KM_count) { // if there is keyboard modules
    I2C_read_data(SCK_KM_address, 14);
    while(I2C_is_communicating);
  }

  if(SCK_PM_count) { // if there is keypad modules
    I2C_read_data(SCK_PM_address, 4);
    while(I2C_is_communicating);
  }

  if(SCK_FM_count) { // if there is fnkey modules
    I2C_read_data(SCK_FM_address, 3);
    while(I2C_is_communicating);
  }
  
  if(SCK_MM_count) { // if there is macro modules
    for(i=0; i<5; i++) {
      if(!I2C_check(SCK_MM_addresses[i])) continue;
      //Serial.println(I2C_reading_data[0], HEX);

      key_mask = 0x80;
      for(j=0; j<4; j++) {
        key_state = I2C_reading_data[0] & key_mask;
        if (SCK_MM_keyset[SCK_key_layer][0][i] & (0x40>>(j*2))) { // if repeat mode
          if (SCK_MM_keyset[SCK_key_layer][0][i] & (0x80>>(j*2))) { // if toggle mode
            toggleRepeat_MM(key_state, j, i, SCK_key_layer);
          } else {
            keyRepeat_MM(key_state, j, i, SCK_key_layer);
          }
        } else {
          if (SCK_MM_keyset[SCK_key_layer][0][i] & (0x80>>(j*2))) { // if toggle mode
            keyToggle_MM(key_state, j, i, SCK_key_layer);
          } else {
            keyCheck_MM(key_state, j, i, SCK_key_layer);
          }
        }
        key_mask >>= 1;
      } // key_mask = 0x08
      for(j=0; j<4; j++) {
        key_state = I2C_reading_data[0] & key_mask;
        if (SCK_MM_keyset[SCK_key_layer][1][i] & (0x40>>(j*2))) { // if repeat mode
          if (SCK_MM_keyset[SCK_key_layer][1][i] & (0x80>>(j*2))) { // if toggle mode
            toggleRepeat_MM(key_state, (j+4), i, SCK_key_layer);
          } else {
            keyRepeat_MM(key_state, (j+4), i, SCK_key_layer);
          }
        } else {
          if (SCK_MM_keyset[SCK_key_layer][1][i] & (0x80>>(j*2))) { // if toggle mode
            keyToggle_MM(key_state, (j+4), i, SCK_key_layer);
          } else {
            keyCheck_MM(key_state, (j+4), i, SCK_key_layer);
          }
        }
        key_mask >>= 1;
      }
    }
  }

  SCK_lock_key = BootKeyboard.getLeds(); // lock key checking
}

/**
 * @brief chack and press a key once
 * 
 * @param key byte, if 0, key is pressed
 * @param keyposV byte, 0 ~ MM_V
 * @param module_num byte, 0 ~ MM_H
 * @param key_layer byte, 0 ~ KEY_LAYERS
 */
void keyCheck_MM(byte key_state, byte keyposV, byte module_num, byte key_layer) {
  if (key_state) { // if pressed
    if (!SCK_MM_pressed[keyposV][module_num]) { // if first detacted
      SCK_keyHandle(SCK_MM_keyset[key_layer][2 + keyposV][module_num], true); // press a key
      SCK_MM_pressed[keyposV][module_num] = true;
    }
  } else if (SCK_MM_pressed[keyposV][module_num]) { // if key released
    SCK_keyHandle(SCK_MM_keyset[key_layer][2 + keyposV][module_num], false); // release a key
    SCK_MM_pressed[keyposV][module_num] = false;
  }
}

/**
 * @brief click a key
 * 
 * @param key_state byte, if not 0, key is pressed
 * @param keyposV byte, 0 ~ MM_V
 * @param module_num byte, 0 ~ MM_H
 * @param key_layer byte, 0 ~ KEY_LAYERS
 */
void keyRepeat_MM(byte key_state, byte keyposV, byte module_num, byte key_layer) {
  if (key_state) { // if pressed
    SCK_MM_pressed[keyposV][module_num] = true;
  } else {
    SCK_MM_pressed[keyposV][module_num] = false;
  }
}

/**
 * @brief chack and toggle key
 * 
 * @param key_state byte, if not 0, key is pressed
 * @param keyposV byte, 0 ~ MM_V
 * @param module_num byte, 0 ~ MM_H
 * @param key_layer byte, 0 ~ KEY_LAYERS
 */
void keyToggle_MM(byte key_state, byte keyposV, byte module_num, byte key_layer) {
  bool TK = SCK_MM_toggled[keyposV][module_num]; // previous state

  if (key_state) { // if pressed
    if (!SCK_MM_pressed[keyposV][module_num]) { // if first detacted
      SCK_MM_toggled[keyposV][module_num] = !TK; // toggle state
      SCK_keyHandle(SCK_MM_keyset[key_layer][2 + keyposV][module_num], !TK); 
      SCK_MM_pressed[keyposV][module_num] = true;
    }
  } else { // if not pressed
    SCK_MM_pressed[keyposV][module_num] = false;
  }
}

/**
 * @brief toggle & click a key
 * 
 * @param key_state byte, if not 0, key is pressed
 * @param keyposV byte, 0 ~ MM_V
 * @param module_num byte, 0 ~ MM_H
 * @param key_layer byte, 0 ~ KEY_LAYERS
 */
void toggleRepeat_MM(byte key_state, byte keyposV, byte module_num, byte key_layer) {
  bool TK = SCK_MM_toggled[keyposV][module_num]; // previous state

  if (key_state) { // if pressed
    if (!SCK_MM_pressed[keyposV][module_num]) { // if first detacted
      SCK_MM_toggled[keyposV][module_num] = !TK; // toggle state
      SCK_MM_pressed[keyposV][module_num] = true;
    }
  } else { // if not pressed
    SCK_MM_pressed[keyposV][module_num] = false;
  }
}