/******************************************************************
 Created with PLATFORMIO IDE for VSCODE - 27.04.2020
 Project     : esp32_demo
 Libraries   : 
 Author      : Mujahid Abdullah
 Description : 
******************************************************************/

#include <Arduino.h>

#define LED 2

void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);            // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED, LOW);           // turn the LED off by making the voltage LOW
  delay(1000);                     // wait for a second
}