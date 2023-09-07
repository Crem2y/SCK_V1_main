#pragma once

#include "sck_functiuon_set.h"
#include "sck_key_handle.h"

/////////////// user function ///////////////

/**
 * @brief sample user function
 * 
 */
void uf_print_sample(void) {
  Keyboard.println("hello world!");
}

/*
void uf_youtube(void) {
  SCK_keyHandle(C_BH, true);
  SCK_keyHandle(C_BH, false);
  delay(500);
  Keyboard.println("https://www.youtube.com/");
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
*/

void uf_pw1(void) {
  Keyboard.println("yongjin0803");
}

void uf_pw2(void) {
  Keyboard.println("srv12345");
}

void uf_comm1(void) {
  Keyboard.print("sudo ");
}

void uf_comm2(void) {
  Keyboard.print("nano ");
}

void uf_comm3(void) {
  Keyboard.print("python3 ");
}

/**
 * @brief load user functions to array
 * 
 */
void user_func_set(void) {
  user_func[0] = uf_pw1;
  user_func[1] = uf_pw2;
  user_func[2] = uf_comm1;
  user_func[3] = uf_comm2;
  user_func[4] = uf_comm3;
}

/**
 * @brief load user function to array
 * 
 * @param func user function, ex) void func(void) {...}
 * @param func_num function number, 1~40
 */
void user_func_set_one(void func(void), unsigned char func_num) {
  if(func_num < 41) {
    user_func[func_num-1] = func;
  }
}