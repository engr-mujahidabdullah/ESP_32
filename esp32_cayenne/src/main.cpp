/******************************************************************
 Created with PLATFORMIO IDE for VSCODE - 30.04.2020
 Project     : esp32_cayenne
 Libraries   : 
 Author      : Mujahid Abdullah
 Description : 
******************************************************************/

#include <Arduino.h>
#include <CayenneMQTTESP32.h>
#include <Wire.h>
#include <DHT.h>
#include <Thread.h>
#include <WiFi.h>

//define user functions
static void read_sensors();
static bool button_switch(int pin);
static bool pb1_switch(int pin);
static bool pb2_switch(int pin);
void init_pwm();
void led_control();

//Wifi network info
const char * ssid =  "MikroStarTechPVT-witribe";             
const char * wifi_password =  "MstOffice123@#45";

//const char *ssid =  "AnchorRecovery";             
//const char *wifi_password =  "12345678";

//cayenne authentication info.
const char * username ="85a0a350-8a9c-11ea-b767-3f1a8f1211ba";
const char * cay_password ="41553ff6b34c9e1567033a3ba05d6db42e35fb49";
const char *client_id ="ed40c6e0-8a9f-11ea-883c-638d8ce4c23d";

//Define sensor pins and types
#define DHTPIN 13  
#define DHTTYPE DHT11
#define pot_ 34
#define button 12
#define joy 35
#define led 32
#define pb_1 25
#define pb_2 33

//Define instances
DHT dht(DHTPIN, DHTTYPE);
Thread sensor_thread = Thread();

//initialize values
float temp = 0;
float humidity = 0;
int pot_val = 0;
static bool button_val = 0;
int joy_val = 0;
bool led_val = 1;
bool pb1_val =0;
bool pb2_val =0;

static bool state_ = LOW;
static bool pre_state = LOW;
static bool state_1 = LOW;
static bool pre1_state = LOW;
static bool state_2 = LOW;
static bool pre2_state = LOW;

//Define Virtual channels
#define VIRTUAL_CHANNEL_0 0
#define VIRTUAL_CHANNEL_1 1
#define VIRTUAL_CHANNEL_2 2
#define VIRTUAL_CHANNEL_3 3
#define VIRTUAL_CHANNEL_4 4
#define VIRTUAL_CHANNEL_5 5
#define VIRTUAL_CHANNEL_6 6
#define VIRTUAL_CHANNEL_7 7

void setup() 
{
  Serial.begin(115200);
  dht.begin();
  Cayenne.begin(username,cay_password,client_id,ssid,wifi_password);
  pinMode(button, INPUT_PULLUP);
  pinMode(pb_1, INPUT_PULLUP);
  pinMode(pb_2, INPUT_PULLUP);
  sensor_thread.onRun(read_sensors);
  sensor_thread.setInterval(500);
  attachInterrupt(button, led_control, FALLING);  
  pinMode(led, OUTPUT);
}

void loop() 
{ 
  Cayenne.loop();
  if( sensor_thread.shouldRun())
		 sensor_thread.run(); 
}

CAYENNE_OUT(VIRTUAL_CHANNEL_0)
{  
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
	Cayenne.digitalSensorWrite(VIRTUAL_CHANNEL_3,!button_val);
  //else{pb1_val = false ; pb2_val = false;}
}

CAYENNE_OUT(VIRTUAL_CHANNEL_4)
{ 
  //button_val = digitalRead(button);
	Cayenne.digitalSensorWrite(VIRTUAL_CHANNEL_4,joy_val);
}

CAYENNE_IN(VIRTUAL_CHANNEL_5)
{ 
  led_val = !getValue.asInt();
  led_val = (bool) led_val;
}

CAYENNE_OUT(VIRTUAL_CHANNEL_6)
{ 
	Cayenne.digitalSensorWrite(VIRTUAL_CHANNEL_6,!pb1_val);
}

CAYENNE_OUT(VIRTUAL_CHANNEL_7)
{ 
	Cayenne.digitalSensorWrite(VIRTUAL_CHANNEL_7,!pb2_val);
}

static void read_sensors()
{
  temp = dht.readTemperature();
  humidity = dht.readHumidity();
  pot_val = analogRead(pot_);
  button_val = button_switch(button);
  joy_val = analogRead(joy);
  pb1_val = pb1_switch(pb_1);
  pb2_val = pb2_switch(pb_2);
  digitalWrite(led,!led_val);
  String g = String(button_val) +"|" + String(pb1_val) +"|" + String(pb2_val) + "|" + String(pre_state) ;
  Serial.println(g);
}

void init_pwm()
{
  // setting PWM properties
  const int freq = 5000;
  const int ledChannel = 0;
  const int resolution = 10; 
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
}

static bool button_switch(int pin)
{
  bool stateButton = digitalRead(pin);
  if(stateButton == HIGH && pre_state == LOW) 
  {
      if(state_ == HIGH){
        state_ = LOW; 
      } else {
        state_ = HIGH; 
      }    
  }
 pre_state = stateButton;
 return state_;
}

static bool pb1_switch(int pin)
{
  bool stateButton = digitalRead(pin);
  if(stateButton == HIGH && pre1_state == LOW) 
  {
      if(state_1 == HIGH){
        state_1 = LOW; 
      } else {
        state_1 = HIGH; 
      }    
  }
 pre1_state = stateButton;
 return state_1;
}

static bool pb2_switch(int pin)
{
  bool stateButton = digitalRead(pin);
  if(stateButton == HIGH && pre2_state == LOW) 
  {
      if(state_2 == HIGH){
        state_2 = LOW; 
      } else {
        state_2 = HIGH; 
      }    
  }
 pre2_state = stateButton;
 return state_2;
}

void led_control()
{
  Serial.println("interrupt detected");
  if (state_ == false)
      {
        pb1_val = false ; pb2_val = false;
      }
      else if (state_ == true)
      {
         Serial.println("check");
        pb1_val = true ; pb2_val = true;
      }
}