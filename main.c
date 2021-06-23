
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "config.h"
#include <stdint.h>

#include "LCD_int.h"
#include "I2C.h"
#include "ADC.h"
#include "EEPROM.h"
#include "SEV_SEG.h"
#include "TIMER.h"
#include "PROJECT_CFG.H"

uint16_t DESIRED_TEMP = 60;
uint16_t ADC_TEMP ;
uint16_t ADC_counter = 0;
uint16_t MODE = WORKING_MODE;
uint16_t MODE_COUNTER = 0;
uint16_t ADC_READINGS [10] ={};
uint16_t READINGS_INDEX = 0;
uint16_t average;
uint16_t LED_COUNTER=0;

void calc_average (void);
void main(void) {
    
    ////////////////// Internals initialization ///////////////////
    COOLER_PIN_DIRECTION=OUTPUT;      //Cooler Pin 
    HEATER_PIN_DIRECTION=OUTPUT;      //Heater Pin
    TEMP_UP_PIN_DIR = INPUT;         //for control pins
    TEMP_DOWN_BUTTON = INPUT;        //for control pins
    RESET_TEMP_DIR = INPUT;
    HEATER_LED_DIR = OUTPUT;
    INTEDG = 0;                      //FIRE AN INTERRUPT ON FALLING EDGE
    INTE = 1;                        //enables RB0 external interrupt
    //////////// Peripherals initialization ///////////////////////
    SEG_Init();         //Initialize Seven Segments
    ADC_Init();         //Initialize ADC
    LCD_Init();         //Initialize LCD
    TIMER1_INIT ();     //Initialize timer1
    I2C_INIT_MASTER();  //Initialize I2C as a Master
    ///////////////////////////////////////////////////////////////
    LCD_SendCommand(DISP_ON_CBON);
    DESIRED_TEMP = (EEPROM_Read(0x00))*10;  //Read Saved Temp first digit  From EEPROM
    DESIRED_TEMP += (EEPROM_Read(0x01)) ;   //Read Saved Temp Second digit From EEPROM
   

    while(1){
        if( (TEMP_UP_BUTTON == PRESSED || TEMP_DOWN_BUTTON == PRESSED) && MODE == WORKING_MODE){ // user pressed temp_down button
            while (TEMP_UP_BUTTON == PRESSED || TEMP_DOWN_BUTTON == PRESSED);   // Do nothing untill button is released
            MODE = SETTING_MODE;                 // change the mode into temprature setting mode
            LCD_SendCommand(CLR_DISPLAY);        // clear the LCD display
            LCD_Write_String("CHANGING TEMP");   
            HEATER = OFF;                        // turn of heater
            COOLER = OFF;                        // turn of cooler
            MODE_COUNTER = 0;                    // set or reset the counter to zero to start counting 5 seconds
            continue;
        }
        if (MODE == SETTING_MODE && TEMP_DOWN_BUTTON == PRESSED && DESIRED_TEMP != MINIMUM_TEMP){ //CHECKS IF THE USER IS ATTEMPTING TO DECREASE TEMP
            while (TEMP_DOWN_BUTTON == PRESSED); 
            DESIRED_TEMP = DESIRED_TEMP - 5;     //decrease the desired temp
            MODE_COUNTER = 0;                    // set or reset the counter to zero to start counting 5 seconds
            EEPROM_Write(FIRST_DIGIT_ADDRESS ,DESIRED_TEMP/10); //write the temp to the eeprom
            EEPROM_Write(SECOND_DIGIT_ADDRESS,DESIRED_TEMP - (DESIRED_TEMP/10)*10);
            continue;
        }
        if (MODE == SETTING_MODE && TEMP_UP_BUTTON == PRESSED && DESIRED_TEMP != MAX_TEMP ){     //CHECKS IF THE USER IS ATTEMPTING TO iNCREASE TEMP
            while (TEMP_UP_BUTTON == PRESSED);   //Do nothing while the key is pressed
            DESIRED_TEMP = DESIRED_TEMP + 5;     //increase the desired temp
            MODE_COUNTER = 0;                    // set or reset the counter to zero to start counting 5 seconds
            EEPROM_Write(0x00,DESIRED_TEMP/10);  // store the first digit of the desired temp
            EEPROM_Write(0x01,DESIRED_TEMP - (DESIRED_TEMP/10)*10);
            continue;
        }
        if (  MODE == WORKING_MODE ){                                // the temp is already set
             if( average > (DESIRED_TEMP+5) && COOLER == OFF){      // start the cooling process
                LCD_SendCommand(CLR_DISPLAY);                        // clears the lcd display
                LCD_Write_String("COOLING");                         // writes cooling state on lcd
                HEATER_LED = OFF;                                    // ensures that the heater led is off
                HEATER=OFF;                                          // TURNS HEATER OFF
                COOLER=ON;                                           // Start cooler
                continue;
            }
             else if( average <(DESIRED_TEMP-5) && HEATER == OFF){
                LCD_SendCommand(CLR_DISPLAY);
                LCD_Write_String("HEATING");
                COOLER=OFF;                                 //ensures that the cooler is off
                HEATER=ON;                                  //Start heater
            }
        }
        
        if (MODE == WORKING_MODE){
         SEG_Write(ADC_TEMP/10); // write the first digit of the temp on the first 7 seg
         __delay_us(80);
         SEG_Write1(ADC_TEMP - (ADC_TEMP/10)*10); // write the second digit of the temp on the sec 7 seg
        }
        else if (MODE == SETTING_MODE){ //ie user is changing the desired temprature 
            SEG_Write(DESIRED_TEMP/10); // write the first digit of the temp on the first 7 seg
            SEG_Write1(DESIRED_TEMP - (DESIRED_TEMP/10)*10 );// write the second digit of the temp on the sec 7 seg
        }
    }
          return;
}


void __interrupt() ISR (void){
    if (TMR1IF){ //if timer1 interrupt flag is set
        if(MODE == SETTING_MODE){
            MODE_COUNTER++; //increment the counter used to changes modes between temp setting and heater/cooler working
            if(MODE_COUNTER==MODE_TRANSITION_PERIOD){ // ie. 5 seconds have passed
                MODE_COUNTER=0; //return the counter to zero 
                MODE = WORKING_MODE; // return the mode back to working
            }
        }
    
        else {
            ADC_counter++;                                 //increment the counter used to get a new ADC reading 
            if(ADC_counter == HUNDREAS_MS_PERIOD){          // ie 100 ms have passed
                ADC_counter=0;   
                ADC_TEMP= ADC_Read();
                ADC_READINGS[READINGS_INDEX] = ADC_TEMP;    //store the reading ine the global variable
                READINGS_INDEX++;
                calc_average();                             //calculate the average of the readings array

                if (READINGS_INDEX == 10){                  //ensure that the index of the array is never more than 9
                    READINGS_INDEX=0;
                }
            }
            if (HEATER == ON){
                LED_COUNTER++;
                if (LED_COUNTER == ONE_SEC_PERIOD){
                    HEATER_LED = ~HEATER_LED;  // BLINKS THE LED EVERY SECOND
                    LED_COUNTER =0;
                }
            
            }
        }
        TMR1IF = 0; // clear the interrupt flag
    }
    if (INTF){
        DESIRED_TEMP = 60;
        EEPROM_Write(FIRST_DIGIT_ADDRESS ,DESIRED_TEMP/10); //write the temp to the eeprom
        EEPROM_Write(SECOND_DIGIT_ADDRESS,DESIRED_TEMP - (DESIRED_TEMP/10)*10);
        INTF = 0;
    }
    
 }
void calc_average (void){       //no arguments have to be passed since the array is global   
    average = 0;
    for (int i = 0 ; i < 10 ; i++){
       average = average + ADC_READINGS[i];    
    }    
    
    average = average/10;

    }
