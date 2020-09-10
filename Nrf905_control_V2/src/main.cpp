/******************************************************************
 Created with PLATFORMIO IDE for VSCODE - 18.05.2020
 Project     : NRF_control-V2
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

Thread led_thr = Thread();

#define led_pin 2

uint8_t query[] = "Acknowleged";
bool check = false; 
bool check_loop = false; 

void setup() 
{
  Serial.begin(115200);

  if (!obj_1.init())
    Serial.println("init failed");

  Serial.println("init NRF_1 done");
  obj_1.setPromiscuous(true);

  pinMode(led_pin, OUTPUT);

 led_thr.onRun(led_blink);
 led_thr.setInterval(500);
}

void loop() 
{
  uint8_t buffer[RH_NRF905_MAX_MESSAGE_LEN];
  uint8_t length = sizeof(buffer);

  while(check_loop == false)
  {
    if(obj_1.available())
    {
    if(obj_1.recv(buffer, &length))
      {
        Serial.print("data recv: "); Serial.println((char*)buffer);
        check = true;
        Serial.flush();
        check_loop = true;
      }
    }
  }

  obj_1.send(query, sizeof(query));
  obj_1.waitPacketSent(); check = true;
  Serial.print("data send: "); Serial.println((char*)query);

    Serial.flush();
    check_loop = false;
    delay(500);

   if( led_thr.shouldRun())
		led_thr.run(); 
}

void led_blink()
{
  if(check == true)
  {
      digitalWrite(led_pin, HIGH);
      vTaskDelay(2000);
      digitalWrite(led_pin, LOW);
      check = false;
  }
}