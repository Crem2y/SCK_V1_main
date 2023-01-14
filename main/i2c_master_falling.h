#pragma once

#include "i2c_master_interrupt.h"

bool I2C_init_falling(void);
bool I2C_check_falling(unsigned char address);
bool I2C_read_byte_falling(unsigned char address);
bool I2C_read_data_falling(unsigned char address, unsigned char length);
bool I2C_write_byte_falling(unsigned char address);
bool I2C_write_data_falling(unsigned char address, unsigned char length);

/**
 * @brief initalize I2C port to master.
 *        clock : 400kHz
 * @return true
 * @return false 
 */
bool I2C_init_falling(void) {
  if (I2C_is_initalized) return false;

  TWBR = 12;
  TWSR = 0x00;
  TWCR = 0x84; // no interrupt

  I2C_target_address = 0x00;
  I2C_bytes_addr = 0;
  I2C_bytes_size = 0;
  
  I2C_is_communicating = false;
  I2C_is_initalized = true;

  return true;
}

/**
 * @brief Check the slave of the entered address. Pause until a response is received or a certain number of times are reached.
 * 
 * @param address I2C slave address
 * @return true 
 * @return false 
 */
bool I2C_check_falling(unsigned char address) {
  if (!I2C_is_initalized || I2C_is_communicating) return false;
  I2C_is_communicating = true;
  
  unsigned char count;
  unsigned char err_code = 0x00;

  TWCR = 0xA4; // START condition
  count = 0;
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x08)) // START complete ?
    { if(count >= 10) {
      err_code = 0x11; // wait ACK with 100us time limit
      break;
      }
      count++;
      delayMicroseconds(10);
    }

  TWDR = (address << 1) | 1; // SLA+R
  TWCR = 0x84;
  count = 0;
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x40)) // SLA+R complete ?
    { if(count >= 10) {
      err_code = 0x12; // wait ACK with 100us time limit
      break;
      }
      count++;
      delayMicroseconds(10);
    }

  
  TWCR = 0x84; // read data with no acknowledge
  count = 0;
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x58)) // data complete ?
    { if(count >= 10) {
      err_code = 0x13; // wait ACK with 100us time limit
      break;
      }
      count++;
      delayMicroseconds(10);
    }
  I2C_reading_data[0] = TWDR; // read data

  TWCR = 0x94; // STOP condition

  delay(5); // delay 5 ms for twr time

  I2C_is_communicating = false;
  
  switch (err_code) {
    case 0x00:
      return true;
    break;
    case 0x11:
      Serial.println("[ERR] Write error at START condition.");
    break;
    case 0x12:
      Serial.println("[ERR] Write error at SLA+R.");
    break;
    case 0x13:
      Serial.println("[ERR] Read error at read data.");
    break;
  }
  return false;
}

/**
 * @brief Reads the byte of the entered address.
 * 
 * @param address I2C slave address
 * @return true 
 * @return false 
 */
bool I2C_read_byte_falling(unsigned char address) {
  if (!I2C_is_initalized || I2C_is_communicating) return false;
  I2C_is_communicating = true;
  
  unsigned char count;
  unsigned char err_code = 0x00;

  TWCR = 0xA4; // START condition
  count = 0;
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x08)) // START complete ?
    { if(count >= 10) {
      err_code = 0x11; // wait ACK with 100us time limit
      break;
      }
      count++;
      delayMicroseconds(10);
    }

  TWDR = (address << 1) | 1; // SLA+R
  TWCR = 0x84;
  count = 0;
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x40)) // SLA+R complete ?
    { if(count >= 10) {
      err_code = 0x12; // wait ACK with 100us time limit
      break;
      }
      count++;
      delayMicroseconds(10);
    }

  
  TWCR = 0x84; // read data with no acknowledge
  count = 0;
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x58)) // data complete ?
    { if(count >= 10) {
      err_code = 0x13; // wait ACK with 100us time limit
      break;
      }
      count++;
      delayMicroseconds(10);
    }
  I2C_reading_data[0] = TWDR; // read data

  TWCR = 0x94; // STOP condition

  delay(5); // delay 5 ms for twr time

  I2C_is_communicating = false;
  
  switch (err_code) {
    case 0x00:
      return true;
    break;
    case 0x11:
      Serial.println("[ERR] Write error at START condition.");
    break;
    case 0x12:
      Serial.println("[ERR] Write error at SLA+R.");
    break;
    case 0x13:
      Serial.println("[ERR] Read error at read data.");
    break;
  }
  return true;
}

/**
 * @brief Reads the bytes of the entered address.
 * 
 * @param address I2C slave address
 * @param length data length
 * @return true 
 * @return false 
 */
bool I2C_read_data_falling(unsigned char address, unsigned char length) {
  if (!I2C_is_initalized || I2C_is_communicating) return false;
  if (length > I2C_READING_BYTES_MAX) return false;
  I2C_is_communicating = true;
  


  I2C_is_communicating = false;
  return true;
}

/**
 * @brief Writes the byte of the entered address.
 * 
 * @param address I2C slave address
 * @return true 
 * @return false 
 */
bool I2C_write_byte_falling(unsigned char address) {
  if (!I2C_is_initalized || I2C_is_communicating) return false;
  I2C_is_communicating = true;
  
  unsigned char count;
  unsigned char err_code = 0x00;

  TWCR = 0xA4; // START condition
  count = 0;
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x08)) // START complete ?
    { if(count >= 10) {
      err_code = 0x11; // wait ACK with 100us time limit
      break;
      }
      count++;
      delayMicroseconds(10);
    }

  TWDR = (address << 1) | 0; // SLA+W
  TWCR = 0x84;
  count = 0;
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x18)) // SLA+W complete ?
    { if(count >= 10) {
      err_code = 0x12; // wait ACK with 100us time limit
      break;
      }
      count++;
      delayMicroseconds(10);
    }

  TWDR = I2C_writing_data[0]; // write data
  TWCR = 0x84;
  count = 0;
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x28)) // data complete ?
    { if(count >= 10) {
      err_code = 0x13; // wait ACK with 100us time limit
      break;
      }
      count++;
      delayMicroseconds(10);
    }
  TWCR = 0x94; // STOP condition

  delay(5); // delay 5 ms for twr time

  I2C_is_communicating = false;
  
  switch (err_code) {
    case 0x00:
    break;
    case 0x11:
      Serial.println("[ERR] Write error at START condition.");
    break;
    case 0x12:
      Serial.println("[ERR] Write error at SLA+W.");
    break;
    case 0x13:
      Serial.println("[ERR] Write error at write data.");
    break;
  }
  return true;
}

/**
 * @brief Writes the bytes of the entered address.
 * 
 * @param address I2C slave address
 * @param length data length
 * @return true 
 * @return false 
 */
bool I2C_write_data_falling(unsigned char address, unsigned char length) {
  if (!I2C_is_initalized || I2C_is_communicating) return false;
  if (length > I2C_WRITING_BYTES_MAX) return false;
  I2C_is_communicating = true;
  


  I2C_is_communicating = false;
  return true;
}