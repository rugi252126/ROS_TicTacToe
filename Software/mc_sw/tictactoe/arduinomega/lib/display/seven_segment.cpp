/**
    seven_segment.cpp
    Purpose: Handles the display of Player1 score, Player2 score and Timer

    @author Alfonso, Rudy Manalo
    @version 1.1. 18/04/2020
*/

#include "seven_segment.h"
#include "io_pins.h"
#include "com.h"

/**
    Class SevenSegment constructor
*/
SevenSegment::SevenSegment() {

    // Initialize all to digital output
    pinMode(DO__CTRL_A, OUTPUT); // PH3
    pinMode(DO__CTRL_B, OUTPUT); // PH4
    pinMode(DO__CTRL_C, OUTPUT); // PH5
    pinMode(DO__CTRL_D, OUTPUT); // PH6
    pinMode(DO__SCORE1_CTRL_LT_BI_RBO, OUTPUT); // PB4
    pinMode(DO__SCORE2_CTRL_LT_BI_RBO, OUTPUT); // PB5
    pinMode(DO__TIMER_CTRL_LT_BI_RBO, OUTPUT);  // PB6

    // Set display state to blank
    display_type_e_ = DisplayType::BLANK_E;
}

/**
    Process the value that need to be displayed and allocate to control ports at the same time
    Player1/2 score boards and timer board are using common control ports hence timing is very critical.
    To avoid having flicking display, direct access of port register is necessary because the port state need to be updated as fast as possible.

    @param  value {0 to 9}
    @return none
*/
void SevenSegment::select_display(uint8_t value) {

    // Clear all display ports
    PORTH = (PORTH & 0x87);
    // Assign new display value. Multiply by 8 because the displays are bit3, bit4, bit5 and bit6.
    value = (value * 8); 
    PORTH = (PORTH | value);     
}

/**
    Monitoring and processing of 7-segement displays
    Player1/2 score boards and timer board are using common control ports hence timing is very critical.
    To avoid having flicking display, direct access of port register is necessary because the port state need to be updated as fast as possible.

    @param  none
    @return none
*/
void SevenSegment::update_display() {

    switch(display_type_e_) {
        case DisplayType::BLANK_E: {
            // Switch-off all control ports.
            PORTB = (PORTB & 0x8f);
            // Move to next state
            display_type_e_ = DisplayType::SCORE1_E;
            break;
        }
        case DisplayType::SCORE1_E: {
            if(score1_ <= DISPLAY_NINE) {
                // Clear all control ports
                PORTB = (PORTB & 0x8f); 
                // Assign new control
                PORTB = (PORTB | 0x10); 
                select_display(score1_);
            }
            else{
                // Invalid value
                // Switch-off only the affected control port. Player1 score board display is blank.
                PORTB = (PORTB & 0xef); 
            }

            // Move to next state
            display_type_e_ = DisplayType::SCORE2_E;
            break;
        }
        case DisplayType::SCORE2_E: {
            if(score2_ <= DISPLAY_NINE) {
                // Clear all control ports
                PORTB = (PORTB & 0x8f); 
                // Assign new control
                PORTB = (PORTB | 0x20); 
                select_display(score2_);
            }
            else{
                // Invalid value
                // Switch-off only the affected control port. Player2 score board display is blank.
                PORTB = (PORTB & 0xdf);
            }
            
            // Move to next state
            display_type_e_ = DisplayType::TIMER_E;
            break;
        }
        case DisplayType::TIMER_E: {       
            if(timer_ <= DISPLAY_NINE) {
                // Clear all control ports
                PORTB = (PORTB & 0x8f); 
                // Assign new control
                PORTB = (PORTB | 0x40); 
                select_display(timer_);
            }
            else{
                // Invalid value
                // Switch-off only the affected control port. Timer board display is blank.
                PORTB = (PORTB & 0xbf);
            }

            // Move to next state
            display_type_e_ = DisplayType::SCORE1_E;
            break;
        }
        default: {
            // no action
            break;
        }
    }
}

/**
    Clear and set to OFF all 7-segment displays
    It will be called once connection is lost.

    @param  none
    @return none
*/
void SevenSegment::clear_display() {
            
    digitalWrite(DO__SCORE1_CTRL_LT_BI_RBO, LOW);
    digitalWrite(DO__SCORE2_CTRL_LT_BI_RBO, LOW);
    digitalWrite(DO__TIMER_CTRL_LT_BI_RBO, LOW);   
    digitalWrite(DO__CTRL_A, LOW);
    digitalWrite(DO__CTRL_B, LOW);
    digitalWrite(DO__CTRL_C, LOW);
    digitalWrite(DO__CTRL_D, LOW); 
}

/**
    Process the data received from T3_CPU

    @param  score1 {Received Player1 score information}
            score2 {Received Player2 score information}
            timer  {Received timer information}
    @return none
*/
void SevenSegment::set_display(uint8_t score1, uint8_t score2, uint8_t timer) {

    score1_ = score1;
    score2_ = score2;
    timer_  = timer;
}