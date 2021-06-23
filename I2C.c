
#include <pic16f877a.h>
#include <stdint.h>

#include "I2C.h"

#define _XTAL_FREQ 8000000


void I2C_Wait(){
    while (SSPSTATbits.R_nW ||(SSPCON2 & 0x1F) )
    {};
    
}
void I2C_INIT_MASTER (){
    SSPCON = 0x28; //enables the i2c and sets it to master mode 
    SSPCON2 = 0x00;
    //SSPCON2bits.RSEN = 1; //enable recieve for mastet
    SSPADD = ((_XTAL_FREQ/4)/I2C_BaudRate) ;
    TRISC3 = 1;
    TRISC4 = 1;

}
void I2C_Start (){
//---[ Initiate I2C Start Condition Sequence ]---
  I2C_Wait();
  SEN = 1;


}

void I2C_Stop()
{
  //---[ Initiate I2C Stop Condition Sequence ]---
  I2C_Wait();
  PEN = 1;
}

void I2C_Res()
{
  //---[ Initiate I2C Restart Condition Sequence ]---
  I2C_Wait();
  RSEN = 1;
}


void I2C_Ack(void)
{
  //---[ Send ACK - For Master Receiver Mode ]---
  I2C_Wait();
  ACKDT = 0; // 0 -> ACK, 1 -> NACK
  ACKEN = 1; // Send ACK Signal!
}


void I2C_Nack(void)
{
  //---[ Send NACK - For Master Receiver Mode ]---
  I2C_Wait();
  ACKDT = 1; // 1 -> NACK, 0 -> ACK
  ACKEN = 1; // Send NACK Signal!
}

uint8_t I2C_Write(uint8_t Data)
{
  //---[ Send Byte, Return The ACK/NACK ]---
  I2C_Wait();
  SSPBUF = Data;
  I2C_Wait();
  return ACKSTAT;
}


uint8_t I2C_Read_Byte(void)
{
  //---[ Receive & Return A Byte ]---
  SSPIF = 0;       // Clear The Interrupt Flag Bit

  RCEN = 1;        // Enable & Start Reception
  while(!SSPIF);   // Wait Until Completion
  SSPIF = 0;       // Clear The Interrupt Flag Bit
  return SSPBUF;   // Return The Received Byte
}