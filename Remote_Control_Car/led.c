//=============================================================================
// File Name: led.c
//
// Description: This file contains the led fucntions
//
// Author: Joshua Swaim
// Date: April 2021
// Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (7.20.1)
//=============================================================================

#include  "macros.h"

//=============================================================================
// Function:    Init_LEDs
// Description: This function sets up the LEDs
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        RED_LED, GRN_LED, IR_LED
//=============================================================================
void Init_LEDs(void){
  Red_Off();
  Green_Off();
  IR_On();
}