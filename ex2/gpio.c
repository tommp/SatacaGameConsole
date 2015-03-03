#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "gpio.h"

/* function to set up GPIO mode and interrupts*/
void setupGPIO(void)
{
    *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO; /* enable GPIO clock*/
    *GPIO_PA_CTRL = 2;  /* set high drive strength */
    *GPIO_PA_MODEH = 0x55555555; /* set pins A8-15 as output */

    /* Set pin 0-7 on port c as input */
    *GPIO_PC_MODEL = 0x33333333;

    /* Enable pullup resistors on input pins*/
    *GPIO_PC_DOUT = 0xffffffff; 

    /************* Enable interrupts *****************/

    /* Enable all lower external pin interrupts on port c */
    *GPIO_EXTIPSELL = 0x22222222;

    /* Enable triggering of ext.int on pin 0-8 on falling edge. */
    *GPIO_EXTIFALL = 0xff;

    /* Enable triggering of ext.int on pin 0-8 on rising edge. */
    //*GPIO_EXTIRISE = 0xff;

    /* Enable external interrupt from pin 0-8 */
    *GPIO_IEN = 0xff;
    
    /* Enable NVIC for odd and even gpio pins */
    *ISER0 |= ISER0_GPIO_EVEN_EN | ISER0_GPIO_ODD_EN;
}




