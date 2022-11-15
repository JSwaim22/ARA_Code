//=============================================================================
// File Name: states.c
//
// Description: This file manages the states
//
// Author: Joshua Swaim
// Date: Mar 2021
// Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (7.20.1)
//=============================================================================

#include "macros.h"

// Globals
unsigned int state = start;
unsigned int prev_state = start;
unsigned int new_state = forwardA;
unsigned int state_wait_counter;
unsigned int state_count_limit;
extern char display_line[displayRows][displayColumns];
extern volatile unsigned char display_changed;
extern unsigned int change;
extern volatile unsigned int adc_left_data[dataSize];
extern volatile unsigned int adc_right_data[dataSize];
extern volatile unsigned int adc_left_average;
extern volatile unsigned int adc_right_average;
extern unsigned int index;
unsigned int direction = right;

//=============================================================================
// Function:    State_Process
// Description: Checks for state changes and executes state dependent functions
// Arguments:   None
// Globals:     change, state, new_state
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void State_Process(void) {
  if(change && new_state == state) {
    State_Change();
  }
  else if(!change && state != new_state) {
    switch(new_state){
      case forwardA:
        Forward_A();
        break;
      case turnA:
        Turn_A();
        break;
      case forwardB:
        Forward_B();
        break;
      case turnB:
        Turn_B();
        break;
      case forwardC:
        Forward_C();
        break;
      case forwardD:
        Forward_D();
        break;
      case pause:
        Pause();
        break;
      case spin:
        Spin();
        break;
      case leftward:
        Left();
        break;
      case rightward:
        Right();
        break;
      case straight:
        Straight();
        break;
      default: break;
    }
    prev_state = state;
    state = new_state;
  }
}

//=============================================================================
// Function:    State_Change
// Description: Handles changing states
// Arguments:   None
// Globals:     state, new_state
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void State_Change() {
  switch(state){
    case forwardA:
      new_state = turnA;
      break;
    case turnA:
      new_state = forwardB;
      break;
    case forwardB:
      new_state = turnB;
      break;
    case turnB:
      new_state = forwardC;
      break;
    case forwardC:
      new_state = forwardD;
      break;
    case forwardD:
      new_state = pause;
      change = FALSE;
      break;
    case pause:
      if(prev_state == forwardD) {
        new_state = spin;
      } else if(prev_state == spin) {
        new_state = straight;
      }
      break;
    case spin:
      new_state = pause;
      break;
    case leftward:
      new_state = straight;
      change = FALSE;
      break;
    case straight:
      if(direction == left) {
        new_state = leftward;
        change = FALSE;
      } else if(direction == right) {
        new_state = rightward;
        change = FALSE;
      }
      break;
    case rightward:
      new_state = straight;
      change = FALSE;
      break;
    default: break;
  }
}

//=============================================================================
// Function:    Forward_A
// Description: Controls the Forward_A state
// Arguments:   None
// Globals:     display_line[][], display_changed
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Forward_A(void) {
  strcpy(display_line[displayRowOne], " BL Start ");
  update_string(display_line[displayRowOne], displayRowOne);
  display_changed = displayChangedHappened;
  All_Forward();
  state_count_limit = 17;
  States_Timer_Start();
}

//=============================================================================
// Function:    Forward_A
// Description: Controls the Forward_A state
// Arguments:   None
// Globals:     display_line[][], display_changed
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Forward_B() {
  All_Forward();
  state_count_limit = 70;
  States_Timer_Start();
}

//=============================================================================
// Function:    Forward_C
// Description: Controls the Forward_A state
// Arguments:   None
// Globals:     display_line[][], display_changed
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Forward_C(void) {
  All_Forward();
  state_count_limit = 25;
  States_Timer_Start();
}

//=============================================================================
// Function:    Forward_C
// Description: Controls the Forward_A state
// Arguments:   None
// Globals:     display_line[][], display_changed
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Forward_D(void) {
  strcpy(display_line[displayRowOne], " Intercept");
  update_string(display_line[displayRowOne], displayRowOne);
  display_changed = displayChangedHappened;
  Left_Reverse();
}

//=============================================================================
// Function:    Forward
// Description: Updates the display for the driving forward state
// Arguments:   None
// Globals:     display_line[][], display_changed
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Turn_A(void) {
  Right_Spin();
  state_count_limit = 13;
  States_Timer_Start();
}

//=============================================================================
// Function:    Forward
// Description: Updates the display for the driving forward state
// Arguments:   None
// Globals:     display_line[][], display_changed
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Turn_B(void) {
  Right_Spin();
  state_count_limit = 13;
  States_Timer_Start();
}

//=============================================================================
// Function:    Left
// Description: Updates the display for the driving left state
// Arguments:   None
// Globals:     display_line[][], display_changed
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Left(void) {
  strcpy(display_line[displayRowZero], "Left      ");
  update_string(display_line[displayRowZero], displayRowZero);
  display_changed = displayChangedHappened;
  Left_Forward();
}

//=============================================================================
// Function:    Right
// Description: Updates the display for the driving right state
// Arguments:   None
// Globals:     display_line[][], display_changed
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Right(void) {
  strcpy(display_line[displayRowZero], "Right     ");
  update_string(display_line[displayRowZero], displayRowZero);
  display_changed = displayChangedHappened;
  Right_Forward();
}

//=============================================================================
// Function:    Straight
// Description: Updates the display for the driving left state
// Arguments:   None
// Globals:     display_line[][], display_changed
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Straight(void) {
  strcpy(display_line[displayRowZero], "Straight  ");
  update_string(display_line[displayRowZero], displayRowZero);
  display_changed = displayChangedHappened;
  Left_Reverse();
}

//=============================================================================
// Function:    Pause
// Description: Updates the display for the waiting state
// Arguments:   None
// Globals:     display_line[][], display_changed, state_count_limit
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Pause(void) {
  strcpy(display_line[displayRowZero], "Black Line");
  update_string(display_line[displayRowZero], displayRowZero);
  strcpy(display_line[displayRowOne], "Detected  ");
  update_string(display_line[displayRowOne], displayRowOne);
  display_changed = displayChangedHappened;
  All_Stop();
  state_count_limit = twoSec;
  States_Timer_Start();
}

//=============================================================================
// Function:    Spin
// Description: Updates the display for the spinning state
// Arguments:   None
// Globals:     display_line[][], display_changed, state_count_limit
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Spin(void) {
  strcpy(display_line[displayRowZero], "Spinning  ");
  update_string(display_line[displayRowZero], displayRowZero);
  strcpy(display_line[displayRowOne], "clockwise ");
  update_string(display_line[displayRowOne], displayRowOne);
  display_changed = displayChangedHappened;
  Left_Spin();
  state_count_limit = twoTenthsSec;
  States_Timer_Start();
}

//=============================================================================
// Function:    States_Timer_Start
// Description: Starts the state changing timer interrupt
// Arguments:   None
// Globals:     change
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void States_Timer_Start(void) {
  state_wait_counter = clear;
  TB1CCTL2 &= ~TBIFG;                       // Clearing interrupt flags
  TB1CCR2 = TB1R + TB1CCR2_INTERVAL;        // Setting the interval
  TB1CCTL2 |= CCIE;                         // Enabling the interrupt
  change = TRUE;
}

//=============================================================================
// Function:    States_Timer_Continue
// Description: Continues the state changing timer interrupt
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void States_Timer_Continue(void) {
  TB1CCR2 += TB1CCR2_INTERVAL;
}

//=============================================================================
// Function:    States_Timer_Stop
// Description: Stops the state changing timer interrupt
// Arguments:   None
// Globals:     change
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void States_Timer_Stop(void) {
  TB1CCTL2 &= ~CCIE;                          // Disabling the interrupt
  TB1CCTL2 &= ~TBIFG;                         // Clearing interrupt flags
  change = FALSE;
}

//=============================================================================
// Function:    States_Line_Check
// Description: Checks for hitting a black line
// Arguments:   None
// Globals:     state, adc_left_data[], adc_right_data[], index, change
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void States_Line_Check(void) {
  // Locals
  unsigned int left_data_holder;
  unsigned int right_data_holder;
  
  // Getting average values
  for(int i = dataStart; i <= dataEnd; i++) {
    left_data_holder = adc_left_data[i];
    right_data_holder = adc_right_data[i];
    adc_left_average += left_data_holder;
    adc_right_average += right_data_holder;
  }
  adc_left_average = adc_left_average >> averager;
  adc_right_average = adc_right_average >> averager;
  
  
  switch(state) {
    case forwardD:
      if(adc_right_data[index] > 775) {
        change = TRUE;
      }
      break;
    case straight:
      if(adc_left_average < 900) {
        change = TRUE;
        direction = right;
      } else if(adc_right_average < 850) {
        change = TRUE;
        direction = left;
      }
      break;
    case leftward:
      if(adc_right_average > 850) {
        change = TRUE;
      }
      break;
    case rightward:
      if(adc_left_average > 900) {
        change = TRUE;
      }
      break;
    default: break;
  }
}