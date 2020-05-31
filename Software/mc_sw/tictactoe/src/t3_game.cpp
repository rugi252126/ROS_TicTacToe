/**
    t3_game.cpp
    Purpose: Handles the underlying behavior of the game. 
    Processing of the information that received from T3_BCU and the information that need to be sent to T3_BCU.

    @author Alfonso, Rudy Manalo
    @version 1.2 31/5/2020
*/

#include "t3_game.h"


//___________________________________________________
//Class MutualAction
//___________________________________________________
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


//___________________________________________________
//Class Player
//___________________________________________________
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


//___________________________________________________
//Class BoardGame
//___________________________________________________
/**
    Class BoardGame constructor
*/
BoardGame::BoardGame() {

    // Set the slot counter to last row/column so that when game is started it will move to first slot(row0/column0).
    slot_ctr_ = ROW_COL_SIZE;

    // Set the whole led board to blank/Off.
    for(int idx = 0; idx < ROW_COL_SIZE; idx++) {
        led_board_[idx] = BOARD_OFF;
    }
}

/**
    Handling the Player's movement/selection and update the led board information at the same time.

    @param  none
    @return none
*/
void BoardGame::select_move() {

    bool slot_b = false;

    for(int idx = 0; idx < ROW_COL_SIZE; idx++) {
        // Clear the last slot if it is not yet selected by the Player.
        if((led_board_[idx] != BOARD_RED_ON) && (led_board_[idx] != BOARD_BLUE_ON)) {
            led_board_[idx] = BOARD_OFF;
        }
    }

    // Move to the next slot
    slot_ctr_ +=1;
    if(slot_ctr_ >= ROW_COL_SIZE) {
        // Last slot has been reached. Reset the slot counter.
        slot_ctr_ = 0;
    }

    // Check the available slot and assign the possible Player's move.
    do {
        if(BOARD_OFF == led_board_[slot_ctr_]) {
            led_board_[slot_ctr_] = BOARD_BLINKING;
            // Set to true to terminate the do-while loop
            slot_b = true;
        }
        else {
            slot_ctr_++;
            if(slot_ctr_ >= ROW_COL_SIZE) {
                // Last slot has been reached. Reset the slot counter.
                slot_ctr_ = 0;
            }
            else {
                // no action
            }
        }
    }while(slot_b != true);
}

/**
    Move to the next possible Player's move and update the led board information at the same time.

    @param  none
    @return none
*/
void BoardGame::next_move() {

    select_move();
}

/**
    Registered the Player selected move and update the led board information at the same time.

    @param  player_tag {either Player1(BOARD_RED_ON) or Player2(BOARD_BLUE_ON)}
    @return none
*/
void BoardGame::update_board_game(uint8_t player_tag) {

    led_board_[slot_ctr_] = player_tag;
}

/**
    Checking for the game winner

    @param  win_tag {Player1 win tag = P1_WIN_TAG; Player2 win tag = P2_WIN_TAG}
    @return none
*/
bool BoardGame::check_winner(int win_tag) {

    bool win_b = false;

    // Check the winner by comparing the win_tag to the summation of respective row/col.
    if(    (win_tag == (led_board_[0] + led_board_[1] + led_board_[2]))
        || (win_tag == (led_board_[3] + led_board_[4] + led_board_[5]))
        || (win_tag == (led_board_[6] + led_board_[7] + led_board_[8]))
        || (win_tag == (led_board_[0] + led_board_[3] + led_board_[6]))
        || (win_tag == (led_board_[1] + led_board_[4] + led_board_[7]))
        || (win_tag == (led_board_[2] + led_board_[5] + led_board_[8]))
        || (win_tag == (led_board_[0] + led_board_[4] + led_board_[8]))
        || (win_tag == (led_board_[2] + led_board_[4] + led_board_[6]))
    ) 
    {
        // There is a winner.
        win_b = true;
    }

    return(win_b);
}

/**
    Checking for the draw

    @param  none
    @return none
*/
bool BoardGame::check_draw() {

    bool draw_b = false;
    int ctr = 0;
    
    for(int idx = 0; idx < ROW_COL_SIZE; idx++) {
        // Check the taken led board slots
        if((BOARD_RED_ON == led_board_[idx]) || (BOARD_BLUE_ON == led_board_[idx])) {
            ctr +=1;
        }
    }

    // Check if there is no empty slot
    if(ctr >= (ROW_COL_SIZE-1)) {
        // Game is draw. No more empty slots but game is not yet over.
        draw_b = true;
    }

    return(draw_b);
}

/**
    Setting the led board slots with common data and update it at the same time

    @param  tag     {The common data that need to be filled in each led board slots}
    @return none
*/
void BoardGame::fill_board_game(uint8_t tag) {

    // Set the slot counter to last row/column so that when game is started it will move to first slot(row0/column0).
    slot_ctr_ = ROW_COL_SIZE;

    // Set the whole led board according to the value of "tag"
    for(int idx = 0; idx < ROW_COL_SIZE; idx++) {
        led_board_[idx] = tag;
    }
}

/**
    It gives back the status of the led board.
    The possible information for each led board slot is defined inside com.h

    @param  led board status
    @return none
*/
void BoardGame::get_led_board(uint8_t board[]) {

    for(int idx = 0; idx < ROW_COL_SIZE; idx++) {
        board[idx] = led_board_[idx];
    }
}