#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "timer.h"

#define CLOCK_FREQUENCY 14000000

#define LFCLOCK_FREQUENCY (32768/2)

/* function to setup the timer */
void setupTimer(uint16_t sample_rate)
{
    //Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0
    *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;

    //Write the period to register TIMER1_TOP
    *TIMER1_TOP = CLOCK_FREQUENCY/sample_rate;

    //Enable timer interrupt generation by writing 1 to TIMER1_IEN
    *TIMER1_IEN |= TIMER1_IEN_INT;

    //Start the timer by writing 1 to TIMER1_CMD
    *TIMER1_CMD |= TIMER1_CMD_START;

    //Enable timer interrupts
    *ISER0 |= ISER0_TIMER1_EN;
}

void setupLETimer(uint16_t sample_rate){  
    //Enable the clock for Low Energy Peripherals
    *CMU_HFCORECLKEN0 |= CMU_HFCORECLKEN0_LETIMER0_EN;

    //Enable the low frequency oscilator
    *CMU_OSCENCMD |= CMU_OSCENCMD_LFRCON;
    
    //Select 32 768 Hz RC oscilator as clk source for LFACLKEN0
    *CMU_LFCLKSEL |= CMU_LFACLKSEL_LFRCO;

    //Clear all prescale bits and set bit 8 for div2 prescale to LETIMER0
    *CMU_LFAPRESC0 &= ~(CMU_LFAPRESC0_LETIMER0_MASK);
    *CMU_LFAPRESC0 |= CMU_LFAPRESC0_LETIMER0_DIV2;

    //Enable Low Energy clock
    *CMU_LFACLKEN0 |= CMU_LFACLKEN0_LETIMER0_EN;

    //Make the value in comp0 the top value
    *LETIMER0_CTRL |= LETIMER0_CTRL_COMP0TOP;

    //Set LETimer in free mode (run until stopped)
    *LETIMER0_CTRL |= LETIMER0_CTRL_REPMODE_ONESHOT_MASK;

    //Write the period to register LETIMER0_COMP0
    *LETIMER0_COMP0 = LFCLOCK_FREQUENCY/sample_rate; 

    //Enable LETimer interrupts
    *ISER0 |= ISER0_LETIMER0_EN;

    //Enable interrupt on compare match 0
    *LETIMER0_IEN |= LETIMER0_IEN_COMP0; 

    *LETIMER0_REP0 |= 0x01;

    //Enable interrupt repeat
    *LETIMER0_IEN |=LETIMER0_IEN_REP0;


}

