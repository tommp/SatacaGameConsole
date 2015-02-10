#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "interrupt_handlers.h"

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
  /*
    TODO feed new samples to the DAC
    remember to clear the pending interrupt by writing 1 to TIMER1_IFC
  */  
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
	gpio_handler();
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
	gpio_handler();
}

void gpio_handler(void)
{
    //Reset GPIO interupts
    *GPIO_IFC = 0xff;

    //Enables coresponding led on button press
    uint32_t input = *GPIO_PC_DIN;
    input = input << 8;

    *GPIO_PA_DOUT = input;
}
