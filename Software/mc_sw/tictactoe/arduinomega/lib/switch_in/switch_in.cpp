/**
    switch_in.cpp
    Purpose: Read the digital input switches and provide the switch state.

    @author Alfonso, Rudy Manalo
    @version 1.1 9/4/2020
*/

#include "switch_in.h"
#include "com.h"
#include "project.h"


/** 
    Class SwitchIn constructor
*/
SwitchIn::SwitchIn(int switch_pin, uint8_t switch_state):
switch_pin_(switch_pin),
switch_state_(switch_state)
{
    // Initialize the switch pin to Input PullUp so that by default the state is high.
    pinMode(switch_pin_, INPUT_PULLUP);
}

/**
    Process the switch information

    @param none
    @return none
*/
void SwitchIn::update_switch_state() {

    bool tmp_b = true;

    // Read raw switch state
    tmp_b = digitalRead(switch_pin_);

    // Switch state plausibility check
    if(    (false == tmp_b)
        || (true  == tmp_b) ) {

        if(false == tmp_b) {
            // Switch is pressed.
            switch_state_ = SWS_PSD;
        }
        else {
            // Switch is not pressed.
            switch_state_ = SWS_NPSD;
        }
    }
    else {
        // Unknown switch state
        switch_state_ = SWS_SNA;
    }
}

/**
    Returns switch state

    @param  none
    @return switch state
*/
uint8_t SwitchIn::get_switch_state() {
    
    return(switch_state_);
}