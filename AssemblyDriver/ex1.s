    .syntax unified
    
    .include "efm32gg.s"

    /////////////////////////////////////////////////////////////////////////////
    //
    // Exception vector table
    // This table contains addresses for all exception handlers
    //
    /////////////////////////////////////////////////////////////////////////////
    
    .section .vectors
    
        .long   stack_top               /* Top of Stack                 */
        .long   _reset                  /* Reset Handler                */
        .long   dummy_handler           /* NMI Handler                  */
        .long   dummy_handler           /* Hard Fault Handler           */
        .long   dummy_handler           /* MPU Fault Handler            */
        .long   dummy_handler           /* Bus Fault Handler            */
        .long   dummy_handler           /* Usage Fault Handler          */
        .long   dummy_handler           /* Reserved                     */
        .long   dummy_handler           /* Reserved                     */
        .long   dummy_handler           /* Reserved                     */
        .long   dummy_handler           /* Reserved                     */
        .long   dummy_handler           /* SVCall Handler               */
        .long   dummy_handler           /* Debug Monitor Handler        */
        .long   dummy_handler           /* Reserved                     */
        .long   dummy_handler           /* PendSV Handler               */
        .long   dummy_handler           /* SysTick Handler              */

        /* External Interrupts */
        .long   dummy_handler
        .long   gpio_handler            /* GPIO even handler */
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   gpio_handler            /* GPIO odd handler */
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler
        .long   dummy_handler

    .section .text

    /////////////////////////////////////////////////////////////////////////////
    //
    // Reset handler
    // The CPU will start executing here after a reset
    //
    /////////////////////////////////////////////////////////////////////////////

.globl  _reset
.type   _reset, %function
.thumb_func
_reset: 

/////////////////////////////// GPIO Clock setup ///////////////////////////////
    //CMU enable GPIO clock
    //Load base address
    ldr r0, =CMU_BASE

    //Load content of [r0, #CMU_HFPERCLKEN0] into r2
    ldr r1, [r0, #CMU_HFPERCLKEN0]

    //Move 1 into r2, r2 =0x00000001 
    mov r2, #1

    //Left shift the 1 by CMU_HFPERCLKEN0_GPIO bits
    lsl r2, r2, #CMU_HFPERCLKEN0_GPIO

    //Use logical or to set the 13th bit while retaining former status, enabling GPIO clock without affecting others
    orr r1, r1, r2

    //Store the result in memory adressed by [r0, #CMU_HFPERCLKEN0]
    str r1, [r0, #CMU_HFPERCLKEN0]

//////////////////////// Energy efficient measures //////////////////////////

    //Set Energy Mode 3, as this is the cheapest that still supports external pin interrupts
    ldr r0, =EMU_BASE
    mov r1, #0
    str r1, [r0, #EMU_CTRL]

    //Enable Deepsleep (EM2)
    ldr r2, =SCR
    mov r3, #0x6
    str r3, [r2]

    //Turn off low energy periferals clock LFACLK/LFBCLK to enter EM3
    mov r1, #0
    //str r1, [r0, #CMU_LFCLKSEL]
    
    //Disable ram block 1-3 as we only need block 0.
    ldr r1, =EMU_BASE
    mov r0, #7
    str r0, [r1, #EMU_MEMCTRL]
    
//////////////////////////// Set up Leds and Buttons /////////////////////////////
    //Configure Nested Vectored Interrupt Controller
    ldr r0, =ISER0
    //Move wide (due to 8+4 bit space limit for values, 0x802 not representable by rotation)
    movw r1, #0x802
    str r1, [r0]

    //GPIO(Pin configurations) set high drive strength
    ldr r0, =GPIO_PA_BASE
    PortA .req r0 //Create port alias for future refrence
    mov r1, #0x2
    str r1, [PortA, #GPIO_CTRL]

    //Set port A pins 8 - 15 to output
    //Load immediate value (representable by rotation, no need for movw and movt)
    mov r1, #0x55555555
    str r1, [PortA, #GPIO_MODEH]

    //Set input pins on port C
    ldr r1, =GPIO_PC_BASE
    PortC .req r1
    mov r2, #0x33333333
    str r2, [PortC, #GPIO_MODEL]

    //Enable pullup resistors on input pins
    mov r2, #0xff
    str r2, [PortC, #GPIO_DOUT]

//////////////////////////// Set up interrupts ////////////////////////////////

    //Enable interrupts
    ldr r2, =GPIO_BASE
    BaseMain .req r2
    mov r3, #0x22222222
    str r3, [BaseMain, #GPIO_EXTIPSELL]

    //Set interrupt on 0 -> 1 transition
    mov r3, #0xff
    str r3, [BaseMain, GPIO_EXTIRISE]

    //Set interrupt on 1 -> 0 transition
    str r3, [BaseMain, GPIO_EXTIFALL]

    //Enable interrupt generation
    str r3, [BaseMain, GPIO_IEN]

    //Enter deep sleep (wait for interrupt)
    wfi

    /////////////////////////////////////////////////////////////////////////////
    //
    // GPIO handler
    // The CPU will jump here when there is a GPIO interrupt
    //
    /////////////////////////////////////////////////////////////////////////////
    
    .thumb_func
gpio_handler:  
        //Clear interrupts
        mov r3, #0xff
        str r3, [BaseMain, #GPIO_IFC]

        //Read input pins
        ldr r3, [PortC, #GPIO_DIN]

        //Left shift and set corresponding leds
        lsl r3, #8
        str r3, [PortA, #GPIO_DOUT]

        //Branch to link register
        bx lr
    
    /////////////////////////////////////////////////////////////////////////////
    
        .thumb_func
dummy_handler:  
        bx lr

