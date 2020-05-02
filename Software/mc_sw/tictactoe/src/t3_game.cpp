/**
    t3_game.cpp
    Purpose: Handles the underlying behavior of the game. 
    Processing of the information that received from T3_BCU and the information that need to be sent to T3_BCU.

    @author Alfonso, Rudy Manalo
    @version 1.1 4/4/2020
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
    Handling the Player's movement/selection and update the led board information at the same time.

    @param  board[] {led board with size of ROW_COL_SIZE}
            slot    {slot number in the led board}
    @return none
*/
void BoardGame::select_move(uint8_t board[], int &slot) {

    bool slot_b = false;

    for(int idx = 0; idx < ROW_COL_SIZE; idx++) {
        // Clear the last slot if it is not yet selected by the Player.
        if((board[idx] != BOARD_RED_ON) && (board[idx] != BOARD_BLUE_ON)) {
            board[idx] = BOARD_OFF;
        }
    }

    // Move to the next slot
    slot +=1;
    if(slot >= ROW_COL_SIZE) {
        // Last slot has been reached. Reset the slot counter.
        slot = 0;
    }

    // Check the available slot and assign the possible Player's move.
    do {
        if(BOARD_OFF == board[slot]) {
            board[slot] = BOARD_BLINKING;
            // Set to true to terminate the do-while loop
            slot_b = true;
        }
        else {
            slot++;
            if(slot >= ROW_COL_SIZE) {
                // Last slot has been reached. Reset the slot counter.
                slot = 0;
            }
            else {
                // no action
            }
        }
    }while(slot_b != true);
}

/**
    Move to the next possible Player's move and update the led board information at the same time.

    @param  board[] {led board with size of ROW_COL_SIZE}
            slot    {slot number in the led board}
    @return none
*/
void BoardGame::next_move(uint8_t board[], int &slot) {

    select_move(board, slot);
}

/**
    Registered the Player selected move and update the led board information at the same time.

    @param  board[]    {led board with size of ROW_COL_SIZE}
            slot       {slot number in the led board}
            player_tag {either Player1(BOARD_RED_ON) or Player2(BOARD_BLUE_ON)}
    @return none
*/
void BoardGame::update_board_game(uint8_t board[], int slot, uint8_t player_tag) {

    board[slot] = player_tag;
}

/**
    Checking for the game winner

    @param  board[] {led board with size of ROW_COL_SIZE}
            win_tag {Player1 win tag = P1_WIN_TAG; Player2 win tag = P2_WIN_TAG}
    @return none
*/
bool BoardGame::check_winner(uint8_t board_game[], int win_tag) {

    bool win_b = false;

    // Check the winner by comparing the win_tag to the summation of respective row/col.
    if(    (win_tag == (board_game[0] + board_game[1] + board_game[2]))
        || (win_tag == (board_game[3] + board_game[4] + board_game[5]))
        || (win_tag == (board_game[6] + board_game[7] + board_game[8]))
        || (win_tag == (board_game[0] + board_game[3] + board_game[6]))
        || (win_tag == (board_game[1] + board_game[4] + board_game[7]))
        || (win_tag == (board_game[2] + board_game[5] + board_game[8]))
        || (win_tag == (board_game[0] + board_game[4] + board_game[8]))
        || (win_tag == (board_game[2] + board_game[4] + board_game[6]))
    ) 
    {
        // There is a winner.
        win_b = true;
    }

    return(win_b);
}

/**
    Checking for the draw

    @param  board[] {led board with size of ROW_COL_SIZE}
    @return none
*/
bool BoardGame::check_draw(uint8_t board[]) {

    bool draw_b = false;
    int ctr = 0;
    
    for(int idx = 0; idx < ROW_COL_SIZE; idx++) {
        // Check the taken led board slots
        if((BOARD_RED_ON == board[idx]) || (BOARD_BLUE_ON == board[idx])) {
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

    @param  board[] {led board with size of ROW_COL_SIZE}
            tag     {The common data that need to be filled in each led board slots}
    @return none
*/
void BoardGame::fill_board_game(uint8_t board[], uint8_t tag) {

    // Set the whole led board according to the value of "tag"
    for(int idx = 0; idx < ROW_COL_SIZE; idx++) {
        board[idx] = tag;
    }
}