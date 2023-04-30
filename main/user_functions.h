#pragma once

#include "sck_functiuon_set.h"
#include "sck_key_handle.h"

/////////////// user function ///////////////

void uf_print_sample(void) {
  Keyboard.println("hello world!");
}


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

/**
 * @brief load user functions to array
 * 
 */
void user_func_set(void) {
  user_func[0] = uf_undo;
  user_func[1] = uf_redo;
  user_func[2] = uf_youtube;
}