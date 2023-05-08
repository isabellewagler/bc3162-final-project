#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <HTTPClient.h>

BLEScan* pBLEScan;

//Bluetooth RSSI - determines proximity of device
const short near_threshold = -55;
const short far_threshold = -82;  
const short timeout = 5; //how long to wait looking for a bluetooth device

//In order to connect to Bluetooth plug - using compatible app IFTTT
const char* turn_on   = "https://maker.ifttt.com/trigger/turnon/with/key/UD7HRSUW";
const char* turn_off = "https://maker.ifttt.com/trigger/turnoff/with/key/YFKYTQB2";
const char* ssid = "Isabelle's Phone";
const char* pswd = "isabellephone10"; //using personal hotspot instead of Columbia wifi

int prox = 0;
bool near = true;
int lit = 0;


void wifiConnect() {
      WiFi.begin(ssid, pswd);
      Serial.print("Connecting to Wifi");
      while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(300);
      }
}


void triggerRequest(const char* action){
// wait for WiFi connection
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

//Necessary to connect to Bluetooth plug-in
    Serial.print("[HTTP] begin...\n");
    http.begin(action); //HTTP
    Serial.print("[HTTP] GET...\n");
    int httpCode = http.GET();

    if(httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK) {
        String addr = http.getString();
        Serial.println(addr);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  
  }else{
    wifiConnect();
    triggerRequest(action);
  }
}


class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        if(true){    //determine if bluetooth devices are in range 
            Serial.print("Found The Device: ");
            prox = (int)advertisedDevice.getRSSI();
            Serial.println(prox);
            lit = millis();
            
            //Determine if bluetooth device is close enough to turn on
            if((prox > near_threshold)&&(near)){
                Serial.println("Device in Close Range");
                Serial.println("******** LIGHT ON ***********");
                triggerRequest(turn_on);
                near = false;
            }

            //determine if bluetooth device is far enough to turn off
            else if((prox < far_threshold) && (!near)){
                Serial.println("Device went away");
                Serial.println("******** LIGHT OFF **********");
                triggerRequest(turn_off);
                near = true;
            }
        }

        //reset bluetooth scan
        pBLEScan->clearResults();
    }
};

void bleScan(){
  // configure bluetooth scan
  BLEDevice::init("CES FINAL PROJECT");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

void setup() {
  Serial.begin(9600);

  //Setup Wifi
  wifiConnect();

  //Setup BLE
  bleScan();
  BLEScanResults foundDevices = pBLEScan->start(0);
}

void loop() {

}
