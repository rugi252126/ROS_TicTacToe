#ifndef ACTION_H_
#define ACTION_H_

enum class SwitchState {
    NPSD_E,
    PSD_E,
    PSD_NPSD_E
};

class MutualAction {
    public:
        // Constructor
        MutualAction();

        // Destructor
        ~MutualAction();

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

#endif // ACTION_H_