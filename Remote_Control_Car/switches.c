//=============================================================================
// File Name: switches.c
//
// Description: This file includes the functions for handling switch presses
//
// Author: Joshua Swaim
// Date: Mar 2021
// Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (7.20.1)
//=============================================================================

#include "macros.h"

//=============================================================================
// Function:    Switches_Timer_Start
// Description: Starts debounce timer interrupt
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Switches_Timer_Start(void) {
    TB0CCTL1 &= ~TBIFG;                       // Clearing interrupt flags
    TB0CCR1 = TB0R + TB0CCR1_INTERVAL;        // Setting the interval
    TB0CCTL1 |= CCIE;                         // Enabling the interrupt
}

//=============================================================================
// Function:    Switches_Timer_Stop
// Description: Stops the debounce timer interrupt
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Switches_Timer_Stop(void) {
    TB0CCTL1 &= ~CCIE;                          // Disabling the interrupt
    TB0CCTL1 &= ~TBIFG;                         // Clearing interrupt flags
}

//=============================================================================
// Function:    Switches_Interrupt_Start
// Description: Starts the switch press interrupt
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        SW1, SW2
//=============================================================================
void Switches_Interrupt_Start(void) {
    P4IFG &= ~SW1;
    P2IFG &= ~SW2;
    P4IE |= SW1;
    P2IE |= SW2;
}

//=============================================================================
// Function:    Switches_Interrupt_Stop
// Description: Stops the switch press interrupt
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        SW1, SW2
//=============================================================================
void Switches_Interrupt_Stop(void) {
    P4IE &= ~SW1;
    P2IE &= ~SW2;
    P4IFG &= ~SW1;
    P2IFG &= ~SW2;
}