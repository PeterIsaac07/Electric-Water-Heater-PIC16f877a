#ifndef _PROJECT_CFG_H
#define _PROJECT_CFG_H
/*
 ///////////////////////////////////////////////////////////////////
 Frequentely needed definations to ensure readability, mentainability 
 and portability of the code
 ///////////////////////////////////////////////////////////////////
 
 */
#define INITIAL_TEMP             60
#define _XTAL_FREQ               8000000
#define SETTING_MODE             0
#define WORKING_MODE             1
#define TEMP_DOWN_BUTTON         RB1
#define TEMP_UP_BUTTON           RB2
#define COOLER                   RC2
#define HEATER                   RC5
#define PRESSED                  0
#define OFF                      0
#define ON                       1
#define MODE_TRANSITION_PERIOD   156
#define FIRST_DIGIT_ADDRESS      0X00
#define SECOND_DIGIT_ADDRESS     0X01
#define MINIMUM_TEMP             35
#define MAX_TEMP                 75
#define HEATER_LED               RB7
#define COOLER_PIN_DIRECTION     TRISC2
#define HEATER_PIN_DIRECTION     TRISC5
#define TEMP_DOWN_PIN_DIR        TRISB1
#define TEMP_UP_PIN_DIR          TRISB2
#define HEATER_LED_DIR           TRISB7
#define RESET_TEMP_DIR           TRISB0
#define INPUT                    1
#define OUTPUT                   0
#define HUNDREAS_MS_PERIOD       5
#define ONE_SEC_PERIOD           45         






   




#endif
