/******************************************************************
 Created with PLATFORMIO IDE for VSCODE - 14.05.2020
 Project     : Ble_scanning
 Libraries   : 
 Author      : Mujahid Abdullah
 Description : 
******************************************************************/

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include "ArduinoLinq.hpp"

// initialize the library by associating any needed LCD interface pin with the esp pin number it is connected to
const int rs = 22 , en = 21, d4 = 5, d5 = 18, d6 = 23, d7 = 19;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

using namespace ArduinoLinq;

// init ble pointers
BLEScan* scan_ble;
BLEAdvertisedDevice scan_device; 
int scan_time = 5; 
BLEScanResults foundDevices;
int x=1;int i =0; int m =0;
String scanned_devices[30];
String scanned[30];
int size = 0; int len = 0;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) 
    {
      //if(advertisedDevice.haveName())
     // {
        scanned_devices[i] =advertisedDevice.toString().c_str();
        i++; 
    //  }
    }
};

void setup() 
{
  Serial.begin(115200);
  lcd.begin(16,2);
  lcd.setCursor(0,1);
  lcd.print("Scanner start");
  delay(1000);
  BLEDevice::init("Scanner");
  scan_ble = BLEDevice::getScan();
  scan_ble->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  scan_ble->setActiveScan(true);
  scan_ble->setInterval(100);
  scan_ble->setWindow(99);  // less or equal setInterval value
  lcd.clear();
}

void loop() 
{
  lcd.setCursor(0,1);
  lcd.print("enter scan key");
  if(Serial.available()>0)
  {
    lcd.clear();
    String ser_dat = Serial.readString();
    Serial.println(ser_dat);
    if(!ser_dat.compareTo("scan") == 0)
    {
      lcd.print("Scanning");
      for(int k = 0; k < 4; k++)
      {
        foundDevices = scan_ble->start(scan_time,false);
        scan_ble->clearResults();
        lcd.print(".");
        Serial.println("");
      }
      lcd.clear();

      while(scanned_devices[size] != '\0')
      {
        size++;
      }

      for(int j = 0; j < size ; j++) Serial.println(scanned_devices[j]);
      auto scanned = from_array(scanned_devices) >> distinct() >> to_vector();
      Serial.println("-----------");
      while(scanned[len] != '\0')
        {
          len++;
        }

      for(int y = 0; y < len ; y++)
      {
          String str = String(y+1) + ": " + String(scanned[y]); 
          lcd.println(str);
            for (int positionCounter = 0; positionCounter < 16; positionCounter++) 
            {
              lcd.scrollDisplayLeft();
              delay(500);
            }
          delay(1000);
          lcd.clear();  
      }  
    }
  }
}