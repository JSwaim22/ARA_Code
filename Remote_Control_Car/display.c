//=============================================================================
// File Name: display.c
//
// Description: This file includes the functions for handling display changes
//
// Author: Joshua Swaim
// Date: April 2021
// Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (7.20.1)
//=============================================================================

#include "macros.h"

// Globals
extern char display_line[displayRows][displayColumns];
extern char *display[displayRows];
extern volatile unsigned char update_display;
extern volatile unsigned char display_changed;

//=============================================================================
// Function:    Init_Display
// Description: This function serves to initialize the display
// Arguments:   None
// Globals:     display_line[][], display[], update_display
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Init_Display(void){
    for(int i = displayColumnZero; i < displayColumnEleven; i++){
        display_line[displayRowZero][i] = RESET_STATE;
        display_line[displayRowOne][i] = RESET_STATE;
        display_line[displayRowTwo][i] = RESET_STATE;
        display_line[displayRowThree][i] = RESET_STATE;
    }
    display_line[displayRowZero][displayColumnTen] = displayZero;
    display_line[displayRowOne][displayColumnTen] = displayZero;
    display_line[displayRowTwo][displayColumnTen] = displayZero;
    display_line[displayRowThree][displayColumnTen] = displayZero;
    display[displayRowZero] = &display_line[displayRowZero][displayColumnZero];
    display[displayRowOne] = &display_line[displayRowOne][displayColumnZero];
    display[displayRowTwo] = &display_line[displayRowTwo][displayColumnZero];
    display[displayRowThree] = &display_line[displayRowThree][displayColumnZero];
    update_display = displayUpdated;
    enable_interrupts();
    enable_display_update();
}

//=============================================================================
// Function:    Clear_Display
// Description: This function clear the display
// Arguments:   None
// Globals:     display_line[][], display[], update_display
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Clear_Display(void) {
  for(int i = displayRowZero; i < displayRows; i++) {
    strcpy(display_line[i], "          ");
    update_string(display_line[i], i);
  }
}

//=============================================================================
// Function:    Set_Display
// Description: This function sets up the initial message on the display
// Arguments:   None
// Globals:     display_line[][], display[], update_display
// Locals:      None
// Return:      None
// Pins:        None
//=============================================================================
void Set_Display(void) {
    strcpy(display_line[displayRowZero], "   ncsu   ");
    update_string(display_line[displayRowZero], displayRowZero);
    strcpy(display_line[displayRowOne], "IP Address");
    update_string(display_line[displayRowOne], displayRowOne);
    strcpy(display_line[displayRowTwo], "  10.153. ");
    update_string(display_line[displayRowTwo], displayRowTwo);
    strcpy(display_line[displayRowThree], "   52.50  ");
    update_string(display_line[displayRowThree], displayRowThree);
    display_changed = displayChangedHappened;
}