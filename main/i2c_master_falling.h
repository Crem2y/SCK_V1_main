#pragma once

#include "i2c_master_interrupt.h"

bool I2C_init_falling(void);
bool I2C_check_falling(unsigned char address);
bool I2C_read_byte_falling(unsigned char address);
bool I2C_read_data_falling(unsigned char address, unsigned char length);
bool I2C_write_byte_falling(unsigned char address);
bool I2C_write_data_falling(unsigned char address, unsigned char length);

volatile bool I2C_falling_initalized = false;           // I2C falling initalized

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
  //TWCR = 0x84; // no interrupt

  I2C_target_address = 0x00;
  I2C_bytes_addr = 0;
  I2C_bytes_size = 0;
  
  I2C_is_communicating = false;
  I2C_falling_initalized = true;

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
  I2C_reading_data[0] = 0xff;
  if(!I2C_read_byte_falling(address)) return false;

  while(I2C_is_communicating);

  Serial.println("[I2C] read data is... 0x");
  Serial.println(I2C_reading_data[0], HEX);

  if(I2C_reading_data[0] != 0xff) return true;
  
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
  if (!I2C_falling_initalized || I2C_is_communicating) return false;
  I2C_is_communicating = true;
  
  TWCR = 0xA4;            // 0b 10100100        // TWINT = 1, TWINT clear / TWSTA = 1, start condition / TWEN = 1, I2C on
  // TWINT   TWEA    TWSTA   TWSTO   TWWC    TWEN    ---     TWIE
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x08)); // START complete? (TWINT = 1 or TWS = 0x08)
  Serial.print("s");
  TWDR = (address << 1) | 0x01;                 // send address & read
  TWCR = 0x84;            // 0b 10000100        // TWINT = 1, TWINT clear
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x40)); // send complete?  (TWINT = 1 or TWS = 0x40)
  Serial.print("a");
  TWCR = 0x84;            // 0b 10000100        // TWINT = 1, TWINT clear
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x58)); // read complete?  (TWINT = 1 or TWS = 0x58)
  Serial.print("r");
  I2C_reading_data[0] = TWDR;                   // read data
  TWCR = 0x94;            // 0b 10010100        // TWINT = 1, TWINT clear / TWSTO = 1, stop condition
  Serial.print("x");
  delay(1);
  I2C_is_communicating = false;
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
  if (!I2C_falling_initalized || I2C_is_communicating) return false;
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
  if (!I2C_falling_initalized || I2C_is_communicating) return false;
  I2C_is_communicating = true;
  
  TWCR = 0xA4;            // 0b 10100100        // TWINT = 1, TWINT clear / TWSTA = 1, start condition / TWEN = 1, I2C on
  // TWINT   TWEA    TWSTA   TWSTO   TWWC    TWEN    ---     TWIE
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x08)); // START complete? (TWINT = 1 or TWS = 0x08)
    
  TWDR = (address << 1) | 0x00;                 // send address & write
  TWCR = 0x84;            // 0b 10000100        // TWINT = 1, TWINT clear
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x18)); // send complete?  (TWINT = 1 or TWS = 0x18)
    
  TWDR = I2C_writing_data[0];
  TWCR = 0x84;            // 0b 10000100        // TWINT = 1, TWINT clear
  while(((TWCR & 0x80) == 0x00) || ((TWSR & 0xF8) != 0x28)); // write complete? (TWINT = 1 or TWS = 0x28)
    
  TWCR = 0x94;            // 0b 10010100        // TWINT = 1, TWINT clear / TWSTO = 1, stop condition

  I2C_is_communicating = false;

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
  if (!I2C_falling_initalized || I2C_is_communicating) return false;
  if (length > I2C_WRITING_BYTES_MAX) return false;
  I2C_is_communicating = true;
  


  I2C_is_communicating = false;
  return true;
}