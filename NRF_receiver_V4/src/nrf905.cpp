#include <nrf905.h>
#include <SPI.h>

SPISettings settingsA(2000000, MSBFIRST, SPI_MODE0);

my_NRF::my_NRF(uint8_t chipEnablePin, uint8_t txEnablePin, uint8_t PWRPin, uint8_t slaveSelectPin, uint8_t DR_PIN)
{
    _chipEnablePin = chipEnablePin;
    _txEnablePin   = txEnablePin;
    _SSPin = slaveSelectPin;
    _PWRPin = PWRPin;
    _DRPin = DR_PIN;
}

void my_NRF::nrfPinSetup()
{
    pinMode(_chipEnablePin, OUTPUT);
    pinMode(_txEnablePin, OUTPUT);
    pinMode(_PWRPin,OUTPUT);
    pinMode(_SSPin,OUTPUT);
    pinMode(_DRPin,INPUT);

    digitalWrite(_chipEnablePin, LOW);
    digitalWrite(_txEnablePin, LOW);
    digitalWrite(_PWRPin, LOW);
    digitalWrite(_SSPin, HIGH);

    SPI.begin();
}

void my_NRF::nrfEnable()
{
    digitalWrite(_chipEnablePin,HIGH);
    digitalWrite(_PWRPin,HIGH);
}

void my_NRF::nrfDisable()
{
    digitalWrite(_chipEnablePin,LOW);
    digitalWrite(_PWRPin,LOW);
}

bool my_NRF::nrfIsDataReady(void)
{
    if(digitalRead(_DRPin) == 0) return false;
    else return true;
}

void my_NRF::nrfSetMode(uint8_t mode)
{
    if(mode == 0) 
    {
        digitalWrite(_txEnablePin,LOW);
        digitalWrite(_chipEnablePin,HIGH);
    }

    if(mode == 1) 
    {
        digitalWrite(_txEnablePin,HIGH);
        digitalWrite(_chipEnablePin,HIGH);
    }  

    if(mode == 2) 
    {
        digitalWrite(_txEnablePin,LOW);
        digitalWrite(_chipEnablePin,LOW);
    }  
}

void my_NRF::nrfInit()
{
    uint8_t cfg_data[10] = {0};
    // set TX/RX mode
    nrfSetMode(NRF_ID_MODE);
    cfg_data[0] = (NRF_CH_NO & 0xFF);
    cfg_data[1] = ((NRF_CH_NO >> 8) & 0x01)
    | ((NRF_HFREQ_PLL & 0x01) << 1)
    | ((NRF_RX_RED_PWR & 0x01) << 4)
    | ((NRF_AUTO_RETRAN & 0x01) << 5);
    cfg_data[2] = (NRF_RX_AFW & 0x07) | ((NRF_TX_AFW & 0x07) << 4);
    cfg_data[3] = (NRF_RX_PW & 0x3F);
    cfg_data[4] = (NRF_TX_PW & 0x3F);
    cfg_data[5] = (NRF_RX_ADDR & 0xFF);
    cfg_data[6] = ((NRF_RX_ADDR >> 8) & 0xFF);
    cfg_data[7] = ((NRF_RX_ADDR >> 16) & 0xFF);
    cfg_data[8] = (NRF_RX_ADDR >> 24);
    cfg_data[9] = (NRF_UP_CLK_FREQ & 0x01)
    | ((NRF_UP_CLK_EN & 0x01) << 2)
    | ((NRF_XOF & 0x03) << 3) | ((NRF_CRC_EN & 0x01) << 6)
    | ((NRF_CRC_MODE & 0x01) << 7);

    nrfDisable();
    digitalWrite(_SSPin,LOW);
    SPI.transfer(NRF_CMD_WR_CFG);
    SPI.transfer(cfg_data,sizeof(cfg_data));
    digitalWrite(_SSPin,HIGH);
    nrfEnable();
}

void my_NRF::nrfWriteAddr(uint32_t addr)
{
    nrfDisable();
    digitalWrite(_SSPin,LOW);
    SPI.transfer(NRF_CMD_WR_TX_ADDR);
    SPI.transfer(addr);
    digitalWrite(_SSPin,HIGH);
    nrfEnable();
}

void my_NRF::nrfWriteData(uint8_t* data, uint8_t len)
{
    nrfDisable();
    digitalWrite(_SSPin,LOW);
    SPI.transfer(NRF_CMD_WR_TX_ADDR);
    SPI.transfer(data, len);
    digitalWrite(_SSPin,HIGH);
    nrfEnable();
}

void my_NRF::nrfSendData(uint32_t addr, uint8_t* data, uint8_t len)
{
    nrfWriteAddr(addr);
    nrfWriteData(data, len);
}

uint32_t my_NRF::nrfReadAddress()
{
    uint32_t addr;
    nrfDisable();
    //spiChipSelect(channel, 0);
    digitalWrite(_SSPin,LOW);
    SPI.transfer(NRF_CMD_RD_TX_ADDR);
    SPI.beginTransaction(settingsA);
    addr = SPI.transfer32(0);
    SPI.endTransaction();
    digitalWrite(_SSPin,HIGH);
    nrfEnable();
    return addr;
}

uint8_t my_NRF::spiReadRegister(uint8_t reg)
{
    return spiRead((reg & RH_NRF905_REG_MASK) | NRF_CMD_RD_CFG);
}

void my_NRF::printRegisters()
{
    uint8_t registers[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

    uint8_t i;
    for (i = 0; i < sizeof(registers); i++)
	Serial.println(spiReadRegister(registers[i]),BIN);
}

uint8_t my_NRF::spiRead(uint8_t reg)
{
    uint8_t val;
    digitalWrite(_SSPin, LOW);
    SPI.transfer(reg & ~RH_SPI_WRITE_MASK); // Send the address with the write mask off
    val = SPI.transfer(0); // The written value is ignored, reg value is read
    digitalWrite(_SSPin, HIGH);
    return val;
}


uint32_t my_NRF::nrfReadData()
{
    uint32_t data_;
    //uint8_t cmd = NRF_CMD_RD_RX_DATA;
    nrfDisable();
    digitalWrite(_SSPin,LOW);

    SPI.transfer(NRF_CMD_RD_RX_DATA);
    SPI.beginTransaction(settingsA);
    data_ = SPI.transfer(0);
    SPI.endTransaction();
    digitalWrite(_SSPin,HIGH);
    nrfEnable();   
    return data_;
}
