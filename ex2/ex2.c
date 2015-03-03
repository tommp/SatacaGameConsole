#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "gpio.h"
#include "interrupt_handlers.h"
#include "timer.h"
#include "dac.h"

/* The period between sound samples, in clock cycles */
#define   SAMPLE_RATE   5000
#define   SAMPLE_RATE2  32768

/* Declaration of peripheral setup functions */
void setupDAC();

/* Your code will start executing here */
int main(void) 
{  
  /* Call the peripheral setup functions */
  setupGPIO();
  //dac_enable();
  //timer_enable(SAMPLE_RATE);
  timer_LE_enable(SAMPLE_RATE);
 
  
  *SCR = 6;
  __asm("WFI");

  while(1);

  return 0;
}
