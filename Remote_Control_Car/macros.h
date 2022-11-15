//=============================================================================
// File Name: macros.h
//
// Description: This file includes the other .h files and the #define values
//
// Author: Joshua Swaim
// Date: Mar 2021
// Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (7.20.1)
//=============================================================================


// Add macros for serial


// .h files
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "ports.h"

// Universal
#define TRUE                    (1)
#define FALSE                   (0)
#define ON                      (1)
#define OFF                     (0)
#define reset                   (0)
#define clear                   (0)
#define start                   (0)

// from main.c
#define displayRows             (4)
#define displayColumns          (11)
#define displayRowZero          (0)
#define displayRowOne           (1)
#define displayRowTwo           (2)
#define displayRowThree         (3)
#define displayColumnZero       (0)
#define displayColumnTen        (10)
#define displayColumnEleven     (11)
#define displayUpdated          (0)
#define displayZero             (0)
#define occurred                (0)
#define displayChangedHappened  (1)
#define ALWAYS                  (1)
#define RESET_STATE             (0)

// from states.c
#define start                   (0)
#define forwardA                (1)
#define leftward                (2)
#define straight                (3)
#define rightward               (4)
#define rightBlack              (5)
#define pause                   (6)
#define forwardB                (7)
#define forwardC                (8)
#define spin                    (9)
#define turnA                   (10)
#define turnB                   (11)
#define forwardD                (12)
#define tenthSec                (1)
#define twoTenthsSec            (3)
#define halfSec                 (5)
#define ninthSec                (9)
#define oneSec                  (10)
#define oneHalfSec              (15)
#define twoSec                  (25)
#define threeSec                (30)
#define sevenSec                (70)
#define fiveSec                 (50)
#define clear                   (0)
#define darkLevel               (650)
#define leftDarkLevel           (750)
#define rightDarkLevel          (500)


// from wheels.c
#define allForward              (1)
#define leftForward             (2)
#define rightForward            (3)
#define allReverse              (4)
#define leftReverse             (5)
#define rightReverse            (6)
#define leftSpin                (7)
#define rightSpin               (8)
#define leftFast                (50000)
#define rightFast               (25000)
#define leftSlow                (10000)
#define rightSlow               (11000)
#define leftRealSlow            (7500)
#define rightRealSlow           (7900)
#define leftMed                 (15000)
#define rightMed                (17500)

// from timers.c
#define timerB0CCR0Vector       (TIMER0_B0_VECTOR)
#define timerB0CCR12OVFLVECTOR  (TIMER0_B1_VECTOR)
#define TB0CCR0_INTERVAL        (25000)
#define TB0CCR1_INTERVAL        (62500)
#define timerB1CCR0Vector       (TIMER1_B0_VECTOR)
#define timerB1CCR12OVFLVECTOR  (TIMER1_B1_VECTOR)
#define TB1CCR0_INTERVAL        (3125)
#define TB1CCR1_INTERVAL        (6250)
#define TB1CCR2_INTERVAL        (12500)
#define timerB2CCR0Vector       (TIMER2_B0_VECTOR)
#define timerB2CCR12OVFLVECTOR  (TIMER2_B1_VECTOR)
#define TB2CCR0_INTERVAL        (62500)
#define timerB3CCR0Vector       (TIMER3_B0_VECTOR)
#define timerB3CCR12OVFLVECTOR  (TIMER3_B1_VECTOR)
#define right_forward_speed     (TB3CCR1)
#define left_forward_speed      (TB3CCR2)
#define right_reverse_speed     (TB3CCR3)
#define left_reverse_speed      (TB3CCR4)
#define wheelPeriod             (50010)

// from ints_timers.c
#define NONE                    (0)
#define CCR1                    (2)
#define CCR2                    (4)
#define OVFL                    (14)

// from adc.c
#define stringSize              (11)
#define dataSize                (8)
#define thumb                   (0)
#define left                    (1)
#define right                   (2)
#define dataStart               (0)
#define dataEnd                 (7)
#define averager                (3)
#define stringBitZero           (0)
#define stringBitOne            (1)
#define stringBitTwo            (2)
#define stringBitFive           (5)
#define stringBitSix            (6)
#define stringBitSeven          (7)
#define quadrupleDigit          (1000)
#define tripleDigit             (100)
#define doubleDigit             (10)
#define asciBase                (0x30)

// from serial.c
#define ringSize                (32)
#define usbInRingRdIndex        (3)
#define usbInRingWrIndex        (4)
#define usbInProcRdIndex        (5)
#define usbInProcWrIndex        (6)
#define usbOutRingRdIndex       (7)
#define usbOutRingWrIndex       (8)
#define iotInRingRdIndex        (9)
#define iotInRingWrIndex        (10)
#define iotInProcRdIndex        (11)
#define iotInProcWrIndex        (12)
#define iotOutRingRdIndex       (13)
#define iotOutRingWrIndex       (14)
#define rando                   (1)
#define trainee                 (2)
#define fieldAgent              (3)
#define specialAgent            (4)
#define director                (5)
#define executive               (6)
#define president               (7)
#define aliens                  (8)

// from clocks.c
#define MCLK_FREQ_MHZ           8                     // MCLK = 8MHz
#define CLEAR_REGISTER          (0X0000)