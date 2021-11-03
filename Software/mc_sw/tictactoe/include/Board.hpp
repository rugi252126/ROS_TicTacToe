#ifndef BOARD_H_
#define BOARD_H_

const int ROW_COL_SIZE = 9;   // for 3x3 board size
const int P1_WIN_TAG   = 759; // Player1 win tag. Contains the summation of respective row/col with value BOARD_RED_ON.
const int P2_WIN_TAG   = 762; // Player2 win tag. Contains the summation of respective row/col with value BOARD_BLUE_ON.

class Board {
    public:
        // Constructor
        Board();

        // Destructor
        ~Board();

        void select_move();
        void next_move();
        void update_board_game(uint8_t player_tag);
        bool check_winner(int win_tag);
        bool check_draw();
        void fill_board_game(uint8_t tag);

        void get_led_board(uint8_t board[]);

    private:
        uint8_t slot_ctr_;                // slot counter
        uint8_t led_board_[ROW_COL_SIZE]; // board game
};

#endif // BOARD_H_