#ifndef SEVEN_SEGMENT_H_
#define SEVEN_SEGMENT_H_

#include <Arduino.h>

class SevenSegment {
    
    public:
        // Constructor
        SevenSegment();

        void update_display();
        void clear_display();

        // Setter
        void set_display(uint8_t score1, uint8_t score2, uint8_t timer);

    private:
        void select_display(uint8_t value);

        enum class DisplayType {
            BLANK_E,  // no display
            SCORE1_E,
            SCORE2_E,
            TIMER_E
        } display_type_e_;

        uint8_t score1_;
        uint8_t score2_;
        uint8_t timer_;
};

#endif // SEVEN_SEGMENT_H_