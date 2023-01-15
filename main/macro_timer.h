#pragma once

#define TIM_DISABLE TIMSK0&=(0<<OCIE0A)
#define TIM_ENABLE TIMSK0|=(1<<OCIE0A)

/**
 * @brief timer set (1kHz = 1ms)
 * 
 */
void timer0_init(void) {
  TCCR0A = 0; //TCCR0A initialize
  TCCR0B = 0; //TCCR0B initialize
  TCNT0 = 0;  //TCNT0 initialize
  
  OCR0A= 250; 
  TCCR0B |= (1<<WGM02);
  TCCR0B |= (1<<CS01) | (1<<CS00);
  TIMSK0 |= (1<<OCIE0A);
}