//=============================================================================
// File Name: ints_switches.c
//
// Description: This file contains the interrupts for the switches
//
// Author: Joshua Swaim
// Date: Mar 2021
// Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (7.20.1)
//=============================================================================

#include "macros.h"

// Globals
unsigned int change = FALSE;
unsigned int flip = FALSE;
extern char display_line[displayRows][displayColumns];
extern volatile unsigned char display_changed;
extern volatile char usb_out_ring_data[ringSize];
extern volatile unsigned int usb_out_ring_rd_index;
extern volatile unsigned int usb_out_ring_wr_index;
extern volatile char iot_out_ring_data[ringSize];
extern volatile unsigned int iot_out_ring_rd_index;
extern volatile unsigned int iot_out_ring_wr_index;

//=============================================================================
// Function:    Switch1_Process
// Description: Interrupt for switch 1 pressing
// Arguments:   None
// Globals:     change
// Locals:      None
// Return:      None
// Pins:        SW1, RED_LED
//=============================================================================
#pragma vector = PORT4_VECTOR

__interrupt void Switch1_Process(void) {
  if(P4IFG & SW1) {
    Switches_Interrupt_Stop();
    Red_On();
    Switches_Timer_Start();
  }
}

//=============================================================================
// Function:    Switch2_Process
// Description: Interrupt for switch 2 pressing
// Arguments:   None
// Globals:     change, flip, display_line[][], display_changed
// Locals:      None
// Return:      None
// Pins:        SW2, GRN_LED
//=============================================================================
#pragma vector = PORT2_VECTOR

__interrupt void Switch2_Process(void) {
  if(P2IFG & SW2) {
    Switches_Interrupt_Stop();
    Green_On();
    Switches_Timer_Start();
  }
}