
#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <LiquidCrystal.h>
#include "Wire.h"

// initialize the library by associating any needed LCD interface pin with the esp pin number it is connected to
const int rs = 22 , en = 21, d4 = 5, d5 = 18, d6 = 23, d7 = 19;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//define UUID
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC1_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC2_UUID "beb5483e-36e1-4688-a7f5-ea07361b26a8"
BLECharacteristic *char_1;

void setup() 
{
  Serial.begin(115200);
  Serial.begin(115200);
  lcd.begin(16,2);
  lcd.setCursor(0, 1);
  BLEDevice::init("MST1_server");
  BLEServer *server = BLEDevice::createServer();
  BLEService *service = server->createService(SERVICE_UUID);
  char_1 = service->createCharacteristic(CHARACTERISTIC1_UUID,BLECharacteristic::PROPERTY_INDICATE|BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE);
  BLECharacteristic *char_2 = service->createCharacteristic(CHARACTERISTIC2_UUID,BLECharacteristic::PROPERTY_NOTIFY|BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE);

  char_1->setValue("Indicate");
  char_2->setValue("Notify");

  service->start();
  BLEAdvertising *advrt = BLEDevice::getAdvertising();
  advrt->addServiceUUID(SERVICE_UUID);
  advrt->setScanResponse(true);
  advrt->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  advrt->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("DONE"); 
}

void loop() 
{
  std::string value1 = char_1->getValue();
  lcd.print(value1.c_str());
  delay(3000);  // put your main code here, to run repeatedly:
  lcd.clear();
}
