# 1 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
// for arduino Leonardo board
# 3 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 2
# 4 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 2
# 5 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 2
# 6 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 2

# 8 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 2
# 9 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 2
# 10 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 2




const byte keyH[4 /* number of horiznal keys*/] 
# 14 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
                       __attribute__((__progmem__)) 
# 14 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
                               = {15,14,16,10}; // metrix pins
const byte keyV[5 /* number of vertical keys*/] 
# 15 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
                       __attribute__((__progmem__)) 
# 15 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
                               = {9,8,7,6,5};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(20 /* number of neopixels*/, 4 /* neopixel data pin*/, ((1 << 6) | (1 << 4) | (0 << 2) | (2)) /*|< Transmit as G,R,B*/ + 0x0000 /*|< 800 KHz data transmission*/);

bool isKeyPressed[5 /* number of vertical keys*/][4 /* number of horiznal keys*/] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}; // key press flags
bool isToggledKey[5 /* number of vertical keys*/][4 /* number of horiznal keys*/] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}; // key toggle flags

signed char mouseSpeed = 1 /* mouseSpeed default value (MS_MAX ~ MS_MIN)*/; // mouse move speed  (-128 ~ 127)
signed char wheelSpeed = 2 /* wheelSpeed default value (WS_MAX ~ WS_MIN)*/; // mouse wheel speed (-128 ~ 127)
unsigned int repeatSpeed = 10 /* repeatSpeed default value (RS_MAX ~ RS_MIN)*/; // delay value in repeat mode (1 ~ 65535ms)

String uartString = "";
byte kset = 0; // keyset number

unsigned int timeCount = 0; // timer count

/**

 * @brief Construct a new ISR object

 * timer int (1kHz = 1ms)

 */
# 35 "c:\\Coding\\SCK_V1_main\\main\\main.ino"

# 35 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
extern "C" void __vector_21 /* Timer/Counter0 Compare Match A */ (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_21 /* Timer/Counter0 Compare Match A */ (void) 
# 35 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
                      {
  timeCount++;
  if (timeCount < repeatSpeed) {
    return;
  }

  byte i; // keyH
  byte j; // keyV
  for(i=0; i<4 /* number of horiznal keys*/; i++) { // key checking
    for(j=0; j<5 /* number of vertical keys*/; j++) {
      if (keySets[kset][j][4 /* number of horiznal keys*/] & (0x40>>(i*2))) { // if repeat mode
        if (keySets[kset][j][4 /* number of horiznal keys*/] & (0x80>>(i*2))) { // if toggle mode
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

  for(i=0; i<4 /* number of horiznal keys*/; i++) { // pin set
    pinMode(keyH[i], 0x1);
    digitalWrite(keyH[i], 1);
  }
  for(i=0; i<5 /* number of vertical keys*/; i++) {
    pinMode(keyV[i], 0x2);
  } // pin set end

  // other funtion set
  BootKeyboard.begin();
  Mouse.begin();
  SurfaceDial.begin();

  Serial.begin(115200);
  pixels.begin();
  // other funtion set end

  // timer set (1kHz = 1ms)
  
# 82 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
 (*(volatile uint8_t *)((0x24) + 0x20)) 
# 82 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
        = 0; //TCCR0A initialize
  
# 83 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
 (*(volatile uint8_t *)((0x25) + 0x20)) 
# 83 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
        = 0; //TCCR0B initialize
  
# 84 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
 (*(volatile uint8_t *)((0x26) + 0x20)) 
# 84 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
       = 0; //TCNT0 initialize

  
# 86 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
 (*(volatile uint8_t *)((0x27) + 0x20))
# 86 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
      = 250;
  
# 87 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
 (*(volatile uint8_t *)((0x25) + 0x20)) 
# 87 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
        |= (1<<
# 87 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
               3
# 87 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
                    );
  
# 88 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
 (*(volatile uint8_t *)((0x25) + 0x20)) 
# 88 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
        |= (1<<
# 88 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
               1
# 88 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
                   ) | (1<<
# 88 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
                           0
# 88 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
                               );
  
# 89 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
 (*(volatile uint8_t *)(0x6E)) 
# 89 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
        |= (1<<
# 89 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
               1
# 89 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
                     );
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

  
# 103 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
 __asm__ __volatile__ ("sei" ::: "memory")
# 103 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
      ;
  // timer set end
}

//////////////////////////////// main loop start ////////////////////////////////
void loop(void) {
  byte i; // keyH
  byte j; // keyV

  while(Serial.available()) { //데이터가 오면
    
# 113 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
   (*(volatile uint8_t *)(0x6E))
# 113 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
   &=(0<<
# 113 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
   1
# 113 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
   ); //disable timer int
    uartString = Serial.readStringUntil('\n');
    commandCheck(uartString);
    
# 116 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
   (*(volatile uint8_t *)(0x6E))
# 116 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
   |=(1<<
# 116 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
   1
# 116 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
   ); //enable timer int
  }

  for(i=0; i<4 /* number of horiznal keys*/; i++) {
    digitalWrite(keyH[i], 1);
  }

  for(i=0; i<4 /* number of horiznal keys*/; i++){ // key checking
    digitalWrite(keyH[i], 0);
    for(j=0; j<5 /* number of vertical keys*/; j++){
      if (keySets[kset][j][4 /* number of horiznal keys*/] & (0x40>>(i*2))) { // if repeat mode
        if (keySets[kset][j][4 /* number of horiznal keys*/] & (0x80>>(i*2))) { // if toggle mode
          toggleRepeat(keyV[j], i, j, kset);
        } else {
          keyRepeat(keyV[j], i, j, kset);
        }
      } else {
        if (keySets[kset][j][4 /* number of horiznal keys*/] & (0x80>>(i*2))) { // if toggle mode
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
# 162 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
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
# 182 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
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
# 198 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
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
# 220 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
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
# 239 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
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
# 262 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_Normal(byte keycode, bool pressed) {
  if (pressed) {
    switch(keycode) {
      case 0x01 /* left_mouse*/: // left_mouse
        Mouse.press((1 << 0));
      break;
      case 0x02 /* right_mouse*/: // right_mouse
        Mouse.press((1 << 1));
      break;
      case 0x03 /* middle_mouse*/: // middle_mouse
        Mouse.press((1 << 2));
      break;
      case 0x04 /* mouse_button_4 (prev)*/: // mouse_button_4
        Mouse.press((1 << 3));
      break;
      case 0x05 /* mouse_button_5 (next)*/: // mouse_button_5
        Mouse.press((1 << 4));
      break;
      case 0x08 /* mouse_up*/: // mouse_up
        Mouse.move(0, -mouseSpeed, 0);
      break;
      case 0x09 /* mouse_down*/: // mouse_down
        Mouse.move(0, mouseSpeed, 0);
      break;
      case 0x0A /* mouse_left*/: // mouse_left
        Mouse.move(-mouseSpeed, 0, 0);
      break;
      case 0x0B /* mouse_right*/: // mouse_right
        Mouse.move(mouseSpeed, 0, 0);
      break;
      case 0x0C /* mouse_ul*/: // mouse_ul
        Mouse.move(-mouseSpeed, -mouseSpeed, 0);
      break;
      case 0x0D /* mouse_ur*/: // mouse_ur
        Mouse.move(mouseSpeed, -mouseSpeed, 0);
      break;
      case 0x0E /* mouse_dl*/: // mouse_dl
        Mouse.move(-mouseSpeed, mouseSpeed, 0);
      break;
      case 0x0F /* mouse_dr*/: // mouse_dr
        Mouse.move(mouseSpeed, mouseSpeed, 0);
      break;
      case 0x10 /* mouse_wheel_up*/: // mouse_wheel_up
        Mouse.move(0, 0, wheelSpeed);
      break;
      case 0x11 /* mouse_wheel_down*/: // mouse_wheel_down
        Mouse.move(0, 0, -wheelSpeed);
      break;
      case 0x12 /* mouse_faster*/: // mouse_faster
        if (mouseSpeed < 120 /* mouseSpeed max value (~ 127)*/) mouseSpeed += 1 /* mouseSpeed change value (1 ~ MS_MIN)*/;
      break;
      case 0x13 /* mouse_slower*/: // mouse_slower
        if (mouseSpeed > 1 /* mouseSpeed min value (0 ~)*/) mouseSpeed -= 1 /* mouseSpeed change value (1 ~ MS_MIN)*/;
      break;
      case 0x14 /* mouse_wheel_faster*/: // mouse_wheel_faster
        if (wheelSpeed < 20 /* wheelSpeed max value (~ 127)*/) wheelSpeed += 1 /* wheelSpeed change value (1 ~ WS_MIN)*/;
      break;
      case 0x15 /* mouse_wheel_slower*/: // mouse_wheel_slower
        if (wheelSpeed > 1 /* wheelSpeed min value (0 ~)*/) wheelSpeed -= 1 /* wheelSpeed change value (1 ~ WS_MIN)*/;
      break;
      case 0x16 /* repeat_faster*/: // repeat_faster
        if (repeatSpeed > 10 /* repeatSpeed min value (1 ~)*/) repeatSpeed -= 1 /* repeatSpeed change value (1 ~ RS_MIN)*/;
      break;
      case 0x17 /* repeat_slower*/: // repeat_slower
        if (repeatSpeed < 500 /* repeatSpeed max value (~ 65535)*/) repeatSpeed += 1 /* repeatSpeed change value (1 ~ RS_MIN)*/;
      break;
      case 0x18 /* function_01*/: // func_01
        func_01();
      break;
      case 0x19 /* function_02*/: // func_02
        func_02();
      break;
      case 0x1A /* function_03*/: // func_03
        func_03();
      break;
      case 0x1B /* function_04*/: // func_04
        func_04();
      break;
      case 0x1C /* function_05*/: // func_05
        func_05();
      break;
      case 0x1D /* function_06*/: // func_06
        func_06();
      break;
      case 0x1E /* function_07*/: // func_07
        func_07();
      break;
      case 0x1F /* function_08*/: // func_08
        func_08();
      break;
    }
  } else {
    switch(keycode) {
      case 0x01 /* left_mouse*/: // left_mouse
        Mouse.release((1 << 0));
      break;
      case 0x02 /* right_mouse*/: // right_mouse
        Mouse.release((1 << 1));
      break;
      case 0x03 /* middle_mouse*/: // middle_mouse
        Mouse.release((1 << 2));
      break;
      case 0x04 /* mouse_button_4 (prev)*/: // mouse_button_4
        Mouse.release((1 << 3));
      break;
      case 0x05 /* mouse_button_5 (next)*/: // mouse_button_5
        Mouse.release((1 << 4));
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
# 380 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_Gamepad(byte keycode, bool pressed) {
  if (pressed) {
    switch(keycode) {
      case 0x88 /* game_left_stick_up*/: // game_left_stick_up
        Gamepad.yAxis(0x8000);
      break;
      case 0x89 /* game_left_stick_down*/: // game_left_stick_down
        Gamepad.yAxis(0x7FFF);
      break;
      case 0x8A /* game_left_stick_left*/: // game_left_stick_left
        Gamepad.xAxis(0x8000);
      break;
      case 0x8B /* game_left_stick_right*/: // game_left_stick_right
        Gamepad.xAxis(0x7FFF);
      break;
      case 0x8C /* game_right_stick_up*/: // game_right_stick_up
        Gamepad.rxAxis(0x8000);
      break;
      case 0x8D /* game_right_stick_down*/: // game_right_stick_down
        Gamepad.rxAxis(0x7FFF);
      break;
      case 0x8E /* game_right_stick_left*/: // game_right_stick_left
        Gamepad.zAxis(0x80);
      break;
      case 0x8F /* game_right_stick_right*/: // game_right_stick_right
        Gamepad.zAxis(0x7F);
      break;
      case 0x90 /* game_dpad_up    (not working well)*/: // game_dpad_up
        Gamepad.dPad2(1);
      break;
      case 0x91 /* game_dpad_down  (not working well)*/: // game_dpad_down
        Gamepad.dPad2(5);
      break;
      case 0x92 /* game_dpad_left  (not working well)*/: // game_dpad_left
        Gamepad.dPad2(7);
      break;
      case 0x93 /* game_dpad_right (not working well)*/: // game_dpad_right
        Gamepad.dPad2(3);
      break;
      case 0x94 /* game_left_Zaxis+*/: // game_left_Zaxis+
        Gamepad.rzAxis(0x80);
      break;
      case 0x95 /* game_left_Zaxis-*/: // game_left_Zaxis-
        Gamepad.rzAxis(0x7F);
      break;
      case 0x96 /* game_right_Zaxis+*/: // game_right_Zaxis+
        Gamepad.ryAxis(0x8000);
      break;
      case 0x97 /* game_right_Zaxis-*/: // game_right_Zaxis-
        Gamepad.ryAxis(0x7FFF);
      break;
      case 0x98 /* game_button_01 (A)*/: // game_button_01
        Gamepad.press(1);
      break;
      case 0x99 /* game_button_02 (B)*/: // game_button_02
        Gamepad.press(2);
      break;
      case 0x9A /* game_button_03 (X)*/: // game_button_03
        Gamepad.press(3);
      break;
      case 0x9B /* game_button_04 (Y)*/: // game_button_04
        Gamepad.press(4);
      break;
      case 0x9C /* game_button_05 (LB)*/: // game_button_05
        Gamepad.press(5);
      break;
      case 0x9D /* game_button_06 (RB)*/: // game_button_06
        Gamepad.press(6);
      break;
      case 0x9E /* game_button_07 (LT)*/: // game_button_07
        Gamepad.press(7);
      break;
      case 0x9F /* game_button_08 (RT)*/: // game_button_08
        Gamepad.press(8);
      break;
      case 0xA0 /* game_button_09 (MENU)*/: // game_button_09
        Gamepad.press(9);
      break;
      case 0xA1 /* game_button_10 (SELECT)*/: // game_button_10
        Gamepad.press(10);
      break;
      case 0xA2 /* game_button_11 (LS)*/: // game_button_11
        Gamepad.press(11);
      break;
      case 0xA3 /* game_button_12 (RS)*/: // game_button_12
        Gamepad.press(12);
      break;
      case 0xA4 /* game_button_13 (DPAD_UP)*/: // game_button_13
        Gamepad.press(13);
      break;
      case 0xA5 /* game_button_14 (DPAD_DOWN)*/: // game_button_14
        Gamepad.press(14);
      break;
      case 0xA6 /* game_button_15 (DPAD_LEFT)*/: // game_button_15
        Gamepad.press(15);
      break;
      case 0xA7 /* game_button_16 (DPAD_RIGHT)*/: // game_button_16
        Gamepad.press(16);
      break;
      case 0xA8 /* game_button_17 (안드로이드-최근 앱)*/: // game_button_17
        Gamepad.press(17);
      break;
      case 0xA9 /* game_button_18 (안드로이드-홈)*/: // game_button_18
        Gamepad.press(18);
      break;
      case 0xAA /* game_button_19 (안드로이드-키보드)*/: // game_button_19
        Gamepad.press(19);
      break;
      case 0xAB /* game_button_20*/: // game_button_20
        Gamepad.press(20);
      break;
      case 0xAC /* game_button_21 (안드로이드-상단바 내림)*/: // game_button_21
        Gamepad.press(21);
      break;
      case 0xAD /* game_button_22 (안드로이드-설정)*/: // game_button_22
        Gamepad.press(22);
      break;
      case 0xAE /* game_button_23 (안드로이드-검색)*/: // game_button_23
        Gamepad.press(23);
      break;
      case 0xAF /* game_button_24*/: // game_button_24
        Gamepad.press(24);
      break;
    }
  } else {
    switch(keycode) {
      case 0x88 /* game_left_stick_up*/: // game_left_stick_up
        Gamepad.yAxis(0x0000);
      break;
      case 0x89 /* game_left_stick_down*/: // game_left_stick_down
        Gamepad.yAxis(0x0000);
      break;
      case 0x8A /* game_left_stick_left*/: // game_left_stick_left
        Gamepad.xAxis(0x0000);
      break;
      case 0x8B /* game_left_stick_right*/: // game_left_stick_right
        Gamepad.xAxis(0x0000);
      break;
      case 0x8C /* game_right_stick_up*/: // game_right_stick_up
        Gamepad.rxAxis(0x0000);
      break;
      case 0x8D /* game_right_stick_down*/: // game_right_stick_down
        Gamepad.rxAxis(0x0000);
      break;
      case 0x8E /* game_right_stick_left*/: // game_right_stick_left
        Gamepad.zAxis(0x00);
      break;
      case 0x8F /* game_right_stick_right*/: // game_right_stick_right
        Gamepad.zAxis(0x00);
      break;
      case 0x90 /* game_dpad_up    (not working well)*/: // game_dpad_up
        Gamepad.dPad2(0);
      break;
      case 0x91 /* game_dpad_down  (not working well)*/: // game_dpad_down
        Gamepad.dPad2(0);
      break;
      case 0x92 /* game_dpad_left  (not working well)*/: // game_dpad_left
        Gamepad.dPad2(0);
      break;
      case 0x93 /* game_dpad_right (not working well)*/: // game_dpad_right
        Gamepad.dPad2(0);
      break;
      case 0x94 /* game_left_Zaxis+*/: // game_left_Zaxis+
        Gamepad.rzAxis(0x00);
      break;
      case 0x95 /* game_left_Zaxis-*/: // game_left_Zaxis-
        Gamepad.rzAxis(0x00);
      break;
      case 0x96 /* game_right_Zaxis+*/: // game_right_Zaxis+
        Gamepad.ryAxis(0x0000);
      break;
      case 0x97 /* game_right_Zaxis-*/: // game_right_Zaxis-
        Gamepad.ryAxis(0x0000);
      break;
      case 0x98 /* game_button_01 (A)*/: // game_button_01
        Gamepad.release(1);
      break;
      case 0x99 /* game_button_02 (B)*/: // game_button_02
        Gamepad.release(2);
      break;
      case 0x9A /* game_button_03 (X)*/: // game_button_03
        Gamepad.release(3);
      break;
      case 0x9B /* game_button_04 (Y)*/: // game_button_04
        Gamepad.release(4);
      break;
      case 0x9C /* game_button_05 (LB)*/: // game_button_05
        Gamepad.release(5);
      break;
      case 0x9D /* game_button_06 (RB)*/: // game_button_06
        Gamepad.release(6);
      break;
      case 0x9E /* game_button_07 (LT)*/: // game_button_07
        Gamepad.release(7);
      break;
      case 0x9F /* game_button_08 (RT)*/: // game_button_08
        Gamepad.release(8);
      break;
      case 0xA0 /* game_button_09 (MENU)*/: // game_button_09
        Gamepad.release(9);
      break;
      case 0xA1 /* game_button_10 (SELECT)*/: // game_button_10
        Gamepad.release(10);
      break;
      case 0xA2 /* game_button_11 (LS)*/: // game_button_11
        Gamepad.release(11);
      break;
      case 0xA3 /* game_button_12 (RS)*/: // game_button_12
        Gamepad.release(12);
      break;
      case 0xA4 /* game_button_13 (DPAD_UP)*/: // game_button_13
        Gamepad.release(13);
      break;
      case 0xA5 /* game_button_14 (DPAD_DOWN)*/: // game_button_14
        Gamepad.release(14);
      break;
      case 0xA6 /* game_button_15 (DPAD_LEFT)*/: // game_button_15
        Gamepad.release(15);
      break;
      case 0xA7 /* game_button_16 (DPAD_RIGHT)*/: // game_button_16
        Gamepad.release(16);
      break;
      case 0xA8 /* game_button_17 (안드로이드-최근 앱)*/: // game_button_17
        Gamepad.release(17);
      break;
      case 0xA9 /* game_button_18 (안드로이드-홈)*/: // game_button_18
        Gamepad.release(18);
      break;
      case 0xAA /* game_button_19 (안드로이드-키보드)*/: // game_button_19
        Gamepad.release(19);
      break;
      case 0xAB /* game_button_20*/: // game_button_20
        Gamepad.release(20);
      break;
      case 0xAC /* game_button_21 (안드로이드-상단바 내림)*/: // game_button_21
        Gamepad.release(21);
      break;
      case 0xAD /* game_button_22 (안드로이드-설정)*/: // game_button_22
        Gamepad.release(22);
      break;
      case 0xAE /* game_button_23 (안드로이드-검색)*/: // game_button_23
        Gamepad.release(23);
      break;
      case 0xAF /* game_button_24*/: // game_button_24
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
# 637 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_Consumer(byte keycode, bool pressed) {
  if (pressed) {
    switch(keycode) {
      case 0xB4 /* con_volume_mute*/: // con_volume_mute
        Consumer.press(MEDIA_VOLUME_MUTE);
        Consumer.release(MEDIA_VOLUME_MUTE);
      break;
      case 0xB5 /* con_volume_up*/: // con_volume_up
        Consumer.press(MEDIA_VOLUME_UP);
      break;
      case 0xB6 /* con_volume_down*/: // con_volume_down
        Consumer.press(MEDIA_VOLUME_DOWN);
      break;
      case 0xB7 /* con_media_play_pause*/: // con_media_play_pause
        Consumer.press(MEDIA_PLAY_PAUSE);
        Consumer.release(MEDIA_PLAY_PAUSE);
      break;
      case 0xB8 /* con_media_stop*/: // con_media_stop
        Consumer.press(MEDIA_STOP);
        Consumer.release(MEDIA_STOP);
      break;
      case 0xB9 /* con_media_next*/: // con_media_next
        Consumer.press(MEDIA_NEXT);
        Consumer.release(MEDIA_NEXT);
      break;
      case 0xBA /* con_media_prev*/: // con_media_prev
        Consumer.press(MEDIA_PREV);
        Consumer.release(MEDIA_PREV);
      break;
      case 0xBB /* con_calculator*/: // con_calculator
        Consumer.press(CONSUMER_CALCULATOR);
        Consumer.release(CONSUMER_CALCULATOR);
      break;
      case 0xBC /* con_email*/: // con_email
        Consumer.press(CONSUMER_EMAIL_READER);
        Consumer.release(CONSUMER_EMAIL_READER);
      break;
      case 0xBD /* con_browser_home*/: // con_browser_home
        Consumer.press(CONSUMER_BROWSER_HOME);
        Consumer.release(CONSUMER_BROWSER_HOME);
      break;
      case 0xBE /* con_browser_back*/: // con_browser_back
        Consumer.press(CONSUMER_BROWSER_BACK);
        Consumer.release(CONSUMER_BROWSER_BACK);
      break;
      case 0xBF /* con_browser_forward*/: // con_browser_forward
        Consumer.press(CONSUMER_BROWSER_FORWARD);
        Consumer.release(CONSUMER_BROWSER_FORWARD);
      break;
      /*

      case C_FI: // con_find

        Consumer.press(HID_CONSUMER_AC_FIND);

        Consumer.release(HID_CONSUMER_AC_FIND);

      break;

      */
# 692 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
    }
  } else {
    switch(keycode) {
      case 0xB5 /* con_volume_up*/: // con_volume_up
        Consumer.release(MEDIA_VOLUME_UP);
      break;
      case 0xB6 /* con_volume_down*/: // con_volume_down
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
# 711 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_Surface(byte keycode, bool pressed) {
  if (pressed) {
    switch(keycode) {
      case 0xEC /* surface_button*/ : // surface_button
        SurfaceDial.press();
      break;
      case 0xED /* surface_clockwise*/: // surface_clockwise
        SurfaceDial.rotate(10);
      break;
      case 0xEE /* surface_counter_clockwise*/: // surface_counter_clockwise
        SurfaceDial.rotate(-10);
      break;
      case 0xEF /* LED_change*/: // LED_change
        Neo_modeChange();
      break;
    }
  } else {
    if(keycode == 0xEC /* surface_button*/) {
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
# 740 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_Function(byte keycode, bool pressed) {
  if (pressed) {
    switch(keycode) {
      case 0xFC /* special_function_01*/: // special_function_01
        sp_func_01();
      break;
      case 0xFD /* special_function_02*/: // special_function_02
        sp_func_02();
      break;
      case 0xFE /* special_function_03*/: // special_function_03
        sp_func_03();
      break;
      case 0xFF /* special_function_04*/: // special_function_04
        sp_func_04();
      break;
    }
  }
}

/////////////// neopixel function ///////////////
byte Neo_bright[20] = {0,};
byte Neo_count = 0;
byte Neo_count2 = 0;
const byte Neo_ranset[20] 
# 763 "c:\\Coding\\SCK_V1_main\\main\\main.ino" 3
                         __attribute__((__progmem__)) 
# 763 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
                                 = {1, 8, 12, 15, 14, 10, 9, 17, 0, 7, 13, 5, 2, 19, 11, 4, 6, 18, 3, 16};
byte Neo_mode = 4;

/**

 * @brief run every loop

 */
# 769 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
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
# 799 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void Neo_modeChange(void) {
  Neo_mode++;
  if (Neo_mode == 6) {
    Neo_mode = 0;
  }
}

/**

 * @brief turn off leds all

 */
# 809 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void Neo_keyLight_OFF(void) {
  for(byte i=0; i<20; i++) {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
  }
}

/**

 * @brief turn on leds by fixed color

 */
# 818 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void Neo_keyLight_Fixed(void) {
  for(byte i=0; i<20; i++) {
    pixels.setPixelColor(i, pixels.Color(15 /* PIXEL_BRIGHTMAX*/,15 /* PIXEL_BRIGHTMAX*/,15 /* PIXEL_BRIGHTMAX*/));
  }
}

/**

 * @brief turn on leds by rainbow

 */
# 827 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void Neo_keyLight_Rainbow(void) {
  for(byte i=0; i<4; i++) {
    pixels.setPixelColor(i, pixels.Color(15 /* PIXEL_BRIGHTMAX*/,0,0));
    pixels.setPixelColor(i+4, pixels.Color(15 /* PIXEL_BRIGHTMAX*/,15 /* PIXEL_BRIGHTMAX*/,0));
    pixels.setPixelColor(i+8, pixels.Color(0,15 /* PIXEL_BRIGHTMAX*/,0));
    pixels.setPixelColor(i+12, pixels.Color(0,15 /* PIXEL_BRIGHTMAX*/,15 /* PIXEL_BRIGHTMAX*/));
    pixels.setPixelColor(i+16, pixels.Color(0,0,15 /* PIXEL_BRIGHTMAX*/));
  }
}

/**

 * @brief random light

 */
# 840 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void Neo_keyLight_Random(void) {
  byte pnum = Neo_ranset[Neo_count2];
  Neo_bright[pnum] = 15 /* PIXEL_BRIGHTMAX*/;

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
# 866 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void Neo_keyLight_Pressed(void) {
  byte pnum = 0;
  for(byte i=0; i<5; i++) {
    for(byte j=0; j<4; j++) {
      if(isKeyPressed[i][j]) {
        Neo_bright[pnum] = 15 /* PIXEL_BRIGHTMAX*/;
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
# 894 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void Neo_keyLight_PressedAll(void) {
  Neo_bright[0] = 0;
  for(byte i=0; i<5; i++) {
    for(byte j=0; j<4; j++) {
      if(isKeyPressed[i][j]) {
        Neo_bright[0] = 1;
      }
    }
  }
  byte b = Neo_bright[0] ? 15 /* PIXEL_BRIGHTMAX*/ : 0;
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
# 915 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
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
# 933 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void eepromSave(void) {
  unsigned int address = 0;
  byte data = 0;

  for(byte i=0; i<16 /* number of key sets*/; i++) { // save key set
    for(byte j=0; j<5 /* number of vertical keys*/; j++) {
      for(byte k=0; k<4 /* number of horiznal keys*/+1; k++) {
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
# 954 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void eepromLoad(void) {
  unsigned int address = 0;

  for(byte i=0; i<16 /* number of key sets*/; i++) { // load key set
    for(byte j=0; j<5 /* number of vertical keys*/; j++) {
      for(byte k=0; k<4 /* number of horiznal keys*/+1; k++) {
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
# 972 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void printData(void) {
  Serial.println("Paste this list at 'keySets'");

  for(byte i=0; i<16 /* number of key sets*/; i++) {
    Serial.print("{");
    for(byte j=0; j<5 /* number of vertical keys*/; j++) {
      Serial.print("{");
      for(byte k=0; k<4 /* number of horiznal keys*/+1; k++) {
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
    if(i != 16 /* number of key sets*/-1) {
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
# 1005 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void setKey(void) {
  String str = "";
  Serial.println("Enter key set number (00~"+String(16 /* number of key sets*/-1)+")");

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
        if(keyset_num < 16 /* number of key sets*/) {
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
  byte buf[4 /* number of horiznal keys*/+1] = {0,};
  bool isError = false;
  for(byte i=0; i<5 /* number of vertical keys*/; i++) {
    Serial.println("Enter Row "+String(i));
    while(1) {
      for(byte j=0; j<4 /* number of horiznal keys*/+1; j++) { //버퍼 초기화
        buf[j] = 0;
      }
      str = "";
      while(!Serial.available());
      while(Serial.available()) {
        str = Serial.readStringUntil('\n');
        //Serial.println(str);
      }
      strpose = 0;
      for(byte j=0; j<4 /* number of horiznal keys*/+1; j++) {
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
    for(byte j=0; j<4 /* number of horiznal keys*/+1; j++) {
      keySets[keyset_num][i][j] = buf[j];
    }

    //체크
    for(byte k=0; k<4 /* number of horiznal keys*/+1; k++) {
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
# 1102 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
unsigned char keyInput(void) {
  byte i;
  byte j;
  for(i=0; i<4 /* number of horiznal keys*/; i++) {
    digitalWrite(keyH[i], 1);
  }
  for(i=0; i<4 /* number of horiznal keys*/; i++){ // key checking
    digitalWrite(keyH[i], 0);
    for(j=0; j<5 /* number of vertical keys*/; j++){
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
# 1125 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void func_01(void) {
}

/**

 * @brief user function 02

 */
# 1131 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void func_02(void) {
}

/**

 * @brief user function 03

 */
# 1137 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void func_03(void) {
}

/**

 * @brief user function 04

 */
# 1143 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void func_04(void) {
}

/**

 * @brief user function 05

 */
# 1149 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void func_05(void) {
}

/**

 * @brief user function 06

 */
# 1155 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void func_06(void) {
}

/**

 * 

 * @brief user function 07

 */
# 1162 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void func_07(void) {
  Keyboard.press(0x80);
  Keyboard.press('s');
  Keyboard.release(0x80);
  Keyboard.release('s');
}

/**

 * @brief user function 08

 */
# 1172 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void func_08(void) {
}

/**

 * @brief special function 01

 */
# 1178 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_func_01(void) {
  Keyboard.print("00");
}

/**

 * @brief special function 02

 */
# 1185 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_func_02(void) {
}

/**

 * @brief special function 03

 */
# 1191 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_func_03(void) {
}

/**

 * @brief special function 04

 */
# 1197 "c:\\Coding\\SCK_V1_main\\main\\main.ino"
void sp_func_04(void) {
}
