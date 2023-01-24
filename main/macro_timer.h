#pragma once

#define TIM_DISABLE TIMSK3 &= (0 << OCIE3A)
#define TIM_ENABLE  TIMSK3 |= (1 << OCIE3A)

/**
 * @brief timer set (1kHz = 1ms)
 * 
 */
void timer_init(void) {
  TCCR3A = 0x00;
  TCCR3B = 0x0A;
  TCNT3  = 0;
  OCR3A  = 999;

  TIFR3  = 0x02;
  TIMSK3 = 0x02;
}