#include <stdint.h>
#include <stdbool.h>

#include "tunes.h"
#include "efm32gg.h"
#include "interrupt_handlers.h"
#include "songs.h"

volatile uint32_t counter_val = 0;

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
  /*
    TODO feed new samples to the DAC
    remember to clear the pending interrupt by writing 1 to TIMER1_IFC
  */  
	//Clear the overflow interrupt flag
	*TIMER1_IFC |= TIMER1_IFC_OF;

	counter_val++;
	*GPIO_PA_DOUT = counter_val;

	*DAC0_CH0DATA = sound0;
	*DAC0_CH1DATA = sound0;

	*TIMER1_TOP = 20000;
}

/* LETIMER0 interrupt handler */
void __attribute__ ((interrupt)) LETIMER0_IRQHandler() 
{  

    if(*LETIMER0_IF &  (1 << 3)){
        *LETIMER0_IFC |= LETIMER0_IFC_REP0;
        playCurrentAndSetNextTune(imperial_march, imperial_march_length,        &imperial_march_current_tune);
    }
    
    //TODO::PUT THIS IN A FUNCTION OR SOMETHING
    if(use_fadeout){
        if(sound0 <= 0){
            fade_direction = 1;
        }
        else if(sound0 >= soundMAX){
            fade_direction = 0;
        }
        if(fade_direction == 1) {
            sound0+=fade_inc;
        }
        else{
            sound0-=fade_inc;
        }
    }

	//Clear the overflow interrupt flag
	*LETIMER0_IFC |= LETIMER0_IFC_COMP0;
    
	counter_val++;
	*GPIO_PA_DOUT = counter_val;

    int rem = counter_val % 2;
    if (rem == 1) {
        *DAC0_CH0DATA = sound0;
        *DAC0_CH1DATA = sound0;
    }
    else {
        *DAC0_CH0DATA = 0;
        *DAC0_CH1DATA = 0;
    }


	//*LETIMER0_COMP0 = 5;
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
    
    //Start the Low Energy Timer
    *LETIMER0_CMD |= LETIMER0_CMD_START;
    *LETIMER0_REP0 |= 0x01;
    //Reset GPIO interupts
    *GPIO_IFC = 0xff;

    //Enables coresponding led on button press
    uint32_t input = *GPIO_PC_DIN;
    input = input << 8;

    *GPIO_PA_DOUT = input;
}
