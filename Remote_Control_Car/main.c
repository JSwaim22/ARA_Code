//=============================================================================
// File Name: main.c
//
// Description: This file contains the Main Routine - "While" Operating System
//
// Author: Joshua Swaim
// Date: April 2021
// Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (7.20.1)
//=============================================================================

#include "macros.h"

extern volatile unsigned int start_state;

void main(void);

//=============================================================================
// Function: main
// Description: This function serves as the main routine for the program
// Arguments: None
// Globals: None
// Locals: None
// Return: None
// Pins: None
//=============================================================================
void main(void){

    // Initilizations
    PM5CTL0 &= ~LOCKLPM5;                // Disabling GPIO high impedance mode
    Init_Ports();                        // Initialize Ports
    Init_Clocks();                       // Initialize Clock System
    Init_Display();                      // Initialize Display
    Init_Timers();                       // Initialize Timers
    Init_LCD();                          // Initialize LCD
    Init_LEDs();                         // Initialize LEDs
    Init_ADC();
    Init_DAC();
    Init_Serial();   
    Set_Display();
    
    
    // Begining of the While Operating System
    while(ALWAYS) {
      Serial_Process();
      if(start_state) {
        State_Process();
      }
      Display_Process();
      Error_Process();
    }
}