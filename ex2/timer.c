#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define CLOCK_FREQUENCY 14000000

/* function to setup the timer */
void setupTimer(uint16_t period)
{
  //Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
  
  //Write the period to register TIMER1_TOP
  *TIMER1_TOP = CLOCK_FREQUENCY/period;
  
  //Enable timer interrupt generation by writing 1 to TIMER1_IEN
  *TIMER1_IEN |= TIMER1_IEN_INT;
  
  //Start the timer by writing 1 to TIMER1_CMD
  *TIMER1_CMD |= TIMER1_CMD_START;
  
  //Enable timer interrupts
  *ISER0 |= ISER0_TIMER1_EN;
}


