
#include <Arduino.h>
#include <RH_NRF905.h>
#include <SPI.h>

//Functions Declarations
void led_blink(void *pvParameters);
void recv_response();
void acknowleged();

// Singleton instance of the radio driver
RH_NRF905 obj_(32,33,5); //chip en, tx en, slave select


//Globals
bool check = false;
uint8_t query[] = "Acknowleged by 1";
uint8_t ID = 1;
uint8_t transmitters[] = {0,100,50,10};
uint8_t header; uint8_t header_f ;
//declarations
#define led_pin 2

void setup() 
{
  Serial.begin(115200);
   if (!obj_.init())
    Serial.println("init failed");

  Serial.println("init done");
  obj_.setChannel(108, false);obj_.spiWriteRegister(2,0x44);obj_.spiWriteRegister(3,0x20);obj_.spiWriteRegister(4,0x20);obj_.spiWriteRegister(5,0xE7);obj_.spiWriteRegister(5,0xE7);obj_.spiWriteRegister(6,0xE7);obj_.spiWriteRegister(7,0xE7);obj_.spiWriteRegister(8,0xE7);
  obj_.setHeaderId(ID);//obj_.setHeaderFrom(2);obj_.setHeaderTo(200);
  //obj_.setPromiscuous(true);
  pinMode(led_pin, OUTPUT);
  xTaskCreatePinnedToCore(led_blink, "led_blink", 10000, NULL, 1, NULL,1);
  Serial.println("Task led_blink Start");
  obj_.printRegisters();

}

void loop() 
{ 
 recv_response();
}

void recv_response()
{
  uint8_t buffer[RH_NRF905_MAX_MESSAGE_LEN];
  uint8_t length = sizeof(buffer);
  
  if(obj_.available()>0)
  { 
    header = obj_.headerId();
    header_f = obj_.headerFrom();
        if(obj_.recv(buffer, &length))
        {
          for(int i = 0; i < sizeof(transmitters); i++)
            {
              if (header == transmitters[i])
              {
                //Serial.print("data recv: "); 
                Serial.println((char*)buffer);
                //Serial.print("data received from :");Serial.println(obj_.headerId());
                check = true;
              }
            }  
            //obj_.clearRxBuf();
            memset(buffer,0,sizeof(buffer));
            acknowleged();
        }  
    }
}

void acknowleged()
{
  for(int i = 0; i < sizeof(transmitters); i++)
    {
      if (header == transmitters[i])
      {
        vTaskDelay(ID * 100); obj_.setHeaderFrom(header_f);
        obj_.send(query, sizeof(query));
        obj_.waitPacketSent();
        //Serial.print("Acknowleged to transmitter :");Serial.println(obj_.headerId());
      }
    }  
}

void led_blink(void *pvParameters)
{
  for(;;)
  {
    if(check == true)
    {
        digitalWrite(led_pin, HIGH);
        vTaskDelay(2000);
        digitalWrite(led_pin, LOW);
        check = false;
    }
    vTaskDelay(1);
  }
}