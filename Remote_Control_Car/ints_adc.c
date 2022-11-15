//=============================================================================
// File Name: ints_adc.c
//
// Description: This file contains the interrupts for the a to d conversions
//
// Author: Joshua Swaim
// Date: Mar 2021
// Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (7.20.1)
//=============================================================================

#include "macros.h"

// Globals
extern volatile unsigned int adc_to_do[];
volatile unsigned int adc_thumb_data[dataSize];
volatile unsigned int adc_left_data[dataSize];
volatile unsigned int adc_right_data[dataSize];
unsigned int index = reset;

//=============================================================================
// Function:    ADC_ISR
// Description: Interrupt for ADC conversions
// Arguments:   None
// Globals:     adc_to_do[], adc_thumb_data[], adc_left_data[], 
//              adc_right_data[], index
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
#pragma vector=ADC_VECTOR

__interrupt void ADC_ISR(void){
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
      case ADCIV_NONE:
        break;
      case ADCIV_ADCOVIFG:      // When a conversion result is written to the ADCMEM0
        break;                  // before its previous conversion result was read.
      case ADCIV_ADCTOVIFG:     // ADC conversion-time overflow
        break;
      case ADCIV_ADCHIIFG:      // Window comparator interrupt flags
        break;
      case ADCIV_ADCLOIFG:      // Window comparator interrupt flag
        break;
      case ADCIV_ADCINIFG:      // Window comparator interrupt flag
        break;
      case ADCIV_ADCIFG:        // ADCMEM0 memory register with the conversion result
        ADCCTL0 &= ~ADCENC;
        if(adc_to_do[thumb]) {
          adc_to_do[thumb] = FALSE;
          adc_thumb_data[index] = ADCMEM0;
          ADC_Conversion_Switch();
        } else if(adc_to_do[left]) {
          adc_to_do[left] = FALSE;
          adc_left_data[index] = ADCMEM0;
          ADC_Conversion_Switch();
        } else if(adc_to_do[right]) {
          adc_to_do[right] = FALSE;
          adc_right_data[index] = ADCMEM0;
          ADC_Conversion_Switch();
        }
        break;
      default: break;
    }
}