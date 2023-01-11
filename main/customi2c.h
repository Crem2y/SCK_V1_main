/**
 * @file customi2c.h
 * @author Crem2y
 * @brief AVR I2C communication code. it may works with ATmega serizes that have HW TWI ports.
 * @version 0.1
 * @date 2023-01-11
 * 
 * @copyright free
 * 
 */
#pragma once

#define I2C_ERR_COUNT_MAX 10              // I2C max error count (1 ~ 255)
#define I2C_READING_BYTES_MAX 255         // I2C max reading bytes (1 ~ 255)
#define I2C_WRITING_BYTES_MAX 255         // I2C max writing bytes (1 ~ 255)

unsigned char I2C_target_address = 0x00;  // I2C target slave address
unsigned char I2C_remain_bytes = 0;       // I2C remain bytes
unsigned char I2C_read_count = 0;         // I2C reading bytes count
unsigned char I2C_write_count = 0;        // I2C writing bytes count
unsigned char I2C_err_count = 0;          // I2C error count

unsigned char I2C_reading_data[I2C_READING_BYTES_MAX] {0,};  // I2C reading data
unsigned char I2C_writing_data[I2C_WRITING_BYTES_MAX] {0,};  // I2C writing data

bool I2C_is_initalized = false;           // I2C is initalized
bool I2C_is_communicating = false;        // I2C is communicating

/**
 * @brief AVR Interrupt Service Routine (TWI)
 * 
 */
ISR(TWI_vect) {
  if(!I2C_is_communicating) { // if not communicating, it's error
    TWCR |= 0x80; //clear TWINT
    return;
  }
  
  switch(TWSR & 0xF8) {
    case 0x00:
      
    break;
  }

  TWCR |= 0x80; // clear TWINT

  if(I2C_remain_bytes == 0) {
    I2C_is_communicating = false;
  }
}

/**
 * @brief initalize I2C port, call sei() to use I2C.
 * 
 * @return true
 * @return false 
 */
bool I2C_init(void) {
  if (I2C_is_initalized) return false;

  I2C_target_address = 0x00;
  I2C_remain_bytes = 0;
  I2C_read_count = 0;
  I2C_write_count = 0;
  I2C_err_count = 0;
  
  I2C_is_communicating = false;
  I2C_is_initalized = true;

  return true;
}

/**
 * @brief deinitalize I2C port
 * 
 * @return true 
 * @return false 
 */
bool I2C_deinit(void) {
  if ((!I2C_is_initalized || I2C_is_communicating)) return false;

  
  I2C_is_initalized = false;
  return true;
}

bool I2C_data_clear(void) {
  if (I2C_is_communicating) return false;

  for(unsigned char i; i<I2C_READING_BYTES_MAX; i++) {
    I2C_reading_data[i] = 0;
  }
  for(unsigned char i; i<I2C_WRITING_BYTES_MAX; i++) {
    I2C_writing_data[i] = 0;
  }

  return true;
}

/**
 * @brief Check the slave of the entered address. Pause until a response is received or a certain number of times are reached.
 * 
 * @param address I2C slave address
 * @return true 
 * @return false 
 */
bool I2C_check(unsigned char address) {
  if (!I2C_is_initalized && I2C_is_communicating) return false;

  I2C_target_address = address;
  I2C_write_count = 1;
  I2C_err_count = 0;
  I2C_is_communicating = true;

  // communication code

  while(I2C_is_communicating);

  if(I2C_err_count < I2C_ERR_COUNT_MAX) return true;

  return false;
}

/**
 * @brief Reads the byte of the entered address.
 * 
 * @param address I2C slave address
 * @return true 
 * @return false 
 */
bool I2C_read_byte(unsigned char address) {
  if (!I2C_is_initalized && I2C_is_communicating) return false;

  I2C_target_address = address;
  I2C_write_count = 1;
  I2C_err_count = 0;
  I2C_is_communicating = true;
  // communication code

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
bool I2C_read_data(unsigned char address, unsigned char length) {
  if (!I2C_is_initalized && I2C_is_communicating) return false;
  if (length > I2C_READING_BYTES_MAX) return false;

  I2C_target_address = address;
  I2C_write_count = length;
  I2C_err_count = 0;
  I2C_is_communicating = true;
  // communication code

  return true;
}

/**
 * @brief Writes the byte of the entered address.
 * 
 * @param address I2C slave address
 * @return true 
 * @return false 
 */
bool I2C_write_byte(unsigned char address) {
  if (!I2C_is_initalized && I2C_is_communicating) return false;

  I2C_target_address = address;
  I2C_write_count = 1;
  I2C_err_count = 0;
  I2C_is_communicating = true;
  // communication code

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
bool I2C_write_data(unsigned char address, unsigned char length) {
  if (!I2C_is_initalized && I2C_is_communicating) return false;
  if (length > I2C_WRITING_BYTES_MAX) return false;

  I2C_target_address = address;
  I2C_write_count = length;
  I2C_err_count = 0;
  I2C_is_communicating = true;
  // communication code

  return true;
}




