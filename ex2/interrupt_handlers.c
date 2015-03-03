#include <stdint.h>
#include <stdbool.h>

#include "tunes.h"
#include "efm32gg.h"
#include "interrupt_handlers.h"
#include "songs.h"
#include "dac.h"
#include "timer.h"

volatile uint32_t counter_val = 0;

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
	//Clear the overflow interrupt flag
	*TIMER1_IFC |= TIMER1_IFC_OF;

    // Show button pushes on leds
	counter_val++;
	*GPIO_PA_DOUT = counter_val;

    // Feed signals to the DAC
	*DAC0_CH0DATA = sound0;
	*DAC0_CH1DATA = sound0;

	*TIMER1_TOP = 20000;
}

/* LETIMER0 interrupt handler */
void __attribute__ ((interrupt)) LETIMER0_IRQHandler() 
{  
    // Check for rep0 underflow
    if(*LETIMER0_IF &  LETIMER0_IF_REP0){
        // Clear interrupt flag
        *LETIMER0_IFC |= LETIMER0_IFC_REP0;

        switch(song_to_use){
            case 0:
                tunes_play_next_note(wallhit, 
                        wallhit_length, 
                        &wallhit_current_note);
                use_fadeout = use_wallhit_fadeout;
                break;
            case 1:
                tunes_play_next_note(shoot, 
                        shoot_length, 
                        &shoot_current_note);
                use_fadeout = use_shoot_fadeout;
                break;
            case 2:
                tunes_play_next_note(zeldas_lullaby, 
                        zeldas_lullaby_length, 
                        &zeldas_lullaby_current_note);
                use_fadeout = use_zeldas_lullaby_fadeout;
                break;
            case 3:
                tunes_play_next_note(imperial_march, 
                        imperial_march_length, 
                        &imperial_march_current_note);
                use_fadeout = use_imperial_march_fadeout;
                break;
            default:   
                break;
        }
    }
    set_fadeout(use_fadeout);

	//Clear the overflow interrupt flag
	*LETIMER0_IFC |= LETIMER0_IFC_COMP0;
    
	counter_val++;
	//*GPIO_PA_DOUT = counter_val;

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
    // Handle button pressed event
	gpio_handler();
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
    // Handle button pressed event
	gpio_handler();
}

void gpio_handler(void)
{
    
    // Start the Low Energy Timer
    timer_LE_start();
    timer_LE_set_repeat_counter(1);

    // Reset GPIO interupts
    *GPIO_IFC = 0xff;

    // Enables coresponding led on button press
    uint32_t input = *GPIO_PC_DIN;


    if(!(*GPIO_PC_DIN & (1 << 0))){
        song_to_use = 0;
    }else if(!(*GPIO_PC_DIN & (1 << 1))){
        song_to_use = 1;
    }else if(!(*GPIO_PC_DIN & (1 << 2))){
        song_to_use = 2;
    }else if(!(*GPIO_PC_DIN & (1 << 3))){
        song_to_use = 3;
    }else if(!(*GPIO_PC_DIN & (1 << 4))){
        song_to_use = 4;
    }else if(!(*GPIO_PC_DIN & (1 << 5))){
        song_to_use = 5;
    }else if(!(*GPIO_PC_DIN & (1 << 6))){
        song_to_use = 6;
    }else if(!(*GPIO_PC_DIN & (1 << 7))){
        song_to_use = 7;
    }


    input = input << 8;

    *GPIO_PA_DOUT = input;
}
