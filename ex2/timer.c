#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "timer.h"

#define CLOCK_FREQUENCY 14000000
#define LFCLOCK_FREQUENCY (32768/2)

/* function to setup the timer */
void timer_enable(uint16_t sample_rate)
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

void timer_LE_enable(){  
    //Enable the clock the for Low Energy Peripherals bus
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

    //Set LETimer in one shot mode
    *LETIMER0_CTRL |= LETIMER0_CTRL_REPMODE_ONESHOT_MASK;

    //Write a period to register LETIMER0_COMP0
    *LETIMER0_COMP0 = 1; 

    //Enable LETimer interrupts
    *ISER0 |= ISER0_LETIMER0_EN;

    //Enable interrupt on compare match 0
    *LETIMER0_IEN |= LETIMER0_IEN_COMP0; 

    //Set repeat counter to legal value
    *LETIMER0_REP0 |= 0x01;

    //Enable interrupt repeat
    *LETIMER0_IEN |=LETIMER0_IEN_REP0;
}

void timer_LE_start(void){
    *LETIMER0_CMD |= LETIMER0_CMD_START;
    //Select 32 768 Hz RC oscilator as clk source for LFACLKEN0
    *CMU_LFCLKSEL |= CMU_LFACLKSEL_LFRCO;
}

void timer_LE_stop(void){
    *LETIMER0_CMD |= LETIMER0_CMD_STOP;
    //Disable low frequency clock
    *CMU_LFCLKSEL = CMU_LFCLKSEL_DIS;
}

void timer_LE_set_repeat_counter(uint8_t val){
    *LETIMER0_REP0 = val;
}

void timer_LE_disable(){
    //Disable the low frequency oscilator
    *CMU_OSCENCMD &= ~(CMU_OSCENCMD_LFRCON);//TODO::fix
    
    //Select 32 768 Hz RC oscilator as clk source for LFACLKEN0
    *CMU_LFCLKSEL &= ~(CMU_LFACLKSEL_LFRCO);

    //Clear all prescale bits and set bit 8 for div2 prescale to LETIMER0
    *CMU_LFAPRESC0 &= ~(CMU_LFAPRESC0_LETIMER0_MASK);

    //Disable Low Energy clock
    *CMU_LFACLKEN0 &= ~(CMU_LFACLKEN0_LETIMER0_EN);

     //Disable the clock for Low Energy Peripherals
    *CMU_HFCORECLKEN0 &= ~(CMU_HFCORECLKEN0_LETIMER0_EN);
}

