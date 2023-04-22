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

void print_hex(byte data, bool next_line) {
  if(data < 16) {
    Serial.print('0');
  }
  Serial.print(data, HEX);

  if(next_line) {
    Serial.println();
  }
}

char hex2byte(char highdata, char lowdata) { // '0' ~ 'F' to 0x00 ~ 0x0F
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
 * @param str String
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

  Serial.println("Saving to EEPROM...");

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

  Serial.println("[com] Save complete!");
  Serial.println("[com] "+String(address)+" of 1024 bytes used");
}

/**
 * @brief load 'keySets' data from EEPROM
 */
void eepromLoad(void) {
  unsigned int address = 0;
  byte i,j,k;
  byte data;
  
  Serial.println("Loading from EEPROM...");

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

  Serial.println("Printing data...");

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

  Serial.println("[com] Print complete!");
}

/**
 * @brief get data from serial and save to 'keysets'
 */
void setKey(void) {
  byte i,j,k;
  byte data;
  String str;

  Serial.println("Setting mode...");

  while(!Serial.available());
  str = Serial.readStringUntil('\n');

  data = hex2byte(str[0], str[1]);
  Neo.key.mode = data;
  data = hex2byte(str[2], str[3]);
  Neo.side.mode = data;

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<KM_V; j++) {
      while(!Serial.available());
      str = Serial.readStringUntil('\n');

      for(k=0; k<KM_H; k++) {
        data = hex2byte(str[k*2], str[k*2+1]);
        SCK_KM_keyset[i][j][k] = data;
      }
    }
  }

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<FM_V; j++) {
      while(!Serial.available());
      str = Serial.readStringUntil('\n');

      for(k=0; k<FM_H; k++) {
        data = hex2byte(str[k*2], str[k*2+1]);
        SCK_FM_keyset[i][j][k] = data;
      }
    }
  }
  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<PM_V; j++) {
      while(!Serial.available());
      str = Serial.readStringUntil('\n');

      for(k=0; k<PM_H; k++) {
        data = hex2byte(str[k*2], str[k*2+1]);
        SCK_PM_keyset[i][j][k] = data;
      }
    }
  }

  for(i=0; i<KEY_LAYERS; i++) {
    for(j=0; j<MM_V+2; j++) {
      while(!Serial.available());
      str = Serial.readStringUntil('\n');

      for(k=0; k<MM_H; k++) {
        data = hex2byte(str[k*2], str[k*2+1]);
        SCK_MM_keyset[i][j][k] = data;
      }
    }
  }

  Serial.println("[com] Key Setting ended!");
}
