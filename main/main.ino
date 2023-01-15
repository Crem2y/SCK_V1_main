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

signed   char mouseSpeed = MS_DEF; // mouse move speed  (-128 ~ 127)
signed   char wheelSpeed = WS_DEF; // mouse wheel speed (-128 ~ 127)
unsigned int repeatSpeed = RS_DEF; // delay value in repeat mode (1 ~ 65535ms)

unsigned char SCK_KM_address = 0x00; // 0x10 ~ 0x17
unsigned char SCK_PM_address = 0x00; // 0x18 ~ 0x1B
unsigned char SCK_FM_address = 0x00; // 0x1C ~ 0x1F
unsigned char SCK_MM_addresses[5] = {0,}; // 0x20 ~ 0x2F

unsigned char SCK_KM_count = 0;
unsigned char SCK_PM_count = 0;
unsigned char SCK_FM_count = 0;
unsigned char SCK_MM_count = 0;

unsigned char SCK_key_layer = 0;

bool SCK_power_status = false;

void setup(void) {
  byte i;

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  delay(3000);
  
  Serial.println("[sys] --SCK V1--");

  BootKeyboard.begin();
  Mouse.begin();
  SurfaceDial.begin();

  //pixels.begin();

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
  for(i=0x10; i<0x30; i++) {
    if(I2C_check(i)) {
      Serial.print("[I2C] 0x");
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
        SCK_MM_addresses[SCK_MM_count] = i;
        SCK_MM_count++;
      }
    }
    delay(1);
  }
  Serial.println("[I2C] all slaves checked!\n");
  // I2C set end

  Serial.print("[sys] keyboard modules : ");
  Serial.println(SCK_KM_count);
  Serial.print("[sys] keypad modules : ");
  Serial.println(SCK_PM_count);
  Serial.print("[sys] fnkey modules : ");
  Serial.println(SCK_FM_count);
  Serial.print("[sys] macro modules : ");
  Serial.println(SCK_MM_count);

  if(SCK_KM_count == 0 && SCK_PM_count == 0 && SCK_FM_count == 0 && SCK_MM_count == 0) {
    Serial.println("[err] no module detected!!");
    while(1);
  }

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
  byte i, j;
  digitalWrite(LED_BUILTIN, HIGH);
  
  while(Serial.available()) { //데이터가 오면
    uartString = Serial.readStringUntil('\n');
    Serial.println(uartString);
  }

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
    for(i=0; i<SCK_MM_count; i++){
      I2C_read_byte(SCK_MM_addresses[i]);
      while(I2C_is_communicating);
      keyCheck_MM(I2C_reading_data[0], i, SCK_key_layer);
    }
  }
  // getting key end

  
  delay(1);

  lock_key = BootKeyboard.getLeds(); // lock key checking

}
//////////////////////////////// main loop end ////////////////////////////////


/////////////// key function ///////////////
/**
 * @brief chack and press a key once
 * 
 * @param key byte, if 0, key is pressed
 * @param keyposV byte, 0 ~ MM_V
 * @param keyposH byte, 0 ~ MM_H
 * @param keyset byte, 0 ~ KEY_LAYERS
 */
void keyCheck_MM(byte keydata, byte module_num, byte key_layer) {
  byte keymask = 0x01;
  for(byte i=0; i<8; i++) {
    if (keydata & keymask) { // if pressed
      if (!SCK_MM_pressed[i][module_num]) { // if first detacted
        keyHandle(SCK_MM_keyset[key_layer][(MM_V+1) - i][module_num], true); // press a key
        SCK_MM_pressed[i][module_num] = true;
      }
    } else if (SCK_MM_pressed[i][module_num]) { // if key released
      keyHandle(SCK_MM_keyset[key_layer][(MM_V+1) - i][module_num], false); // release a key
      SCK_MM_pressed[i][module_num] = false;
    }
    keymask = keymask << 1;
  }
}


/**
 * @brief check keycode and execute correct function
 * 
 * @param keycode unsigned char, 0~255
 * @param pressed bool, if true, key is pressed
 */
void keyHandle(unsigned char keycode, bool pressed) {
  if (keycode < 0x20) {
    sp_Normal(keycode, pressed);
  } else if (keycode > 0x87 && keycode < 0xB0) {
    //sp_Gamepad(keycode, pressed);
  } else if (keycode > 0xB3 && keycode < 0xC1) {
    sp_Consumer(keycode, pressed);
  } else if (keycode > 0xEB && keycode < 0xF0) {
    sp_Surface(keycode, pressed);
  } else if (keycode > 0xFB) {
    sp_Function(keycode, pressed);
  } else {
    if (pressed) Keyboard.press(keycode);
    else Keyboard.release(keycode);
  }
}

/**
 * @brief special keycode for mouse, repeat speed, and user function
 * 
 * @param keycode unsigned char, 0x01 ~ 0x1F (31)
 * @param pressed bool, if true, key is pressed
 */
void sp_Normal(byte keycode, bool pressed) {
  if (pressed) {
    switch(keycode) {
      case M_LB: // left_mouse
        Mouse.press(MOUSE_LEFT);
      break;
      case M_RB: // right_mouse
        Mouse.press(MOUSE_RIGHT);
      break;
      case M_MB: // middle_mouse
        Mouse.press(MOUSE_MIDDLE);
      break;
      case M_4B: // mouse_button_4
        Mouse.press(MOUSE_PREV);
      break;
      case M_5B: // mouse_button_5
        Mouse.press(MOUSE_NEXT);
      break;
      case M_U: // mouse_up
        Mouse.move(0, -mouseSpeed, 0);
      break;
      case M_D: // mouse_down
        Mouse.move(0, mouseSpeed, 0);
      break;
      case M_L: // mouse_left
        Mouse.move(-mouseSpeed, 0, 0);
      break;
      case M_R: // mouse_right
        Mouse.move(mouseSpeed, 0, 0);
      break;
      case M_UL: // mouse_ul
        Mouse.move(-mouseSpeed, -mouseSpeed, 0);
      break;
      case M_UR: // mouse_ur
        Mouse.move(mouseSpeed, -mouseSpeed, 0);
      break;
      case M_DL: // mouse_dl
        Mouse.move(-mouseSpeed, mouseSpeed, 0);
      break;
      case M_DR: // mouse_dr
        Mouse.move(mouseSpeed, mouseSpeed, 0);
      break;
      case MH_U: // mouse_wheel_up
        Mouse.move(0, 0, wheelSpeed);
      break;
      case MH_D: // mouse_wheel_down
        Mouse.move(0, 0, -wheelSpeed);
      break;
      case M_F: // mouse_faster
        if (mouseSpeed < MS_MAX) mouseSpeed += MS_CNG;
      break;
      case M_S: // mouse_slower
        if (mouseSpeed > MS_MIN) mouseSpeed -= MS_CNG;
      break;
      case MH_F: // mouse_wheel_faster
        if (wheelSpeed < WS_MAX) wheelSpeed += WS_CNG;
      break;
      case MH_S: // mouse_wheel_slower
        if (wheelSpeed > WS_MIN) wheelSpeed -= WS_CNG;
      break;
      case R_F: // repeat_faster
        if (repeatSpeed > RS_MIN) repeatSpeed -= RS_CNG;
      break;
      case R_S: // repeat_slower
        if (repeatSpeed < RS_MAX) repeatSpeed += RS_CNG;
      break;
      case F_01: // func_01
        func_01();
      break;
      case F_02: // func_02
        func_02();
      break;
      case F_03: // func_03
        func_03();
      break;
      case F_04: // func_04
        func_04();
      break;
      case F_05: // func_05
        func_05();
      break;
      case F_06: // func_06
        func_06();
      break;
      case F_07: // func_07
        func_07();
      break;
      case F_08: // func_08
        func_08();
      break;
    }
  } else {
    switch(keycode) {
      case M_LB: // left_mouse
        Mouse.release(MOUSE_LEFT);
      break;
      case M_RB: // right_mouse
        Mouse.release(MOUSE_RIGHT);
      break;
      case M_MB: // middle_mouse
        Mouse.release(MOUSE_MIDDLE);
      break;
      case M_4B: // mouse_button_4
        Mouse.release(MOUSE_PREV);
      break;
      case M_5B: // mouse_button_5
        Mouse.release(MOUSE_NEXT);
      break;
    }
  }
}

/**
 * @brief special keycode for consumer key
 * 
 * @param keycode unsigned char, 0xB4 ~ 0xC0 (13)
 * @param pressed bool, if true, key is pressed
 */
void sp_Consumer(byte keycode, bool pressed) {
  if (pressed) {
    switch(keycode) {
      case C_VM: // con_volume_mute
        Consumer.press(MEDIA_VOLUME_MUTE);
        Consumer.release(MEDIA_VOLUME_MUTE);
      break;
      case C_VU: // con_volume_up
        Consumer.press(MEDIA_VOLUME_UP);
      break;
      case C_VD: // con_volume_down
        Consumer.press(MEDIA_VOLUME_DOWN);
      break;
      case C_MP: // con_media_play_pause
        Consumer.press(MEDIA_PLAY_PAUSE);
        Consumer.release(MEDIA_PLAY_PAUSE);
      break;
      case C_MS: // con_media_stop
        Consumer.press(MEDIA_STOP);
        Consumer.release(MEDIA_STOP);
      break;
      case C_MN: // con_media_next
        Consumer.press(MEDIA_NEXT);
        Consumer.release(MEDIA_NEXT);
      break;
      case C_MR: // con_media_prev
        Consumer.press(MEDIA_PREV);
        Consumer.release(MEDIA_PREV);
      break;
      case C_CA: // con_calculator
        Consumer.press(CONSUMER_CALCULATOR);
        Consumer.release(CONSUMER_CALCULATOR);
      break;
      case C_EM: // con_email
        Consumer.press(CONSUMER_EMAIL_READER);
        Consumer.release(CONSUMER_EMAIL_READER);
      break;
      case C_BH: // con_browser_home
        Consumer.press(CONSUMER_BROWSER_HOME);
        Consumer.release(CONSUMER_BROWSER_HOME);
      break;
      case C_BB: // con_browser_back
        Consumer.press(CONSUMER_BROWSER_BACK);
        Consumer.release(CONSUMER_BROWSER_BACK);
      break;
      case C_BF: // con_browser_forward
        Consumer.press(CONSUMER_BROWSER_FORWARD);
        Consumer.release(CONSUMER_BROWSER_FORWARD);
      break;
      /*
      case C_FI: // con_find
        Consumer.press(HID_CONSUMER_AC_FIND);
        Consumer.release(HID_CONSUMER_AC_FIND);
      break;
      */
    }
  } else {
    switch(keycode) {
      case C_VU: // con_volume_up
        Consumer.release(MEDIA_VOLUME_UP);
      break;
      case C_VD: // con_volume_down
        Consumer.release(MEDIA_VOLUME_DOWN);
      break;
    }
  }
}

/**
 * @brief special keycode for surface dial
 * 
 * @param keycode unsigned char, 0xEC ~ 0xEF (4)
 * @param pressed bool, if true, key is pressed
 */
void sp_Surface(byte keycode, bool pressed) {
  if (pressed) {
    switch(keycode) {
      case S_B : // surface_button
        SurfaceDial.press();
      break;
      case S_CW: // surface_clockwise
        SurfaceDial.rotate(10);
      break;
      case S_CC: // surface_counter_clockwise
        SurfaceDial.rotate(-10);
      break;
      case L_CN: // LED_change
        //Neo_modeChange();
      break;
    }
  } else {
    if(keycode == S_B) {
      SurfaceDial.release();
    }
  }
}

/**
 * @brief special keycode for special function
 * 
 * @param keycode unsigned char, 0xFC ~ 0xFF (4)
 * @param pressed bool, if true, key is pressed
 */
void sp_Function(byte keycode, bool pressed) {
  if (pressed) {
    switch(keycode) {
      case F_S1: // special_function_01
        sp_func_01();
      break;
      case F_S2: // special_function_02
        sp_func_02();
      break;
      case F_S3: // special_function_03
        sp_func_03();
      break;
      case F_S4: // special_function_04
        sp_func_04();
      break;
    }
  }
}

/////////////// user function ///////////////

/**
 * @brief user function 01
 */
void func_01(void) {
}

/**
 * @brief user function 02
 */
void func_02(void) {
}

/**
 * @brief user function 03
 */
void func_03(void) {
}

/**
 * @brief user function 04
 */
void func_04(void) {
}

/**
 * @brief user function 05
 */
void func_05(void) {
}

/**
 * @brief user function 06
 */
void func_06(void) {
}

/**
 * 
 * @brief user function 07
 */
void func_07(void) {
}

/**
 * @brief user function 08
 */
void func_08(void) {
}

/**
 * @brief special function 01
 */
void sp_func_01(void) {
}

/**
 * @brief special function 02
 */
void sp_func_02(void) {
}

/**
 * @brief special function 03
 */
void sp_func_03(void) {
}

/**
 * @brief special function 04
 */
void sp_func_04(void) {
}