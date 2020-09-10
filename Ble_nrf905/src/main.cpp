/******************************************************************
 Created with PLATFORMIO IDE for VSCODE - 18.05.2020
 Project     : Ble_nrf905
 Libraries   : 
 Author      : Mujahid Abdullah
 Description : 
******************************************************************/

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <RH_NRF905.h>
#include <SPI.h>

//define UUID
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
//define BLE instances
BLEServer *server;
BLEService *service;
BLECharacteristic *char_;
BLEAdvertising *advrt;
std::string pre_value = "";

//define NRF objects
RH_NRF905 obj_(32,33,5);

void setup() 
{
  Serial.begin(115200);
  BLEDevice::init("SERVER_1");
  server = BLEDevice::createServer();
  service = server->createService(SERVICE_UUID);
  char_ = service->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE);
  char_->setValue("-----");

  service->start();
  advrt = BLEDevice::getAdvertising();
  advrt -> addServiceUUID(SERVICE_UUID);
  advrt -> setScanResponse(true);
  advrt -> setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("BLE Adverting Start");
  if (!obj_.init())
    Serial.println("init failed");
  Serial.println("NRF INIT start");
}

void loop() 
{
  uint8_t buffer[RH_NRF905_MAX_MESSAGE_LEN];
  uint8_t length = sizeof(buffer);
  if(obj_.available())
  {
    if(obj_.recv(buffer, &length))
    {
      char_ ->setValue((char*)buffer);
      Serial.print("data recv: "); Serial.println((char*)buffer);
      Serial.flush();
    }
  }
  std::string value = char_->getValue();
  

  if(value.compare(pre_value) != 0)
  {
    String send_val = value.c_str();
    send_val.toCharArray((char*)buffer, length);

    obj_.send((uint8_t*)buffer, length);
    Serial.print("data send: "); Serial.println((char*)buffer);

    obj_.waitPacketSent();
    pre_value = value;
    Serial.flush();
  }
}