#include <Arduino.h>
#include <nrf905.h>

my_NRF obj_(32,33,35,25,34);
void setup()
{
  Serial.begin(115200);

  obj_.nrfPinSetup();
  obj_.nrfInit();
  obj_.nrfSetMode(1);
  obj_.printRegisters();
  obj_.nrfSetMode(NRF_ID_MODE);
}

void loop()
{
  uint8_t dat[] ={100};
  obj_.nrfSetMode(NRF_TX_MODE);
  
  obj_.nrfSendData(0x20, dat, sizeof(dat));
  obj_.nrfSetMode(NRF_ID_MODE);
  obj_.printRegisters();
  delay(3000);
}
