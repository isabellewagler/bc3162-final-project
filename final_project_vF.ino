#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <HTTPClient.h>

BLEScan* pBLEScan;

//Bluetooth RSSI - determines proximity of device
const short near_thrsh = -55;
const short far_thrsh = -82;  
const short timeout = 5; //how long to wait looking for a bluetooth device

//In order to connect to Bluetooth plug - using compatible app IFTTT
const char* turn_on   = "https://maker.ifttt.com/trigger/turnon/with/key/UD7HRSUW";
const char* turn_off = "https://maker.ifttt.com/trigger/turnoff/with/key/YFKYTQB2";
const char* ssid = "Isabelle's Phone";
const char* pswd = "isabellephone10"; //using personal hotspot instead of Columbia wifi

short prox = 0;
bool near = true;
long lit = 0;


void wifiTask() {
      WiFi.begin(ssid, pswd);
      Serial.print("Connecting to Wifi");
      while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(300);
      }
}


void wifis(const char* action){
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
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  
  }else{
    wifiTask();
    wifis(action);
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
            if((prox > near_thrsh)&&(near)){
                Serial.println("Device in Close Range");
                Serial.println("******** LIGHT ON ***********");
                wifis(turn_on);
                near = false;
            }

            //determine if bluetooth device is far enough to turn off
            else if((prox < far_thrsh) && (!near)){
                Serial.println("Device went away");
                Serial.println("******** LIGHT OFF **********");
                wifis(turn_off);
                near = true;
            }
        }

        //reset bluetooth scan
        pBLEScan->clearResults();
    }
};

void bleTask(){
  // configure bluetooth scan
  BLEDevice::init("CES FINAL PROJECT");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //Setup Wifi
  wifiTask();

  //Setup BLE
  bleTask();
  BLEScanResults foundDevices = pBLEScan->start(0);
}

void loop() {

}
