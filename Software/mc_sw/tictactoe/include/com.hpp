#ifndef COM_H_
#define COM_H_

// T3_CPU
typedef unsigned char uint8_t;

//! Constant declarations. Information are based on Send-Receive Communication Matrix. Please see communication requirements for more information.
// Push button switch
const uint8_t SWS_NDEF0 = 0;
const uint8_t SWS_NPSD  = 1;
const uint8_t SWS_PSD   = 2;
const uint8_t SWS_SNA   = 3;

// Led indicator
const uint8_t LED_OFF      = 0;
const uint8_t LED_BLINKING = 1;
const uint8_t LED_ON       = 2;
const uint8_t LED_SNA      = 3;

// Buzzer
const uint8_t BUZZER_OFF             = 0;
const uint8_t BUZZER_SHORT_SOUND     = 1;
const uint8_t BUZZER_BEEP_BEEP_SOUND = 2;
const uint8_t BUZZER_LONG_SOUND      = 3;
const uint8_t BUZZER_ON              = 4;
const uint8_t BUZZER_NDEF0           = 5;
const uint8_t BUZZER_NDEF1           = 6;
const uint8_t BUZZER_SNA             = 7;

// 7-Segment Display
const uint8_t DISPLAY_ZERO  = 0;
const uint8_t DISPLAY_ONE   = 1;
const uint8_t DISPLAY_TWO   = 2;
const uint8_t DISPLAY_THREE = 3;
const uint8_t DISPLAY_FOUR  = 4;
const uint8_t DISPLAY_FIVE  = 5;
const uint8_t DISPLAY_SIX   = 6;
const uint8_t DISPLAY_SEVEN = 7;
const uint8_t DISPLAY_EIGHT = 8;
const uint8_t DISPLAY_NINE  = 9;
const uint8_t DISPLAY_NDEF0 = 10;
const uint8_t DISPLAY_NDEF1 = 11;
const uint8_t DISPLAY_NDEF2 = 12;
const uint8_t DISPLAY_NDEF3 = 13;
const uint8_t DISPLAY_NDEF4 = 14;
const uint8_t DISPLAY_SNA   = 15;

// Led board game
const uint8_t BOARD_OFF      = 0;
const uint8_t BOARD_BLINKING = 1;
const uint8_t BOARD_RED_ON   = 253;
const uint8_t BOARD_BLUE_ON  = 254;
const uint8_t BOARD_SNA      = 255;

// Game setting
const uint8_t SETTING_NDEF0 = 0;
const uint8_t SETTING_OFF   = 1;
const uint8_t SETTING_ON    = 2;
const uint8_t SETTING_SNA   = 3;

// Mode setting
const uint8_t MODE_SET_NDEF0 = 0;
const uint8_t MODE_SET_OFF   = 1;
const uint8_t MODE_SET_ON    = 2;
const uint8_t MODE_SET_SNA   = 3;

// Move time setting
const uint8_t MOVE_TIME_SET_NDEF0 = 0;
const uint8_t MOVE_TIME_SET_OFF   = 1;
const uint8_t MOVE_TIME_SET_ON    = 2;
const uint8_t MOVE_TIME_SET_SNA   = 3;

// Mode information
const uint8_t MODE_NDEF0 = 0;
const uint8_t MODE1      = 1; // winner takes all
const uint8_t MODE2      = 2; // Best of 3
const uint8_t MODE3      = 3; // Best of 5
const uint8_t MODE4      = 4; // Best of 7
const uint8_t MODE5      = 5; // Best of 9
const uint8_t MODE_NDEF1 = 6;
const uint8_t MODE_SNA   = 7;

// Move time information
const uint8_t MOVE_TIME_NDEF0 = 0;
const uint8_t MOVE_TIME1      = 1; // Unlimited
const uint8_t MOVE_TIME2      = 2; // 3sec.
const uint8_t MOVE_TIME3      = 3; // 5sec.
const uint8_t MOVE_TIME4      = 4; // 7sec.
const uint8_t MOVE_TIME5      = 5; // 9sec.
const uint8_t MOVE_TIME_NDEF1 = 6;
const uint8_t MOVE_TIME_SNA   = 7;

#endif // COM_H_