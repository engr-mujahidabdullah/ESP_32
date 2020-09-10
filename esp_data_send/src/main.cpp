/******************************************************************
 Created with PLATFORMIO IDE for VSCODE - 27.04.2020
 Project     : esp_data_send
 Libraries   : 
 Author      : Mujahid Abdullah
 Description : 
******************************************************************/

#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <dht.h>

#define dht_ 13
#define sensor_pin 34
#define x_axis 35
#define y_axis 32

void start_up(const char *ssid_, const char *pass_);
void write_data (WiFiClient cli,float field_1,float field_2,int field_3, int field_4, int field_5);

String apiKey = "UN0K0V9HM4RLE6B2";                         //  Enter your Write API key from ThingSpeak
const char *ssid =  "MikroStarTechPVT-witribe";             // replace with your wifi ssid and password
const char *pass =  "MstOffice123@#45";
const char* server = "api.thingspeak.com";


WiFiClient client;
dht sensor;

int sen_data = 0;
int x_data = 0;
int y_data = 0;

void setup() 
{ 
  Serial.begin(115200);
  start_up(ssid, pass); 
  pinMode(sensor_pin,INPUT);
  pinMode(x_axis,INPUT);
  pinMode(y_axis,INPUT);
}


void loop() 
{
  sensor.read11(dht_);
  float temp = sensor.temperature;
  float humidity = sensor.humidity;
  sen_data = analogRead(sensor_pin);
  x_data = analogRead(x_axis);
  y_data = analogRead(y_axis);

  write_data(client, temp, humidity, sen_data,x_data,y_data);    
  delay(10000);
}

void write_data (WiFiClient cli, float field_1,float field_2,int field_3,int field_4, int field_5)
{
  if(cli.connect(server,80))
  {  
       String Link="GET /update?api_key="+apiKey+"&field1=";  //Requeste webpage  
       Link = Link + String(field_1);
       Link = Link + "&field2=" + String(field_2) + "&field3=" + String(field_3);
       Link = Link + "&field4=" + String(field_4) + "&field5=" + String(field_5);
       Link = Link + " HTTP/1.1\r\n" + "Host: " + server + "\r\n" + "Connection: close\r\n\r\n";     
       cli.print(Link);
  }
  cli.stop();
}

void start_up(const char *ssid_, const char *pass_)
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