/******************************************************************
 Created with PLATFORMIO IDE for VSCODE - 5.05.2020
 Project     : esp32_server
 Libraries   : 
 Author      : Mujahid Abdullah
 Description : 
******************************************************************/

//include libraries
#include <Arduino.h>
#include "WiFi.h"

//functions 
void start_up(const char *ssid_,const char *pass_);

//Globals
int Port = 80;
bool check_client = false;

//wifi parameters
const char *ssid = "MikroStarTechPVT-witribe" ;
const char *password = "MstOffice123@#45" ;
int status = WL_IDLE_STATUS;


//instances 
WiFiServer server_(Port);
WiFiClient client_;

void setup() 
{
  Serial.begin(115200);
  start_up(ssid,password);
  server_.begin();
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

void loop() 
{
  client_ = server_.available();
  if(client_)
    {
      Serial.println("Client connected");
      while (client_.connected()) 
      {
        if (client_.available()>0) 
          {
            String c = client_.readString(); 
            Serial.println("data received: " + c);
            client_.flush();
          }

        if (Serial.available()>0)
          {
            String x = Serial.readString();
            client_.println(x);
            Serial.println("data send: " + x);
            Serial.flush();
          }
        delay(10);
      }
      client_.stop();
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
