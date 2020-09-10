
#include <Arduino.h>
#include <RH_NRF905.h>
#include <SPI.h>
#include <RHSPIDriver.h>

//Functions Declarations
void led_blink(void *pvParameters);
void get_resp();
void data_transmit();

// Singleton instance of the radio driver
RH_NRF905 obj_2(32,33,25); //chip en, tx en, slave select
RH_NRF905 obj_1(34,33,5); //chip en, tx en, slave select

//Globals
bool check = false; bool check_loop = false;
String resp = "";
uint8_t node_address = 0xE7;
uint8_t *addr = &node_address;

//declarations
#define led_pin 2


void setup() 
{
  Serial.begin(115200);

  //obj_1.setPromiscuous(true);obj_2.setPromiscuous(true);

  if (!obj_2.init())
    Serial.println("init failed");
  obj_2.setChannel(108, false);obj_2.spiWriteRegister(2,0x44);obj_2.spiWriteRegister(3,0x20);obj_2.spiWriteRegister(4,0x20);;obj_2.spiWriteRegister(5,0xE7);obj_2.spiWriteRegister(5,0xE7);obj_2.spiWriteRegister(6,0xE7);obj_2.spiWriteRegister(7,0xE7);obj_2.spiWriteRegister(8,0xE7);
  obj_2.printRegisters();

  if (!obj_1.init())
    Serial.println("init failed");
  obj_1.setChannel(108, false);obj_1.spiWriteRegister(2,0x44);obj_1.spiWriteRegister(3,0x20);obj_1.spiWriteRegister(4,0x20);;obj_1.spiWriteRegister(5,0xE7);obj_1.spiWriteRegister(5,0xE7);obj_1.spiWriteRegister(6,0xE7);obj_1.spiWriteRegister(7,0xE7);obj_1.spiWriteRegister(8,0xE7);
  obj_1.printRegisters(); 



  Serial.println("init obj_1 & Obj_2 done");
  pinMode(led_pin, OUTPUT);
  xTaskCreatePinnedToCore(led_blink, "led_blink", 10000, NULL, 1, NULL,1);
  Serial.println("Task led_blink Start");


}

void loop() 
{
  get_resp();
  data_transmit();
}

void get_resp()
{
  if(resp.compareTo("") == false)
  {
    Serial.println("Select Transmitter 1 for one and 2 for other");
    while(!Serial.available());
      resp = Serial.readString();
      Serial.println(resp);
      Serial.print("Selected Transmitter key is :"); Serial.println(resp);
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

void data_transmit()
{
  check = false; check_loop = false;
  uint8_t buffer1[RH_NRF905_MAX_MESSAGE_LEN];
  uint8_t length1 = sizeof(buffer1);

  uint8_t buffer1_[RH_NRF905_MAX_MESSAGE_LEN];
  uint8_t length1_ = sizeof(buffer1_);

  uint8_t buffer2[RH_NRF905_MAX_MESSAGE_LEN];
  uint8_t length2 = sizeof(buffer2);

  uint8_t buffer2_[RH_NRF905_MAX_MESSAGE_LEN];
  uint8_t length2_ = sizeof(buffer2_);

  memset(buffer1,0,sizeof(buffer1));memset(buffer2,0,sizeof(buffer2));
  //obj_1.setModeTx();obj_2.setModeTx();

  if(resp.compareTo("1") == 0)
    {
    
      Serial.println("Selected transmitter is 1");
      Serial.println("Enter data you want to send");
      while(!Serial.available()) vTaskDelay(1);
      
      if(Serial.available()>0)
      {
        String send_data = Serial.readString();
        send_data.toCharArray((char*)buffer1, length1);
        obj_1.send((uint8_t*)buffer1, length1); check = true;
        Serial.print("data send from transmitter 1: "); Serial.println((char*)buffer1);
        obj_1.waitPacketSent();
        Serial.flush();
        while(check_loop == false)
        {
          if(obj_1.available())
          {
            if(obj_1.recv(buffer1_, &length1_))
              {
                Serial.print("data recveive at Transmitter_1: "); Serial.println((char*)buffer1_);
                obj_1.clearRxBuf();obj_2.clearRxBuf();;vTaskDelay(5);
                check_loop = true;
              }vTaskDelay(5);
          }vTaskDelay(5);
        }
        resp = "";vTaskDelay(5);
      }
    }

  if(resp.compareTo("2") == 0)
    {
  
      Serial.println("Selected transmitter is 2");
      Serial.println("Enter data you want to send");
      while(!Serial.available()) vTaskDelay(1);
      
      if(Serial.available()>0)
      {
        String send_data = Serial.readString();
        send_data.toCharArray((char*)buffer2, length2);
        obj_2.send((uint8_t*)buffer2, length2); check = true;
        Serial.print("data send from transmitter 2: "); Serial.println((char*)buffer2);
        obj_2.waitPacketSent();
        Serial.flush();
        while(check_loop == false)
        {
          obj_2.setModeRx();
          if(obj_2.available())
          {
            if(obj_2.recv(buffer2_, &length2_))
              {
                Serial.print("data receive at Transmitter_2 : "); Serial.println((char*)buffer2_);
               obj_1.clearRxBuf();obj_2.clearRxBuf();;vTaskDelay(5);
                check_loop = true;
              }vTaskDelay(5);
          }vTaskDelay(5);
        }
        resp = "";vTaskDelay(5);
      }
    }
  if(resp.compareTo("") != 0)
  {
    Serial.println("Invalid Transmitter Key");
    resp = "";vTaskDelay(1);
  }
  vTaskDelay(5);
  memset(buffer1,0,sizeof(buffer1));memset(buffer2,0,sizeof(buffer2));
  memset(buffer1_,0,sizeof(buffer1_));memset(buffer2_,0,sizeof(buffer2_));
  obj_1.clearRxBuf();obj_2.clearRxBuf();Serial.flush();
}