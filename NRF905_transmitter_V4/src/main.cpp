//libraries
#include <Arduino.h>
#include <RH_NRF905.h>
#include <SPI.h>
#include <RHSPIDriver.h>

//Define functions
void NRF_init(RH_NRF905 _obj_, int cs);
void get_resp();
void select_trans();

//Var defines
#define cs1 25
#define cs2 5
#define led_pin 2

// Singleton instance of the radio driver
RH_NRF905 obj_(32,33,5); //chip en, tx en, slave select

//Globals
String resp = "";
bool check = true;
bool check_loop = false;

void setup() 
{
  Serial.begin(115200);

  NRF_init(obj_, cs1);  
  obj_.setChannel(108, false);obj_.spiWriteRegister(2,0x44);obj_.spiWriteRegister(3,0x20);obj_.spiWriteRegister(4,0x20);obj_.spiWriteRegister(5,0xE7);obj_.spiWriteRegister(6,0xE7);obj_.spiWriteRegister(7,0xE7);obj_.spiWriteRegister(8,0xE7);
  //obj_.printRegisters();

  NRF_init(obj_, cs2); 
  obj_.setChannel(108, false);obj_.spiWriteRegister(2,0x43);obj_.spiWriteRegister(3,0x20);obj_.spiWriteRegister(4,0x20);obj_.spiWriteRegister(5,0xE7);obj_.spiWriteRegister(6,0xE7);obj_.spiWriteRegister(7,0xE7);obj_.spiWriteRegister(8,0xE7);
  //obj_.printRegisters();

  Serial.println("init obj_ done");
}

void loop() 
{
  get_resp();
}

void NRF_init(RH_NRF905 _obj_, int cs)
{
  _obj_.setSlaveSelectPin(cs);

  if (!_obj_.init())
    Serial.println("init failed");
}

void get_resp()
{
  if(resp.compareTo("") == false)
  {
    Serial.println("Select Transmitter 1 for one and 2 for other");
    while(!Serial.available());
      resp = Serial.readString();
      Serial.println(resp);
      select_trans();
  }
}

void select_trans()
{
  if(resp.compareTo("1") == 0)
    {
      NRF_init(obj_,cs1); Serial.println("Transmitter_1 select");
      obj_.printRegisters();
    }

  if(resp.compareTo("2") == 0)
    {
      NRF_init(obj_,cs2); Serial.println("Transmitter_2 select");
      obj_.printRegisters();
    }
  resp = "";
}