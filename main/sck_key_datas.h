#pragma once

#include "sck_key_code.h"

#define KEY_LAYERS 2
#define KM_V 6
#define KM_H 14
#define FM_V 5
#define FM_H 3
#define PM_V 5
#define PM_H 4
#define MM_V 8
#define MM_H 5

byte SCK_KM_keyset[KEY_LAYERS][KM_V][KM_H] = { // for Keyboard Module (FN, V, H)
  {{0xB1,K___,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD}, // normal key layer
   {'`' ,'1' ,'2' ,'3' ,'4' ,'5' ,'6' ,'7' ,'8' ,'9' ,'0' ,'-' ,'=' ,0xB2},
   {0xB3,'q' ,'w' ,'e' ,'r' ,'t' ,'y' ,'u' ,'i' ,'o' ,'p' ,'[' ,']' ,'\\'},
   {0xC1,'a' ,'s' ,'d' ,'f' ,'g' ,'h' ,'j' ,'k' ,'l' ,';' ,'\'',K___,0xB0},
   {0x81,'z' ,'x' ,'c' ,'v' ,'b' ,'y' ,'m' ,',' ,'.' ,'/' ,K___,K___,0x85},
   {0x80,0x83,0x82,K___,K___,' ' ,K___,K___,K___,0x86,FK_1,0xED,K___,0x84}},

  {{0xB1,K___,C_CA,C_VD,C_VU,C_VM,C_MS,C_MR,C_MP,C_MN,C_EM,C_BH,C_BB,C_BF}, // fn key layer
   {'`' ,'1' ,'2' ,'3' ,'4' ,'5' ,'6' ,'7' ,'8' ,'9' ,'0' ,'-' ,'=' ,0xB2},
   {0xB3,'q' ,'w' ,'e' ,'r' ,'t' ,'y' ,'u' ,'i' ,'o' ,'p' ,'[' ,']' ,'\\'},
   {0xC1,'a' ,'s' ,'d' ,'f' ,'g' ,'h' ,'j' ,'k' ,'l' ,';' ,'\'',K___,0xB0},
   {0x81,'z' ,'x' ,'c' ,'v' ,'b' ,'y' ,'m' ,',' ,'.' ,'/' ,K___,K___,0x85},
   {0x80,0x83,0x82,K___,K___,' ' ,K___,K___,K___,0x86,FK_1,0xED,K___,0x84}}
};

byte SCK_FM_keyset[KEY_LAYERS][FM_V][FM_H] = { // for Fnkey Module (FN, V, H)
  {{0xCE,0xCF,0xD0}, // normal key layer
   {0xD1,0xD2,0xD3},
   {0xD4,0xD5,0xD6},
   {K___,0xDA,K___},
   {0xD8,0xD9,0xD7}},

  {{0xCE,0xCF,0xD0}, // fn key layer
   {0xD1,0xD2,0xD3},
   {0xD4,0xD5,0xD6},
   {K___,0xDA,K___},
   {0xD8,0xD9,0xD7}}
};

byte SCK_PM_keyset[KEY_LAYERS][PM_V][PM_H] = { // for keyPad Module (FN, V, H)
  {{0xDB,0xDC,0xDD,0xDE}, // normal key layer
   {0xE7,0xE8,0xE9,0xDF},
   {0xE4,0xE5,0xE6,K___},
   {0xE1,0xE2,0xE3,0xE0},
   {0xEA,K___,0xEB,K___}},

  {{0xDB,0xDC,0xDD,0xDE}, // fn key layer
   {0xE7,0xE8,0xE9,0xDF},
   {0xE4,0xE5,0xE6,K___},
   {0xE1,0xE2,0xE3,0xE0},
   {0xEA,K___,0xEB,K___}}
};

// -- How to set the mode-code --
// set bit 7~0, if key is {1,2,3,4}
// bit 7 : rsw ccw toggle mode? (not recommended)
// bit 6 : rsw ccw repeat mode? (not recommended)
// bit 5 : rsw cw  toggle mode? (not recommended)
// bit 4 : rsw cw  repeat mode? (not recommended)
// bit 3 : rsw sw toggle mode?
// bit 2 : rsw sw repeat mode?
// bit 1 : key 1 toggle mode?
// bit 0 : key 1 repeat mode? 

// bit 7 : key 2 toggle mode?
// bit 6 : key 2 repeat mode?
// bit 5 : key 3 toggle mode?
// bit 4 : key 3 repeat mode?
// bit 3 : key 4 toggle mode?
// bit 2 : key 4 repeat mode?
// bit 1 : key 5 toggle mode?
// bit 0 : key 5 repeat mode? 

byte SCK_MM_keyset[KEY_LAYERS][MM_V+2][MM_H] = { // for Macro Module (FN, V, MM_num)
  {{0x00,0x01,0x00,0x00,0x00}, // normal key layer
   {0x00,0x00,0x00,0x00,0x00},
   {M_L ,M_U ,0x00,0x00,0x00}, // key data start
   {M_R ,M_D ,0x00,0x00,0x00},
   {M_LB,M_RB,0x00,0x00,0x00},
   {M_LB,M_RB,0x00,0x00,0x00},
   {'0' ,'e' ,0x00,0x00,0x00},
   {'6' ,'f' ,0x00,0x00,0x00},
   {F_01,'g' ,0x00,0x00,0x00},
   {FK_1,'h' ,0x00,0x00,0x00}},

  {{0x00,0x00,0x00,0x00,0x00}, // fn key layer
   {0x00,0x00,0x00,0x00,0x00},
   {0x00,0x00,0x00,0x00,0x00}, // key data start
   {0x00,0x00,0x00,0x00,0x00},
   {0x00,0x00,0x00,0x00,0x00},
   {0x00,0x00,0x00,0x00,0x00},
   {'1' ,0x00,0x00,0x00,0x00},
   {0x00,0x00,0x00,0x00,0x00},
   {0x00,0x00,0x00,0x00,0x00},
   {FK_1,0x00,0x00,0x00,0x00}},
};

bool SCK_KM_pressed[KM_V][KM_H] = {0,}; // Keyboard Module key press flags
bool SCK_KM_toggled[KM_V][KM_H] = {0,}; // Keyboard Module key toggle flags

bool SCK_FM_pressed[FM_V][FM_H] = {0,}; // Fnkey Module key press flags
bool SCK_FM_toggled[FM_V][FM_H] = {0,}; // Fnkey Module key toggle flags

bool SCK_PM_pressed[PM_V][PM_H] = {0,}; // keyPad Module key press flags
bool SCK_PM_toggled[PM_V][PM_H] = {0,}; // keyPad Module key toggle flags

bool SCK_MM_pressed[MM_V][MM_H] = {0,}; // Macro Module key press flags
bool SCK_MM_toggled[MM_V][MM_H] = {0,}; // Macro Module key toggle flags