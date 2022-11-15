//=============================================================================
// File Name: wheels.c
//
// Description: This file contains the fucntions for wheel movement
//
// Author: Joshua Swaim
// Date: Mar 2021
// Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (7.20.1)
//=============================================================================

#include "macros.h"

// Globals
unsigned int movement;

//=============================================================================
// Function:    Wheels_Process
// Description: Operates motor pins based on wheel state
// Arguments:   None
// Globals:     movement
// Locals:      None
// Return:      None
// Pins:        L_FORWARD, R_FORWARD, L_REVERSE, R_REVERSE
//=============================================================================
void Wheels_Process(void) {
  switch(movement) {
      case allForward:
          left_forward_speed = leftMed;
          right_forward_speed = rightMed;
          break;
      case leftForward:
          left_forward_speed = leftSlow;
          right_reverse_speed = rightMed;
          break;
      case rightForward:
          left_reverse_speed = leftMed;
          right_forward_speed = rightSlow;
          break;
      case allReverse:
          left_forward_speed = leftSlow;
          break;
      case leftReverse:
          left_forward_speed = leftSlow;
          right_forward_speed = rightSlow;
          break;
      case rightReverse:
          right_forward_speed = rightSlow;
          break;
      case leftSpin:
          left_forward_speed = leftFast;
          right_reverse_speed = rightFast;
          break;
      case rightSpin:
          left_reverse_speed = leftMed;
          right_forward_speed = rightMed;
          break;
      default: break;
  }
}

//=============================================================================
// Function:    Error_Process
// Description: Checks for forward and reverse error
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Error_Process(void) {
  if(right_forward_speed != OFF && right_reverse_speed != OFF) {
    All_Stop();
    Red_On();
    Green_On();
    while(ALWAYS) {
      // STOPING CODE
    }
  }
  if(left_forward_speed != OFF && left_reverse_speed != OFF) {
    All_Stop();
    Red_On();
    Green_On();
    while(ALWAYS) {
      // STOPING CODE
    }
  }
}

//=============================================================================
// Function:    all_forward
// Description: Moves both motors forward
// Arguments:   None
// Globals:     movement
// Locals:      None
// Return:      None
// Pins:        L_FORWARD, R_FORWARD
//=============================================================================
void All_Forward(void) {
  All_Stop();
  movement = allForward;
  Wheels_Timer_Start();
}

//=============================================================================
// Function:    left_forward
// Description: Moves the left motor forward
// Arguments:   None
// Globals:     movement
// Locals:      None
// Return:      None
// Pins:        L_FORWARD
//=============================================================================
void Left_Forward(void) {
  All_Stop();
  movement = leftForward;
  Wheels_Timer_Start();
}

//=============================================================================
// Function:    right_forward
// Description: Moves the right motor forward
// Arguments:   None
// Globals:     movement
// Locals:      None
// Return:      None
// Pins:        R_FORWARD
//=============================================================================
void Right_Forward(void) {
  All_Stop();
  movement = rightForward;
  Wheels_Timer_Start();
}

//=============================================================================
// Function:    all_reverse
// Description: Moves both motors backwards
// Arguments:   None
// Globals:     movement
// Locals:      None
// Return:      None
// Pins:        L_REVERSE, R_REVERSE
//=============================================================================
void All_Reverse(void) {
  All_Stop();
  movement = allReverse;
  Wheels_Timer_Start();
}

//=============================================================================
// Function:    left_reverse
// Description: Moves the left motor backwards
// Arguments:   None
// Globals:     movement
// Locals:      None
// Return:      None
// Pins:        L_REVERSE
//=============================================================================
void Left_Reverse(void) {
  All_Stop();
  movement = leftReverse;
  Wheels_Timer_Start();
}

//=============================================================================
// Function:    right_reverse
// Description: Moves the right motor backwards
// Arguments:   None
// Globals:     movement
// Locals:      None
// Return:      None
// Pins:        R_REVERSE
//=============================================================================
void Right_Reverse(void) {
  All_Stop();
  movement = rightReverse;
  Wheels_Timer_Start();
}

//=============================================================================
// Function:    left_spin
// Description: Moves the left motor forward and the right motor backwards
// Arguments:   None
// Globals:     movement
// Locals:      None
// Return:      None
// Pins:        L_FORWARD, R_REVERSE
//=============================================================================
void Left_Spin(void) {
  All_Stop();
  movement = leftSpin;
  Wheels_Timer_Start();
}

//=============================================================================
// Function:    right_spin
// Description: Moves the right motor forward and the left motor backwards
// Arguments:   None
// Globals:     movement
// Locals:      None
// Return:      None
// Pins:        R_FORWARD, L_REVERSE
//=============================================================================
void Right_Spin(void) {
  All_Stop();
  movement = rightSpin;
  Wheels_Timer_Start();
}

//=============================================================================
// Function:    all_stop
// Description: Stops all motors
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        L_FORWARD, R_FORWARD, L_REVERSE, R_REVERSE
//=============================================================================
void All_Stop(void) {
  left_forward_speed = OFF;
  left_reverse_speed = OFF;
  right_forward_speed = OFF;
  right_reverse_speed = OFF;
}

//=============================================================================
// Function:    wheels_timer_start
// Description: Starts timer for wheels
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Wheels_Timer_Start(void) {
  TB1CCTL1 &= ~TBIFG;                       // Clearing interrupt flags
  TB1CCR1 = TB1R + TB1CCR1_INTERVAL;        // Setting the interval
  TB1CCTL1 |= CCIE;                         // Enabling the interrupt
}

//=============================================================================
// Function:    wheels_timer_stop
// Description: Stops timer for wheels
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Wheels_Timer_Stop(void) {
  TB1CCTL1 &= ~CCIE;                          // Disabling the interrupt
  TB1CCTL1 &= ~TBIFG;                         // Clearing interrupt flags
}