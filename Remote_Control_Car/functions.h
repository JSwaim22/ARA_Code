//=============================================================================
// File Name: functions.h
//
// Description: This file contains the function prototypes
//
// Author: Joshua Swaim
// Date: Mar 2021
// Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (7.20.1)
//=============================================================================

// #define psuedo Functions
#define Red_Off() (P1OUT &= ~RED_LED)
#define Red_On()  (P1OUT |= RED_LED)
#define Red_Tgl() (P1OUT ^= RED_LED)

#define Green_Off() (P6OUT &= ~GRN_LED)
#define Green_On()  (P6OUT |= GRN_LED)
#define Green_Tgl() (P6OUT ^= GRN_LED)

#define IR_Off() (P2OUT &= ~IR_LED)
#define IR_On()  (P2OUT |= IR_LED)
#define IR_Tgl() (P2OUT ^= IR_LED)

#define LCD_Off() (P6OUT &= ~LCD_BACKLIGHT)
#define LCD_On()  (P6OUT |= LCD_BACKLIGHT)
#define LCD_Tgl() (P6OUT ^= LCD_BACKLIGHT)


// Functions
void Software_Trim(void);

// Initialization
void Init_Conditions(void);

// Interrupts
void enable_interrupts(void);

// Clocks
void Init_Clocks(void);

// LED Configurations
void Init_LEDs(void);
void IR_LED_control(char selection);
void Backlite_control(char selection);

  // LCD
void Init_Display(void);
void Clear_Display(void);
void Set_Display(void);
void Display_Process(void);
void Display_Update(char p_L1,char p_L2,char p_L3,char p_L4);
void enable_display_update(void);
void update_string(char *string_data, int string);
void Init_LCD(void);
void lcd_clear(void);
void lcd_putc(char c);
void lcd_puts(char *s);
void lcd_power_on(void);
void lcd_write_line1(void);
void lcd_write_line2(void);
//void lcd_draw_time_page(void);
//void lcd_power_off(void);
void lcd_enter_sleep(void);
void lcd_exit_sleep(void);
//void lcd_write(unsigned char c);
//void out_lcd(unsigned char c);
void Write_LCD_Ins(char instruction);
void Write_LCD_Data(char data);
void ClrDisplay(void);
void ClrDisplay_Buffer_0(void);
void ClrDisplay_Buffer_1(void);
void ClrDisplay_Buffer_2(void);
void ClrDisplay_Buffer_3(void);
void SetPostion(char pos);
void DisplayOnOff(char data);
void lcd_BIG_mid(void);
void lcd_4line(void);
void lcd_out(char *s, char line, char position);
void lcd_rotate(char view);
//void lcd_write(char data, char command);
void lcd_write(unsigned char c);
void lcd_write_line1(void);
void lcd_write_line2(void);
void lcd_write_line3(void);
void lcd_command( char data);
void LCD_test(void);
void LCD_iot_meassage_print(int nema_index);

// Ports
void Init_Ports(void);
void Init_Port1(void);
void Init_Port2(void);
void Init_Port3(void);
void Init_Port4(void);
void Init_Port5(void);
void Init_Port6(void);

// SPI
void Init_SPI_B1(void);
void SPI_B1_write(char byte);
void spi_rs_data(void);
void spi_rs_command(void);
void spi_LCD_idle(void);
void spi_LCD_active(void);
void SPI_test(void);
void WriteIns(char instruction);
void WriteData(char data);

// Switches
void Switches_Timer_Start(void);
void Switches_Timer_Stop(void);
void Switches_Interrupt_Start(void);
void Switches_Interrupt_Stop(void);
__interrupt void Switch1_Process(void);
__interrupt void Switch2_Process(void);

// Timers
void Init_Timers(void);
void Init_Timer_B0(void);
void Init_Timer_B1(void);
void Init_Timer_B2(void);
void Init_Timer_B3(void);
__interrupt void TimerB0CCR0_ISR(void);
__interrupt void TimerB0CCR12OVFL_ISR(void);

// ADC
void Init_ADC(void);
void ADC_L_On(void);
void ADC_R_On(void);
void ADC_T_On(void);
void ADC_Advance_Index(void);
void ADC_Conversion_Switch(void);
void ADC_Timer_Start(void);
void ADC_Timer_Stop(void);
void ADC_Display(void);
void Val_To_String(int value, int start_bit);

// DAC
void Init_DAC(void);
void Init_REF(void);
void DAC_Timer_Start(void);
void DAC_Timer_Continue(void);
void DAC_Timer_Stop(void);

// States
void State_Process(void);
void State_Change(void);
void States_Timer_Start(void);
void States_Timer_Continue(void);
void States_Timer_Stop(void);
void Menu(void);
void Left(void);
void Left_Black(void);
void Right(void);
void Straight(void);
void Right_Black(void);
void Forward_A(void);
void Forward_B(void);
void Forward_C(void);
void Forward_D(void);
void Turn_A(void);
void Turn_B(void);
void Pause(void);
void Spin(void);
void Circling(void);
void States_Line_Check(void);

// Wheels
void Wheels_Process(void);
void Error_Process(void);
void All_Forward(void);
void Left_Forward(void);
void Right_Forward(void);
void All_Reverse(void);
void Left_Reverse(void);
void Right_Reverse(void);
void Left_Spin(void);
void Right_Spin(void);
void All_Stop(void);
void Wheels_Timer_Start(void);
void Wheels_Timer_Stop(void);

// Serial
void Init_Serial(void);
void Init_Serial_UCA0(void);
void Init_Serial_UCA1(void);
void Index_Incrementer(int what_index);
void USB_Array_Setup(void);
void IOT_Array_Setup(void);
void Serial_Process(void);
void IOT_Serial_Process(void);
void USB_Serial_Process(void);
void Command_Process(void);
void Send_Open_Port_Message(void);

// Consider commenting
void usleep(unsigned int usec);
void usleep10(unsigned int usec);
void five_msec_sleep(unsigned int msec);
void measure_delay(void);
void out_control_words(void);