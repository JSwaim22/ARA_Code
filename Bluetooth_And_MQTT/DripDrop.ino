#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include "BLEDevice.h"


const char* ssid = "Wifi Network";
const char* password =  "Wifi Password";
const char* mqttServer = "Mqtt Address";
const int mqttPort = 15117;
const char* mqttUser = "Mqtt User";
const char* mqttPassword = "Mqtt Password";

int LED = 0;
int datacounter = 0;
int t;
int h;
String tempurl = "stringtempurl";
String humurl = "stringhumurl";

static BLEUUID serviceUUID("bacb8775-a181-4d74-8d06-13d09a0e7a0b");
static BLEUUID    charUUID("ec27e946-4d94-408a-a96b-32317ac8d4e9");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;

static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

unsigned long oldtime = 0;
int interval = 45000;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  Serial.print("Data from sensor characteristic ");
  Serial.println(pBLERemoteCharacteristic->getUUID().toString().c_str());
  if (datacounter == 1) {
    h = pData[0];
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.println(" %");
    datacounter = 2;
  }
  if (datacounter == 0) {
    t = pData[0];
    Serial.print("Temperature: ");
    Serial.println(t);
    Serial.println(" C");
    datacounter = 1;
  }
  if (datacounter == 2) {
    Serial.println("");
    datacounter = 0;
  }

}

class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {}
    void onDisconnect(BLEClient* pclient) {
      connected = false;
      Serial.println("onDisconnect");
    }
};

bool connectToServer() {
  Serial.print("Forming a connection to ");
  Serial.println(myDevice->getAddress().toString().c_str());

  BLEClient*  pClient  = BLEDevice::createClient();
  Serial.println(" - Created client");

  pClient->setClientCallbacks(new MyClientCallback());

  pClient->connect(myDevice);
  Serial.println(" - Connected to server");

  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);

  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our service");

  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);

  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our characteristic");

  if (pRemoteCharacteristic->canNotify())
    pRemoteCharacteristic->registerForNotify(notifyCallback);
  connected = true;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.print("BLE Advertised Device found: ");
      Serial.println(advertisedDevice.toString().c_str());

      if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
        BLEDevice::getScan()->stop();
        myDevice = new BLEAdvertisedDevice(advertisedDevice);
        doConnect = true;
        doScan = true;
      }
    }
};


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(25, OUTPUT);
  digitalWrite(25, LOW);

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  Serial.println(WiFi.status());

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
      Serial.println("Connected to MQTT");
      client.publish("/pump", "Connected to MQTT");
    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.publish("/pump", "Online");
  client.subscribe("/pump");
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");

  LED = (char)payload[0];
  Serial.println(LED);
  if (LED == 48) {
    digitalWrite(25, LOW);
  }
  if (LED == 49) {
    digitalWrite(25, HIGH);
  }
}

void loop() {
  client.loop();
  unsigned long newtime = millis();

  if ((unsigned long)(newtime - oldtime) >= (interval - 1)) {
    client.publish("/pump", "Going offline");
    Serial.println("Going offline");
  }

  if ((unsigned long)(newtime - oldtime) >= interval) {
    WiFi.mode(WIFI_OFF);
    Serial.println("Starting BLE Client...");

    BLEDevice::init("DripDrop");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);

    if (doConnect == true) {
      if (connectToServer()) {
        Serial.println("Connected to the sensor.");
      } else {
        Serial.println("Failed to connect to the sensor");
      }
      doConnect = false;
    }

    delay(3000);
    btStop();
    Serial.println("Stopping BLE CLient...");
    delay(6000);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
    delay(1000);

    String urlt = String("https://us-central1-riego-app.cloudfunctions.net/sendTemperature?value=");
    String urlh = String("https://us-central1-riego-app.cloudfunctions.net/sendHumidity?value=");
    String temp = String(t);
    String hum = String(h);
    String tempurl = String(urlt + temp);
    String humurl = String(urlh + hum);

    HTTPClient http;

    Serial.print("Posting temperature data to ");
    Serial.println(tempurl);
    http.begin(tempurl);  //Specify destination for HTTP request
    http.addHeader("Content-Type", "text/plain");             //Specify content-type header
    int httpResponseCode = http.POST("POSTING from ESP32");   //Send the actual POST request
    if (httpResponseCode > 0) {
      String response = http.getString();                       //Get the response to the request
      Serial.println(httpResponseCode);   //Print return code
      Serial.println(response);           //Print request answer
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
    Serial.print("Posting humidity data to ");
    Serial.println(humurl);
    http.begin(humurl);  //Specify destination for HTTP request
    http.addHeader("Content-Type", "text/plain");             //Specify content-type header
    int httpResponseCode2 = http.POST("POSTING from ESP32");   //Send the actual POST request
    if (httpResponseCode2 > 0) {
      String response = http.getString();                       //Get the response to the request
      Serial.println(httpResponseCode2);   //Print return code
      Serial.println(response);           //Print request answer
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode2);
    }

    http.end();  //Free resources
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    if (client.connected()) {
      Serial.println("Connected to MQTT");
      client.publish("/pump", "Connected to MQTT");
    }

    while (!client.connected()) {
      Serial.println("Connecting to MQTT...");
      if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
        Serial.println("Connected to MQTT");
        client.publish("/pump", "Connected to MQTT");
      } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
      }
    }

    client.publish("/pump", "Back online");
    client.subscribe("/pump");
    oldtime = newtime;
  }
}
