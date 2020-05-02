#ifndef IO_PINS_H_
#define IO_PINS_H_

#include <Arduino.h>

//! Digital IO pins constant declarations. Please see the ROS_TicTacToe schematic diagram for more information.

// Push Button Switch
const int DI__PLAYER1_SELECT_SWITCH = 14;
const int DI__PLAYER2_SELECT_SWITCH = 15;
const int DI__ENTER_SWITCH          = 16;

// Led indicator
const int DO__PLAYER1_LED_IND    = 3;
const int DO__PLAYER2_LED_IND    = 2;
const int DO__CONNECTION_LED_IND = 22;

const int DO__BUZZER        = 5;

// 7-segment display
const int DO__CTRL_A = 6;
const int DO__CTRL_B = 7;
const int DO__CTRL_C = 8;
const int DO__CTRL_D = 9;
const int DO__SCORE1_CTRL_LT_BI_RBO = 10;
const int DO__SCORE2_CTRL_LT_BI_RBO = 11;
const int DO__TIMER_CTRL_LT_BI_RBO  = 12;

// Bi-color Led board
// Row 1
const int DO__R0_C0_RED   = 32;
const int DO__R0_C0_BLUE  = 33;
const int DO__R0_C1_RED   = 34;
const int DO__R0_C1_BLUE  = 35;
const int DO__R0_C2_RED   = 36;
const int DO__R0_C2_BLUE  = 37;
// Row 2
const int DO__R1_C0_RED   = 38;
const int DO__R1_C0_BLUE  = 39;
const int DO__R1_C1_RED   = 40;
const int DO__R1_C1_BLUE  = 41;
const int DO__R1_C2_RED   = 42;
const int DO__R1_C2_BLUE  = 43;
// Row 3
const int DO__R2_C0_RED   = 44;
const int DO__R2_C0_BLUE  = 45;
const int DO__R2_C1_RED   = 46;
const int DO__R2_C1_BLUE  = 47;
const int DO__R2_C2_RED   = 30;
const int DO__R2_C2_BLUE  = 31;

// Debug port
const int DO__TOGGLE_PORT1 = 24;
const int DO__TOGGLE_PORT2 = 25;

#endif // IO_PINS_H_