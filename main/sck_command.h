#pragma once

#include <EEPROM.h> // ATmega32U4 has 1024 bytes

#include "sck_key_datas.h"
#include "neopixel_handle.h"

void commandCheck(String str);
void eepromSave(void);
void eepromLoad(void);
void printData(void);
void setKey(void);
char* firm_ver;

//////////////////////////////// functions ////////////////////////////////

/**
 * @brief printing char data to hex
 *  
 * @param data hex data, 0x00 ~ 0xFF
 * @param next_line if true, add '\n'.
 */
void print_hex(byte data, bool next_line) {
  if(data < 16) {
    Serial.print('0');
  }
  Serial.print(data, HEX);

  if(next_line) {
    Serial.println();
  }
}

/**
 * @brief change "00" ~ "FF" to 0x00 ~ 0xFF
 * 
 * @param highdata hex high data, '0' ~ 'F'
 * @param lowdata hex low data, '0' ~ 'F'
 * @return char 
 */
char hex2byte(char highdata, char lowdata) {
  char data = 0;

  if(highdata > 0x29 && highdata < 0x3A)
  {
    data = (highdata - '0');
  }
  else if(highdata > 0x40 && highdata < 0x47)
  {
    data = ((highdata - 'A') + 10);
  }

  data = data << 4;

  if(lowdata > 0x29 && lowdata < 0x3A)
  {
    data += (lowdata - '0') & 0x0F;
  }
  else if(lowdata > 0x40 && lowdata < 0x47)
  {
    data += ((lowdata - 'A') + 10) & 0x0F;
  }

  return data;
}

/**
 * @brief check if String is command
 * 
 * @param str command
 */
void commandCheck(String str) {
  //Serial.print("[com] ");
  //Serial.println(str);

  if(str == "SAVE") {
    eepromSave();
  } else if(str == "LOAD") {
    eepromLoad();
  } else if(str == "PRINT") {
    printData();
  } else if(str == "SETMODE") {
    setKey();
  } else if(str == "FIRMVER") {
    Serial.println(firm_ver);
  } else {
    if(str[0] != '\n')
      Serial.println("No command!");
  }
}

/**
 * @brief save 'keySets' data to EEPROM
 */
void eepromSave(void) {
  unsigned int address = 0;
  byte i,j,k;
  byte data;

  Serial.println(F("Saving to EEPROM..."));

  EEPROM.write(address, Neo.key.mode);
  address++;

  EEPROM.write(address, Neo.side.mode);
  address++;

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<KM_V; j++) {
      for(k=0; k<KM_H; k++) {
        data = SCK_KM_keyset[i][j][k];
        EEPROM.write(address, data);
        address++;
      }
    }
  }

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<FM_V; j++) {
      for(k=0; k<FM_H; k++) {
        data = SCK_FM_keyset[i][j][k];
        EEPROM.write(address, data);
        address++;
      }
    }
  }

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<PM_V; j++) {
      for(k=0; k<PM_H; k++) {
        data = SCK_PM_keyset[i][j][k];
        EEPROM.write(address, data);
        address++;
      }
    }
  }

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<MM_V+2; j++) {
      for(k=0; k<MM_H; k++) {
        data = SCK_MM_keyset[i][j][k];
        EEPROM.write(address, data);
        address++;
      }
    }
  }

  Serial.println(F("[com] Save complete!"));
  Serial.println("[com] "+String(address)+" of 1024 bytes used");
}

/**
 * @brief load 'keySets' data from EEPROM
 */
void eepromLoad(void) {
  unsigned int address = 0;
  byte i,j,k;
  byte data;
  
  Serial.println(F("Loading from EEPROM..."));

  Neo.key.mode = EEPROM.read(address);
  address++;

  Neo.side.mode = EEPROM.read(address);
  address++;

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<KM_V; j++) {
      for(k=0; k<KM_H; k++) {
        data = EEPROM.read(address);
        SCK_KM_keyset[i][j][k] = data;
        address++;
      }
    }
  }

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<FM_V; j++) {
      for(k=0; k<FM_H; k++) {
        data = EEPROM.read(address);
        SCK_FM_keyset[i][j][k] = data;
        address++;
      }
    }
  }

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<PM_V; j++) {
      for(k=0; k<PM_H; k++) {
        data = EEPROM.read(address);
        SCK_PM_keyset[i][j][k] = data;
        address++;
      }
    }
  }

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<MM_V+2; j++) {
      for(k=0; k<MM_H; k++) {
        data = EEPROM.read(address);
        SCK_MM_keyset[i][j][k] = data;
        address++;
      }
    }
  }

  Serial.println("[com] "+String(address)+" bytes load complete!");
}

/**
 * @brief print 'keySets' data to serial
 */
void printData(void) {
  byte i,j,k;
  byte data;

  Serial.println(F("Printing data..."));

  print_hex(Neo.key.mode, false);
  Serial.print(' ');
  print_hex(Neo.side.mode, true);
  Serial.println();

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<KM_V; j++) {
      for(k=0; k<KM_H; k++) {
        data = SCK_KM_keyset[i][j][k];
        print_hex(data, false);
        Serial.print(' ');
      }
      Serial.println();
    }
    Serial.println();
  }
  Serial.println();

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<FM_V; j++) {
      for(k=0; k<FM_H; k++) {
        data = SCK_FM_keyset[i][j][k];
        print_hex(data, false);
        Serial.print(' ');
      }
      Serial.println();
    }
    Serial.println();
  }
  Serial.println();

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<PM_V; j++) {
      for(k=0; k<PM_H; k++) {
        data = SCK_PM_keyset[i][j][k];
        print_hex(data, false);
        Serial.print(' ');
      }
      Serial.println();
    }
    Serial.println();
  }
  Serial.println();

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<MM_V+2; j++) {
      for(k=0; k<MM_H; k++) {
        data = SCK_MM_keyset[i][j][k];
        print_hex(data, false);
        Serial.print(' ');
      }
      Serial.println();
    }
    Serial.println();
  }

  Serial.println(F("[com] Print complete!"));
}

/**
 * @brief get data from serial and save to 'keysets'
 */
void setKey(void) {
  byte i,j,k;
  byte data;
  String str;

  Serial.println(F("Setting mode..."));

  while(!Serial.available());
  str = Serial.readStringUntil('\n');

  Serial.println(F("Other data..."));

  data = hex2byte(str[0], str[1]);
  Neo.key.mode = data;
  data = hex2byte(str[3], str[4]);
  Neo.side.mode = data;

  Serial.println(F("Other data ok"));

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<KM_V; j++) {
      Serial.println("Keyboard Module data ("+String(i+1)+"/"+String(KEY_LAYERS)+","+String(j+1)+"/"+String(KM_V)+")");
      while(!Serial.available());
      str = Serial.readStringUntil('\n');
      for(k=0; k<KM_H; k++) {
        data = hex2byte(str[k*3], str[k*3+1]);
        SCK_KM_keyset[i][j][k] = data;
      }
    }
  }
  Serial.println(F("Keyboard Module data ok"));

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<FM_V; j++) {
      Serial.println("Fnkey Module data ("+String(i+1)+"/"+String(KEY_LAYERS)+","+String(j+1)+"/"+String(FM_V)+")");
      while(!Serial.available());
      str = Serial.readStringUntil('\n');
      for(k=0; k<FM_H; k++) {
        data = hex2byte(str[k*3], str[k*3+1]);
        SCK_FM_keyset[i][j][k] = data;
      }
    }
  }
  Serial.println(F("Fnkey Module data ok"));

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<PM_V; j++) {
      Serial.println("keyPad Module data ("+String(i+1)+"/"+String(KEY_LAYERS)+","+String(j+1)+"/"+String(PM_V)+")");
      while(!Serial.available());
      str = Serial.readStringUntil('\n');
      for(k=0; k<PM_H; k++) {
        data = hex2byte(str[k*3], str[k*3+1]);
        SCK_PM_keyset[i][j][k] = data;
      }
    }
  }
  Serial.println(F("keyPad Module data ok"));

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<MM_V+2; j++) {
      Serial.println("Macro Module data ("+String(i+1)+"/"+String(KEY_LAYERS)+","+String(j+1)+"/"+String(MM_V+2)+")");
      while(!Serial.available());
      str = Serial.readStringUntil('\n');
      for(k=0; k<MM_H; k++) {
        data = hex2byte(str[k*3], str[k*3+1]);
        SCK_MM_keyset[i][j][k] = data;
      }
    }
  }
  Serial.println("Macro Module data ok");

  Serial.println("[com] Key Setting ended!");
}
