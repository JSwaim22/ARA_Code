//=============================================================================
// File Name: ints_serial.c
//
// Description: This file includes the functions for handling switch presses
//
// Author: Joshua Swaim
// Date: April 2021
// Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (7.20.1)
//=============================================================================

#include "macros.h"

// Globals

//      USB
extern volatile char usb_in_ring_data[ringSize];
extern volatile unsigned int usb_in_ring_wr_index;
extern volatile char usb_out_ring_data[ringSize];
extern volatile unsigned int usb_out_ring_rd_index;
extern volatile unsigned int usb_out_ring_wr_index;

//      IOT
extern volatile char iot_in_ring_data[ringSize];
extern volatile unsigned int iot_in_ring_wr_index;
extern volatile char iot_out_ring_data[ringSize];
extern volatile unsigned int iot_out_ring_rd_index;
extern volatile unsigned int iot_out_ring_wr_index;

//=============================================================================
// Function:    eUSCI_A0_ISR
// Description: ISR for serial recieving and transmitting on the A0 port
// Arguments:   None
// Globals:     None
// Locals:      temp
// Return:      None
// Pins:        None
//=============================================================================
#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
  unsigned int temp;
  unsigned int temp2;
  
  switch(__even_in_range(UCA0IV,0x08)){
    case 0:                                             // Vector 0 - no interrupt
      
      break;
    case 2:                                             // Vector 2 - RXIFG
      temp = iot_in_ring_wr_index;
      temp2 = usb_out_ring_wr_index;
      iot_in_ring_data[temp] = UCA0RXBUF;                    // RX -> USB_Char_Rx character
      usb_out_ring_data[temp2] = iot_in_ring_data[temp];
      if(usb_out_ring_data[temp2] == '\r') {
        UCA1IE |= UCTXIE;
      }
      Index_Incrementer(iotInRingWrIndex);
      Index_Incrementer(usbOutRingWrIndex);
      break;
    case 4:                                             // Vector 4 – TXIFG
      temp = iot_out_ring_rd_index;
      UCA0TXBUF = iot_out_ring_data[temp];             // Transmit Current Indexed value
      iot_out_ring_data[temp] = '\0';                    // Null Location of Transmitted value
      Index_Incrementer(iotOutRingRdIndex);
      temp = iot_out_ring_rd_index;
      if(iot_out_ring_data[temp] == '\0'){             // Is the next pb_index location NULL - End of Command
        UCA0IE &= ~UCTXIE;                              // Disable TX interrupt
      }
      break;
    default: break;
  }
}

//=============================================================================
// Function:    eUSCI_A1_ISR
// Description: ISR for serial recieving and transmitting on the A1 port
// Arguments:   None
// Globals:     None
// Locals:      temp, temp2
// Return:      None
// Pins:        None
//=============================================================================
#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
  unsigned int temp;
  unsigned int temp2;
  
  switch(__even_in_range(UCA1IV,0x08)){
    case 0:                                             // Vector 0 - no interrupt
      
      break;
    case 2:                                             // Vector 2 - RXIFG
      temp = usb_in_ring_wr_index;
      temp2 = iot_out_ring_wr_index;
      usb_in_ring_data[temp] = UCA1RXBUF;                    // RX -> USB_Char_Rx character 
      iot_out_ring_data[temp2] = usb_in_ring_data[temp];
      if(iot_out_ring_data[temp2] == '\r') {
        UCA0IE |= UCTXIE;
      }
      Index_Incrementer(usbInRingWrIndex);
      Index_Incrementer(iotOutRingWrIndex);
      break;
    case 4:                                             // Vector 4 – TXIFG
      temp = usb_out_ring_rd_index;
      UCA1TXBUF = usb_out_ring_data[temp];             // Transmit Current Indexed value
      usb_out_ring_data[temp] = '\0';                    // Null Location of Transmitted value
      Index_Incrementer(usbOutRingRdIndex);
      temp = usb_out_ring_rd_index;
      if(usb_out_ring_data[temp] == '\0'){             // Is the next pb_index location NULL - End of Command
        UCA1IE &= ~UCTXIE;                              // Disable TX interrupt
      }
      break;
    default: break;
  }
}