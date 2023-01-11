#include <Arduino.h>
#line 1 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
// for arduino Leonardo board
#include <EEPROM.h> // ATmega32U4 has 1024 bytes
#include <HID-Project.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#include "customcode.h"
#include "keydatas.h"
#include "constants.h"

#define TIM_DISABLE TIMSK0&=(0<<OCIE0A)
#define TIM_ENABLE TIMSK0|=(1<<OCIE0A)

const byte keyH[KEYS_H] PROGMEM = {15,14,16,10};  // metrix pins
const byte keyV[KEYS_V] PROGMEM = {9,8,7,6,5};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);

bool isKeyPressed[KEYS_V][KEYS_H] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}; // key press flags
bool isToggledKey[KEYS_V][KEYS_H] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}; // key toggle flags

signed   char mouseSpeed = MS_DEF; // mouse move speed  (-128 ~ 127)
signed   char wheelSpeed = WS_DEF; // mouse wheel speed (-128 ~ 127)
unsigned int repeatSpeed = RS_DEF; // delay value in repeat mode (1 ~ 65535ms)

String uartString = "";
byte kset = 0; // keyset number

unsigned int timeCount = 0; // timer count

/**
 * @brief Construct a new ISR object
 * timer int (1kHz = 1ms)
 */
#line 61 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void setup(void);
#line 108 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void loop(void);
#line 162 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void keyCheck(byte key, byte keyposH, byte keyposV, byte keyset);
#line 182 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void keyRepeat(byte key, byte keyposH, byte keyposV, byte keyset);
#line 198 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void keyToggle(byte key, byte keyposH, byte keyposV, byte keyset);
#line 220 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void toggleRepeat(byte key, byte keyposH, byte keyposV, byte keyset);
#line 239 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void keyHandle(unsigned char keycode, bool pressed);
#line 262 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_Normal(byte keycode, bool pressed);
#line 380 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_Gamepad(byte keycode, bool pressed);
#line 637 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_Consumer(byte keycode, bool pressed);
#line 711 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_Surface(byte keycode, bool pressed);
#line 740 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_Function(byte keycode, bool pressed);
#line 769 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void Neo_loop(void);
#line 799 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void Neo_modeChange(void);
#line 809 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void Neo_keyLight_OFF(void);
#line 818 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void Neo_keyLight_Fixed(void);
#line 827 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void Neo_keyLight_Rainbow(void);
#line 840 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void Neo_keyLight_Random(void);
#line 866 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void Neo_keyLight_Pressed(void);
#line 894 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void Neo_keyLight_PressedAll(void);
#line 915 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void commandCheck(String str);
#line 933 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void eepromSave(void);
#line 954 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void eepromLoad(void);
#line 972 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void printData(void);
#line 1005 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void setKey(void);
#line 1102 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
unsigned char keyInput(void);
#line 1125 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void func_01(void);
#line 1131 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void func_02(void);
#line 1137 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void func_03(void);
#line 1143 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void func_04(void);
#line 1149 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void func_05(void);
#line 1155 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void func_06(void);
#line 1162 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void func_07(void);
#line 1172 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void func_08(void);
#line 1178 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_func_01(void);
#line 1185 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_func_02(void);
#line 1191 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_func_03(void);
#line 1197 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_func_04(void);
#line 35 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
ISR(TIMER0_COMPA_vect) {
  timeCount++;
  if (timeCount < repeatSpeed) {
    return;
  }

  byte i; // keyH
  byte j; // keyV
  for(i=0; i<KEYS_H; i++) { // key checking
    for(j=0; j<KEYS_V; j++) {
      if (keySets[kset][j][KEYS_H] & (0x40>>(i*2))) { // if repeat mode
        if (keySets[kset][j][KEYS_H] & (0x80>>(i*2))) { // if toggle mode
          if (isToggledKey[j][i]) { // if toggle flag on
            keyHandle(keySets[kset][j][i], true); // click a key
            keyHandle(keySets[kset][j][i], false);
          }
        } else if (isKeyPressed[j][i]) { // not toggle mode & key flag on
          keyHandle(keySets[kset][j][i], true); // click a key
          keyHandle(keySets[kset][j][i], false);
        }
      }
    }
  }
  timeCount = 0;
}

void setup(void) {
  byte i;

  for(i=0; i<KEYS_H; i++) { // pin set
    pinMode(keyH[i], OUTPUT);
    digitalWrite(keyH[i], 1);
  }
  for(i=0; i<KEYS_V; i++) {
    pinMode(keyV[i], INPUT_PULLUP);
  } // pin set end

  // other funtion set
  BootKeyboard.begin();
  Mouse.begin();
  SurfaceDial.begin();

  Serial.begin(115200);
  pixels.begin();
  // other funtion set end

  // timer set (1kHz = 1ms)
  TCCR0A = 0; //TCCR0A initialize
  TCCR0B = 0; //TCCR0B initialize
  TCNT0 = 0;  //TCNT0 initialize
  
  OCR0A= 250; 
  TCCR0B |= (1<<WGM02);
  TCCR0B |= (1<<CS01) | (1<<CS00);
  TIMSK0 |= (1<<OCIE0A);
  // timer set end

  pixels.setPixelColor(0, pixels.Color(15,15,15));
  pixels.show();
  for(byte i=1; i<20; i++) {
    delay(50);
    pixels.setPixelColor(i-1, pixels.Color(0,0,0));
    pixels.setPixelColor(i, pixels.Color(15,15,15));
    pixels.show();
  }
  pixels.setPixelColor(19, pixels.Color(0,0,0));
  pixels.show();

  sei();
  // timer set end
}

//////////////////////////////// main loop start ////////////////////////////////
void loop(void) { 
  byte i; // keyH
  byte j; // keyV

  while(Serial.available()) { //데이터가 오면
    TIM_DISABLE; //disable timer int
    uartString = Serial.readStringUntil('\n');
    commandCheck(uartString);
    TIM_ENABLE; //enable timer int
  }

  for(i=0; i<KEYS_H; i++) {
    digitalWrite(keyH[i], 1);
  }

  for(i=0; i<KEYS_H; i++){ // key checking
    digitalWrite(keyH[i], 0);
    for(j=0; j<KEYS_V; j++){
      if (keySets[kset][j][KEYS_H] & (0x40>>(i*2))) { // if repeat mode
        if (keySets[kset][j][KEYS_H] & (0x80>>(i*2))) { // if toggle mode
          toggleRepeat(keyV[j], i, j, kset);
        } else {
          keyRepeat(keyV[j], i, j, kset);
        }
      } else {
        if (keySets[kset][j][KEYS_H] & (0x80>>(i*2))) { // if toggle mode
          keyToggle(keyV[j], i, j, kset);
        } else {
          keyCheck(keyV[j], i, j, kset);
        }
      }
    }
    digitalWrite(keyH[i], 1);
  } // key checking end
  //delay(1);
  
  byte keyleds = BootKeyboard.getLeds(); // lock key checking

  // lock key checking end

  Neo_loop(); // neopixel display
  // neopixel display end
}
//////////////////////////////// main loop end ////////////////////////////////

/////////////// key function ///////////////
/**
 * @brief chack and press a key once
 * 
 * @param key byte, if 0, key is pressed
 * @param keyposH byte, 0 ~ KEYS_H
 * @param keyposV byte, 0 ~ KEYS_V
 * @param keyset byte, 0 ~ KEYSET_MAX
 */
void keyCheck(byte key, byte keyposH, byte keyposV, byte keyset) {
  if (!digitalRead(key)) { // if pressed
    if (!isKeyPressed[keyposV][keyposH]) { // if first detacted
      keyHandle(keySets[keyset][keyposV][keyposH], true); // press a key
      isKeyPressed[keyposV][keyposH] = true;
    }
  } else if (isKeyPressed[keyposV][keyposH]) { // if key released
    keyHandle(keySets[keyset][keyposV][keyposH], false); // release a key
    isKeyPressed[keyposV][keyposH] = false;
  }
}

/**
 * @brief click a key
 * 
 * @param key byte, if 0, key is pressed
 * @param keyposH byte, 0 ~ KEYS_H
 * @param keyposV byte, 0 ~ KEYS_V
 * @param keyset byte, 0 ~ KEYSET_MAX
 */
void keyRepeat(byte key, byte keyposH, byte keyposV, byte keyset) {
  if (!digitalRead(key)) { // if pressed
    isKeyPressed[keyposV][keyposH] = true;
  } else {
    isKeyPressed[keyposV][keyposH] = false;
  }
}

/**
 * @brief chack and toggle key
 * 
 * @param key byte, if 0, key is pressed
 * @param keyposH byte, 0 ~ KEYS_H
 * @param keyposV byte, 0 ~ KEYS_V
 * @param keyset byte, 0 ~ KEYSET_MAX
 */
void keyToggle(byte key, byte keyposH, byte keyposV, byte keyset) {
  bool TK = isToggledKey[keyposV][keyposH]; // previous state

  if (!digitalRead(key)) { // if pressed
    if (!isKeyPressed[keyposV][keyposH]) { // if first detacted
      isToggledKey[keyposV][keyposH] = !TK; // toggle state
      keyHandle(keySets[keyset][keyposV][keyposH], !TK); 
      isKeyPressed[keyposV][keyposH] = true;
    }
  } else { // if not pressed
    isKeyPressed[keyposV][keyposH] = false;
  }
}

/**
 * @brief toggle & click a key
 * 
 * @param key byte, if 0, key is pressed
 * @param keyposH byte, 0 ~ KEYS_H
 * @param keyposV byte, 0 ~ KEYS_V
 * @param keyset byte, 0 ~ KEYSET_MAX
 */
void toggleRepeat(byte key, byte keyposH, byte keyposV, byte keyset) {
  bool TK = isToggledKey[keyposV][keyposH]; // previous state

  if (!digitalRead(key)) { // if pressed
    if (!isKeyPressed[keyposV][keyposH]) { // if first detacted
      isToggledKey[keyposV][keyposH] = !TK; // toggle state
      isKeyPressed[keyposV][keyposH] = true;
    }
  } else { // if not pressed
    isKeyPressed[keyposV][keyposH] = false;
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
    sp_Gamepad(keycode, pressed);
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
 * @brief special keycode by gamepad
 * 
 * @param keycode unsigned char, 0x88 ~ 0xAF (40)
 * @param pressed bool, if true, key is pressed
 */
void sp_Gamepad(byte keycode, bool pressed) {
  if (pressed) {
    switch(keycode) {
      case G_LU: // game_left_stick_up
        Gamepad.yAxis(0x8000);
      break;
      case G_LD: // game_left_stick_down
        Gamepad.yAxis(0x7FFF);
      break;
      case G_LL: // game_left_stick_left
        Gamepad.xAxis(0x8000);
      break;
      case G_LR: // game_left_stick_right
        Gamepad.xAxis(0x7FFF);
      break;
      case G_RU: // game_right_stick_up
        Gamepad.rxAxis(0x8000);
      break;
      case G_RD: // game_right_stick_down
        Gamepad.rxAxis(0x7FFF);
      break;
      case G_RL: // game_right_stick_left
        Gamepad.zAxis(0x80);
      break;
      case G_RR: // game_right_stick_right
        Gamepad.zAxis(0x7F);
      break;
      case G_DU: // game_dpad_up
        Gamepad.dPad2(GAMEPAD_DPAD_UP);
      break;
      case G_DD: // game_dpad_down
        Gamepad.dPad2(GAMEPAD_DPAD_DOWN);
      break;
      case G_DL: // game_dpad_left
        Gamepad.dPad2(GAMEPAD_DPAD_LEFT);
      break;
      case G_DR: // game_dpad_right
        Gamepad.dPad2(GAMEPAD_DPAD_RIGHT);
      break;
      case G_ZU: // game_left_Zaxis+
        Gamepad.rzAxis(0x80);
      break;
      case G_ZD: // game_left_Zaxis-
        Gamepad.rzAxis(0x7F);
      break;
      case G_ZL: // game_right_Zaxis+
        Gamepad.ryAxis(0x8000);
      break;
      case G_ZR: // game_right_Zaxis-
        Gamepad.ryAxis(0x7FFF);
      break;
      case G_01: // game_button_01
        Gamepad.press(1);
      break;
      case G_02: // game_button_02
        Gamepad.press(2);
      break;
      case G_03: // game_button_03
        Gamepad.press(3);
      break;
      case G_04: // game_button_04
        Gamepad.press(4);
      break;
      case G_05: // game_button_05
        Gamepad.press(5);
      break;
      case G_06: // game_button_06
        Gamepad.press(6);
      break;
      case G_07: // game_button_07
        Gamepad.press(7);
      break;
      case G_08: // game_button_08
        Gamepad.press(8);
      break;
      case G_09: // game_button_09
        Gamepad.press(9);
      break;
      case G_10: // game_button_10
        Gamepad.press(10);
      break;
      case G_11: // game_button_11
        Gamepad.press(11);
      break;
      case G_12: // game_button_12
        Gamepad.press(12);
      break;
      case G_13: // game_button_13
        Gamepad.press(13);
      break;
      case G_14: // game_button_14
        Gamepad.press(14);
      break;
      case G_15: // game_button_15
        Gamepad.press(15);
      break;
      case G_16: // game_button_16
        Gamepad.press(16);
      break;
      case G_17: // game_button_17
        Gamepad.press(17);
      break;
      case G_18: // game_button_18
        Gamepad.press(18);
      break;
      case G_19: // game_button_19
        Gamepad.press(19);
      break;
      case G_20: // game_button_20
        Gamepad.press(20);
      break;
      case G_21: // game_button_21
        Gamepad.press(21);
      break;
      case G_22: // game_button_22
        Gamepad.press(22);
      break;
      case G_23: // game_button_23
        Gamepad.press(23);
      break;
      case G_24: // game_button_24
        Gamepad.press(24);
      break;
    }
  } else {
    switch(keycode) {
      case G_LU: // game_left_stick_up
        Gamepad.yAxis(0x0000);
      break;
      case G_LD: // game_left_stick_down
        Gamepad.yAxis(0x0000);
      break;
      case G_LL: // game_left_stick_left
        Gamepad.xAxis(0x0000);
      break;
      case G_LR: // game_left_stick_right
        Gamepad.xAxis(0x0000);
      break;
      case G_RU: // game_right_stick_up
        Gamepad.rxAxis(0x0000);
      break;
      case G_RD: // game_right_stick_down
        Gamepad.rxAxis(0x0000);
      break;
      case G_RL: // game_right_stick_left
        Gamepad.zAxis(0x00);
      break;
      case G_RR: // game_right_stick_right
        Gamepad.zAxis(0x00);
      break;
      case G_DU: // game_dpad_up
        Gamepad.dPad2(GAMEPAD_DPAD_CENTERED);
      break;
      case G_DD: // game_dpad_down
        Gamepad.dPad2(GAMEPAD_DPAD_CENTERED);
      break;
      case G_DL: // game_dpad_left
        Gamepad.dPad2(GAMEPAD_DPAD_CENTERED);     
      break;
      case G_DR: // game_dpad_right
        Gamepad.dPad2(GAMEPAD_DPAD_CENTERED);
      break;
      case G_ZU: // game_left_Zaxis+
        Gamepad.rzAxis(0x00);
      break;
      case G_ZD: // game_left_Zaxis-
        Gamepad.rzAxis(0x00);
      break;
      case G_ZL: // game_right_Zaxis+
        Gamepad.ryAxis(0x0000);
      break;
      case G_ZR: // game_right_Zaxis-
        Gamepad.ryAxis(0x0000);
      break;
      case G_01: // game_button_01
        Gamepad.release(1);
      break;
      case G_02: // game_button_02
        Gamepad.release(2);
      break;
      case G_03: // game_button_03
        Gamepad.release(3);
      break;
      case G_04: // game_button_04
        Gamepad.release(4);
      break;
      case G_05: // game_button_05
        Gamepad.release(5);
      break;
      case G_06: // game_button_06
        Gamepad.release(6);
      break;
      case G_07: // game_button_07
        Gamepad.release(7);
      break;
      case G_08: // game_button_08
        Gamepad.release(8);
      break;
      case G_09: // game_button_09
        Gamepad.release(9);
      break;
      case G_10: // game_button_10
        Gamepad.release(10);
      break;
      case G_11: // game_button_11
        Gamepad.release(11);
      break;
      case G_12: // game_button_12
        Gamepad.release(12);
      break;
      case G_13: // game_button_13
        Gamepad.release(13);
      break;
      case G_14: // game_button_14
        Gamepad.release(14);
      break;
      case G_15: // game_button_15
        Gamepad.release(15);
      break;
      case G_16: // game_button_16
        Gamepad.release(16);
      break;
      case G_17: // game_button_17
        Gamepad.release(17);
      break;
      case G_18: // game_button_18
        Gamepad.release(18);
      break;
      case G_19: // game_button_19
        Gamepad.release(19);
      break;
      case G_20: // game_button_20
        Gamepad.release(20);
      break;
      case G_21: // game_button_21
        Gamepad.release(21);
      break;
      case G_22: // game_button_22
        Gamepad.release(22);
      break;
      case G_23: // game_button_23
        Gamepad.release(23);
      break;
      case G_24: // game_button_24
        Gamepad.release(24);
      break;
    }
  }
  Gamepad.write();
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
        Neo_modeChange();
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

/////////////// neopixel function ///////////////
byte Neo_bright[20] = {0,};
byte Neo_count = 0;
byte Neo_count2 = 0;
const byte Neo_ranset[20] PROGMEM = {1, 8, 12, 15, 14, 10, 9, 17, 0, 7, 13, 5, 2, 19, 11, 4, 6, 18, 3, 16};
byte Neo_mode = 4;

/**
 * @brief run every loop
 */
void Neo_loop(void) { 
  switch (Neo_mode) {
    case 0:
      Neo_keyLight_OFF();
    break;
    case 1:
      Neo_keyLight_Fixed();
    break;
    case 2:
      Neo_keyLight_Rainbow();
    break;
    case 3:
      Neo_keyLight_Random();
    break;
    case 4:
      Neo_keyLight_Pressed();
    break;
    case 5:
      Neo_keyLight_PressedAll();
    break;
    default:
      Neo_keyLight_OFF();
    break;
  }
  pixels.show();
}

/**
 * @brief change mode
 */
void Neo_modeChange(void) { 
  Neo_mode++;
  if (Neo_mode == 6) {
    Neo_mode = 0;
  }
}

/**
 * @brief turn off leds all
 */
void Neo_keyLight_OFF(void) { 
  for(byte i=0; i<20; i++) {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
  }
}

/**
 * @brief turn on leds by fixed color
 */
void Neo_keyLight_Fixed(void) { 
  for(byte i=0; i<20; i++) {
    pixels.setPixelColor(i, pixels.Color(P_BMAX,P_BMAX,P_BMAX));
  }
}

/**
 * @brief turn on leds by rainbow
 */
void Neo_keyLight_Rainbow(void) { 
  for(byte i=0; i<4; i++) {
    pixels.setPixelColor(i,    pixels.Color(P_BMAX,0,0));
    pixels.setPixelColor(i+4,  pixels.Color(P_BMAX,P_BMAX,0));
    pixels.setPixelColor(i+8,  pixels.Color(0,P_BMAX,0));
    pixels.setPixelColor(i+12, pixels.Color(0,P_BMAX,P_BMAX));
    pixels.setPixelColor(i+16, pixels.Color(0,0,P_BMAX));
  }
}

/**
 * @brief random light
 */
void Neo_keyLight_Random(void) { 
  byte pnum = Neo_ranset[Neo_count2];
  Neo_bright[pnum] = P_BMAX;

  for(byte i=0; i<20; i++) {
    byte b = Neo_bright[i];
    pixels.setPixelColor(i, pixels.Color(b,b,b));
    if(Neo_count == 20) {
      if (b > 0) {
        Neo_bright[i] -= 1;
      }
    }
  }
  if(Neo_count2 == 20) {
    Neo_count2 = 0;
  }
  if(Neo_count == 20) {
    Neo_count = 0;
    Neo_count2++;
  }
  Neo_count++;
}

/**
 * @brief turn on led pressed key only
 */
void Neo_keyLight_Pressed(void) {
  byte pnum = 0;
  for(byte i=0; i<5; i++) {
    for(byte j=0; j<4; j++) {
      if(isKeyPressed[i][j]) {
        Neo_bright[pnum] = P_BMAX;
      }
      pnum++;
    }
  }
  for(byte i=0; i<20; i++) {
    byte b = Neo_bright[i];
    pixels.setPixelColor(i, pixels.Color(b,b,b));
    if(Neo_count == 20) {
      if (b > 0) {
        Neo_bright[i] -= 1;
      }
    }
  }
  if(Neo_count == 20) {
    Neo_count = 0;
  }
  Neo_count++;
}

/**
 * @brief turn on led if key is pressed
 */
void Neo_keyLight_PressedAll(void) {
  Neo_bright[0] = 0;
  for(byte i=0; i<5; i++) {
    for(byte j=0; j<4; j++) {
      if(isKeyPressed[i][j]) {
        Neo_bright[0] = 1;
      }
    }
  }
  byte b = Neo_bright[0] ? P_BMAX : 0;
  for(byte i=0; i<20; i++) {
    pixels.setPixelColor(i, pixels.Color(b,b,b));
  }
}

/////////////// serial function ///////////////
/**
 * @brief check if String is command
 * 
 * @param str String
 */
void commandCheck(String str) {
  Serial.println(str);
  if(str == "SAVE") {
    eepromSave();
  } else if(str == "LOAD") {
    eepromLoad();
  } else if(str == "PRINT") {
    printData();
  } else if(str == "SETMODE") {
    setKey();
  } else {
    Serial.println("No command!");
  }
}

/**
 * @brief save 'keySets' data to EEPROM
 */
void eepromSave(void) {
  unsigned int address = 0;
  byte data = 0;

  for(byte i=0; i<KEYSET_MAX; i++) { // save key set
    for(byte j=0; j<KEYS_V; j++) {
      for(byte k=0; k<KEYS_H+1; k++) {
        data = keySets[i][j][k];
        EEPROM.write(address, data);
        address++;
      }
    }
  }

  Serial.println("Save complete!");
  Serial.println(String(address)+" of 1024 bytes used");
}

/**
 * @brief load 'keySets' data from EEPROM
 */
void eepromLoad(void) {
  unsigned int address = 0;
  
  for(byte i=0; i<KEYSET_MAX; i++) { // load key set
    for(byte j=0; j<KEYS_V; j++) {
      for(byte k=0; k<KEYS_H+1; k++) {
        keySets[i][j][k] = EEPROM.read(address);
        address++;
      }
    }
  }

  Serial.println(String(address)+" bytes Load complete!");
}

/**
 * @brief print 'keySets' data at serial
 */
void printData(void) {
  Serial.println("Paste this list at 'keySets'");

  for(byte i=0; i<KEYSET_MAX; i++) {
    Serial.print("{");
    for(byte j=0; j<KEYS_V; j++) {
      Serial.print("{");
      for(byte k=0; k<KEYS_H+1; k++) {
        char buf[5];
        sprintf(buf, "0x%02X", keySets[i][j][k]);
        Serial.print(buf);
        if(k != 4) {
          Serial.print(",");
        }
      }
      Serial.print("}");
      if(j != 4) {
        Serial.print(",");
      }
    }
    Serial.print("}");
    if(i != KEYSET_MAX-1) {
      Serial.print(",");
    } else {
      Serial.print(" ");
    }
    Serial.println("// "+String(i));
  }
}

/**
 * @brief get data by serial and save to 'keysets'
 */
void setKey(void) {
  String str = "";
  Serial.println("Enter key set number (00~"+String(KEYSET_MAX-1)+")");

  byte keyset_num = 0;
  while(1) {
    keyset_num = 0;
    str = "";
    while(!Serial.available());
    while(Serial.available()) {
      str = Serial.readStringUntil('\n');
      Serial.println(str);
    }
    if(str[0]>='0' && str[0]<='9') {
      keyset_num += 10*(str[0]-'0');
      if(str[1]>='0' && str[1]<='9') {
        keyset_num += (str[1]-'0');
        if(keyset_num < KEYSET_MAX) {
          break;
        }
      }
    }
    Serial.println("ERROR! Please re-enter");
  }
  Serial.println("key set "+String(keyset_num)+" selected");

  Serial.println("Enter key set config in hex");
  Serial.println("Ex: 0x50,0x61,0xEB,0xDF,0x00");

  byte strpose;
  byte buf[KEYS_H+1] = {0,};
  bool isError = false;
  for(byte i=0; i<KEYS_V; i++) {
    Serial.println("Enter Row "+String(i));
    while(1) {
      for(byte j=0; j<KEYS_H+1; j++) { //버퍼 초기화
        buf[j] = 0;
      }
      str = "";
      while(!Serial.available());
      while(Serial.available()) {
        str = Serial.readStringUntil('\n');
        //Serial.println(str);
      }
      strpose = 0;
      for(byte j=0; j<KEYS_H+1; j++) {
        //공백무시하고 시작위치 찾아서...
        isError = true;
        for(byte k=strpose; k<255; k++) {
          if(str[k] == '0' && str[k+1] == 'x') {
            strpose = k+2;
            isError = false;
            break;
          }
        }
        if(isError) break;
        //하나씩 숫자로 변환
        for(byte k=0; k<2; k++) {
          buf[j] = buf[j] << 4;
          if(str[strpose]>='0' && str[strpose]<='9') {
            buf[j] += str[strpose]-'0';
          } else if (str[strpose]>='A' && str[strpose]<='F') {
            buf[j] += str[strpose]-'A'+0x0A;
          } else isError = true;
          strpose++;
        }
      }
      //에러 체크
      if(!isError) break;
      else Serial.println("ERROR! Please re-enter");
    }

    //저장
    for(byte j=0; j<KEYS_H+1; j++) {
      keySets[keyset_num][i][j] = buf[j];
    }

    //체크
    for(byte k=0; k<KEYS_H+1; k++) {
      char buf[5];
      sprintf(buf, "0x%02X", keySets[keyset_num][i][k]);
      Serial.print(buf);
      if(k != 4) {
        Serial.print(",");
      }
    }
    Serial.println();
    Serial.println("Row "+String(i)+" saved!");

  }
  Serial.println("key set "+String(keyset_num)+" saved!");
}

/////////////// other function ///////////////
/**
 * @brief check key input a~t (but NO keyboard output)
 */
unsigned char keyInput(void) {
  byte i;
  byte j;
  for(i=0; i<KEYS_H; i++) {
    digitalWrite(keyH[i], 1);
  }
  for(i=0; i<KEYS_H; i++){ // key checking
    digitalWrite(keyH[i], 0);
    for(j=0; j<KEYS_V; j++){
      if(!digitalRead(keyV[j])) {
        return 'a'+i + j*4;
      }
    }
    digitalWrite(keyH[i], 1);
  } // key checking end
  return 0;
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
  Keyboard.press(0x80);
  Keyboard.press('s');
  Keyboard.release(0x80);
  Keyboard.release('s');
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
  Keyboard.print("00");
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
