//=============================================================================
// File Name: dac.c
//
// Description: This file manages the dac power control
//
// Author: Joshua Swaim
// Date: Mar 2021
// Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (7.20.1)
//=============================================================================

#include "macros.h"

// Globals
volatile unsigned int DAC_Volt;

void Init_DAC(void){
    Init_REF();
    
    SAC3DAC = DACSREF_1;                // Select int Vref as DAC reference
    SAC3DAC |= DACLSEL_0;               // DAC latch loads when DACDAT written
    SAC3OA = NMUXEN;                    // SAC Negative input MUX controL
    SAC3OA |= PMUXEN;                   // SAC Positive input MUX control
    SAC3OA |= PSEL_1;                   // 12-bit reference DAC source selected
    SAC3OA |= NSEL_1;                   // Select negative pin input
    SAC3OA |= OAPM;                     // Select low speed and low power mode
    SAC3PGA = MSEL_1;                   // Set OA as buffer mode
    SAC3OA |= SACEN;                    // Enable SAC
    SAC3OA |= OAEN;                     // Enable OA
    
    DAC_Volt = 4000;                    // Value between 0x000 and 0x0FFF
    SAC3DAT = DAC_Volt;                 // Initial DAC data
    
    P3SELC |= DAC_CNTL;
    
    // SAC3DAC |= DACIE;                // Generate an interrupt
    SAC3DAC |= DACEN;                   // Enable DAC
    DAC_Timer_Start();
}

void Init_REF(void){
    PMMCTL0_H = PMMPW_H;                // Unlock the PMM registers
    PMMCTL2 = INTREFEN;                 // Enable internal reference
    PMMCTL2 |= REFVSEL_2;               // Select 2.5V reference
    while(!(PMMCTL2 & REFGENRDY));      // Poll till internal reference settles
}

//=============================================================================
// Function:    DAC_Timer_Start
// Description: Starts the DAC voltage setting timer interrupt
// Arguments:   None
// Globals:     change
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void DAC_Timer_Start(void) {
  TB2CCTL0 &= ~TBIFG;                       // Clearing interrupt flags
  TB2CCR0 = TB2R + TB2CCR0_INTERVAL;        // Setting the interval
  TB2CCTL0 |= CCIE;                         // Enabling the interrupt
}

//=============================================================================
// Function:    DAC_Timer_Continue
// Description: Continues the DAC voltage setting timer interrupt
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void DAC_Timer_Continue(void) {
  TB2CCR0 += TB2CCR0_INTERVAL;
}

//=============================================================================
// Function:    DAC_Timer_Stop
// Description: Stops the DAC voltage setting timer interrupt
// Arguments:   None
// Globals:     change
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void DAC_Timer_Stop(void) {
  TB2CCTL0 &= ~CCIE;                          // Disabling the interrupt
  TB2CCTL0 &= ~TBIFG;                         // Clearing interrupt flags
}