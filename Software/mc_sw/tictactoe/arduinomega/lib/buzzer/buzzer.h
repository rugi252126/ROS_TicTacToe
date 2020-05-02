#ifndef BUZZER_H_
#define BUZZER_H_

#include <Arduino.h>

class Buzzer {

    public:
        // Constructor
        Buzzer(int buzzer_pin, uint8_t buzzer_state);

        void update_buzzer_state();
        void clear_buzzer();

        // setter
        void set_buzzer_state(uint8_t state);

    private:
        int buzzer_pin_;
        uint8_t buzzer_state_;
        int buzzer_ctr_;
        int beep_beep_sound_ctr_;
        bool beep_beep_sound_state_b_;

};

#endif // BUZZER_H_