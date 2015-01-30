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
		  //CMU enable GPIO clock
		  //Load base address
	      ldr r0, =CMU_BASE

	      //Load content of [r0, #CMU_HFPERCLKEN0] into r1
	      ldr r1, [r0, #CMU_HFPERCLKEN0]

	      //Move 1 into r2, r2 =0x00000001 
	      mov r2, #1

	      //Left shift the 1 by CMU_HFPERCLKEN0_GPIO bits
	      lsl r2, r2, #CMU_HFPERCLKEN0_GPIO

	      //Use logical or to set the 13th bit while retaining former status, enabling GPIO clock without affecting others
	      orr r1, r1, r2

	      //Store the result in memory adressed by [r0, #CMU_HFPERCLKEN0]
	      str r1, [r0, #CMU_HFPERCLKEN0]

	      //NVIC
	      ldr r0, =ISER0
	      ldr r1, [r0]
	      //Move wide (due to 8+4 bit space limit for values, 0x802 not representable by rotation)
	      movw r2, #0x802
	      orr r1, r1, r2
	      str r1, [r0]

	      //GPIO(Pin configurations) set high drive strength
	      ldr r0, =GPIO_PA_BASE
	      PortA .req r0 //Create port alias for future refrence
	      ldr r1, [PortA, #GPIO_CTRL]
	      orr r1, r1, #0x2
	      str r1, [PortA, #GPIO_CTRL]

	      //Set port A pins 8 - 15 to output
	      //Load immediate value (representable by rotation, no need for movw and movt)
	      ldr r1, =0x55555555
	      str r1, [PortA, #GPIO_MODEH]

	      //Set input pins on port C
	      ldr r1, =GPIO_PC_BASE
	      PortC .req r1
	      ldr r2, =0x33333333
	      str r2, [PortC, #GPIO_MODEH]

	      //Enable pullup resistors on input pins
	      ldr r2, [PortC, #GPIO_DOUT]
	      orr r2, r2, #0xff
	      str r2, [PortC, #GPIO_DOUT]
        
              b buttons_loop

              .thumb_func
buttons_loop:
              ldr r4, [PortC, #GPIO_DIN]
              lsl r4, r4, #8 //Left shift input 8bits
	      mov r5, #0
	      eor r4, r4, r5
	      str r4, [PortA, #GPIO_DOUT] //write back to lights
	
              b buttons_loop

	
	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:  
        b . // do nothing
	
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
       b .  // do nothing
