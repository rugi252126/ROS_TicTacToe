#ifndef LED_BOARD_H_
#define LED_BOARD_H_

#include <Arduino.h>

const int ROW_COL_SIZE = 9; // for 3x3 board size

class LedBoard {

    public:
        // Constructor
        LedBoard();

        void update_led_board();
        void clear_led_board();

        // Setter
        void set_led_board(uint8_t board[]);

    private:
        // Red Led
        void color1_led_control(uint8_t idx, bool led_state_b);
        // Blue Led
        void color2_led_control(uint8_t idx, bool led_state_b);

        uint8_t bcu_led_board_[ROW_COL_SIZE];
        int led_board_blinking_ctr_;
        int led_board_blinking_stat_b_;
};

#endif // LED_BOARD_H_c