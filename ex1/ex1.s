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
		  //CMU (GPIO clock)----------------------------------------------------------
		  //Load base address
	      ldr r0, =CMU_BASE

	      //Load content of [r0, #CMU_HFPERCLKEN0] into r2
	      ldr r1, [r0, #CMU_HFPERCLKEN0]

	      //Move 1 into r2, r2 =0x00000001 
	      mov r2, #1

	      //Left shift the 1 by CMU_HFPERCLKEN0_GPIO bits
	      lsl r2, r2, #CMU_HFPERCLKEN0_GPIO

	      //Use logical or to set the 13'th bit while retaining former status, enabling GPIO clock without affecting others
	      orr r1, r1, r2

	      //Store the result in memory adressed by [r0, #CMU_HFPERCLKEN0]
	      str r1, [r0, #CMU_HFPERCLKEN0]

	      //NVIC----------------------------------------------------------------------
	      //Load memory address
	      ldr r0, =ISER0

	      //Load content from address
	      ldr r1, [r0]

	      //Move wide (due to 8+4 bit space limit for values, 0x802 not representable by rotation)
	      movw r2, #0x802

	      //Use logical or to preserve untouched bits
	      orr r1, r1, r2

	      //Store the results
	      str r1, [r0]

	      //GPIO(Pin configurations) -------------------------------------------------
	      //Set high drive strength --------------------------------------------------
	      //Load base address for port A
	      ldr r0, =GPIO_PA_BASE

	      //Create port alias for future refrence
	      PA .req r0

	      //Load content
	      ldr r1, [PA, #GPIO_CTRL]

	      //Apply masking
	      orr r1, r1, #0x2

	      //Store the results
	      str r1, [PA, #GPIO_CTRL]

	      //Set port A pins 8 - 15 to output -----------------------------------------
	      //Load port A modeh content
	      ldr r1, [PA, #GPIO_MODEH]

	      //Load immediate value (representable by rotation, no need for movw and movt)
	      ldr r2, =0x55555555

	      //Or to preserve, unsure if needed, could probably just set it directly
	      orr r1, r1, r2

	      //Store the results
	      str r1, [PA, #GPIO_MODEH]


	      //Set input pins on port C -------------------------------------------------
	      //Load base address
	      ldr r1, =GPIO_PC_BASE

	      //Create port alias for future refrence
	      PC .req r0

	      //Load content
	      ldr r2, [PC, #GPIO_CTRL]

	      //Load immediate value (representable by rotation, no need for movw and movt)
	      ldr r3, =0x33333333

	      //Or to preserve, unsure if needed, could probably just set it directly
	      orr r2, r2, r3

	      //Store the results
	      str r2, [PC, #GPIO_MODEH]

	      //Enable pullup resistors on input pins -------------------------------------
	      //Load content
	      ldr r2, [PC, #GPIO_DOUT]

	      //Or to preserve
	      orr r2, r2, #0xff

	      //Store the results
	      str r2, [[PC, #GPIO_DOUT]]





	
	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:  

	      b .  // do nothing
	
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
        b .  // do nothing

