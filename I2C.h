#ifndef _I2C_H
#define _I2C_H


#define I2C_BaudRate 100000

void I2C_Wait();
void I2C_INIT_MASTER ();
void I2C_Start ();
void I2C_Stop();
void I2C_Res();
void I2C_Ack(void);
void I2C_Nack(void);
uint8_t I2C_Write( uint8_t Data);
uint8_t I2C_Read_Byte(void);





#endif
