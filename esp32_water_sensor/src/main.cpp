/******************************************************************
 Created with PLATFORMIO IDE for VSCODE - 27.04.2020
 Project     : esp32_water_sensor
 Libraries   : 
 Author      : Mujahid Abdullah
 Description : 
******************************************************************/

#include  <Arduino.h>


#define sensor 2
#define alarm 15

int value = 0;

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 10; 

void setup() 
{
  Serial.begin(9600);
  pinMode(sensor, INPUT);
  pinMode(alarm, OUTPUT);

  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  // attach the channel to  GPIO2 to be controlled
  ledcAttachPin(alarm, ledChannel);
}

void loop() 
{
  value = analogRead(sensor);
  Serial.println(value); 
  if (value >= 0 && value <700) 
  {
    ledcWrite(ledChannel, 0); 
  }
  else if (value >= 701 && value <1300) 
  {
    ledcWrite(ledChannel, 125); 
  }

  else if (value >= 1301) 
  {
    ledcWrite(ledChannel, 1024); 
  }
  delay(200);  
}