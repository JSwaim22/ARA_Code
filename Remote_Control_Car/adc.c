//=============================================================================
// File Name: adc.c
//
// Description: This file contains the functions for a to d conversions
//
// Author: Joshua Swaim
// Date: Mar 2021
// Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (7.20.1)
//=============================================================================

#include "macros.h"

// Globals
volatile unsigned int adc_on[] = {ON,ON,ON};
volatile unsigned int adc_to_do[] = {ON,ON,ON};
volatile unsigned int adc_left_average;
volatile unsigned int adc_right_average;
extern volatile unsigned int adc_left_data[];
extern volatile unsigned int adc_right_data[];
char adc_string[stringSize];
extern unsigned int index;
extern char display_line[displayRows][displayColumns];
extern volatile unsigned char display_changed;

//=============================================================================
// Function:    Init_ADC
// Description: Initializes ADC registers and starts conversions
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Init_ADC(void){
    // ADCCTL0 Register
    ADCCTL0 = reset;            // Reset
    ADCCTL0 |= ADCSHT_2;        // 16 ADC clocks
    ADCCTL0 |= ADCMSC;          // MSC
    ADCCTL0 |= ADCON;           // ADC ON
    
    // ADCCTL1 Register
    ADCCTL2 = reset;            // Reset
    ADCCTL1 |= ADCSHS_0;        // 00b = ADCSC bit
    ADCCTL1 |= ADCSHP;          // ADC sample-and-hold SAMPCON signal from sampling timer.
    ADCCTL1 &= ~ADCISSH;        // ADC invert signal sample-and-hold.
    ADCCTL1 |= ADCDIV_0;        // ADC clock divider - 000b = Divide by 1
    ADCCTL1 |= ADCSSEL_0;       // ADC clock MODCLK
    ADCCTL1 |= ADCCONSEQ_0;     // ADC conversion sequence 00b = Single-channel single-conversion

    // ADCCTL2 Register
    ADCCTL2 = reset;            // Reset
    ADCCTL2 |= ADCPDIV0;        // ADC pre-divider 00b = Pre-divide by 1
    ADCCTL2 |= ADCRES_1;        // ADC resolution 10b = 12 bit (14 clock cycle conversion time)
    ADCCTL2 &= ~ADCDF;          // ADC data read-back format 0b = Binary unsigned.
    ADCCTL2 &= ~ADCSR;          // ADC sampling rate 0b = ADC buffer supports up to 200 ksps
    
    // ADCMCTL0 Register
    ADCMCTL0 |= ADCSREF_0;      // VREF - 000b = {VR+ = AVCC and VR– = AVSS }
    ADCMCTL0 |= ADCINCH_5;      // V_THUMB (0x20) Pin 5 A5
    
    // Starting Conversions
    ADCIE |= ADCIE0;            // Enable ADC conv complete interrupt
    ADCCTL0 |= ADCENC;          // ADC enable conversion.
    ADCCTL0 |= ADCSC;           // ADC start conversion.
}

//=============================================================================
// Function:    ADC_L_On
// Description: Sets the next ADC to the left detector
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void ADC_L_On(void) {
  ADCMCTL0 &= ~ADCINCH_5;
  ADCMCTL0 &= ~ADCINCH_3;
  ADCMCTL0 |= ADCINCH_2;
}

//=============================================================================
// Function:    ADC_R_On
// Description: Sets the next ADC to the right detector
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void ADC_R_On(void) {
  ADCMCTL0 &= ~ADCINCH_5;
  ADCMCTL0 &= ~ADCINCH_2;
  ADCMCTL0 |= ADCINCH_3;
}

//=============================================================================
// Function:    ADC_T_On
// Description: Sets the next ADC to the thumb wheel
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void ADC_T_On(void) {
  ADCMCTL0 &= ~ADCINCH_2;
  ADCMCTL0 &= ~ADCINCH_3;
  ADCMCTL0 |= ADCINCH_5;
}

//=============================================================================
// Function:    ADC_Advance_Index
// Description: Advances the index for storing ADC values in arrays
// Arguments:   None
// Globals:     index
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void ADC_Advance_Index(void) {
  if(index == dataEnd)
    index = reset;
  else {
    index++;
  }
}

//=============================================================================
// Function:    ADC_Conversion_Switch
// Description: Switches between the different ADC options
// Arguments:   None
// Globals:     adc_to_do[]
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void ADC_Conversion_Switch(void) {
  if(adc_to_do[thumb]) {
    ADC_T_On();
    ADCCTL0 |= ADCENC;
    ADCCTL0 |= ADCSC;
  } else if(adc_to_do[left]) {
    ADC_L_On();
    ADCCTL0 |= ADCENC;
    ADCCTL0 |= ADCSC;
  } else if(adc_to_do[right]) {
    ADC_R_On();
    ADCCTL0 |= ADCENC;
    ADCCTL0 |= ADCSC;
  } else {
    States_Line_Check();
    ADC_Advance_Index();
    ADC_Timer_Start();
  }
}

//=============================================================================
// Function:    ADC_Timer_Start
// Description: Starts ADC timer interrupt
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void ADC_Timer_Start(void) {
  TB1CCTL0 &= ~TBIFG;                       // Clearing interrupt flags
  TB1CCR0 = TB1R + TB1CCR0_INTERVAL;        // Setting the interval
  TB1CCTL0 |= CCIE;                         // Enabling the interrupt
}

//=============================================================================
// Function:    ADC_Timer_Stop
// Description: Stops the ADC timer interrupt
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void ADC_Timer_Stop(void) {
  TB1CCTL0 &= ~CCIE;                          // Disabling the interrupt
  TB1CCTL0 &= ~TBIFG;                         // Clearing interrupt flags
}

//=============================================================================
// Function:    ADC_Display
// Description: Sets the next ADC to the left detector
// Arguments:   None
// Globals:     adc_left_average, adc_right_average, adc_left_data[], 
//              adc_right_data[], adc_string, display_line[][], display_changed
// Locals:      left_data_holder, right_data_holder
// Return:      None
// Pins:        None
//=============================================================================
void ADC_Display(void) {
  // Building String
  adc_string[stringBitZero] = 'R';
  adc_string[stringBitOne] = '=';
  Val_To_String(adc_right_average, stringBitTwo);
  adc_right_average = reset;
  adc_string[stringBitFive] = 'L';
  adc_string[stringBitSix] = '=';
  Val_To_String(adc_left_average, stringBitSeven);
  adc_left_average = reset;
  
  // Pushing to display
  strcpy(display_line[displayRowThree], adc_string);
  update_string(display_line[displayRowThree], displayRowThree);
  display_changed = displayChangedHappened;
}

//=============================================================================
// Function:    Val_To_String
// Description: Plugs a interger value into an array of chars
// Arguments:   value, start_bit
// Globals:     adc_string
// Locals:      counter
// Return:      None
// Pins:        None
//=============================================================================
void Val_To_String(int value, int start_bit){
  int counter = reset;
  while (value >= quadrupleDigit){
    value -= quadrupleDigit;
  }
  while (value >= tripleDigit){
    value -= tripleDigit;
    counter++;
  }
  adc_string[start_bit] = asciBase + counter;
  start_bit++;
  counter = reset;
  while (value >= doubleDigit){
    value -= doubleDigit;
    counter++;
  }
  adc_string[start_bit] = asciBase + counter;
  start_bit++;
  adc_string[start_bit] = asciBase + value;
}