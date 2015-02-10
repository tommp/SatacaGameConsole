#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void setupDAC()
{
    //Enable the DAC clock by setting bit 17 in CMU_HFPERCLKEN0
    *CMU_HFPERCLKEN0 |= (1 << 17);
    
    //Prescale DAC clock by writing 0x50010 to DAC0_CTRL
    *DAC0_CTRL = 0x050010;
    
    //Enable left and right audio channels by writing 1 to DAC0_CH0CTRL and DAC0_CH1CTRL
    *DAC0_CH0CTRL |= DAC_CHnCTRL_EN;
    *DAC0_CH1CTRL |= DAC_CHnCTRL_EN;
    
    //Write a continuous stream of samples to the DAC data registers, DAC0_CH0DATA and DAC0_CH1DATA, for example from a timer interrupt
    //TODO
    
}