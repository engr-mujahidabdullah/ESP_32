/******************************************************************
 Created with PLATFORMIO IDE for VSCODE - 6.05.2020
 Project     : esp32_client
 Libraries   : 
 Author      : Mujahid Abdullah
 Description : 
******************************************************************/

#include <Arduino.h>
#include "WiFi.h"

//functions 
void start_up(const char *ssid_,const char *pass_);
void ctrl_led(String str);

//led control structure
struct led_cont
{
  int pin;
  String key;
  int state;
};

//define led
int red = 33;
int yellow = 26;
int green = 25;
int led[] = {red, yellow, green};

//wifi parameters
const char *ssid = "MikroStarTechPVT-witribe" ;
const char *password = "MstOffice123@#45" ;

//server parameters
const int port = 2200;
const char* host = "192.168.15.58"; 

//instances 
WiFiClient client;



void setup() 
{
   Serial.begin(115200);
   start_up(ssid,password);
   for(int i = 0; i < 3; i++)
   {pinMode(led[i],OUTPUT);digitalWrite(led[i],LOW);}
    
}

void loop() 
{
  if(!client.connect(host, port))
  {
    Serial.println("Connection failed");
    delay(2000);
    return;
  }

  while(client.connected())
  {  
    if (client.available()>0) 
      {
        String c = client.readString();
        Serial.print(c);
        ctrl_led(c);
      }

    if (Serial.available()>0)
      {
        String x = Serial.readString();
        client.println(x);
        Serial.println("data send: " + x);
        Serial.flush();
      }

    if (!client.connected())
      {
        client.stop();
      }  
  }
}

void start_up(const char *ssid_,const char *pass_)
{
    delay(10);
    Serial.println("Connecting to ");
    Serial.println(ssid_);
    WiFi.begin(ssid_, pass_);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
}

void ctrl_led(String str)
{
 char s = str.charAt(str.length()-1); int state = s - '0';
 String key = str.substring(0,str.length()-2);
 if(!key.compareTo("red")) digitalWrite(red,state);
 if(!key.compareTo("yellow")) digitalWrite(yellow,state);
 if(!key.compareTo("green")) digitalWrite(green,state);
}