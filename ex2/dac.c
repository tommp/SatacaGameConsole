#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void setupDAC()
{
    //Enable the DAC clock by setting bit 17 in CMU_HFPERCLKEN0
    *CMU_HFPERCLKEN0 |= (1 << 17);
    
    //Prescale DAC clock by writing 0x50010 to DAC0_CTRL
    *DAC0_CTRL = 0x050010;

    /* Why is 0x050010 written to DAC0_CTRL? 
	0x050010 = 0b1010000000000010000, the 10msb's are resvered, doing nothing, 
	and the fourth enables output, see "29.5.1 DACn_CTRL - Control Register" in the referance manual
    */
    
    //Enable left and right audio channels by writing 1 to DAC0_CH0CTRL and DAC0_CH1CTRL
    *DAC0_CH0CTRL |= DAC_CHnCTRL_EN;
    *DAC0_CH1CTRL |= DAC_CHnCTRL_EN;
    
    //Write a continuous stream of samples to the DAC data registers, DAC0_CH0DATA and DAC0_CH1DATA, for example from a timer interrupt
    //TODO
    
}

void disableDAC(){
	//Clear all settings
	*DAC0_CTRL = 0;

	//Disable left and right audioc hannels
	*DAC0_CH0CTRL = 0;
	*DAC0_CH1CTRL = 0;

	//Disable the DAC clock
    *CMU_HFPERCLKEN0 &= ~(1 << 17);
}