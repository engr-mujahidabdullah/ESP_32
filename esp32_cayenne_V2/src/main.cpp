//import lib
#include <Arduino.h>
#include <CayenneMQTTESP32.h>
#include <Wire.h>
#include <DHT.h>
#include <Thread.h>
#include <WiFi.h>

//init functions
static void read_sensors();
//void IRAM_ATTR  button_check();
void master(int bt);

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
#define button_ 12
#define joy 35
#define led 32
#define pb_1 25
#define pb_2 33

//button struct
struct Button 
{
    int PIN;
    bool _state_;
    bool pressed;
};

//define buttons
Button button0 = {button_,true,false};
Button button1 = {pb_1,false,false};
Button button2 = {pb_2,false,false};

//Define instances
DHT dht(DHTPIN, DHTTYPE);
Thread sensor_thread = Thread();

//initialize values
float temp = 0;
float humidity = 0;
int pot_val = 0;
int joy_val = 0;
bool led_val = 1;

//Define Virtual channels
#define VIRTUAL_CHANNEL_0 0
#define VIRTUAL_CHANNEL_1 1
#define VIRTUAL_CHANNEL_2 2
#define VIRTUAL_CHANNEL_3 3
#define VIRTUAL_CHANNEL_4 4
#define VIRTUAL_CHANNEL_5 5
#define VIRTUAL_CHANNEL_6 6
#define VIRTUAL_CHANNEL_7 7

//functions 
void IRAM_ATTR  button_check(void* arg) 
{
    Button* s = static_cast<Button*>(arg);
    s->pressed = digitalRead(s->PIN);
    //Serial.println(s->_state_);
    if(s->pressed == true) 
    {
      //Serial.println("interuppt");
      if(s->_state_ == HIGH)
      {
        s->_state_ = LOW; 
      } 
      else if (s->_state_ == LOW)
      {
        s->_state_ = HIGH; 
      }  
    }
 
}

void master(void* arg)
{
  Button* s = static_cast<Button*>(arg);
  s->pressed = digitalRead(s->PIN);
  if (s->pressed == HIGH)
  {
    if(s->_state_ == HIGH)
    {
      button0._state_ = false, button1._state_ = true;button2._state_ = true;
    }
    else if(s->_state_ == LOW)
    {
      button0._state_ = true, button1._state_ = false;button2._state_ = false;
    }
  }
}

void setup() 
{
  Serial.begin(115200);
  dht.begin();
  Cayenne.begin(username,cay_password,client_id,ssid,wifi_password);
  pinMode(button_, INPUT_PULLUP);
  pinMode(button1.PIN, INPUT_PULLUP);
  pinMode(button2.PIN, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  sensor_thread.onRun(read_sensors);
  sensor_thread.setInterval(500);
  //attachInterrupt(button_, master, RISING);
  attachInterruptArg(button0.PIN, master, &button0, RISING);
  attachInterruptArg(button1.PIN, button_check, &button1, RISING);
  attachInterruptArg(button2.PIN, button_check, &button2, RISING);
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
	Cayenne.digitalSensorWrite(VIRTUAL_CHANNEL_3,!button0._state_);
}

CAYENNE_OUT(VIRTUAL_CHANNEL_4)
{ 
  //button_val = digitalRead(button);
	Cayenne.virtualWrite(VIRTUAL_CHANNEL_4,joy_val);
}

CAYENNE_IN(VIRTUAL_CHANNEL_5)
{ 
  led_val = !getValue.asInt();
  led_val = (bool) led_val;
}

CAYENNE_OUT(VIRTUAL_CHANNEL_6)
{ 
	Cayenne.digitalSensorWrite(VIRTUAL_CHANNEL_6,button1._state_);
}

CAYENNE_OUT(VIRTUAL_CHANNEL_7)
{ 
	Cayenne.digitalSensorWrite(VIRTUAL_CHANNEL_7,button2._state_);
}


static void read_sensors()
{
   temp = dht.readTemperature();	  //read temperature using DHT11 sensor 
  humidity = dht.readHumidity();	  //read humidity using DHT11 sensor 
  pot_val = analogRead(pot_);	      //read analog value from potentiometer
  joy_val = analogRead(joy);		    //read analog value of joystick axis 
  digitalWrite(led,!led_val);	      //Digital write on LED pin
    String g = String(!button0._state_) +"|" + String(button1._state_) +"|" + String(button2._state_) + "|" ;
    Serial.println(g);
}
