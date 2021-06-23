
#include <pic16f877a.h>
void TIMER1_INIT (){

    TMR1IE = 1;     //Enable interrupt of Timer1
    PEIE = 1;       // Enable peripheral interrupt 
    GIE = 1;        // global interrupt enable
    T1CON = 0b00000001; //TIMER1 Config
    TMR1 = 0;
}