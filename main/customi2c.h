#include "i2c_status_code.h"

#define I2C_READING_BYTES_MAX 32          // I2C max reading bytes (1 ~ 255)
#define I2C_WRITING_BYTES_MAX 32          // I2C max writing bytes (1 ~ 255)

volatile unsigned char I2C_target_address = 0x00;  // I2C target slave address
volatile unsigned char I2C_mode = 0;               // I2C mode (0 : write , 1 : read)
volatile unsigned char I2C_bytes_addr = 0;         // I2C bytes address (0 ~ I2C_READING_BYTES_MAX or I2C_WRITING_BYTES_MAX)
volatile unsigned char I2C_bytes_size = 0;         // I2C bytes size (1 ~ I2C_READING_BYTES_MAX or I2C_WRITING_BYTES_MAX)
volatile unsigned char I2C_err_count = 0;          // I2C error count

volatile unsigned char I2C_reading_data[I2C_READING_BYTES_MAX] = {0,};  // I2C reading data
volatile unsigned char I2C_writing_data[I2C_WRITING_BYTES_MAX] = {0,};  // I2C writing data

volatile bool I2C_is_initalized = false;           // I2C is initalized
volatile bool I2C_is_communicating = false;        // I2C is communicating

bool I2C_read_byte(unsigned char address);

/**
 * @brief AVR Interrupt Service Routine (TWI)
 * 
 */
ISR(TWI_vect) {

  switch(I2C_SC) {
    case I2C_SC_MT_SC:
      TWDR = (I2C_target_address << 1) | I2C_mode;
      TWCR = 0x84; // clear TWINT, clear TWSTA
    break;
    case I2C_SC_MT_RSC:
      TWCR = 0x84; // clear TWINT
      // repeated start unused
    break;
    case I2C_SC_MT_SWA:
      I2C_bytes_addr = 0;
      TWDR = I2C_writing_data[I2C_bytes_addr];
      TWCR = 0x84; // clear TWINT
    break;
    case I2C_SC_MT_SWN:
      TWCR = 0x94; // clear TWINT, STOP Condition
      I2C_err_count++;
      I2C_is_communicating = false;
    break;
    case I2C_SC_MT_DBA:
      I2C_bytes_addr++;
      if(I2C_bytes_addr == I2C_bytes_size) { // if it was last byte
        TWCR = 0x94; // clear TWINT, STOP Condition
        I2C_is_communicating = false;
      } else {
        TWDR = I2C_writing_data[I2C_bytes_addr];
        TWCR = 0x84; // clear TWINT
      }
    break;
    case I2C_SC_MT_DBN:
      TWCR = 0x94; // clear TWINT, STOP Condition
      I2C_is_communicating = false;
    break;
    case I2C_SC_MT_AL:
      TWCR = 0x84; // clear TWINT
      // Arbitration unused
    break;
    case I2C_SC_MR_SRA:
      I2C_bytes_addr = 0;
      if(I2C_bytes_size == 1) { // if next is last byte
        TWCR = 0x84; // clear TWINT, NOT ACK
      } else {
        TWCR = 0xC4; // clear TWINT, ACK
      }
    break;
    case I2C_SC_MR_SRN:
      TWCR = 0x94; // clear TWINT, STOP Condition
      I2C_err_count++;
      I2C_is_communicating = false;
    break;
    case I2C_SC_MR_DBA:
      I2C_bytes_addr++;
      I2C_reading_data[I2C_bytes_addr - 1] = TWDR;
      if(I2C_bytes_addr == I2C_bytes_size) { // if it was last byte
        TWCR |= 0x10; // STOP Condition
      } else if (I2C_bytes_addr + 1 == I2C_bytes_size) { // if next is last byte
        TWCR = 0x84; // clear TWINT, NOT ACK
      } else {
        TWCR = 0xC4; // clear TWINT, ACK
      }
    break;
    case I2C_SC_MR_DBN:
      I2C_reading_data[I2C_bytes_addr - 1] = TWDR;
      TWCR = 0x94; // clear TWINT, STOP Condition
      I2C_is_communicating = false;
    break;
    default: // error!
      TWCR = 0x84; // clear TWINT
      I2C_err_count++;
    break;
  }
}

/**
 * @brief initalize I2C port to master, call sei() to use I2C.
 *        clock : 400kHz
 * @return true
 * @return false 
 */
bool I2C_init(void) {
  if (I2C_is_initalized) return false;

  TWBR = 12;
  TWSR = 0x00;
  TWCR = 0x85;  // I2C enable, interrupt enable

  I2C_target_address = 0x00;
  I2C_bytes_addr = 0;
  I2C_bytes_size = 0;
  
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
  if (!I2C_is_initalized || I2C_is_communicating) return false;

  TWCR = 0x00;
  TWBR = 0x00;
  TWSR = 0x00;
  
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
  I2C_reading_data[0] = 0xFF;
  if(!I2C_read_byte(address)) return false;

  while(I2C_is_communicating);

  Serial.println(I2C_reading_data[0], HEX);

  if(I2C_err_count == 0) return true;

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
  if (!I2C_is_initalized || I2C_is_communicating) return false;

  I2C_target_address = address;
  I2C_mode = 1;
  I2C_bytes_size = 1;
  I2C_err_count = 0;
  I2C_is_communicating = true;

  TWCR = 0xA4; // clear TWINT & START Condition

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
  if (!I2C_is_initalized || I2C_is_communicating) return false;
  if (length > I2C_READING_BYTES_MAX) return false;

  I2C_target_address = address;
  I2C_mode = 1;
  I2C_bytes_size = length;
  I2C_err_count = 0;
  I2C_is_communicating = true;

  TWCR = 0xA4; // clear TWINT & START Condition

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
  if (!I2C_is_initalized || I2C_is_communicating) return false;

  I2C_target_address = address;
  I2C_mode = 0;
  I2C_bytes_size = 1;
  I2C_err_count = 0;
  I2C_is_communicating = true;
  
  TWCR = 0xA4; // clear TWINT & START Condition

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
  if (!I2C_is_initalized || I2C_is_communicating) return false;
  if (length > I2C_WRITING_BYTES_MAX) return false;

  I2C_target_address = address;
  I2C_mode = 0;
  I2C_bytes_size = length;
  I2C_err_count = 0;
  I2C_is_communicating = true;
  
  TWCR = 0xA4; // clear TWINT & START Condition

  return true;
}

/* -------- falling function -------- */

/**
 * @brief initalize I2C port to master, call sei() to use I2C.
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