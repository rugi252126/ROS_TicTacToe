/**
    Action.cpp
    Purpose: Handles the underlying behavior of the game. 
    Processing of the information that received from T3_BCU and the information that need to be sent to T3_BCU.

    @author Alfonso, Rudy Manalo
    @version 1.2 31/5/2020
*/

#include "com.hpp"
#include "Action.hpp"

/**
    Class constructor

    @param  none
    @return none
*/
MutualAction::MutualAction() :
  switch_state_e_(SwitchState::NPSD_E), 
  display_(DISPLAY_SNA)  {
}

/**
    Class destructor

    @param  none
    @return none
*/
MutualAction::~MutualAction() {
}

/**
    Push button switches state handling (from system state to application state)

    @param  state {switch state - see com.h for more information}
    @return none
*/
void MutualAction::set_switch_state(uint8_t state) {

    if(SWS_PSD == state) { // Button is pressed.
        switch_state_e_ = SwitchState::PSD_E;
        // Save the pressed state to be used later
        switch_psd_npsd_state_b_ = true; 
    }
    else if(SWS_NPSD == state) { // Button is not pressed.
        if(switch_psd_npsd_state_b_ != false) { // Button was pressed before.
            // Set pressed and then not pressed state
            switch_state_e_ = SwitchState::PSD_NPSD_E;
        }
        else {
            // Set to not pressed state
            switch_state_e_ = SwitchState::NPSD_E;
        }
    }
    else { // Button is in invalid state. Set to not pressed state
        switch_state_e_ = SwitchState::NPSD_E;        
    }
}

/**
    Resetting PSD_NPSD_E status flag.

    @param  none
    @return none
*/
void MutualAction::reset_psd_npsd_state() {

    // Clear it after PSD_NPSD_E state is read.
    switch_psd_npsd_state_b_ = false;
}

/**
    Setting of 7-segement display value

    @param  value {7-segment display value - see com.h for more information}
    @return none
*/
void MutualAction::set_display(uint8_t value) {

    // Plausibility check
    if((value >= DISPLAY_ZERO) && (value <= DISPLAY_NINE)) {
        // Valid display value. Assign and use it.
        display_ = value;        
    }
    else {
        // Invalid display. Set to SNA.
        display_ = DISPLAY_SNA;          
    }
}

/**
    Setting of buzzer state

    @param  state {buzzer state - see com.h for more information}
    @return none
*/
void MutualAction::set_buzzer_state(uint8_t state) {

    // Plausibility check
    if((state >=BUZZER_OFF) && (state <= BUZZER_ON)) {
        // Valid state. Assign and use it.
        buzzer_state_ = state;
    }
    else {
        // Invalid state. Set the buzzer state to Off.
        buzzer_state_ = BUZZER_OFF;
    }
}

/**
    Returns the switch state

    @param  none
    @return application switch state {see enum SwitchState}
*/
SwitchState MutualAction::get_switch_state() {

    return(switch_state_e_);
}

/**
    Returns the 7-segment display value

    @param  none
    @return 7-segment value {see com.h for more information}
*/
uint8_t MutualAction::get_display() {

    return(display_);
}

/**
    Returns the buzzer state

    @param  none
    @return buzzer state {see com.h for more information}    
*/
uint8_t MutualAction::get_buzzer_state() {

    return(buzzer_state_);
}
