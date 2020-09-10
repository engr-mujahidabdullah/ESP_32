/******************************************************************
 Created with PLATFORMIO IDE for VSCODE - 19.05.2020
 Project     : NRF_control
 Libraries   : 
 Author      : Mujahid Abdullah
 Description : 
******************************************************************/

#include <Arduino.h>
#include <RHGenericDriver.h>
#include <RH_NRF905.h>
#include <SPI.h>
#include <RHSPIDriver.h>
#include <RHHardwareSPI.h>
#include <RHNRFSPIDriver.h>
#include <Thread.h>

//functions
void led_blink();

// Singleton instance of the radio driver
RH_NRF905 obj_1(32,33,5);
RH_NRF905 obj_2(32,33,4);
Thread led_thr = Thread();

#define led_pin 2

uint8_t query[] = "Boot_up";
bool check = false;
bool check_loop = false;

void setup() 
{
  Serial.begin(115200);

  if (!obj_1.init())
    Serial.println("init 1 failed");
  if (!obj_2.init())
    Serial.println("init 2 failed");

  Serial.println("init NRF_1 done");
  Serial.println("init NRF_2 done");
  obj_2.setPromiscuous(true);
  obj_1.setPromiscuous(true);

  pinMode(led_pin, OUTPUT);
  led_thr.onRun(led_blink);
  led_thr.setInterval(400);
}

void loop() 
{
  check_loop = false; check = false;
  uint8_t buffer[RH_NRF905_MAX_MESSAGE_LEN];
  uint8_t length = sizeof(buffer);
  uint8_t buffer0[RH_NRF905_MAX_MESSAGE_LEN];
  uint8_t length0 = sizeof(buffer0);


  if(Serial.available()>0)
  {
    String send_data = Serial.readString();
    send_data.toCharArray((char*)buffer, length);
    Serial.println((int)buffer);
    Serial.println(obj_2.send((uint8_t*)buffer, length)); check = true;
    Serial.print("data send: "); Serial.println((char*)buffer);

    obj_2.waitPacketSent();
    Serial.flush();
    while(check_loop == false)
    {
      if(obj_1.available() || obj_2.available())
      {
      if(obj_2.recv(buffer, &length))
        {
          Serial.print("data recv: "); Serial.println((char*)buffer);
          Serial.println("data receive at obj_2");
          Serial.flush();
          check_loop = true;
        }

      if(obj_1.recv(buffer, &length))
        {
          Serial.print("data recv: "); Serial.println((char*)buffer);
          Serial.println("data receive at obj_1");
          Serial.flush();
          check_loop = true;
        }
      }
    }
  }

    if( led_thr.shouldRun())
		  led_thr.run(); 
}


void led_blink()
{
  if(check == true)
  {
    for(int i = 0; i<4; i++)
    {
      digitalWrite(led_pin, HIGH);
      delay(250);
      digitalWrite(led_pin, LOW);
      delay(250);
    }
    digitalWrite(led_pin, HIGH);
    delay(2000);
    digitalWrite(led_pin, LOW);
    check = false;
  }
}