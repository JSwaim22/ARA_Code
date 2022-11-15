//=============================================================================
// File Name: ports.c
//
// Description: This file contains the functions for initializing the ports
//
// Author: Joshua Swaim
// Date: Feb 2021
// Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (7.20.1)
//=============================================================================

#include "macros.h"

//===========================================================================
// Function:    Init_Ports
// Description: This function initializes the ports
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//===========================================================================
void Init_Ports(void) {
    Init_Port1();
    Init_Port2();
    Init_Port3();
    Init_Port4();
    Init_Port5();
    Init_Port6();
} 

//===========================================================================
// Function:    Init_Port1
// Description: This function initializes the pins in port 1
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        All of the port one pins to be setup
//===========================================================================
void Init_Port1(void)	{
  P1OUT = clear; 				// Set P1 Low
  P1DIR = clear; 				// Set P1 direction to output
  
  //	P1 PIN 0
        P1SEL0 &= ~RED_LED;			// RED_LED GPIO operation
        P1SEL1 &= ~RED_LED;			// RED_LED GPIO operation
        P1OUT &= ~RED_LED;			// Set RED_LED low
        P1DIR |= RED_LED; 			// Set RED_LED direction to output

  //	P1 PIN 1
        P1SELC |= A1_SEEED;			// A1_SEEED operation

  //	P1 PIN 2
        P1SELC |= V_DETECT_L;		        // V_DETECT_L operation

  //	P1 PIN 3
        P1SELC |= V_DETECT_R;			// V_DETECT_R operation

  //	P1 PIN 4
        P1SELC |= A4_SEEED;			// A4_SEEED operation

  //	P1 PIN 5
        P1SELC |= V_THUMB;			// V_THUMB operation

  //	P1 PIN 6
        P1SEL0 |= UCA0RXD;			// UCA0RXD operation
        P1SEL1 &= ~UCA0RXD;			// UCA0RXD operation

  //	P1 PIN 7 
        P1SEL0 |= UCA0TXD;			// UCA0TXD operation
        P1SEL1 &= ~UCA0TXD;			// UCA0TXD operation
}

//===========================================================================
// Function:    Init_Port2
// Description: This function initializes the pins in port 2
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        All of the port two pins to be setup
//===========================================================================
void Init_Port2(void)	{
  P2OUT = clear; 				// Set P2 low
  P2DIR = clear; 				// Set P2 direction to output

  //	P2 PIN 0
        P2SEL0 &= ~P2_0;			// P2_0 GPIO operation
        P2SEL1 &= ~P2_0;			// P2_0 GPIO operation
        P2DIR &= ~P2_0; 			// Set P2_0 direction to input

  //	P2 PIN 1
        P2SEL0 &= ~IR_LED;			// IR_LED GPIO operation
        P2SEL1 &= ~IR_LED;			// IR_LED GPIO operation
        P2OUT &= ~IR_LED;			// Set IR_LED low
        P2DIR |= IR_LED;			// Set IR_LED direction to output

  //	P2 PIN 2
        P2SEL0 &= ~P2_2;			// P2_2 GPIO operation
        P2SEL1 &= ~P2_2;			// P2_2 GPIO operation
        P2DIR &= ~P2_2;				// Set P2_2 direction to input

  //	P2 PIN 3
        P2SEL0 &= ~SW2;				// SW2 GPIO operation
        P2SEL1 &= ~SW2;				// SW2 GPIO operation
        P2DIR &= ~SW2;				// Set SW2 direction to input
        P2OUT |= SW2; 				// Configure pullup resistor					
        P2REN |= SW2; 				// Enable pullup resistor
        P2IES &= ~SW2;                          // SW1 Lo/Hi edge interrupt
        P2IFG &= ~SW2;                          // IFG SW1 cleared
        P2IE |= SW2;                            // SW1 interrupt Enabled

  //	P2 PIN 4
        P2SEL0 &= ~IOT_PROG_SEL;		// IOT_PROG_SEL GPIO operation
        P2SEL1 &= ~IOT_PROG_SEL;		// IOT_PROG_SEL GPIO operation
        P2DIR &= ~IOT_PROG_SEL;			// Set IOT_PROG_SEL direction to input

  //	P2 PIN 5
        P2SEL0 &= ~P2_5;			// P2_5 GPIO operation
        P2SEL1 &= ~P2_5;			// P2_5 GPIO operation
        P2DIR &= ~P2_5;				// Set P2_5 direction to input

  //	P2 PIN 6
        P2SEL0 &= ~LFXOUT;			// LFXOUT GPIO operation
        P2SEL1 |= LFXOUT;			// LFXOUT GPIO operation

  //	P2 PIN 7
        P2SEL0 &= ~LFXIN;			// LFXIN GPIO operation
        P2SEL1 |= LFXIN;			// LFXIN GPIO operation	
}

//===========================================================================
// Function:    Init_Port3
// Description: This function initializes the pins in port 3
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        All of the port three pins to be setup
//===========================================================================
void Init_Port3(void)	{
  P3OUT = clear; 				// Set P3 low
  P3DIR = clear; 				// Set P3 direction to output

  //	P3 PIN 0
        P3SEL0 &= ~TEST_PROBE; 			// TEST_PROBE GPIO operation
        P3SEL1 &= ~TEST_PROBE;			// TEST_PROBE GPIO operation
        P3DIR &= ~TEST_PROBE;			// Set TEST_PROBE direction to input
  
  //	P3 PIN 1
        P3SEL0 &= ~OPT_INT;			// OPT_INT GPIO operation
        P3SEL1 &= ~OPT_INT;			// OPT_INT GPIO operation
        P3DIR &= ~OPT_INT;			// Set OPT_INT direction to input

  //	P3 PIN 2
        P3SEL0 &= ~OA2N;			// OA2N GPIO operation
        P3SEL1 &= ~OA2N;			// OA2N GPIO operation
        P3DIR &= ~OA2N;				// Set OA2N direction to input

  //	P3 PIN 3
        P3SEL0 &= ~OA2P;			// OA2P GPIO operation
        P3SEL1 &= ~OA2P;			// OA2P GPIO operation
        P3DIR &= ~OA2P;				// Set OA2P direction to input

  //	P3 PIN 4
        P3SEL0 &= ~SMCLK_OUT;			// SMCLK_OUT GPIO operation
        P3SEL1 &= ~SMCLK_OUT;			// SMCLK_OUT GPIO operation
        P3DIR &= ~SMCLK_OUT;			// Set SMCLK_OUT direction to input

  //	P3 PIN 5
        P3SEL0 &= ~DAC_CNTL;			// DAC_CNTL GPIO operation
        P3SEL1 &= ~DAC_CNTL;			// DAC_CNTL GPIO operation
        //P3DIR &= ~DAC_CNTL;			// Set DAC_CNTL direction to input
        P3OUT |= DAC_CNTL;
        P3DIR |= DAC_CNTL;
        
  //	P3 PIN 6
        P3SEL0 &= ~IOT_LINK;			// IOT_LINK GPIO operation
        P3SEL1 &= ~IOT_LINK;			// IOT_LINK GPIO operation
        P3DIR &= ~IOT_LINK;			// Set IOT_LINK direction to input

  //	P3 PIN 7
        P3SEL0 &= ~IOT_RESET;			// IOT_RESET GPIO operation
        P3SEL1 &= ~IOT_RESET;			// IOT_RESET GPIO operation
        P3OUT &= ~IOT_RESET;                    // Set IOT_RESET low
        P3DIR |= IOT_RESET;			// Set IOT_RESET direction to output
}

//===========================================================================
// Function:    Init_Port4
// Description: This function initializes the pins in port 4
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        All of the port four pins to be setup
//===========================================================================
void Init_Port4(void)	{
  P4OUT = clear; 				// Set P4 low
  P4DIR = clear; 				// Set P4 direction to output

  //    P4 PIN 0
        P4SEL0 &= ~RESET_LCD; 			// RESET_LCD GPIO operation
        P4SEL1 &= ~RESET_LCD; 			// RESET_LCD GPIO operation
        P4OUT &= ~RESET_LCD; 			// Set RESET_LCD low
        P4DIR |= RESET_LCD; 			// Set RESET_LCD direction to output

  //    P4 PIN 1
        P4SEL0 &= ~SW1; 			// SW1 GPIO operation
        P4SEL1 &= ~SW1; 			// SW1 GPIO operation
        P4DIR &= ~SW1; 				// Set SW1 direction to input
        P4OUT |= SW1; 				// Configure pullup resistor
        P4REN |= SW1; 				// Enable pullup resistor
        P4IES &= ~SW1;                          // SW1 Lo/Hi edge interrupt
        P4IFG &= ~SW1;                          // IFG SW1 cleared
        P4IE |= SW1;                            // SW1 interrupt Enabled

  //    P4 PIN 2
        P4SEL0 |= UCA1RXD; 			// USCI_A1 UART operation
        P4SEL1 &= ~UCA1RXD; 			// USCI_A1 UART operation
  
  //    P4 PIN 3
        P4SEL0 |= UCA1TXD; 			// USCI_A1 UART operation
        P4SEL1 &= ~UCA1TXD; 			// USCI_A1 UART operation

  //    P4 PIN 4
        P4SEL0 &= ~UCB1_CS_LCD; 		// UCB1_CS_LCD GPIO operation
        P4SEL1 &= ~UCB1_CS_LCD; 		// UCB1_CS_LCD GPIO operation
        P4OUT |= UCB1_CS_LCD; 			// Set SPI_CS_LCD high
        P4DIR |= UCB1_CS_LCD; 			// Set SPI_CS_LCD direction to output

  //    P4 PIN 5
        P4SEL0 |= UCB1CLK; 			// UCB1CLK SPI BUS operation
        P4SEL1 &= ~UCB1CLK; 			// UCB1CLK SPI BUS operation

  //    P4 PIN 6
        P4SEL0 |= UCB1SIMO; 			// UCB1SIMO SPI BUS operation
        P4SEL1 &= ~UCB1SIMO; 			// UCB1SIMO SPI BUS operation

  //    P4 PIN 7
        P4SEL0 |= UCB1SOMI; 			// UCB1SOMI SPI BUS operation
        P4SEL1 &= ~UCB1SOMI; 			// UCB1SOMI SPI BUS operation
}

//===========================================================================
// Function:    Init_Port5
// Description: This function initializes the pins in port 5
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        All of the port five pins to be setup
//===========================================================================
void Init_Port5(void)	{
  P5OUT = clear; 				// Set P5 low
  P5DIR = clear; 				// Set P5 direction to output

  //    P5 PIN 0
        P5SEL0 &= ~CHECK_BAT; 			// CHECK_BAT GPIO operation
        P5SEL1 &= ~CHECK_BAT; 			// CHECK_BAT GPIO operation
        P5DIR &= ~CHECK_BAT; 			// Set CHECK_BAT direction to input

  //    P5 PIN 1
        P5SEL0 &= ~V_BAT; 			// V_BAT GPIO operation
        P5SEL1 &= ~V_BAT; 			// V_BAT GPIO operation
        P5DIR &= ~V_BAT; 			// Set V_BAT direction to input

  //    P5 PIN 2
        P5SEL0 &= ~V_DAC; 		        // V_DAC GPIO operation
        P5SEL1 &= ~V_DAC; 		        // V_DAC GPIO operation
        P5DIR &= ~V_DAC; 		        // Set V_DAC direction to input

  //    P5 PIN 3
        P5SEL0 &= ~V_3_3; 			// V_3_3 GPIO operation
        P5SEL1 &= ~V_3_3; 			// V_3_3 GPIO operation
        P5DIR &= ~V_3_3; 			// Set V_3_3 direction to input

  //    P5 PIN 4
        P5SEL0 &= ~IOT_PROG_MODE; 		// IOT_PROG_MODE GPIO operation
        P5SEL1 &= ~IOT_PROG_MODE; 		// IOT_PROG_MODE GPIO operation
        P5DIR &= ~IOT_PROG_MODE; 		// Set IOT_PROG_MODE direction to input
}

//===========================================================================
// Function: Init_Port6
// Description: This function initializes the pins in port 6
// Arguments: None
// Globals: None
// Locals: None
// Return: None
// Pins: All of the port six pins to be setup
//===========================================================================
void Init_Port6(void)	{
  P6OUT = clear; 				// Set P6 low
  P6DIR = clear; 				// Set P6 direction to output

  //    P6 PIN 0
        P6SEL0 |= R_FORWARD; 			// R_FORWARD PWM operation
        P6SEL1 &= ~R_FORWARD; 			// R_FORWARD PWM operation
        P6DIR |= R_FORWARD; 			// Set R_FORWARD direction to output

  //    P6 PIN 1
        P6SEL0 |= L_FORWARD; 			// L_FORWARD PWM operation
        P6SEL1 &= ~L_FORWARD; 			// L_FORWARD PWM operation
        P6DIR |= L_FORWARD; 			// Set L_FORWARD direction to output

  //    P6 PIN 2
        P6SEL0 |= R_REVERSE; 			// R_REVERSE PWM operation
        P6SEL1 &= ~R_REVERSE; 			// R_REVERSE PWM operation
        P6DIR |= R_REVERSE; 			// SET R_REVERSE direction to output

  //    P6 PIN 3
        P6SEL0 |= L_REVERSE; 			// L_REVERSE PWM operation
        P6SEL1 &= ~L_REVERSE; 			// L_REVERSE PWM operation
        P6DIR |= L_REVERSE; 			// Set L_REVERSE direction to output

  //    P6 PIN 4
        P6SEL0 &= ~LCD_BACKLIGHT; 		// LCD_BACKLIGHT GPIO operation
        P6SEL1 &= ~LCD_BACKLIGHT; 		// LCD_BACKLIGHT GPIO operation
        P6OUT |= LCD_BACKLIGHT; 		// Set LCD_BACKLIGHT high
        P6DIR |= LCD_BACKLIGHT; 		// Set LCD_BACKLIGHT direction to output

  //    P6 PIN 5
        P6SEL0 &= ~P6_5; 			// P6_5 GPIO operation
        P6SEL1 &= ~P6_5; 			// P6_5 GPIO operation
        P6DIR &= ~P6_5; 			// Set P6_5 direction to input

  //    P6 PIN 6
        P6SEL0 &= ~GRN_LED; 			// GRN_LED GPIO operation
        P6SEL1 &= ~GRN_LED; 			// GRN_LED GPIO operation
        P6OUT |= GRN_LED; 			// Set GRN_LED high
        P6DIR |= GRN_LED; 			// Set GRN_LED direction to output	
}