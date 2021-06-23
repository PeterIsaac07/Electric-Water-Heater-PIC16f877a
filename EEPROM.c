#include <stdint.h>

#include "I2C.h"

uint8_t EEPROM_Read(uint8_t address){
    uint8_t data;
    I2C_Start();
    I2C_Write(0b10100000); // Slave I2C Device Address + Write
    I2C_Write(address); // Slave I2C Device Address + Write
    I2C_Res();
    I2C_Write(0b10100001); // Slave I2C Device Address + Read
    
    data = I2C_Read_Byte();
    I2C_Nack();
    I2C_Stop();
    return data;
}

void EEPROM_Write (uint8_t address , uint8_t data){

    I2C_Start();
    I2C_Write(0b10100000); // Slave I2C Device Address + Write
    I2C_Write(address); // Data To Be Sent
    I2C_Write(data);
    I2C_Stop();

}