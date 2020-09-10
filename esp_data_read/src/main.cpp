/******************************************************************
 Created with PLATFORMIO IDE for VSCODE - 28.04.2020
 Project     : esp_data_read
 Libraries   : 
 Author      : Mujahid Abdullah
 Description : 
******************************************************************/

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#define led 2

void start_up(const char *ssid_, const char *pass_);
bool skipResponseHeaders();
void get_value(char *json_array );

String apiKey = "1A6QQ9SHHNXHGS7E";
String ID = "1047437";                         //  Enter your read API key from ThingSpeak
const char *ssid =  "AnchorRecovery";             // replace with your wifi ssid and password
const char *pass =  "12345678";
const char* server = "api.thingspeak.com";

WiFiClient client;

int timeout = 500 ;

void setup() 
{
    Serial.begin(115200);
    start_up(ssid, pass);
    pinMode(led, OUTPUT); 
}

void loop() 
{
  if(client.connect(server,80))
  { 
       String Link="GET /channels/" + ID + "/feeds.json?api_key=" + apiKey + "&results=1";
       Link = Link + " HTTP/1.1\r\n" + "Host: " + server + "\r\n" + "Connection: close\r\n\r\n";
       client.print(Link);
  }
    while (!skipResponseHeaders());  
    //String dat = client.readStringUntil('\n');
    //Serial.println(dat);
    while(client.connected()){
        if (client.available())
        {
            String dat = client.readStringUntil('\n');
            // Serial.println(dat);
            char buffer[dat.length()];
            if ( dat.indexOf('{',0) >= 0 ) 
            { 
                dat.toCharArray(buffer,dat.length());
                get_value(buffer);
            }
            //client.flush();
        }
    }
    //client.stop();
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

bool skipResponseHeaders() 
{ 
    char endOfHeaders[] = "\r\n\r\n";
    client.setTimeout(timeout); 
    bool ok = client.find(endOfHeaders); 
    //if (!ok) { Serial.println("No response or invalid response!"); } 
    return ok; 
} 

void get_value(char *json_array )
{
    StaticJsonBuffer<3*1024> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json_array);
    JsonObject& feed = root["feeds"][0];
    String field_ = feed["field1"];
    Serial.println(field_)  ;
    int _field_ = field_.toInt();
    if(_field_ == 1) digitalWrite(led,HIGH);
    else digitalWrite(led,LOW);
}