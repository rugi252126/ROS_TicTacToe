#ifndef SWITCH_IN_H_
#define SWITCH_IN_H_

#include <Arduino.h>

class SwitchIn {
    public:
        // Constructor
        SwitchIn(int switch_pin, uint8_t switch_state);

        void update_switch_state();

        // Getter
        uint8_t get_switch_state();

    private:
        uint8_t switch_state_;
        int switch_pin_;
};

#endif // SWITCH_IN_H_