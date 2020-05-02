/**
    led_board.cpp
    Purpose: Handles tje TicTacToe Led Board

    @author Alfonso, Rudy Manalo
    @version 1.1 19/04/2020
*/

#include "led_board.h"
#include "io_pins.h"
#include "com.h"
#include "project.h"

// Blinking frequency is 4Hz. Equivalent to 250ms.
const int LED_BOARD_BLINKING_FREQ_MS = (250/LED_BOARD_TASK_MS);
// Red Leds
const int LED_BOARD_RED_DO_PINS[] = {
    // Row 1
    DO__R0_C0_RED,
    DO__R0_C1_RED,
    DO__R0_C2_RED,
    // Row 2
    DO__R1_C0_RED,
    DO__R1_C1_RED,
    DO__R1_C2_RED,
    // Row 3
    DO__R2_C0_RED,
    DO__R2_C1_RED,
    DO__R2_C2_RED
};
// Blue Leds
const int LED_BOARD_BLUE_DO_PINS[] = {
    // Row 1
    DO__R0_C0_BLUE,
    DO__R0_C1_BLUE,
    DO__R0_C2_BLUE,
    // Row 2
    DO__R1_C0_BLUE,
    DO__R1_C1_BLUE,
    DO__R1_C2_BLUE,
    // Row 3
    DO__R2_C0_BLUE,
    DO__R2_C1_BLUE,
    DO__R2_C2_BLUE
};


/**
    Class LedBoard constructor
*/
LedBoard::LedBoard() {

    led_board_blinking_stat_b_ = false;
    led_board_blinking_ctr_ = LED_BOARD_BLINKING_FREQ_MS;

    // Initialize led board pins to digital output
    for(int idx = 0; idx < ROW_COL_SIZE; idx++) {
        pinMode(LED_BOARD_RED_DO_PINS[idx], OUTPUT);
        pinMode(LED_BOARD_BLUE_DO_PINS[idx], OUTPUT);

        bcu_led_board_[idx] = BOARD_OFF;
    }

    clear_led_board();
}

/**
    Controlling Red Led

    @param  idx         {led board slot}
            led_state_b {TRUE set port to ON; FALSE set port to FF}
    @return none
*/
void LedBoard::color1_led_control(uint8_t idx, bool led_state_b) {

    if(led_state_b != false) {
        // Switch-on the led
        digitalWrite(LED_BOARD_RED_DO_PINS[idx], HIGH);
    }
    else {
        // Switch-off the led
        digitalWrite(LED_BOARD_RED_DO_PINS[idx], LOW);
    }
}

/**
    Controlling Blue Led

    @param  idx         {led board slot}
            led_state_b {TRUE set port to ON; FALSE set port to FF}
    @return none
*/
void LedBoard::color2_led_control(uint8_t idx, bool led_state_b) {

    if(led_state_b != false) {
        // Switch-on the led
        digitalWrite(LED_BOARD_BLUE_DO_PINS[idx], HIGH);
    }
    else {
        // Switch-off the led
        digitalWrite(LED_BOARD_BLUE_DO_PINS[idx], LOW);
    }
}

/**
    Monitoring and processing of led board states

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{64} <br/>

    @param  none
    @return none
*/
void LedBoard::update_led_board() {

    for(int idx = 0; idx < ROW_COL_SIZE; idx++) {
        if(BOARD_OFF == bcu_led_board_[idx]) { // Led board is Off.
            color1_led_control(idx, false);
            color2_led_control(idx, false);
        }
        else if(BOARD_BLINKING == bcu_led_board_[idx]) { // Led board is blinking.
            // Start the blinking counter and once counter is expired, blink from one Led to another
            led_board_blinking_ctr_++;
            if(led_board_blinking_ctr_ >= LED_BOARD_BLINKING_FREQ_MS) {
                led_board_blinking_ctr_ = 0;

                if(led_board_blinking_stat_b_ != false) {
                    led_board_blinking_stat_b_ = false;
                    // Red led is Off. Blue led is On.
                    color1_led_control(idx, false);
                    color2_led_control(idx, true);
                }
                else {
                    led_board_blinking_stat_b_ = true;
                    // Red led is On. Blue led is Off.
                    color1_led_control(idx, true);
                    color2_led_control(idx, false);
                }
            }
        }
        else if(BOARD_RED_ON == bcu_led_board_[idx]) { // Led board is Red.
            // Set Red Led to ON.
            color1_led_control(idx, true);
            // Set Blue Led to OFF.
            color2_led_control(idx, false);
        }
        else if(BOARD_BLUE_ON == bcu_led_board_[idx]) { // Led board is Blue.
            // Set Blue Led to ON.
            color2_led_control(idx, true);
            // Set Red Led to OFF.
            color1_led_control(idx, false);
        }
        else {
            // no action
        }
    } 
}

/**
    Clear and set to OFF the whole led board.
    It will be called once connection is lost.

    @param  none
    @return none
*/
void LedBoard::clear_led_board() {

    // Switch-off all leds
    for(int idx = 0; idx < ROW_COL_SIZE; idx++) {
        digitalWrite(LED_BOARD_RED_DO_PINS[idx], LOW);
        digitalWrite(LED_BOARD_BLUE_DO_PINS[idx], LOW);
    }    
}

/**
    Process the data received from T3_CPU

    @param  board[] {recevied led board information}
    @return none
*/
void LedBoard::set_led_board(uint8_t board[]) {

    // Copy the data from T3_CPU to T3_BCU led_board module
    for(int idx = 0; idx < ROW_COL_SIZE; idx++) {
        bcu_led_board_[idx] = board[idx];
    } 
}