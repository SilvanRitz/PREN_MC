/* ###################################################################
**     Filename    : main.c
**     Project     : blink
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-10-19, 20:19, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "LED1.h"
#include "LEDpin1.h"
#include "BitIoLdd1.h"
#include "SYS1.h"
#include "RTT1.h"
#include "WAIT2.h"
#include "Cap1.h"
#include "CaptureLdd1.h"
#include "Stell_Val.h"
#include "KSDK1.h"
#include "WAIT1.h"
#include "CLS1.h"
#include "UTIL1.h"
#include "AS1.h"
#include "ASerialLdd1.h"
#include "CS1.h"
#include "AD1.h"
#include "AdcLdd1.h"
#include "LENK_SERVO2.h"
#include "Pwm2.h"
#include "PwmLdd3.h"
#include "GREIF_SERVO3.h"
#include "Pwm5.h"
#include "PwmLdd6.h"
#include "LADEN_SERVO4.h"
#include "Pwm6.h"
#include "PwmLdd7.h"
#include "ENTLADEN_SERVO5.h"
#include "Pwm7.h"
#include "PwmLdd8.h"
#include "PWM3.h"
#include "PwmLdd4.h"
#include "Bit_DC_Vor.h"
#include "BitIoLdd2.h"
#include "TU3.h"
#include "Bit_5V_2_Enable.h"
#include "BitIoLdd4.h"
#include "IR_In_Interrupt.h"
#include "ExtIntLdd1.h"
#include "Encoder_A.h"
#include "ExtIntLdd2.h"
#include "Encoder_B.h"
#include "ExtIntLdd3.h"
#include "BitIoLdd5.h"
#include "Bit_5V_1_Enable.h"
#include "BitIoLdd9.h"
#include "DebugBit.h"
#include "Bit_DC_Ruck.h"
#include "BitIoLdd3.h"
#include "TU1.h"
#include "FRTOS1.h"
#include "XF1.h"
#include "CAM_SERVO1.h"
#include "Pwm1.h"
#include "PwmLdd2.h"
#include "TRIG.h"
#include "WAIT_UltaSonic.h"
#include "TU2.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "UART_Shell.h"
#include "RTOS.h"


/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
//enum runStates_ runStates;//=DEBUG;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
   PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  //runStates=DEBUG;
  if(!CreateTasks()){
	  debugPrintf("Fehler beim erstellen der Tasks\r\n");
  }



  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
