//=============================================================================
// File Name: ints_timers.c
//
// Description: This file contains the interrupts for the timers
//
// Author: Joshua Swaim
// Date: Mar 2021
// Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (7.20.1)
//=============================================================================

#include "macros.h"

// Globals
extern unsigned int state_wait_counter;
extern unsigned int state_count_limit;
extern volatile unsigned char update_display;
extern unsigned int change;
extern volatile unsigned int adc_to_do[];
extern volatile unsigned int adc_on[];
extern volatile unsigned int DAC_Volt;
extern volatile unsigned int running;
extern volatile unsigned int queued;
extern volatile unsigned int start_state;

//=============================================================================
// Function:    TimerB0CCR0_ISR
// Description: interrupt for timerB0CCR0
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
#pragma vector = timerB0CCR0Vector

__interrupt void TimerB0CCR0_ISR(void){
    //ADC_Display();
    //Serial_Display();
    update_display = TRUE;
    TB0CCR0 += TB0CCR0_INTERVAL;
}

//=============================================================================
// Function:    TimerB0CCR12OVFL_ISR
// Description: interrupt for timerB0CCR12OVFL
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
#pragma vector = timerB0CCR12OVFLVECTOR

__interrupt void TimerB0CCR12OVFL_ISR(void){
    switch(__even_in_range(TB0IV,OVFL)){
        case NONE:
            // No code needed
            break;
        case CCR1:
            Switches_Timer_Stop();
            Red_Off();
            Green_Off();
            change = FALSE;
            Switches_Interrupt_Start();
            break;
        case CCR2:
            // DO NOT USE UNLESS YOU WANT IT ALWAYS ON!!!
            break;
        case OVFL:
            P3OUT  |= IOT_RESET;
            TB0CTL &= ~TBIE;
            break;
        default: break;
    }
}

//=============================================================================
// Function:    TimerB1CCR0_ISR
// Description: interrupt for timerB1CCR0
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
#pragma vector = timerB1CCR0Vector

__interrupt void TimerB1CCR0_ISR(void){
    ADC_Timer_Stop();
    adc_to_do[thumb] = adc_on[thumb]; 
    adc_to_do[left] = adc_on[left];
    adc_to_do[right] = adc_on[right];
    ADC_Conversion_Switch();
}

//=============================================================================
// Function:    TimerB1CCR12OVFL_ISR
// Description: interrupt for timerB1CCR12OVFL
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
#pragma vector = timerB1CCR12OVFLVECTOR

__interrupt void TimerB1CCR12OVFL_ISR(void){
    switch(__even_in_range(TB1IV,OVFL)){
        case NONE:
            // No code needed
            break;
        case CCR1:
            Wheels_Timer_Stop();
            Wheels_Process();
            break;
        case CCR2:
            state_wait_counter++;
            if(state_wait_counter == state_count_limit) {
              All_Stop();
              States_Timer_Stop();
              if(!start_state) {
                Set_Display();
                running = FALSE;
                if(queued == TRUE) {
                  Command_Process();
                }
              }
            }
            else {
              States_Timer_Continue();
            }
            break;
        case OVFL:
            // Put overflow code here 
            break;
        default: break;
    }
}

//=============================================================================
// Function:    TimerB2CCR0_ISR
// Description: interrupt for timerB2CCR0
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
#pragma vector = timerB2CCR0Vector

__interrupt void TimerB2CCR0_ISR(void){
  if(DAC_Volt != 1200) {
    DAC_Volt -= 200;
    SAC3DAT = DAC_Volt;
    DAC_Timer_Continue();
  }
  else {
    DAC_Timer_Stop();
  }
}

//=============================================================================
// Function:    TimerB2CCR12OVFL_ISR
// Description: interrupt for timerB2CCR12OVFL
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
#pragma vector = timerB2CCR12OVFLVECTOR

__interrupt void TimerB2CCR12OVFL_ISR(void){
    switch(__even_in_range(TB2IV,OVFL)){
        case NONE:
            // No code needed
            break;
        case CCR1:
            // Put CCR1 code here
            break;
        case CCR2:
            // Put CCR2 code here
            break;
        case OVFL:
            // Put overflow code here 
            break;
        default: break;
    }
}

//=============================================================================
// Function:    TimerB3CCR0_ISR
// Description: interrupt for timerB3CCR0
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
#pragma vector = timerB3CCR0Vector

__interrupt void TimerB3CCR0_ISR(void){
    // Put CCR0 code here
}

//=============================================================================
// Function:    TimerB3CCR12OVFL_ISR
// Description: interrupt for timerB3CCR12OVFL
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
#pragma vector = timerB3CCR12OVFLVECTOR

__interrupt void TimerB3CCR12OVFL_ISR(void){
    switch(__even_in_range(TB3IV,OVFL)){
        case NONE:
            // No code needed
            break;
        case CCR1:
            // Put CCR1 code here
            break;
        case CCR2:
            // Put CCR2 code here
            break;
        case OVFL:
            // Put overflow code here 
            break;
        default: break;
    }
}