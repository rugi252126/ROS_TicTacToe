#ifndef PLAYER_H_
#define PLAYER_H_

#include "Action.hpp"

class Player: public MutualAction {
    public:
        // Constructor
        Player();

        // Destructor
        ~Player();

        // setter
        void set_led_indicator_state(uint8_t state);

        // getter
        uint8_t get_led_indicator_state();

    private:
        uint8_t led_indicator_;
};

#endif // PLAYER_H_