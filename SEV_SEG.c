
#include <pic16f877a.h>
#include <pic.h>
#include <stdint.h>
#define _XTAL_FREQ 8000000

                                                                                                                                            
uint8_t numbers [10] = {0b00111111 , 0b00000110 , 0b01011011 , 0b01001111, 0b01100110 , 0b01101101 , 0b01111101 , 0b01000111 , 0b01111111 , 0b01100111 };

void SEG_Init (void){
    TRISD = 0x00;
    TRISA3 = 0;
    TRISA4 = 0;
    TRISA5 = 0;
    
}

void SEG_Write (uint16_t number){
        
 
        RA3 = 1;
        RA4 = 0;
        PORTD = (numbers[number]);
        //number -= (number/10)*10;
    


    
}


void SEG_Write1 (uint16_t number){
        
        
        RA3 = 0;
        RA4 = 1;
        PORTD = (numbers[number] );
        //RD0 = 1;


    
}