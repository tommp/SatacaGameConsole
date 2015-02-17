#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define CLOCK_FREQUENCY 14000000

#define LFCLOCK_FREQUENCY 32768

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

void setupLETimer(uint16_t period){  
    //Enable the low frequency oscilator
    *CMU_OSCENCMD |= CMU_OSCENCMD_LFRCON;
    
    //Select 32 768 Hz RC oscilator as clk source for LFACLKEN0
    *CMU_LFCLKSEL |= CMU_LFACLKSEL_LFRCO;

    //Clear all prescale bits for div1 prescale to LETIMER0
    *CMU_LFAPRESC0 &= ~(CMU_LFAPRESC0_LETIMER0_MASK);

    //Enable Low Energy clock
    *CMU_LFACLKEN0 |= CMU_LFACLKEN0_LETIMER0_EN;

    //Make the value in comp0 the top value (clear counter)
    *LETIMER0_CTRL |= LETIMER0_CTRL_COMP0TOP;

    //Set LETimer in free mode (run until stopped)
    *LETIMER0_CTRL |= LETIMER0_CTRL_REPMODE_FREE_MASK;

    //Enable LETimer interrupts
    *ISER0 |= ISER0_LETIMER0_EN;

    //Enable interrupt on compare match 0
    *LETIMER0_IEN |= LETIMER0_IEN_COMP0; 

    //Start the Low Energy Timer
    *LETIMER0_CMD |= LETIMER0_CMD_START;

    //Set Top value
    *LETIMER0_COMP0 = LFCLOCK_FREQUENCY/period;
}

