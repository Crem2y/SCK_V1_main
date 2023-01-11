// for arduino Leonardo board
#include <EEPROM.h> // ATmega32U4 has 1024 bytes
#include <HID-Project.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <LiquidCrystal.h>
#include "customcode.h"
#include "keydatas.h"
#include "lcdchars.h"
#include "constants.h"

#define TIM_DISABLE TIMSK0&=(0<<OCIE0A)
#define TIM_ENABLE TIMSK0|=(1<<OCIE0A)

int rotpinA = 0;
int rotpinB = 1;

volatile int previous = 0;
volatile int counter = 0;

const byte keyH[KEYS_H] = {15,14,16,10};  // metrix pins
const byte keyV[KEYS_V] = {9,8,7,6,5};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

bool isKeyPressed[KEYS_V][KEYS_H] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}; // key press flags
bool isToggledKey[KEYS_V][KEYS_H] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}; // key toggle flags

signed   char mouseSpeed = MS_DEF; // mouse move speed  (-128 ~ 127)
signed   char wheelSpeed = WS_DEF; // mouse wheel speed (-128 ~ 127)
unsigned int repeatSpeed = RS_DEF; // delay value in repeat mode (1 ~ 65535ms)

String uartString = "";
byte kset = 0; // keyset number
unsigned int modekey_time = 0; // count modekey pressed time

byte lcd_time = 0; // lcd display time
String lcd_varname = ""; // lcd display var
int lcd_var = 0; // lcd display var

unsigned int timeCount = 0; // timer count

/**
 * @brief Construct a new ISR object
 * timer int (1kHz = 1ms)
 */
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
  //Consumer.begin();
  Mouse.begin();
  SurfaceDial.begin();

  lcd.createChar(1, lcd_char_num_lock);
  lcd.createChar(2, lcd_char_cap_lock);
  lcd.createChar(3, lcd_char_scr_lock);
  lcd.createChar(4, lcd_char_locked);
  lcd.createChar(5, lcd_char_unlocked);
  lcd.createChar(7, lcd_char_rhythm);
  lcd.begin(16,2);
  lcd.print(" Custom keypad  ");
  lcd.setCursor(0, 1);
  lcd.print("   by Cremmy    ");

  Serial.begin(9600);
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

  pinMode(rotpinA, INPUT_PULLUP); 
  pinMode(rotpinB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(rotpinA), Rotery_changed, CHANGE); 
  attachInterrupt(digitalPinToInterrupt(rotpinB), Rotery_changed, CHANGE);

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

  keysetChange(kset);

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
  lcd.setCursor(8, 0);
  lcd.write((keyleds & LED_NUM_LOCK) ? '\4' : '\5');
  lcd.setCursor(11, 0);
  lcd.write((keyleds & LED_CAPS_LOCK) ? '\4' : '\5');
  lcd.setCursor(14, 0);
  lcd.write((keyleds & LED_SCROLL_LOCK) ? '\4' : '\5');
  // lock key checking end

  if(isKeyPressed[0][0]) { // mode key hold checking // 0, 150, 300, 450
    modekey_time++;
    if(modekey_time > 600) {
      keysetSelect();
      /*
      kset++;
      if (kset >= KEYSET_MAX) kset = 0;
      keysetChange(kset);
      modekey_time = 0;
      */
    } else if (modekey_time == 450) {
      LCD_printSet("   holding..    ", 0);
    } else if (modekey_time == 300) {
      LCD_printSet("   holding.     ", 0);
    } else if (modekey_time == 150) {
      LCD_printSet("   holding      ", 0);
    }
  } else modekey_time = 0;
  // mode key hold checking end

  if(lcd_time > 0) { // lcd time checking
    if(lcd_time==255) {
      lcd.setCursor(0, 1);
      lcd.print(modeString[kset]);
      lcd_time = 0;
    } else if(lcd_time==1) {
      LCD_varPrint(lcd_varname, lcd_var);
      lcd_time++;
    } else {
      lcd_time++;
    }
  } // lcd time checking end

  if(counter >= 4) { // rotery checking
    keyHandle(C_VU, true);
    keyHandle(C_VU, false);
    counter -= 4;
  } else if(counter <= -4) {
    keyHandle(C_VD, true);
    keyHandle(C_VD, false);
    counter += 4;
  } // rotery checking end

  Neo_keyLight_KB(); // neopixel display
  // neopixel display end
}
//////////////////////////////// main loop end ////////////////////////////////

/**
 * @brief select keyset by key input
 */
void keysetSelect(void) {
  Keyboard.releaseAll();
  Mouse.releaseAll();
  Neo_keyLight_OFF();
  for(byte i=0; i<KEYS_V; i++){
    for(byte j=0; j<KEYS_H; j++){
      isKeyPressed[i][j] = false;
      isToggledKey[i][j] = false;
    }
  }

  lcd.home();
  lcd.print(" Enter mode num ");
  lcd.setCursor(0, 1);
  lcd.print(" (00~15)   (__) ");
  lcd.setCursor(12, 1);
  delay(500);
  byte setnum1 = 0;
  byte setnum2 = 0;
  byte setnum = 0;
  byte keynumnum = 0;

  while(1) {
    byte keynum = 99;
    byte key = keyInput();
    switch(key) {
      case 'q': // 0
      keynum = 0;
      break;
      case 'm': // 1
      keynum = 1;
      break;
      case 'n': // 2
      keynum = 2;
      break;
      case 'o': // 3
      keynum = 3;
      break;
      case 'i': // 4
      keynum = 4;
      break;
      case 'j': // 5
      keynum = 5;
      break;
      case 'k': // 6
      keynum = 6;
      break;
      case 'e': // 7
      keynum = 7;
      break;
      case 'f': // 8
      keynum = 8;
      break;
      case 'g': // 9
      keynum = 9;
      break;
      case 'p': // enter
      if(keynumnum>1) {
        if(setnum < KEYSET_MAX) {
          keysetChange(setnum);
        } else if(setnum == 99){
          keyTestMode();
          keysetChange(0);
        } else {
          keysetChange(0);
        }
        lcd.noCursor();
        return;
      }
      case 't': // backspace
      if(keynumnum == 2) {
        lcd.setCursor(13, 1);
        lcd.write('_');
        keynumnum = 1;
        setnum2 = 0;
      } else if(keynumnum == 1) {
        lcd.setCursor(12, 1);
        lcd.write('_');
        keynumnum = 0;
        setnum1 = 0;
      } else {
        //none
      }
      delay(100);
      break;
      default:
        delay(10);
      break;
    }
    if(keynum != 99) { // 숫자가 입력됐으면
      if(keynumnum != 0) {
        setnum2 = keynum;
        keynumnum = 2;
        lcd.setCursor(13, 1);
      } else {
        setnum1 = keynum;
        keynumnum = 1;
        lcd.setCursor(12, 1);
      }
      lcd.write(keynum + '0');
      setnum = setnum1 * 10 + setnum2;
      lcd.home();
      if(keynumnum == 2){
        if(setnum < KEYSET_MAX) {
          lcd.print(modeString[setnum]);
        } else if(setnum == 99) {
          lcd.print("      ????      ");
        } else {
          lcd.print(" invaild number ");
        }
      } else {
        lcd.print(" Enter mode num ");
      }
      delay(100);
    }
  }
}

/**
 * @brief change keyset
 * 
 * @param keyset byte, 0 ~ KEYSET_MAX
 */
void keysetChange(byte keyset) {
  repeatSpeed = MS_DEF;
  lcd.begin(16,2);
  lcd.print("M:_    \1_ \2_ \3_ ");
  lcd.setCursor(2, 0);
  lcd.print(keyset);
  LCD_print(modeString[keyset]);
  kset = keyset;
  delay(500);
}

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
 * @brief special keycode by mouse, repeat speed, and user function
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
        LCD_printSet("mouse:", mouseSpeed);
      break;
      case M_S: // mouse_slower
        if (mouseSpeed > MS_MIN) mouseSpeed -= MS_CNG;
        LCD_printSet("mouse:", mouseSpeed);
      break;
      case MH_F: // mouse_wheel_faster
        if (wheelSpeed < WS_MAX) wheelSpeed += WS_CNG;
        LCD_printSet("wheel:", wheelSpeed);
      break;
      case MH_S: // mouse_wheel_slower
        if (wheelSpeed > WS_MIN) wheelSpeed -= WS_CNG;
        LCD_printSet("wheel:", wheelSpeed);
      break;
      case R_F: // repeat_faster
        if (repeatSpeed > RS_MIN) repeatSpeed -= RS_CNG;
        LCD_printSet("delay:", repeatSpeed);
      break;
      case R_S: // repeat_slower
        if (repeatSpeed < RS_MAX) repeatSpeed += RS_CNG;
        LCD_printSet("delay:", repeatSpeed);
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
 * @brief special keycode by consumer key
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
 * @brief special keycode by surface dial
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
    }
  } else {
    if(keycode == S_B) {
      SurfaceDial.release();
    }
  }
}

/**
 * @brief special keycode by special function
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

/////////////// lcd function ///////////////
/**
 * @brief set string to print (about 1s)
 * 
 * @param msg String
 * @param var int
 */
void LCD_printSet(String msg, int var) {
  lcd_varname = msg;
  lcd_var = var;
  lcd_time = 1;
}

/**
 * @brief print string at (0,1) + var
 * 
 * @param msg String
 * @param var int
 */
void LCD_varPrint(String msg, int var) {
  lcd.setCursor(0, 1);
  lcd.print(LCD_BLANK);
  lcd.setCursor(0, 1);
  lcd.print(msg);
  lcd.print(var);
}

/**
 * @brief print string at (0,1)
 * 
 * @param msg String
 */
void LCD_print(String msg) {
  lcd.setCursor(0, 1);
  lcd.print(msg);
}

/////////////// neopixel function ///////////////
byte Neo_bright[20] = {0,};
byte Neo_count = 0;
byte Neo_count2 = 0;
byte Neo_ranset[20] = {1, 8, 12, 15, 14, 10, 9, 17, 0, 7, 13, 5, 2, 19, 11, 4, 6, 18, 3, 16};

/**
 * @brief turn off leds all
 */
void Neo_keyLight_OFF(void) { 
  for(byte i=0; i<20; i++) {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
  }
  pixels.show();
}

/**
 * @brief turn on leds by fixed color
 */
void Neo_keyLight_NN(void) { 
  for(byte i=0; i<20; i++) {
    pixels.setPixelColor(i, pixels.Color(P_BMAX,P_BMAX,P_BMAX));
  }
  pixels.show();
}

/**
 * @brief turn on leds by rainbow
 */
void Neo_keyLight_NR(void) { 
  for(byte i=0; i<4; i++) {
    pixels.setPixelColor(i,    pixels.Color(P_BMAX,0,0));
    pixels.setPixelColor(i+4,  pixels.Color(P_BMAX,P_BMAX,0));
    pixels.setPixelColor(i+8,  pixels.Color(0,P_BMAX,0));
    pixels.setPixelColor(i+12, pixels.Color(0,P_BMAX,P_BMAX));
    pixels.setPixelColor(i+16, pixels.Color(0,0,P_BMAX));
  }
  pixels.show();
}

/**
 * @brief random light
 */
void Neo_keyLight_RN(void) { 
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
  pixels.show();
}

/**
 * @brief turn on led pressed key only
 */
void Neo_keyLight_KB(void) {
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
  pixels.show();
}

/**
 * @brief turn on led if key is pressed
 */
void Neo_keyLight_KA(void) {
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
  pixels.show();
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
    LCD_print("   SAVING...    ");
    eepromSave();
  } else if(str == "LOAD") {
    LCD_print("   LOADING...   ");
    eepromLoad();
  } else if(str == "PRINT") {
    LCD_print("  PRINTING...   ");
    printData();
  } else if(str == "SETMODE") {
    LCD_print("  SETTING MODE  ");
    setKey();
  } else {
    Serial.println("No command!");
  }
  LCD_print(modeString[kset]);
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

  for(byte i=0; i<KEYSET_MAX; i++) { // save string
    for(byte j=0; j<16; j++) {
      data = modeString[i][j];
      EEPROM.write(address, data);
      address++;
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

  for(byte i=0; i<KEYSET_MAX; i++) { // load string
    for(byte j=0; j<16; j++) {
      modeString[i][j] = EEPROM.read(address);
      address++;
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

  Serial.println("Paste this list at 'modeString'");
  for(byte i=0; i<KEYSET_MAX; i++) {
    Serial.print("\"");
    for(byte j=0; j<16; j++) {
      Serial.print(modeString[i][j]);
    }
    Serial.print("\"");
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

/**
 * @brief key test mode
 */
void keyTestMode(void) {
  lcd.home();
  lcd.print(" key test mode  ");
  lcd.setCursor(0, 1);
  lcd.print(" key test mode  ");
  delay(500);
  lcd.clear();
  lcd.cursor();
    while(1) {
    byte key = keyInput();
    switch(key) {
      case 'q': // 0
        lcd.write('0');
      break;
      case 'm': // 1
        lcd.write('1');
      break;
      case 'n': // 2
        lcd.write('2');
      break;
      case 'o': // 3
        lcd.write('3');
      break;
      case 'i': // 4
        lcd.write('4');
      break;
      case 'j': // 5
        lcd.write('5');
      break;
      case 'k': // 6
        lcd.write('6');
      break;
      case 'e': // 7
        lcd.write('7');
      break;
      case 'f': // 8
        lcd.write('8');
      break;
      case 'g': // 9
        lcd.write('9');
      break;
      case 'p': // enter
        lcd.write('n');
      break;
      case 'h': // +
        lcd.write('+');
      break;
      case 'd': // -
        lcd.write('-');
      break;
      case 'c': // *
        lcd.write('*');
      break;
      case 'b': // /
        lcd.write('/');
      break;
      case 'a': // mode
        lcd.noCursor();
        return;
      break;
      default:
        delay(10);
      break;
    }
    if(key && (key != 'a')) {
      delay(100);
    }
  }
}

/**
 * @brief if rotery pin changed
 */
void Rotery_changed(void) {
  int A = digitalRead(rotpinA); 
  int B = digitalRead(rotpinB);

  int current = (A << 1) | B;
  int combined  = (previous << 2) | current;
   
  if(combined == 0b0010 || 
     combined == 0b1011 ||
     combined == 0b1101 || 
     combined == 0b0100) {
    counter++;
  }
   
  if(combined == 0b0001 ||
     combined == 0b0111 ||
     combined == 0b1110 ||
     combined == 0b1000) {
    counter--;
  }

  previous = current;
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