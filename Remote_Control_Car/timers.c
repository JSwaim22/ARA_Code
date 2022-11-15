//=============================================================================
// File Name: timers.c
//
// Description: This file contains the functions for the timers
//
// Author: Joshua Swaim
// Date: Mar 2021
// Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (7.20.1)
//=============================================================================

#include "macros.h"

//=============================================================================
// Function:    Init_Timers
// Description: This function initializes the timers
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Init_Timers(void) {
    Init_Timer_B0();
    Init_Timer_B1();
    Init_Timer_B2();
    Init_Timer_B3();
}

//=============================================================================
// Function:    Init_Timer_B0
// Description: This function initializes the timer B0
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Init_Timer_B0(void) {
    TB0CTL = TBSSEL__SMCLK;             // SMCLK source
    TB0CTL |= TBCLR;                    // Resets TB0R, clock divider, count direction
    TB0CTL |= MC__CONTINOUS;            // Continuous up
    TB0CTL |= ID__8;                    // Divide clock by 8
    TB0EX0 = TBIDEX__8;                 // Divide clock by an additional 8
    
    // CCR 0
    TB0CCR0 = TB0CCR0_INTERVAL;
    TB0CCTL0 |= CCIE;                   // enable interrupt
    
    // Overflow
    TB0CTL &= ~TBIFG;                   // clear overflow interrupt flag
    TB0CTL |= TBIE;                    // Enable interrupt
}

//=============================================================================
// Function:    Init_Timer_B1
// Description: This function initializes the timer B1
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Init_Timer_B1(void) {
    TB1CTL = TBSSEL__SMCLK;             // SMCLK source
    TB1CTL |= TBCLR;                    // Resets TB0R, clock divider, count direction
    TB1CTL |= MC__CONTINOUS;            // Continuous up
    TB1CTL |= ID__8;                    // Divide clock by 8
    TB1EX0 = TBIDEX__8;                 // Divide clock by an additional 8
    
    // Overflow
    TB1CTL &= ~TBIE;                    // disable interrupt
    TB1CTL &= ~TBIFG;                   // clear overflow interrupt flag
}

//=============================================================================
// Function:    Init_Timer_B2
// Description: This function initializes the timer B2
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Init_Timer_B2(void) {
    TB2CTL = TBSSEL__SMCLK;             // SMCLK source
    TB2CTL |= TBCLR;                    // Resets TB0R, clock divider, count direction
    TB2CTL |= MC__CONTINOUS;            // Continuous up
    TB2CTL |= ID__8;                    // Divide clock by 8
    TB2EX0 = TBIDEX__8;                 // Divide clock by an additional 8
    
    // Overflow
    TB2CTL &= ~TBIE;                    // disable interrupt
    TB2CTL &= ~TBIFG;                   // clear overflow interrupt flag
}

//=============================================================================
// Function:    Init_Timer_B3
// Description: This function initializes the timer B3
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Init_Timer_B3(void) {
     TB3CTL = TBSSEL__SMCLK;                    // SMCLK
     TB3CTL |= MC__UP;                          // Up Mode
     TB3CTL |= TBCLR;                           // Clear TAR
     
     TB3CCR0 = wheelPeriod;                     // PWM Period
     TB3CCTL1 = OUTMOD_7;                       // CCR1 reset/set
     right_forward_speed = OFF;                 // P6.0 Right Forward PWM duty cycle
     TB3CCTL2 = OUTMOD_7;                       // CCR2 reset/set
     left_forward_speed = OFF;                  // P6.1 Left Forward PWM duty cycle
     TB3CCTL3 = OUTMOD_7;                       // CCR3 reset/set
     right_reverse_speed = OFF;                 // P6.2 Right Reverse PWM duty cycle
     TB3CCTL4 = OUTMOD_7;                       // CCR4 reset/set
     left_reverse_speed = OFF;                  // P6.3 Left Reverse PWM duty cycle
}