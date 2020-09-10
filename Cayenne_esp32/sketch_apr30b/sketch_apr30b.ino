/******************************************************************
 Created with PLATFORMIO IDE for VSCODE - 30.04.2020
 Project     : esp32_cayenne
 Libraries   : 
 Author      : Mujahid Abdullah
 Description : 
******************************************************************/

//#include <Arduino.h>
#include <CayenneMQTTESP32.h>
#include <Wire.h>
#include <DHT.h>
#include <Thread.h>

//define user functions
void read_sensors();

//Wifi network info
const char * ssid =  "AnchorRecovery";             
const char * wifi_password =  "12345678";

//cayenne authentication info.
const char * username ="85a0a350-8a9c-11ea-b767-3f1a8f1211ba";
const char * cay_password ="41553ff6b34c9e1567033a3ba05d6db42e35fb49";
const char *client_id ="ed40c6e0-8a9f-11ea-883c-638d8ce4c23d";

//Define sensor pins and types
#define DHTPIN 13  
#define DHTTYPE DHT11
#define pot_ 34
#define button 12

//Define instances
//dht sensor;
DHT dht(DHTPIN, DHTTYPE);
Thread sensor_thread = Thread();

//initialize values
float temp = 0;
float humidity = 0;
int pot_val = 0;
int button_val = 0;

//Define Virtual channels
#define VIRTUAL_CHANNEL_0 0
#define VIRTUAL_CHANNEL_1 1
#define VIRTUAL_CHANNEL_2 2
#define VIRTUAL_CHANNEL_3 3

void setup() 
{
  Serial.begin(115200);
  dht.begin();
  Cayenne.begin(username,cay_password,client_id,ssid,wifi_password);
  pinMode(button, INPUT_PULLUP);
  sensor_thread.onRun(read_sensors);
  sensor_thread.setInterval(500);
  
}

void loop() 
{ 
  Cayenne.loop();
  if( sensor_thread.shouldRun())
     sensor_thread.run(); 
}

CAYENNE_OUT(VIRTUAL_CHANNEL_0)
{  
  //sensor.read11(dht_);
  //temp = sensor.temperature;
  //humidity = sensor.humidity;
  
  Cayenne.celsiusWrite(VIRTUAL_CHANNEL_0, temp);
}

CAYENNE_OUT(VIRTUAL_CHANNEL_1)
{ 
  Cayenne.luxWrite(VIRTUAL_CHANNEL_1, humidity);
}

CAYENNE_OUT(VIRTUAL_CHANNEL_2)
{ 
  //pot_val = analogRead(pot_);
  Cayenne.virtualWrite(VIRTUAL_CHANNEL_2,pot_val);
}

CAYENNE_OUT(VIRTUAL_CHANNEL_3)
{ 
  //button_val = digitalRead(button);
  Cayenne.digitalSensorWrite(VIRTUAL_CHANNEL_3,button_val);
}

void read_sensors()
{
 // sensor.read11(dht_);
  temp = dht.readTemperature();
  humidity = dht.readHumidity();
  pot_val = analogRead(pot_);
  button_val = digitalRead(button);
  Serial.println(temp);
}
