/**
    buzzer.cpp
    Purpose: The module handles the buzzer sound behavior like short-sound, beep-beep sound and so on.

    @author: Alfonso, Rudy Manalo
    @version 1.1 24/04/2020
*/

#include "buzzer.h"
#include "project.h"
#include "com.h"

// 1sec. sound
int const SHORT_SOUND_TIME_MS = (1000/BUZZER_TASK_MS);
// 3secs. sound
int const LONG_SOUND_TIME_MS = (3000/BUZZER_TASK_MS);
// Frequency is 4Hz. Equivalent to 250ms.
int const BEEP_BEEP_SOUND_FREQ_MS = (250/BUZZER_TASK_MS);
// 1.5 second beep-beep sound count
int const BEEP_BEEP_SOUND_IN_SEC = (1500/BEEP_BEEP_SOUND_FREQ_MS);

/**
    Class Buzzer constructor
*/
Buzzer::Buzzer(int buzzer_pin, uint8_t buzzer_state):
buzzer_pin_(buzzer_pin),
buzzer_state_(buzzer_state),
buzzer_ctr_(0),
beep_beep_sound_ctr_(0),
beep_beep_sound_state_b_(false)
{
    // Initialize buzzer control pin to digital output
    pinMode(buzzer_pin_, OUTPUT);
}

/**
    Monitoring and processing of buzzer states

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{27} <br/>
    @req_SYS_TTT{28} <br/>
    @req_SYS_TTT{44} <br/>
    @req_SYS_TTT{53} <br/>
    @req_SYS_TTT{83} <br/>

    @param  none
    @return none
*/
void Buzzer::update_buzzer_state() {

    switch(buzzer_state_) {
        case BUZZER_OFF: { // No sound
            digitalWrite(buzzer_pin_, LOW);
            break;
        }
        case BUZZER_SHORT_SOUND: { // 1sec sound
            if(buzzer_ctr_ < SHORT_SOUND_TIME_MS) {
                buzzer_ctr_++;
                digitalWrite(buzzer_pin_, HIGH);
            }
            else {
                digitalWrite(buzzer_pin_, LOW);
            }
            break;
        }
        case BUZZER_BEEP_BEEP_SOUND: { // 1sec beep-beep sound
            // Start the beep-beep counter
            buzzer_ctr_++;
            if(buzzer_ctr_ >= BEEP_BEEP_SOUND_FREQ_MS) {
                buzzer_ctr_ = 0;
                beep_beep_sound_ctr_ +=1;

                if(beep_beep_sound_state_b_ != false) {
                    beep_beep_sound_state_b_ = false;
                    // Switch-off the buzzer.
                    digitalWrite(buzzer_pin_, LOW);
                }
                else {
                    beep_beep_sound_state_b_ = true;
                    // Switch-on the buzzer.
                    digitalWrite(buzzer_pin_, HIGH);
                }
            }

            // Protection to make sure that the buzzer is Off when timer has elapsed.
            if(beep_beep_sound_ctr_ >= BEEP_BEEP_SOUND_IN_SEC) {
                beep_beep_sound_ctr_ = 0;
                // Switch-off the buzzer.
                digitalWrite(buzzer_pin_, LOW);                
            }
            break;
        }
        case BUZZER_LONG_SOUND: { // 3secs sound
            if(buzzer_ctr_ < LONG_SOUND_TIME_MS) {
                buzzer_ctr_++;
                digitalWrite(buzzer_pin_, HIGH);
            }
            else {
                digitalWrite(buzzer_pin_, LOW);
            }
            break;
        }
        case BUZZER_ON: { // Continous sound
            digitalWrite(buzzer_pin_, HIGH);
            break;
        }
        default: {
            // Invalid state. Switch-Off the buzzer.
            digitalWrite(buzzer_pin_, LOW);
            break;
        }
    }
}

/**
    Clear and set to OFF the buzzer.
    It will be called once connection is lost.

    @param  none
    @return none
*/
void Buzzer::clear_buzzer() {

    digitalWrite(buzzer_pin_, LOW);
}

/**
    Process the data received from T3_CPU

    @param  state {recevied buzzer information}
    @return none
*/
void Buzzer::set_buzzer_state(uint8_t state) {
    
    if(buzzer_state_ != state) {
        // Reset the buzzer counter if current state is not equal to new state
        buzzer_ctr_ = 0;
    }

    buzzer_state_ = state;
}