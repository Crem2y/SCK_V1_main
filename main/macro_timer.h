#pragma once

#define TIM_DISABLE TIMSK0&=(0<<OCIE0A)
#define TIM_ENABLE TIMSK0|=(1<<OCIE0A)

/**
 * @brief timer set (1kHz = 1ms)
 * 
 */
void macro_init(void) {
  TCCR3A = 0x00;
  TCCR3B = 0x0A;
  TCNT3 = 0;
  OCR3A = 999;

  TIFR3 = 0x02;
  TIMSK3 = 0x02;
}