/**
    led_indicator.cpp
    Purpose: Handles the behavior of Led indicators

    @author Alfonso, Rudy Manalo
    @version 1.1 18/04/2020
*/

#include "led_indicator.h"
#include "com.h"
#include "project.h"

// Blinking frequency is 2Hz. Equivalent to 500ms.
const int LED_IND_BLINKING_FREQ_MS = 500/LED_INDICATOR_TASK_MS;

/**
    Class LedIndicator constructor
*/
LedIndicator::LedIndicator(int led_pin, uint8_t led_state):
led_pin_(led_pin),
led_state_(led_state),
led_ind_blinking_ctr_(LED_IND_BLINKING_FREQ_MS),
led_ind_blinking_stat_b_(false)
{
    // Initialize to digital ouput
    pinMode(led_pin_, OUTPUT);
}

/**
    Monitoring and processing of led indicator states

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{72} <br/>

    @param  none
    @return none
*/
void LedIndicator::update_led_indicator_state() {

    switch(led_state_) {
        case LED_OFF: {
            // Led is Off.
            digitalWrite(led_pin_, LOW);
            break;
        }
        case LED_BLINKING: {
            // Start the blinking counter
            led_ind_blinking_ctr_++;
            if(led_ind_blinking_ctr_ >= LED_IND_BLINKING_FREQ_MS) {
                led_ind_blinking_ctr_ = 0;

                if(led_ind_blinking_stat_b_ != false) {
                    led_ind_blinking_stat_b_ = false;
                    // Led is Off.
                    digitalWrite(led_pin_, LOW);
                }
                else {
                    led_ind_blinking_stat_b_ = true;
                    // Led is On.
                    digitalWrite(led_pin_, HIGH);
                }
            }
            break;
        }
        case LED_ON: {
            // Led is On.
            digitalWrite(led_pin_, HIGH);
            break;
        }
        default: {
            // Invalid state, Switch-off the Led.
            digitalWrite(led_pin_, LOW);
            break;
        }
    }
}

/**
    Clear and set to OFF the led indicator.
    It will be called once connection is lost.

    @param  none
    @return none
*/
void LedIndicator::clear_led_indicator() {

    digitalWrite(led_pin_, LOW);    
}

/**
    Process the data received from T3_CPU

    @param  led_state {recevied led indicator information}
    @return none
*/
void LedIndicator::set_led_indicator_state(uint8_t led_state) {

    led_state_ = led_state;
}
