#pragma once

#include <EEPROM.h> // ATmega32U4 has 1024 bytes
#include "key_datas.h"

void commandCheck(String str);
void eepromSave(void);
void eepromLoad(void);
void printData(void);
void setKey(void);

/**
 * @brief check if String is command
 * 
 * @param str String
 */
void commandCheck(String str) {
  Serial.print("[com] command : ");
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
    Serial.println("[com] No command!");
  }
}

/**
 * @brief save 'keySets' data to EEPROM
 */
void eepromSave(void) {
  unsigned int address = 0;
  byte data = 0;
  Serial.println("[com] Saving to EEPROM...");

  Serial.println("[com] Save complete!");
  Serial.println("[com] "+String(address)+" of 1024 bytes used");
}

/**
 * @brief load 'keySets' data from EEPROM
 */
void eepromLoad(void) {
  Serial.println("[com] Loading from EEPROM...");

  Serial.println("[com] Load complete!");
}

/**
 * @brief print 'keySets' data to serial
 */
void printData(void) {
  Serial.println("[com] Printing data...");

  Serial.println("[com] Print complete!");
}

/**
 * @brief get data from serial and save to 'keysets'
 */
void setKey(void) {
  Serial.println("[com] Setting mode...");

  Serial.println("[com] Key Setting complete!");
}
