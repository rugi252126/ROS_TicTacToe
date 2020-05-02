#ifndef LED_INDICATOR_H_
#define LED_INDICATOR_H_

#include <Arduino.h>

class LedIndicator {

    public:
        // Constructor
        LedIndicator(int led_pin, uint8_t led_state);

        void update_led_indicator_state();
        void clear_led_indicator();

        // Setter
        void set_led_indicator_state(uint8_t led_state);

    private:
        int led_pin_;
        uint8_t led_state_;
        int led_ind_blinking_ctr_;
        bool led_ind_blinking_stat_b_;
};

#endif // LED_INDICATOR_H_