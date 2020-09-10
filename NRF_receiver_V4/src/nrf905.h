//nrf905.h

#include <SPI.h>
#include <Arduino.h>
//#include <RH_NRF905.h>
// ..... NRF905 commands .....

// write configuration
#define NRF_CMD_WR_CFG  0x00
// read configuration
#define NRF_CMD_RD_CFG  0x10
// write transmit data
#define NRF_CMD_WR_TX_DATA  0x20
// write transmit data
#define NRF_CMD_RD_TX_DATA  0x21
// write transmit address
#define NRF_CMD_WR_TX_ADDR  0x22
// read transmit data
#define NRF_CMD_RD_TX_ADDR  0x23
// read receive data
#define NRF_CMD_RD_RX_DATA  0x24

// ...... NRF905 configuration parameters ......

// NRF905 peer address should be configured by user
#define NRF_ADDR 0xE7E7E7E7
// NRF905 address length can be set to 1 to 4 bytes
#define NRF_ADDR_LENGTH  4
// NRF905 data length can be set to 1 to 32 bytes
#define NRF_DATA_LENGTH  32
// For receive mode
#define NRF_RX_MODE  0
// For transmit mode
#define NRF_TX_MODE  1
//For idle mode
#define NRF_ID_MODE 2

// select centre frequency together with NRF_HFREQ_PLL
#define NRF_CH_NO  108
// PLL mode = 433
#define NRF_HFREQ_PLL  0
// output power = -10dBm
#define NRF_PA_PWR  0
// reduced power operation disabled
#define NRF_RX_RED_PWR  0
// no retransmission of data packet
#define NRF_AUTO_RETRAN  0
// RX address width
#define NRF_RX_AFW  NRF_ADDR_LENGTH
// TX address width
#define NRF_TX_AFW  NRF_ADDR_LENGTH
// RX payload width
#define NRF_RX_PW  NRF_DATA_LENGTH
// TX payload width
#define NRF_TX_PW  NRF_DATA_LENGTH
// self-address
#define NRF_RX_ADDR  NRF_ADDR
// output clock frequency = 500KHz
#define NRF_UP_CLK_FREQ  0
// external clock signal enabled
#define NRF_UP_CLK_EN  0
// crystal oscillator = 16MHz
#define NRF_XOF  3
// CRC check enabled
#define NRF_CRC_EN  1
// 16 bit CRC mode
#define NRF_CRC_MODE  1

// This is the bit in the SPI address that marks it as a write
#define RH_SPI_WRITE_MASK 0x80

#define RH_NRF905_REG_MASK 0x0f

class my_NRF
{
    public:
    my_NRF(uint8_t chipEnablePin = 8, uint8_t txEnablePin = 9, uint8_t PWRPin = 10, uint8_t slaveSelectPin = 5, uint8_t DR_PIN = 34);
    
    void nrfPinSetup(void);

    void nrfInit(void); 

    void nrfEnable(void);

    void nrfDisable(void);

    //void nrfMode(uint8_t mode);

    void nrfSetMode(uint8_t mode);

    bool nrfIsDataReady();

    //void nrfInit(void);

    void nrfSendData(uint32_t addr, uint8_t* data, uint8_t len);

    void nrfWriteAddr(uint32_t addr);

    void nrfWriteData(uint8_t* data, uint8_t len);

    bool nrfReceiveData(uint32_t* addr, uint8_t* data, uint8_t len);

    uint32_t nrfReadAddress(void);

    uint32_t nrfReadData(void);

    uint8_t spiRead(uint8_t reg);

    uint8_t spiReadRegister(uint8_t reg);

    void printRegisters(void);

    private:
    /// This idle mode chip configuration
    uint8_t             _configuration;

    /// the number of the chip enable pin
    uint8_t             _chipEnablePin;

    /// The number of the transmit enable pin
    uint8_t             _txEnablePin;

    /// Number of octets in the buffer
    uint8_t             _bufLen;
    
    /// The receiver/transmitter buffer
    uint8_t             _buf[NRF_DATA_LENGTH];

    /// True when there is a valid message in the buffer
    bool                _rxBufValid;

    uint8_t             _PWRPin;

    uint8_t             _SSPin;
    uint8_t             _DRPin;

};