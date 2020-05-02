#ifndef T3_GAME_H_
#define T3_GAME_H_

#include "com.h"

const int ROW_COL_SIZE = 9;   // for 3x3 board size
const int P1_WIN_TAG   = 759; // Player1 win tag. Contains the summation of respective row/col with value BOARD_RED_ON.
const int P2_WIN_TAG   = 762; // Player2 win tag. Contains the summation of respective row/col with value BOARD_BLUE_ON.

enum class SwitchState {
    NPSD_E,
    PSD_E,
    PSD_NPSD_E
};

class MutualAction {
    public:
        // Constructor
        MutualAction(): switch_state_e_(SwitchState::NPSD_E), display_(DISPLAY_SNA) {}

        // setter
        void set_switch_state(uint8_t state);
        void set_display(uint8_t value);
        void set_buzzer_state(uint8_t state);
        void reset_psd_npsd_state();

        // getter
        SwitchState get_switch_state();
        uint8_t get_display();
        uint8_t get_buzzer_state();

    protected:
        // 7 segment display value
        uint8_t display_;
        // Buzzer state
        uint8_t buzzer_state_;
        // TRUE switch is pressed; FALSE when PSD_NPSD_E state is already read
        bool switch_psd_npsd_state_b_;
        // Application switch state
        SwitchState switch_state_e_;
};

class Player: public MutualAction {
    public:
        // Constructor
        Player(): led_indicator_(LED_OFF) {}

        // setter
        void set_led_indicator_state(uint8_t state);

        // getter
        uint8_t get_led_indicator_state();

    private:
        uint8_t led_indicator_;
};

class BoardGame {
    public:
        // Constructor
        BoardGame(){}

        void select_move(uint8_t board[], int &slot);
        void next_move(uint8_t board[], int &slot);
        void update_board_game(uint8_t board[], int slot, uint8_t player_tag);
        void fill_board_game(uint8_t board[], uint8_t tag);
        bool check_winner(uint8_t board[], int win_tag);
        bool check_draw(uint8_t board[]);

    private: 
};

#endif // T3_GAME_H_