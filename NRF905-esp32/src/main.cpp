/******************************************************************
 Created with PLATFORMIO IDE for VSCODE - 15.05.2020
 Project     : NRF905_esp32
 Libraries   : 
 Author      : Mujahid Abdullah
 Description : 
******************************************************************/

#include <Arduino.h>
#include <RH_NRF905.h>
#include <SPI.h>

// Singleton instance of the radio driver
RH_NRF905 obj_(32,33,5);

//globals
//uint8_t buffer[RH_NRF905_MAX_MESSAGE_LEN];
//uint8_t length = sizeof(buffer);
void setup() 
{
  Serial.begin(115200);
  if (!obj_.init())
    Serial.println("init failed");

  Serial.println("init done");
  
  obj_.setChannel(108, false);obj_.spiWriteRegister(2,0x44);obj_.spiWriteRegister(3,0x20);obj_.spiWriteRegister(4,0x20);;obj_.spiWriteRegister(5,0xE7);obj_.spiWriteRegister(5,0xE7);obj_.spiWriteRegister(6,0xE7);obj_.spiWriteRegister(7,0xE7);obj_.spiWriteRegister(8,0xE7);
  obj_.printRegisters();
  obj_.setHeaderId(200);
}

void loop() 
{
  uint8_t buffer[RH_NRF905_MAX_MESSAGE_LEN];
  uint8_t length = sizeof(buffer);

  if(obj_.available())
  {
    if(obj_.recv(buffer, &length))
    {
      Serial.print("data recv: "); Serial.println((char*)buffer);
      Serial.flush();
      //Serial.println(obj_.headerFrom());
    }
  }

  if(Serial.available()>0)
  {
    String send_data = Serial.readString();
    send_data.toCharArray((char*)buffer, length);

    obj_.send((uint8_t*)buffer, length);
    Serial.print("data send: "); Serial.println((char*)buffer);
    

    obj_.waitPacketSent();
    Serial.flush();memset(buffer,0,sizeof(buffer));
  }
}