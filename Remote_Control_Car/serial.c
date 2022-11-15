//=============================================================================
// File Name: serial.c
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
volatile char usb_in_ring_data[ringSize];
volatile unsigned int usb_in_ring_rd_index = start;
volatile unsigned int usb_in_ring_wr_index = start;
volatile char usb_out_ring_data[ringSize];
volatile unsigned int usb_out_ring_rd_index = start;
volatile unsigned int usb_out_ring_wr_index = start;

//      IOT
volatile char iot_in_ring_data[ringSize];
volatile unsigned int iot_in_ring_rd_index = start;
volatile unsigned int iot_in_ring_wr_index = start;
volatile char iot_out_ring_data[ringSize];
volatile unsigned int iot_out_ring_rd_index = start;
volatile unsigned int iot_out_ring_wr_index = start;
unsigned int iot_clearance = rando;

//      Other
char command_direction;
char command_time;
extern unsigned int state_count_limit;
extern char display_line[displayRows][displayColumns];
extern volatile unsigned char display_changed;
volatile unsigned int running = FALSE;
volatile unsigned int queued = FALSE;
volatile unsigned int start_state = FALSE;

//=============================================================================
// Function:    Command_Process
// Description: Processes the incoming commands
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Command_Process(void) {
  // F = all forward
  // G = left forward
  // H = right forward
  // B = all backwards
  // C = left backwards
  // D = right backwards
  // L = left spin
  // R = right spin
  // S = stop
  // N = none
  running = TRUE;
  queued = FALSE;
  switch (command_direction) {
    case 'N':
      break;
    case 'X':
      command_direction = 'N';
      start_state = TRUE;
      Clear_Display();
      strcpy(display_line[displayRowOne], "Black Line");
      update_string(display_line[displayRowOne], displayRowOne);
      lcd_BIG_mid();
      display_changed = displayChangedHappened;
      break;
    case 'F':
      command_direction = 'N';
      Clear_Display();
      strcpy(display_line[displayRowOne], "  FORWARD ");
      update_string(display_line[displayRowOne], displayRowOne);
      lcd_BIG_mid();
      display_changed = displayChangedHappened;
      All_Forward();
      state_count_limit = (command_time - '0') * 10;
      States_Timer_Start();
      break;
    case 'G':
      command_direction = 'N';
      Clear_Display();
      strcpy(display_line[displayRowOne], "   LEFT   ");
      update_string(display_line[displayRowOne], displayRowOne);
      lcd_BIG_mid();
      display_changed = displayChangedHappened;
      Left_Forward();
      state_count_limit = (command_time - '0') * 10;
      States_Timer_Start();
      break;
    case 'H':
      command_direction = 'N';
      Clear_Display();
      strcpy(display_line[displayRowOne], "  RIGHT   ");
      update_string(display_line[displayRowOne], displayRowOne);
      lcd_BIG_mid();
      display_changed = displayChangedHappened;
      Right_Forward();
      state_count_limit = (command_time - '0') * 10;
      States_Timer_Start();
      break;
    case 'B':
      command_direction = 'N';
      Clear_Display();
      strcpy(display_line[displayRowOne], "  REVERSE ");
      update_string(display_line[displayRowOne], displayRowOne);
      lcd_BIG_mid();
      display_changed = displayChangedHappened;
      All_Reverse();
      state_count_limit = (command_time - '0') * 10;
      States_Timer_Start();
      break;
    case 'C':
      command_direction = 'N';
      Clear_Display();
      strcpy(display_line[displayRowOne], "   LEFT   ");
      update_string(display_line[displayRowOne], displayRowOne);
      lcd_BIG_mid();
      display_changed = displayChangedHappened;
      Left_Reverse();
      state_count_limit = (command_time - '0') * 10;
      States_Timer_Start();
      break;
    case 'D':
      command_direction = 'N';
      Clear_Display();
      strcpy(display_line[displayRowOne], "  RIGHT   ");
      update_string(display_line[displayRowOne], displayRowOne);
      lcd_BIG_mid();
      display_changed = displayChangedHappened;
      Right_Reverse();
      state_count_limit = (command_time - '0') * 10;
      States_Timer_Start();
      break;
    case 'L':
      command_direction = 'N';
      Clear_Display();
      strcpy(display_line[displayRowOne], "   SPIN   ");
      update_string(display_line[displayRowOne], displayRowOne);
      lcd_BIG_mid();
      display_changed = displayChangedHappened;
      Left_Spin();
      state_count_limit = (command_time - '0') * 10;
      States_Timer_Start();
      break;
    case 'R':
      command_direction = 'N';
      Clear_Display();
      strcpy(display_line[displayRowOne], "   SPIN   ");
      update_string(display_line[displayRowOne], displayRowOne);
      lcd_BIG_mid();
      display_changed = displayChangedHappened;
      Right_Spin();
      state_count_limit = (command_time - '0') * 10;
      States_Timer_Start();
      break;
    case 'S':
      command_direction = 'N';
      Clear_Display();
      strcpy(display_line[displayRowOne], "   STOP   ");
      update_string(display_line[displayRowOne], displayRowOne);
      lcd_BIG_mid();
      display_changed = displayChangedHappened;
      All_Stop();
      state_count_limit = (command_time - '0') * 10;
      States_Timer_Start();
      break;
    default: break;
  }
}


//=============================================================================
// Function:    Serial_Process
// Description: Processes the incoming serial data
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Serial_Process(void) {
  USB_Serial_Process();
  IOT_Serial_Process();
}

//=============================================================================
// Function:    USB_Serial_Process
// Description: Processes the incoming usb serial data
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void USB_Serial_Process(void) {
  //    Process
}

//=============================================================================
// Function:    IOT_Serial_Process
// Description: Processes the incoming iot serial data
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void IOT_Serial_Process(void) {
  unsigned int temp;
  temp = iot_in_ring_rd_index;
  
  if(iot_in_ring_wr_index != temp) {
    
    // ***MAX SECURITY PROTOCOL*** HIGH LEVEL CLEARANCE REQUIRED
    switch (iot_clearance) {
      case rando:
        if(iot_in_ring_data[temp] == '=' || iot_in_ring_data[temp] == 'N') {
          iot_clearance = trainee;
        }
        break;
      case trainee:
        if(iot_in_ring_data[temp] == '|' || iot_in_ring_data[temp] == 'W') {
          iot_clearance = fieldAgent;
        } else {
          iot_clearance = rando;
        }
        break;
      case fieldAgent:
        if(iot_in_ring_data[temp] == ':' || iot_in_ring_data[temp] == 'C') {
          iot_clearance = specialAgent;
        } else {
          iot_clearance = rando;
        }
        break;
      case specialAgent:
        if(iot_in_ring_data[temp] == '{' || iot_in_ring_data[temp] == 'O') {
          iot_clearance = director;
        } else {
          iot_clearance = rando;
        }
        break;
      case director:
        if(iot_in_ring_data[temp] == ' ' || iot_in_ring_data[temp] == 'N') {
          iot_clearance = executive;
        } else {
          iot_clearance = rando;
        }
        break;
      case executive:
        if(iot_in_ring_data[temp] == '>' || iot_in_ring_data[temp] == 'N') {
          iot_clearance = president;
        } else {
          iot_clearance = rando;
        }
        break;
      case president:
        if(iot_in_ring_data[temp] != '-') {  
          command_direction = iot_in_ring_data[temp];
        }
        iot_clearance = aliens;
        break;
      case aliens:
        if(iot_in_ring_data[temp] == 'S') {
          Send_Open_Port_Message();
        } else if(iot_in_ring_data[temp] == 'F') {
          //reconnecting command here
        } else {
          command_time = iot_in_ring_data[temp];
          if(!running) {
            Command_Process();
          } else {
            queued = TRUE;
          }
        }
        iot_clearance = rando;
        break;
      default: break;
    }
    iot_in_ring_data[temp] = '\n';
    Index_Incrementer(iotInRingRdIndex);
  }
}

//=============================================================================
// Function:    Init_Serial
// Description: Initializes the Serial Ports
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Init_Serial(void) {
  Init_Serial_UCA0();
  Init_Serial_UCA1();
}

//=============================================================================
// Function:    Init_Serial_UCA1
// Description: Initializes the UCA1 Serial Port
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Init_Serial_UCA0(void){
  IOT_Array_Setup();
  
  // Configure UART 0
  UCA0CTLW0 = 0;                                // Use word register
  UCA0CTLW0 |= UCSWRST;                         // Set Software reset enable
  UCA0CTLW0 |= UCSSEL__SMCLK;                   // Set SMCLK as fBRCLK
  UCA0BRW = 4;                                 // 115,200 Baud
  UCA0MCTLW = 0x5551;
  UCA0CTLW0 &= ~UCSWRST;                       // Set Software reset enable
  UCA0IE |= UCRXIE;                             // Enable RX interrupt
  UCA0TXBUF = 0X00;                             // Priming the pump
}
  
//=============================================================================
// Function:    Init_Serial_UCA1
// Description: Initializes the UCA1 Serial Port
// Arguments:   None
// Globals:     None
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Init_Serial_UCA1(void){
  USB_Array_Setup();
  
  // Configure UART 0
  UCA1CTLW0 = 0;                                // Use word register
  UCA1CTLW0 |= UCSWRST;                         // Set Software reset enable
  UCA1CTLW0 |= UCSSEL__SMCLK;                   // Set SMCLK as fBRCLK
  UCA1BRW = 4;                                 // 115,200 Baud
  UCA1MCTLW = 0x5551;
  UCA1CTLW0 &= ~UCSWRST;                       // Set Software reset enable
  UCA1IE |= UCRXIE;                             // Enable RX interrupt
  UCA1TXBUF = 0X00;                             // Priming the pump
}

//=============================================================================
// Function:    IOT_Array_Setup
// Description: Sets up the arrays for the IOT serial port
// Arguments:   None
// Globals:     iot_in_proc_data[], iot_in_ring_data[], iot_out_ring_data[]
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void IOT_Array_Setup(void) {
  for(int i = start; i < ringSize; i++){
      iot_in_ring_data[i] = '\0';
      iot_out_ring_data[i] = '\0';
  }
}

//=============================================================================
// Function:    USB_Array_Setup
// Description: Sets up the arrays for the USB serial port
// Arguments:   None
// Globals:     usb_in_proc_data[], usb_in_ring_data[], usb_out_ring_data[]
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void USB_Array_Setup(void) {
  for(int i = start; i < ringSize; i++){
      usb_in_ring_data[i] = '\0';
      usb_out_ring_data[i] = '\0';
  }
}

//=============================================================================
// Function:    Index_Incrementer
// Description: Increments the index of each of the serial arrays
// Arguments:   what_index
// Globals:     usb_in_ring_rd_index, usb_in_ring_wr_index, usb_in_proc_rd_index, 
//              usb_in_proc_wr_index, usb_out_ring_rd_index, usb_out_ring_wr_index,
//              iot_in_ring_rd_index, iot_in_ring_wr_index, iot_in_proc_rd_index, 
//              iot_in_proc_wr_index, iot_out_ring_rd_index, iot_out_ring_wr_index
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Index_Incrementer(int what_index) {
  switch (what_index) {
    case usbInRingRdIndex: 
      usb_in_ring_rd_index++;
      if(usb_in_ring_rd_index == ringSize) {
        usb_in_ring_rd_index = start;
      }
      break;
    case usbInRingWrIndex:
      usb_in_ring_wr_index++;
      if(usb_in_ring_wr_index == ringSize) {
        usb_in_ring_wr_index = start;
      }
      break;
    case usbOutRingRdIndex:
      usb_out_ring_rd_index++;
      if(usb_out_ring_rd_index == ringSize) {
        usb_out_ring_rd_index = start;
      }
      break;
    case usbOutRingWrIndex:
      usb_out_ring_wr_index++;
      if(usb_out_ring_wr_index == ringSize) {
        usb_out_ring_wr_index = start;
      }
      break;
    case iotInRingRdIndex: 
      iot_in_ring_rd_index++;
      if(iot_in_ring_rd_index == ringSize) {
        iot_in_ring_rd_index = start;
      }
      break;
    case iotInRingWrIndex:
      iot_in_ring_wr_index++;
      if(iot_in_ring_wr_index == ringSize) {
        iot_in_ring_wr_index = start;
      }
      break;
    case iotOutRingRdIndex:
      iot_out_ring_rd_index++;
      if(iot_out_ring_rd_index == ringSize) {
        iot_out_ring_rd_index = start;
      }
      break;
    case iotOutRingWrIndex:
      iot_out_ring_wr_index++;
      if(iot_out_ring_wr_index == ringSize) {
        iot_out_ring_wr_index = start;
      }
      break;
    default: break;
  }
}

void Send_Open_Port_Message(void) {
  iot_out_ring_data[iot_out_ring_wr_index] = 'A';
  Index_Incrementer(iotOutRingWrIndex);
  iot_out_ring_data[iot_out_ring_wr_index] = 'T';
  Index_Incrementer(iotOutRingWrIndex);
  iot_out_ring_data[iot_out_ring_wr_index] = '+';
  Index_Incrementer(iotOutRingWrIndex);
  iot_out_ring_data[iot_out_ring_wr_index] = 'N';
  Index_Incrementer(iotOutRingWrIndex);
  iot_out_ring_data[iot_out_ring_wr_index] = 'S';
  Index_Incrementer(iotOutRingWrIndex);
  iot_out_ring_data[iot_out_ring_wr_index] = 'T';
  Index_Incrementer(iotOutRingWrIndex);
  iot_out_ring_data[iot_out_ring_wr_index] = 'C';
  Index_Incrementer(iotOutRingWrIndex);
  iot_out_ring_data[iot_out_ring_wr_index] = 'P';
  Index_Incrementer(iotOutRingWrIndex);
  iot_out_ring_data[iot_out_ring_wr_index] = '=';
  Index_Incrementer(iotOutRingWrIndex);
  iot_out_ring_data[iot_out_ring_wr_index] = '2';
  Index_Incrementer(iotOutRingWrIndex);
  iot_out_ring_data[iot_out_ring_wr_index] = '2';
  Index_Incrementer(iotOutRingWrIndex);
  iot_out_ring_data[iot_out_ring_wr_index] = '2';
  Index_Incrementer(iotOutRingWrIndex);
  iot_out_ring_data[iot_out_ring_wr_index] = '2';
  Index_Incrementer(iotOutRingWrIndex);
  iot_out_ring_data[iot_out_ring_wr_index] = '2';
  Index_Incrementer(iotOutRingWrIndex);
  iot_out_ring_data[iot_out_ring_wr_index] = ',';
  Index_Incrementer(iotOutRingWrIndex);
  iot_out_ring_data[iot_out_ring_wr_index] = '1';
  Index_Incrementer(iotOutRingWrIndex);
  iot_out_ring_data[iot_out_ring_wr_index] = '\r';
  Index_Incrementer(iotOutRingWrIndex);
  UCA0IE |= UCTXIE;
}
  