//General libraries
    #include <analogWrite.h>  //Needed for using PWM on the ESP32, can be downloaded here https://github.com/ERROPiX/ESP32_AnalogWrite
    #include <SPI.h>          //Included in the ESP32 Arduino core found here https://github.com/espressif/arduino-esp32
    #include<Wire.h>          //Included in the ESP32 Arduino core found here https://github.com/espressif/arduino-esp32
//----------------//

//Inclduing the display library, setting it up, and initializing it
    #include <Adafruit_GFX.h>      //Find by searching Adafruit GFX in the libraray manager
    #include <Adafruit_SSD1306.h>  //Find by seraching Adafruit SSD1306 in the library manager
    #define SCREEN_WIDTH 128 
    #define SCREEN_HEIGHT 64 
    #define OLED_RESET     4 
    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//----------------------------------------------------------------//

//Inclduing the distance sensor library and initializing it
    #include "Adafruit_VL6180X.h"   //Find at this link https://github.com/adafruit/Adafruit_VL6180X
    Adafruit_VL6180X vl = Adafruit_VL6180X();
//--------------------------------------------------------//

//Defining a variable for using the light sensor
    #define Addr 0x4A
//----------------------------------------------//

//Variables
    int button = 0;     //Keeps track of the state of the button
    int pot = 0;        //Holds the initial state of the potentiometer
    int pot2 = 0;       //Holds the updated state of the potentiometer to be compared to pot
    int timer = 0;      //Keeps track of time
    int counter = 1;    //Keeps track of iterations of the distance value loops
    int motion = 0;     //Holds the state of the motion sensor
    int LV = 0;         //Holds the previous light value to fade from
    int LV2 = 0;        //Holds the new light value to fade to
    int fademax = 10;   //How "smooth" the lights fade in and out
//---------//

//Settings
    int LSLV = 0;     //Light sensor lower value: the lowest value the light sensor measures
    int LSUV = 100;   //Light sensor upper value: the highest value the light sensor measures
    int LLV = 75;     //Lower light value: the lowest brightness the lights can shine
    int LUV = 255;    //Upper light value: the highest brightness the lights can shine
    int DV = 100;     //Distance value: the distance at which the lights turn on
    int TV = 30;      //Time value: the time in seconds that the lights stay on without being triggered by the motion sensor
//-------//

//Pins
    int motionsensorpin = 17;
    int lightspin = 16;
    int buttonpin = 4;
    int potpin = 34;
//---//

void setup()
{

//Setting up the pins
    pinMode(motionsensorpin, INPUT_PULLUP);
    pinMode(lightspin, OUTPUT);
    pinMode(buttonpin, INPUT_PULLUP);
    pinMode(potpin, INPUT);
//------------------//

//Showing the screens beginning animation and clearing it
    display.display();
    delay(2000);
    display.clearDisplay();
    display.display();
    delay(2000);
//-----------------------------------------------------//

//Setting up the light sensor
    Wire.begin();
    Wire.beginTransmission(Addr);
    Wire.write(0x02);
    Wire.write(0x40);
    Wire.endTransmission();
    delay(300);
//-------------------------//

}
 
void loop()
{ 

  //Keeping track of time
  timer = timer + 250;
  
  //Clearing display and resetting cursor with appropriate color and size
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  //Measuring the distance value from the sensor
  uint8_t distance = vl.readRange();
  uint8_t status = vl.readRangeStatus();
  if (status == VL6180X_ERROR_NONE) {  //Checking to see that there are no errors
    display.print(F("D:")); display.println(distance);  //Printing the distance value to the screen
  } 
  else {
    display.println("D:Error"); //Printing if there is an error with reading the distance value 
                                //(this will be displayed if the distance is behyond the sensor's range)
  }
  
  //Measuring the ambient light level from the sensor
  unsigned int data[2];
  Wire.beginTransmission(Addr);
  Wire.write(0x03);
  Wire.endTransmission();
  Wire.requestFrom(Addr, 2);
  if (Wire.available() == 2)  {
      data[0] = Wire.read();
      data[1] = Wire.read();
    }
  int exponent = (data[0] & 0xF0) >> 4;
  int mantissa = ((data[0] & 0x0F) << 4) | (data[1] & 0x0F);
  float LSV = pow(2, exponent) * mantissa * 0.045;
  display.print(F("L:"));
  display.print(LSV);       //Printing the light value to the screen
  
  display.display();   //Updating the screen
  
  //Settings Loop
      button = digitalRead(buttonpin);  //Checking the button
      
      if(button == LOW) {   //If the button is pressed...
        
        while(button == LOW) {    //While the button is being pressed...
          button = digitalRead(buttonpin);  //Check the button 
          delay(20);                        //Wait
        }
      
        pot = analogRead(potpin);   //The variable pot is set to the current position of the potentiometer
        
        while(button == HIGH) {       //While the button is released...
          button = digitalRead(buttonpin);      //Check the button 
          pot2 = analogRead(potpin);        //Set another variable pot2 equal to the new position of the potentiometer
          if(!(pot2 <= (pot+100) && pot2 >= (pot-100))) {  //If pot2 if much different than pot...
              LSLV = map(pot2, 0, 4095, 0, 200);               //The light sensor lower value (LSLV) is set according to the value of the potentiometer
              pot = 5000;                                      //Making pot a very high number so the if statement now always triggers
          }
          display.clearDisplay();     //Clearing the display
          display.setCursor(0,0);     //Setting the cursor
          display.print(F("LSLV:"));  
          display.print(LSLV);        //Printing the current value set by the potentiometer
          display.display();          //Updating the display
          delay(100);                 //Pausing
        }
        
        while(button == LOW) {    //While the button is being pressed...
          button = digitalRead(buttonpin);  //Check the button 
          delay(20);                        //Wait
        }
      
        pot = analogRead(potpin);   //The variable pot is set to the current position of the potentiometer
          
        while(button == HIGH) {       //While the button is released...
          button = digitalRead(buttonpin);      //Check the button 
          pot2 = analogRead(potpin);        //Set another variable pot2 equal to the new position of the potentiometer
          if(!(pot2 <= (pot+100) && pot2 >= (pot-100))) {  //If pot2 if much different than pot...
              LSUV = map(pot2, 0, 4095, 1, 200);               //The light sensor upper value (LSUV) is set according to the value of the potentiometer
              pot = 5000;                                      //Making pot a very high number so the if statement now always triggers
          }
          display.clearDisplay();     //Clearing the display
          display.setCursor(0,0);     //Setting the cursor
          display.print(F("LSUV:"));
          display.print(LSUV);        //Printing the value set by the potentiometer
          display.display();          //Updating the display
          delay(100);                 //Pausing
        }
        
        while(button == LOW) {    //While the button is being pressed...
          button = digitalRead(buttonpin);  //Check the button 
          delay(20);                        //Wait
        }
      
        pot = analogRead(potpin);   //The variable pot is set to the current position of the potentiometer
          
        while(button == HIGH) {       //While the button is released...
          button = digitalRead(buttonpin);      //Check the button 
          pot2 = analogRead(potpin);        //Set another variable pot2 equal to the new position of the potentiometer
          if(!(pot2 <= (pot+100) && pot2 >= (pot-100))) {  //If pot2 if much different than pot...
              LLV = map(pot2, 0, 4095, 0, 255);               //The light lower value (LLV) is set according to the value of the potentiometer
              pot = 5000;                                      //Making pot a very high number so the if statement now always triggers
          }
          display.clearDisplay();     //Clearing the display
          display.setCursor(0,0);     //Setting the cursor
          display.print(F("LLV:"));
          display.print(LLV);         //Printing the value set by the potentiometer
          display.display();          //Updating the display
          analogWrite(lightspin, LLV);       //Setting the lights to LLV
          delay(100);                 //Pausing
        }
        
        while(button == LOW) {    //While the button is being pressed...
          button = digitalRead(buttonpin);  //Check the button 
          delay(20);                        //Wait
        }
      
        pot = analogRead(potpin);   //The variable pot is set to the current position of the potentiometer
          
        while(button == HIGH) {       //While the button is released...
          button = digitalRead(buttonpin);      //Check the button 
          pot2 = analogRead(potpin);        //Set another variable pot2 equal to the new position of the potentiometer
          if(!(pot2 <= (pot+100) && pot2 >= (pot-100))) {  //If pot2 if much different than pot...
              LUV = map(pot2, 0, 4095, 0, 255);               //The light upper value (LUV) is set according to the value of the potentiometer
              pot = 5000;                                      //Making pot a very high number so the if statement now always triggers
          }
          display.clearDisplay();     //Clearing the display
          display.setCursor(0,0);     //Setting the cursor
          display.print(F("LUV:"));
          display.print(LUV);         //Printing the value set by the potentiometer
          display.display();          //Updating the display
          analogWrite(lightspin, LUV);       //Setting the lights to LUV
          delay(100);                 //Pausing
        }
        
        while(button == LOW) {    //While the button is being pressed...
          button = digitalRead(buttonpin);  //Check the button 
          delay(20);                        //Wait
        }
      
        pot = analogRead(potpin);   //The variable pot is set to the current position of the potentiometer
        analogWrite(lightspin, 0);     //Turning the lights off
          
        while(button == HIGH) {       //While the button is released...
          button = digitalRead(buttonpin);      //Check the button 
          pot2 = analogRead(potpin);        //Set another variable pot2 equal to the new position of the potentiometer
          if(!(pot2 <= (pot+100) && pot2 >= (pot-100))) {  //If pot2 if much different than pot...
              DV = map(pot2, 0, 4095, 20, 180);               //The distance value (DV) is set according to the value of the potentiometer
              pot = 5000;                                      //Making pot a very high number so the if statement now always triggers
          }
          display.clearDisplay();     //Clearing the display
          display.setCursor(0,0);     //Setting the cursor
          display.print(F("DV:"));
          display.print(DV);          //Printing the value set by the potentiometer
          display.display();          //Updating the display
          delay(100);                 //Pausing
        }
        
        while(button == LOW) {    //While the button is being pressed...
          button = digitalRead(buttonpin);  //Check the button 
          delay(20);                        //Wait
        }
      
        pot = analogRead(potpin);   //The variable pot is set to the current position of the potentiometer
          
        while(button == HIGH) {       //While the button is released...
          button = digitalRead(buttonpin);      //Check the button 
          pot2 = analogRead(potpin);        //Set another variable pot2 equal to the new position of the potentiometer
          if(!(pot2 <= (pot+100) && pot2 >= (pot-100))) {  //If pot2 if much different than pot...
              TV = map(pot2, 0, 4095, 5, 300);               //The time value (TV) is set according to the value of the potentiometer
              pot = 5000;                                      //Making pot a very high number so the if statement now always triggers
          }
          display.clearDisplay();     //Clearing the display
          display.setCursor(0,0);     //Setting the cursor
          display.print(F("TV:"));
          display.print(TV);          //Printing the value set by the potentiometer
          display.display();          //Updating the display
          delay(100);                 //Pausing
        }
        
        while(button == LOW) {    //While the button is being pressed...
          button = digitalRead(buttonpin);  //Check the button 
          delay(20);                        //Wait
        }
      }
      
  //------------//
  
  
  LSV = constrain(LSV, LSLV, LSUV);      //Keeping the light sensor value within the upper and lower bounds
  LV2 = map(LSV, LSLV, LSUV, LLV, LUV);  //Setting the new light value based on the light sensor value and light value lower and upper bounds
  
  motion = digitalRead(motionsensorpin);  //Reading the motion sensor
  
  if(distance >= DV) {    //If the distance from the sensor is greater than the distance value setting...
    
    if(counter == 1) {    //If this is the first time through this loop...
      timer = 0;              //Set the timer equal to 0 
      counter = 2;            //Set the counter equal to two so that this loop won't run again and reset the timer until after the distance from the sensor drops below the distance value setting
    }
  
    if(motion == HIGH) {  //If the motion sensor is triggered...
      timer = 0;              //Reset the timer
    }
  
    if(timer < (1000 * TV)) {     //If the timer is less than the time value in msec...
        for(int fade = 0; fade <= fademax; fade++) {   //Fade the lights to the new light value (LV2)
          analogWrite(lightspin, LV + ((fade * (LV2-LV))/fademax));
          delay((250/fademax));
        }
        LV = LV2;   //Set the old light value equal to the new one
    }

    if(timer >= (1000 * TV)) {     //If the timer is less than the time value in msec...
        for(int fade = 0; fade <= fademax; fade++) {   //Fade out the lights
          analogWrite(lightspin, LV - ((fade * LV)/fademax));
          delay((250/fademax));
          }
        LV = 0;   //Set the old light value equal to zero
    } 
  
  }
  
  if(distance < DV) {    //If the distance from the sensor is less than the distance value setting...
      for(int fade = 0; fade <= fademax; fade++) {      //Fade out the lights
          analogWrite(lightspin, LV - ((fade * LV)/fademax));
          delay((250/fademax));
        }
        LV = 0;       //Set the old light value equal to zero
        counter = 1;  //Reset the counter 
  }
  
}
