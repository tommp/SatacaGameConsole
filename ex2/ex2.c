#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "gpio.h"
#include "interrupt_handlers.h"
#include "timer.h"
#include "dac.h"

/* Your code will start executing here */
int main(void) {  

    /* Call the peripheral setup functions */
    gpio_setup();
    //timer_enable(SAMPLE_RATE);
    timer_LE_enable();

    /*Disable RAM*/
    *EMU_MEMCTRL = EMU_MEMCTRL_BLK123;

    /* Go to sleep */
    *SCR = 6;
    __asm("WFI");

    return 0;
}
