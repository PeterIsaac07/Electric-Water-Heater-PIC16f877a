/***************************************************************************************/
/******************************           LCD_DRIVER         *********************************/
/***************************************************************************************/

#include <xc.h>

#include <pic16f877a.h>
#include <stdint.h>

#include "LCD_int.h"
#define _XTAL_FREQ 8000000


void LCD_Init_8BIT(uint8_t Disp_mode)
{
    
    __delay_ms(30);
    LCD_SendCommand(I8BIT_2LINE);
    __delay_ms(1);
    LCD_SendCommand(Disp_mode);
    __delay_ms(1);
    LCD_SendCommand(CLR_DISPLAY);
    __delay_ms(2);
    LCD_SendCommand(ENTRY_MODE);
    
}

void LCD_SendData(uint8_t Data)
{
    //DIO_SetPinValue(Ctrl_Port, RS, HIGH);
    RE2 = 1;

    //DIO_SetPinValue(Ctrl_Port, RW, LOW);
    
    //DIO_SetPinValue(Ctrl_Port, E, HIGH);
    RE1 = 1;

   // DIO_SetPortValue(Data_Port, Data);
    PORTD = Data;

    //DIO_SetPinValue(Ctrl_Port, E, LOW);
    RE1 = 0;

    __delay_ms(5);
    //DIO_SetPinValue(Ctrl_Port, E, HIGH);
    RE1 = 1;

}

void LCD_SendCommand(uint8_t Inst)
{
    //DIO_SetPinValue(Ctrl_Port, RS, LOW);
    RE2 = 0;
    //DIO_SetPinValue(Ctrl_Port, RW, LOW);

    
    //DIO_SetPinValue(Ctrl_Port, E, HIGH);
    RE1 = 1;

    //DIO_SetPortValue(Data_Port, Inst);
    PORTD = Inst;
    
   // DIO_SetPinValue(Ctrl_Port, E, LOW);
    RE1 = 0;

    __delay_ms(5);
    //DIO_SetPinValue(Ctrl_Port, E, HIGH);
    RE1 = 1;

}

void LCD_GoToXY(uint8_t Row, uint8_t Col)
{
    uint8_t inst = 0x80 | Col | Row;
    LCD_SendCommand(inst);
}

void LCD_SendStr(uint8_t* ptrStr)
{// edit with gotoxy so that it can write a long string on the 2 lines
    uint8_t i;
    for(i=0; ptrStr[i] != '\0'; i++)
    {
        LCD_SendData(ptrStr[i]);
    }
}



void LCD_SEND (uint8_t data){
    DATA_PORT = data;
}


void LCD_CMD( uint8_t CMD)
{
  // Select Command Register
  RS = 0;
  // Move The Command Data To LCD
  LCD_SEND(CMD);
  // Send The EN Clock Signal
  EN = 1;
  __delay_us(1000);
  EN = 0;
}


void LCD_Init()
{
  // IO Pin Configurations
  LCD_DATA_PORT_D = 0x00; // make the dataPORT output
  LCD_RS_D = 0;   //RS pin direction
  LCD_EN_D = 0;     //EN pin direction
  // The Init. Procedure
 // LCD_SEND(0x00);
  __delay_ms(30);
  __delay_us(1000);
  LCD_CMD(0x30);
  __delay_ms(5);
  LCD_CMD(0x30);
  __delay_us(150);
  LCD_CMD(0x30);
   
  
  LCD_CMD(0x30);
  LCD_CMD(0x80);
  LCD_CMD(0x01); 
  LCD_CMD(0x06); 
 
}

void LCD_Write_uint8_t(uint8_t Data)
{
  RS = 1;
  LCD_SEND(Data);
   EN = 1;
  __delay_us(1000);
  EN = 0;

}

void LCD_Write_String(uint8_t *str)
{
  int i;
  for(i=0;str[i]!='\0';i++)
    LCD_Write_uint8_t(str[i]);
}







void LCD_WriteNum(uint16_t num)
{
    if(num >= 10000)
    {
    	LCD_Write_uint8_t((num/10000)+'0');
    	num -= (num/10000)*10000;
    	LCD_Write_uint8_t((num/1000)+'0');
    	num -= (num/1000)*1000;
    	LCD_Write_uint8_t((num/100)+'0');
    	num -= (num/100)*100;
    	LCD_Write_uint8_t((num/10)+'0');
    	num -= (num/10)*10;
    	LCD_Write_uint8_t((num)+'0');
    	return ;
    }
    if(num >= 1000)
	{
		LCD_Write_uint8_t((num/1000)+'0');
		num -= (num/1000)*1000;
		LCD_Write_uint8_t((num/100)+'0');
		num -= (num/100)*100;
		LCD_Write_uint8_t((num/10)+'0');
		num -= (num/10)*10;
		LCD_Write_uint8_t((num)+'0');
		return ;
	}
    
    if(num >= 100)
	{
		LCD_Write_uint8_t((num/100)+'0');
		num -= (num/100)*100;
		LCD_Write_uint8_t((num/10)+'0');
		num -= (num/10)*10;
		LCD_Write_uint8_t((num)+'0');
		return ;
	}
    if(num >= 10)
	{
		LCD_Write_uint8_t((num/10)+'0');
		num -= (num/10)*10;
		LCD_Write_uint8_t((num)+'0');
		return ;
	}
    
    LCD_Write_uint8_t((num)+'0');
    return ;

}
