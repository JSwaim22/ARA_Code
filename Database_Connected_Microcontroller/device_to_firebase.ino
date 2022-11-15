/*------------------------------------------------------------
 * 
 *              ECE SENIOR DESIGN DEVICE SOFTWARE
 *                        Joshua Swaim
 * 
-------------------------------------------------------------*/


// Libraries
#include <Arduino.h>
#include <Hash.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include "wiring_private.h"
#include "pins_arduino.h"

// Insert your network credentials
#define WIFI_SSID "ncsu"
#define WIFI_PASSWORD ""

// Insert Firebase project API Key
#define API_KEY "AIzaSyBUDAqWboCrTCVDK8MouFgOTbOX4c7aU5Y"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://ultrasonicwoodtester-default-rtdb.firebaseio.com" 

// Define Firebase objects
FirebaseData stream;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variables to save database paths
String listenerPath = "testButton";
String dataPath;
String humDataPath;
String tempDataPath;
String tofDataPath;
String nameDataPath;
String sampleNumberPath = "currentSample/number";
String testNamePath = "currentTest/name";
String testNumberPath;

String testNumberString = "0";
int testNumberInt = 0;
String testNameString = "test";
String sampleNumberString = "0";

bool signupOK = false;
bool getNewData = false;
bool firstTime = true;

float tempValue = 0.0;
float humValue = 0.0;
unsigned long tofValue = 0;

volatile unsigned long transmit_time = micros();
volatile unsigned long receive_time = micros();
unsigned long pulse_time = 0;
volatile bool firstReceive = false;

// GPIO Pins
#define TEST_LED              0
#define CONNECTED_LED         2
#define DHTPIN                14
#define SINE                  5
#define TRANSMIT_DETECT       4
#define RECEIVE_DETECT        12
#define TRANSMIT              13

DHT dht(DHTPIN, DHT11);

// Callback function that runs on database changes
void streamCallback(FirebaseStream data){
  if (data.dataTypeEnum() == fb_esp_rtdb_data_type_integer && firstTime == false){
    Serial.println("Button pressed. Incrementing test number.");
    digitalWrite(TEST_LED, HIGH);
    getNewData = true;
  } else {
    firstTime = false;
  }
}

// Timeout error function
void streamTimeoutCallback(bool timeout){
  if (timeout)
    Serial.println("stream timeout, resuming...\n");
  if (!stream.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
}

// Transmit Time Interrupt Function
void IRAM_ATTR transmitInterrupt() {
    transmit_time = micros();
}

// Receive Time Interrupt Function
void IRAM_ATTR receiveInterrupt() {
    if(firstReceive) {
      receive_time = micros();
      firstReceive = false;
    }
}

void setup(){
  // Initialize Outputs
  pinMode(TEST_LED, OUTPUT);
  digitalWrite(TEST_LED, LOW);
  pinMode(CONNECTED_LED, OUTPUT);
  digitalWrite(CONNECTED_LED, LOW);
  pinMode(TRANSMIT, OUTPUT);
  digitalWrite(TRANSMIT, LOW);
  pinMode(TRANSMIT_DETECT, INPUT);
  pinMode(RECEIVE_DETECT, INPUT);
  attachInterrupt(digitalPinToInterrupt(TRANSMIT_DETECT), transmitInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(RECEIVE_DETECT), receiveInterrupt, RISING);
  pinMode(SINE, OUTPUT);
  analogWriteFreq(40000);
  analogWrite(SINE, 127);
  
  // Setting up Serial
  Serial.begin(115200);
  delay(100);

  // Connecting to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(300);
  }
  Serial.println();
  Serial.print("Connected to Wi-Fi with IP: ");
  Serial.println(WiFi.localIP());

  // Setting up DHT Sensor
  dht.begin();
  dht.readHumidity();
  dht.readTemperature(true);
  
  // Connecting to Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Connected to firebase");
    Serial.println("");
    signupOK = true;
    digitalWrite(CONNECTED_LED, HIGH);
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Setting up streaming
  if (!Firebase.RTDB.beginStream(&stream, listenerPath.c_str())) {
    Serial.printf("stream begin error, %s\n\n", stream.errorReason().c_str());
  }
  // Assign a calback function to run when it detects changes on the database
  Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);
  
  delay(2000);
}

void loop(){
  if (Firebase.ready() && signupOK && getNewData) {
    // Getting Sample Number
    if (Firebase.RTDB.getInt(&fbdo, sampleNumberPath)) {
      sampleNumberString = fbdo.to<String>();
      testNumberPath = "currentTest/number/" + sampleNumberString;
      // Getting Test Number
      if (Firebase.RTDB.getInt(&fbdo, testNumberPath)) {
        testNumberInt = fbdo.to<int>() + 1;
        testNumberString = String(testNumberInt);
        if (Firebase.RTDB.setFloat(&fbdo, testNumberPath, testNumberInt)){
          Serial.println("  Incrementing test number success");
        } else {
          Serial.println("  Incrementing test number failure");
        }

        dataPath = "tests/" + sampleNumberString + "/" + testNumberString;
        Serial.print("Sending sensor readings to ");
        Serial.print(dataPath);
        Serial.println("...");

        // Sending Name Data
        if (Firebase.RTDB.getString(&fbdo, testNamePath)) {
          testNameString = fbdo.to<String>();
          nameDataPath = dataPath + "/name";
          if (Firebase.RTDB.setString(&fbdo, nameDataPath, testNameString)){
            Serial.println("  Name sending success");
          } else {
            Serial.println("  Name sending failure");
          }
        } else {
          Serial.println("  Name getting failure");
        }

        // Getting Humidity and Temperature Data
        humValue = dht.readHumidity();
        if (isnan(humValue)) {
          Serial.println("  Humidity getting failure");
          humValue = 0;
        } else {
          Serial.println("  Humidity getting success");
        }
        tempValue = dht.readTemperature(true);
        if (isnan(tempValue)) {
          Serial.println("  Temperature getting failure");
          tempValue = 0;
        } else {
          Serial.println("  Temperature getting success");
        }
    
        // Sending Humidity Data
        humDataPath = dataPath + "/humidity";
        if (Firebase.RTDB.setFloat(&fbdo, humDataPath, humValue)){
          Serial.println("  Humidity sending success");
        } else {
          Serial.println("  Humidity sending failure");
        }
    
        // Sending Temperature Data
        tempDataPath = dataPath + "/temperature";
        if (Firebase.RTDB.setFloat(&fbdo, tempDataPath, tempValue)){
          Serial.println("  Temperature sending success");
        } else {
          Serial.println("  Temperature sending failure");
        }

        // Getting Time of Flight Data
        firstReceive = true;
        digitalWrite(TRANSMIT, HIGH);
        delayMicroseconds(200);
        digitalWrite(TRANSMIT, LOW);
        tofValue = receive_time - transmit_time;
        Serial.println(tofValue);
        if (tofValue > 500) tofValue = 0;
        if (tofValue == 0) {
          Serial.println("  Time of Flight getting failure");
        } else {
          Serial.println("  Time of Flight getting success");
        }
        
        // Sending Time of Flight Data
        tofDataPath = dataPath + "/timeOfFlight";
        if (Firebase.RTDB.setFloat(&fbdo, tofDataPath, tofValue)){
          Serial.println("  Time of Flight sending success");
        } else {
          Serial.println("  Time of Flight sending failure");
        }

         
      } else {
        Serial.println("Test number getting failure");
        Serial.println(fbdo.errorReason());
      }
    } else {
      Serial.println("Sample number getting failure");
      Serial.println(fbdo.errorReason());
    }
    
    Serial.println("");
    digitalWrite(TEST_LED, LOW);
    getNewData = false;
  }
}
