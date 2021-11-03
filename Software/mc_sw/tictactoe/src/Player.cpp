/**
    Player.cpp
    Purpose: Handles the underlying behavior of the game. 
    Processing of the information that received from T3_BCU and the information that need to be sent to T3_BCU.

    @author Alfonso, Rudy Manalo
    @version 1.2 31/5/2020
*/

#include "com.hpp"
#include "Player.hpp"

/**
    Class constructor

    @param  none
    @return none
*/
Player::Player() :
  led_indicator_(LED_OFF) {
}

/**
    Class destructor

    @param  none
    @return none
*/
Player::~Player() {
}

/**
    Led indicator state handling

    @param  state {Led state - see com.h for more information}
    @return none
*/
void Player::set_led_indicator_state(uint8_t state) {

    // Plausibility check
    if(state < LED_SNA) {
        // Valid state. Assign and use it.
        led_indicator_ = state;        
    }
    else {
        // Invalid state. Set it to Off.
        led_indicator_ = LED_OFF;         
    }
}


/**
    Returns the Led state

    @param  none
    @return Led state {see com.h for more information}
*/
uint8_t Player::get_led_indicator_state() {

    return(led_indicator_);
}