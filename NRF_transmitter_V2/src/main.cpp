/******************************************************************
 Created with PLATFORMIO IDE for VSCODE - 22.05.2020
 Project     : NRF_transmitter_V2
 Libraries   : 
 Author      : Mujahid Abdullah
 Description : 
******************************************************************/

//Libraries
#include <Arduino.h>
#include <RH_NRF905.h>
#include <SPI.h>

//Define Functions
void get_resp();
void data_transmit(void *pvParameters);

//Declare Globals
String resp = "";
bool check = false;
bool check_loop = false; uint8_t node_address = 1;
uint8_t *addr = &node_address;


//Init Instances
RH_NRF905 obj_1(32,33,5);
RH_NRF905 obj_2(32,33,4);

void setup() 
{
  Serial.begin(115200);
  if (!obj_1.init())
    Serial.println("init 1 failed");
  if (!obj_2.init())
    Serial.println("init 2 failed");

  Serial.println("Init Both NRF Modules");
  obj_2.setPromiscuous(true);// obj_2.setNetworkAddress(addr,4);obj_2.setChannel(2106, false);
  obj_1.setPromiscuous(true);// obj_1.setNetworkAddress(addr,4);obj_1.setChannel(2106, false);

  xTaskCreatePinnedToCore(data_transmit, "data_transmit", 10000, NULL, 1, NULL,0);
  Serial.println("Task Start");
  obj_1.printRegisters();
  obj_2.printRegisters();
}

void loop() 
{  
  get_resp();vTaskDelay(5);
}

void get_resp()
{
  if(resp.compareTo("") == false)
  {
    Serial.println("Select Transmitter 0 for one and 1 for other");
    while(!Serial.available());
      resp = Serial.readString();
      Serial.println(resp);
      Serial.print("Selected Transmitter key is :"); Serial.println(resp);
  }
}

void data_transmit(void *pvParameters)
{
  while(1)
  {
  check_loop = false; check = false;
  uint8_t buffer[RH_NRF905_MAX_MESSAGE_LEN];
  uint8_t length = sizeof(buffer);vTaskDelay(5);

  if(resp.compareTo("0") == 0)
    {
      Serial.println("Selected transmitter is 1");
      Serial.println("Enter data you want to send");
      while(!Serial.available()) vTaskDelay(1);
      
      if(Serial.available()>0)
      {
        String send_data = Serial.readString();
        send_data.toCharArray((char*)buffer, length);
        obj_1.send((uint8_t*)buffer, length); check = true;
        Serial.print("data send: "); Serial.println((char*)buffer);
        obj_1.waitPacketSent();
        Serial.flush();vTaskDelay(5);
        while(check_loop == false)
        {
          if(obj_1.available())
          {
            if(obj_1.recv(buffer, &length))
              {
                Serial.print("data recv: "); Serial.println((char*)buffer);
                Serial.println("data receive at Transmitter_1");
                Serial.flush();
                check_loop = true; vTaskDelay(5);
              } vTaskDelay(5);
          }vTaskDelay(5);
        }
        resp = "";vTaskDelay(5);
      }
    }
  
  if(resp.compareTo("1") == 0)
    {
      Serial.println("Selected transmitter is 2");
      Serial.println("Enter data you want to send");
      while(!Serial.available()) vTaskDelay(1);
      
      if(Serial.available()>0)
      {
        String send_data = Serial.readString();
        send_data.toCharArray((char*)buffer, length);
        obj_2.send((uint8_t*)buffer, length); check = true;
        Serial.print("data send: "); Serial.println((char*)buffer);
        obj_2.waitPacketSent();
        Serial.flush();vTaskDelay(5);
        while(check_loop == false)
        {
          if(obj_2.available())
          {
            if(obj_2.recv(buffer, &length))
              {
                Serial.print("data recv: "); Serial.println((char*)buffer);
                Serial.println("data receive at Transmitter_2");
                Serial.flush();vTaskDelay(5);
                check_loop = true;
              }vTaskDelay(5);
          }vTaskDelay(5);
        }
        resp = "";vTaskDelay(5);
      }
    }
    if(resp.compareTo("") != 0)
    {
      Serial.println("Invalid Transmitter Key");
      resp = "";vTaskDelay(1);
    }
    vTaskDelay(5);
  }vTaskDelay(1);
}