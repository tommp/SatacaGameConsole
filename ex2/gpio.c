#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to set up GPIO mode and interrupts*/
void setupGPIO()
{
    /* TODO set input and output pins for the joystick */
    
    /* Example of HW access from C code: turn on joystick LEDs D4-D8
        check efm32gg.h for other useful register definitions
    */
    *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO; /* enable GPIO clock*/
    *GPIO_PA_CTRL = 2;  /* set high drive strength */
    *GPIO_PA_MODEH = 0x55555555; /* set pins A8-15 as output */
    //*GPIO_PA_DOUT = 0x0700; /* turn on LEDs D4-D8 (LEDs are active low) */

    /* Set pin 0-7 on port c as input */
    *GPIO_PC_MODEL = 0x33333333;

    /* Enable pullup resistors on input pins*/
    *GPIO_PC_DOUT = 0xffffffff; 

    /************* Enable interrupts *****************/

    /* Enable all lower external pin interrupts on port c */
    *GPIO_EXTIPSELL = 0x22222222;

    /* Enable triggering of ext.int on pin 0-8 on falling edge. */
    *GPIO_EXTIFALL = 0xf; // TODO:: check if it chould be 0xff

    /* Enable triggering of ext.int on pin 0-8 on rising edge. */
    *GPIO_EXTIRISE = 0xf; // TODO:: check if it chould be 0xff

    /* Enable external interrupt from pin 0-8 */
    *GPIO_IEN = 0xf; // TODO:: check if it chould be 0xff

    *SCR = 6; // TODO:: remove
}




