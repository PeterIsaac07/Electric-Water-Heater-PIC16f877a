#include <xc.h>
#include <pic16f877a.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000


    void ADC_Init (void){
    ADCON0 = 0b01010001;
    ADCON1 = 0b11001001;
    __delay_us(30);    
}
uint16_t ADC_Read (){
    float reading ;
    GO_DONE = 1;  // Start A/D Conversion
    while(ADCON0bits.GO_DONE); //POLLING
    reading = (ADRESH << 8) + ADRESL;
    
    return reading*0.4888;
    

}