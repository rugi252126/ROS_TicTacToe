/**
    t3_cpu.cpp
    Purpose: T3_CPU handles the main game algorithm and central processing of TicTacToe game.
    Task scheduling, publishing and subscribing of ROS messages.

    @author Alfonso, Rudy Manalo
    @version 1.2 31/5/2020
*/

#include "t3_cpu.h"
#include "t3_game.h"
#include <stdlib.h> // for srand, rand
#include <time.h>   // for time, it would be good to have when generating random values.
#include <tictactoe_msgs/T3_Indicator_Msg.h>
#include <tictactoe_msgs/T3_7Segment_Display_Msg.h>
#include <tictactoe_msgs/T3_Led_Board_Game_Msg.h>
#include <tictactoe_msgs/T3_Game_Mode_Msg.h>

//! Constant declaration
// Application timers in milli-second form
const int TASK_TIME_MS             = 50;
const int SETTING_TIME_MS          = (3000/TASK_TIME_MS);
const int SETTING_TO_PLAY_TIME_MS  = (3000/TASK_TIME_MS);
const int DRAW_TO_NEW_GAME_TIME_MS = (3000/TASK_TIME_MS);
const int GAME_INTERRUPT_TIME_MS   = (1000/TASK_TIME_MS);
const int ONE_SEC_TIME_MS          = (1000/TASK_TIME_MS);
const int STARTUP_TIME_MS          = (1500/TASK_TIME_MS);
const int NEW_GAME_TIME_MS         = (2000/TASK_TIME_MS);

class Game {
    public:
        Game();
        void game_task();

    private:
        void setting_off();
        void setting_on();
        void start_up();
        void game_setting();
        void play();
        void draw();
        void resume();
        void game_over();

        void new_game();
        void first_move();
        void move_time_monitoring();
        void check_matches();
        void check_game_interruption();

        enum class CpuState {
            STARTUP_E,
            SETTING_E,
            NEW_GAME_E,
            PLAY_E,
            DRAW_E,
            STOP_E,
            GAME_OVER_E
        } cpu_state_e_;

        enum class GameSettingState {
            OFF_E,
            ON_E,
        } game_setting_state_e_;

        enum class GameModes {
            SETTING_UP_E,
            MODE_E,
            MOVE_TIME_E,
            EXIT_E
        };

        enum class GameStatus {
            UNKNOWN_E,
            PLAYER1_E,
            PLAYER2_E
        };

        GameStatus turn_e_;
        GameStatus winner_e_;

        GameModes game_modes_e_;
        GameModes game_modes_buf_e_;

        struct {
            // TRUE pressed; FALSE otherwise
            bool enter_switch_psd_state_b_;
            // TRUE move time expired; FALSE otherwise
            bool move_time_b_;
            // TRUE interruption is about to happen(e.g. game stopping/resetting); FALSE otherwise. 
            bool game_interruption_b_;
        } status_flag_s;

        struct {
            // cpu_state_e_ transition counter (when moving from one state to another)
            int cpu_state_transition_ctr_;
            // cpu state transition counter from setting Off to setting ON and vice-versa
            int game_setting_transition_ctr_;
            // reset counter. the game will be re-started once the counter has elapsed.
            int restart_ctr_;
            // Player should make a move before the counter has expired.
            int move_time_ctr_;
            // Handles 1 second counter to update the timer display.
            int move_time_1sec_ctr_;
            // modes counter to run all available modes(game mode and move time)
            int modes_ctr_;
            // Player1 winner counter
            int p1_win_ctr_;
            // Player2 winner counter
            int p2_win_ctr_;
        } counter_s;

        struct {
            uint8_t game_setting_;
            uint8_t mode_setting_;
            uint8_t move_time_setting_;
            uint8_t mode_info_;
            uint8_t move_time_info_;
        } game_mode_s;
};

// Object instances
Game game            = Game();
Player p1            = Player();
Player p2            = Player();
MutualAction p1_2    = MutualAction();
BoardGame board_game = BoardGame();

/**
    Class T3CpuNode constructor

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{116} <br/>
*/
T3CpuNode::T3CpuNode() {
    // subscribe
    sws_button_sub_ = nh_.subscribe("button", 50, &T3CpuNode::push_button_switch_subscribeCallBack, this);
    
    // publish
    indicator_pub_ = nh_.advertise<tictactoe_msgs::T3_Indicator_Msg>("indicator", 50);
    display_pub_   = nh_.advertise<tictactoe_msgs::T3_7Segment_Display_Msg>("display", 50);
    led_board_pub_ = nh_.advertise<tictactoe_msgs::T3_Led_Board_Game_Msg>("led_board", 50);
    game_mode_pub_ = nh_.advertise<tictactoe_msgs::T3_Game_Mode_Msg>("game_mode", 50);
}

/**
    Class Game constructor
*/
Game::Game() {
    cpu_state_e_                           = CpuState::STARTUP_E;
    game_setting_state_e_                  = GameSettingState::OFF_E;
    game_modes_e_                          = GameModes::SETTING_UP_E;
    game_modes_buf_e_                      = GameModes::SETTING_UP_E;
    turn_e_                                = GameStatus::UNKNOWN_E;
    winner_e_                              = GameStatus::UNKNOWN_E;
    game_mode_s.game_setting_              = SETTING_OFF;
    game_mode_s.mode_info_                 = MODE1;
    game_mode_s.move_time_info_            = MOVE_TIME1;
    status_flag_s.enter_switch_psd_state_b_= false;
    status_flag_s.move_time_b_             = false;
    status_flag_s.game_interruption_b_     = false;
    counter_s.cpu_state_transition_ctr_    = 0;
    counter_s.game_setting_transition_ctr_ = 0;
    counter_s.restart_ctr_                 = 0;
    counter_s.modes_ctr_                   = 0;
    counter_s.move_time_ctr_               = 0;
    counter_s.move_time_1sec_ctr_          = 0;
}


/**
    For every new game, function checks who will do the first move(Player1/Player2).

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{65} <br/>
    @req_SYS_TTT{66} <br/>
    @req_SYS_TTT{67} <br/>
    @req_SYS_TTT{68} <br/>

    @param  none
    @return none
*/
void Game::first_move() {

    int random;

    if((MODE1 == game_mode_s.mode_info_) || (GameStatus::UNKNOWN_E == winner_e_)) {
        // Game is fresh/new or the game mode is set to 1. 
        // The first move will be randomly choosen.
        srand(time(NULL));
        // Generate random number 1 or 2
        random = rand() % 2 + 1; 

        if(1 == random) { // 1, Player1
            turn_e_ = GameStatus::PLAYER1_E;
        }
        else {
            turn_e_ = GameStatus::PLAYER2_E;           
        }     
    }
    else {
        // The first move will be done by the winner from the previous match.
        turn_e_ = winner_e_;
    }
}

/**
    Monitoring Players' movements and at the same time handling of timer display.
    It will be called when in PLAY_E state.

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{77} <br/>
    @req_SYS_TTT{78} <br/>
    @req_SYS_TTT{79} <br/>
    @req_SYS_TTT{83} <br/>

    @param  none
    @return none
*/
void Game::move_time_monitoring() {
 
    int timer_ctr = 0;

    const int timer[] = {
        0, // Dummy NDEF0
        0, // Dummy because Move Time1 means unlimited
        3, // 3secs.
        5, // 5secs.
        7, // 7secs.
        9  // 9 secs.
    };

    if(game_mode_s.move_time_info_ != MOVE_TIME1) {

        counter_s.move_time_1sec_ctr_++;
        if(counter_s.move_time_1sec_ctr_ >= ONE_SEC_TIME_MS) { // 1sec.
            counter_s.move_time_1sec_ctr_ = 0;

            // Increment every 1sec.
            counter_s.move_time_ctr_++;
            if(counter_s.move_time_ctr_ > timer[game_mode_s.move_time_info_]) {
                // reset the move time counter to start new move time monitoring
                counter_s.move_time_ctr_ = 0;
                // Set buzzer state to Off.
                p1_2.set_buzzer_state(BUZZER_OFF);  
            }
            else if(counter_s.move_time_ctr_ == timer[game_mode_s.move_time_info_]) {
                // Move time counter has expired.
                status_flag_s.move_time_b_ = true;

                // Set buzzer state to short-sound to indicate that timer has expired.
                p1_2.set_buzzer_state(BUZZER_SHORT_SOUND);          
            }
            else {
                // no action
            }
        }

        // Negative result will not happen as counter is reset back to 0 once counter is above the target.
        timer_ctr = timer[game_mode_s.move_time_info_] - counter_s.move_time_ctr_;
        // Set the latest timer display
        p1_2.set_display(timer_ctr); 
    }
    else {
        // Set to FALSE because move time configuration is unlimited.
        status_flag_s.move_time_b_ = false;
        // Move Time is unlimited. Timer display is always set to 0.
        p1_2.set_display(DISPLAY_ZERO);
    }
}

/**
    Checks and decide whether game is over or new game is required.
    It will be called when in PLAY_E state.

    Satisty Requirement(s): <br/>
    @req_SYS_TTT{89}

    @param  none
    @return none
*/
void Game::check_matches() {

    const int best_wins[] = {
        0, // Dummy NDEF0
        0, // Dummy because Mode1 means winner takes all
        2, // best of 3. Player needs to win at least 2 to declare as winner.
        3, // best of 5. Player needs to win at least 3 to declare as winner.
        4, // best of 7. Player needs to win at least 4 to declare as winner.
        5  // best of 9. Player needs to win at least 5 to declare as winner.
    };

    if(game_mode_s.mode_info_ != MODE1) {
        // Check if game is over
        if(    (best_wins[game_mode_s.mode_info_] == counter_s.p1_win_ctr_) 
            || (best_wins[game_mode_s.mode_info_] == counter_s.p2_win_ctr_)
        )
        {
            // Matches are over. Game is over.
            cpu_state_e_ = CpuState::GAME_OVER_E;
        }
        else {
            // Game is not yet over. Start new game
            cpu_state_e_ = CpuState::NEW_GAME_E;
        }
    }
    else {

        // For MODE1, maximum score is 9. Players can reset the game to start all over again.
        if(counter_s.p1_win_ctr_ > DISPLAY_NINE) {
            counter_s.p1_win_ctr_ = DISPLAY_NINE;
        }

        if(counter_s.p2_win_ctr_ > DISPLAY_NINE) {
            counter_s.p2_win_ctr_ = DISPLAY_NINE;
        }
        // For MODE1, each game consider as game is over.
        cpu_state_e_ = CpuState::GAME_OVER_E;
    }
}

/**
    Processing conditions outside the game setting
    Transition from setting OFF_E to ON_E
    Transition from cpu state SETTING_E to NEW_GAME_E

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{29} <br/>
    @req_SYS_TTT{32} <br/>
    @req_SYS_TTT{33} <br/>
    @req_SYS_TTT{34} <br/>
    @req_SYS_TTT{37} <br/>
    @req_SYS_TTT{107} <br/>

    @param  none
    @return none
*/
void Game::setting_off() {

    if(SwitchState::PSD_E == p1_2.get_switch_state()) {
        // Reset psd npsd state to be ready for next button pressed/released action.
        p1_2.reset_psd_npsd_state();

        counter_s.cpu_state_transition_ctr_ = 0;
        // Set flag indicates enter button is pressed
        status_flag_s.enter_switch_psd_state_b_ = true;
        // Set led indicators to On
        p1.set_led_indicator_state(LED_ON);
        p2.set_led_indicator_state(LED_ON);

        // Enter button is pressed. move to setting On after timer has elapsed.
        counter_s.game_setting_transition_ctr_++;
        if(counter_s.game_setting_transition_ctr_ >= SETTING_TIME_MS) {
            // No need to reset the counter here. It will be cleared inside setting On.

            status_flag_s.enter_switch_psd_state_b_ = false;
            // Set led indicators to blinking
            p1.set_led_indicator_state(LED_BLINKING);
            p2.set_led_indicator_state(LED_BLINKING);
            // Move to setting On state
            game_setting_state_e_ = GameSettingState::ON_E;
            game_mode_s.game_setting_ = SETTING_ON;
        }        
    }
    else if(true == status_flag_s.enter_switch_psd_state_b_) {
        counter_s.game_setting_transition_ctr_ = 0;
                
        // Enter button is pressed and released. start to play the game after timer has elapsed.
        counter_s.cpu_state_transition_ctr_++;
        if(counter_s.cpu_state_transition_ctr_ >= SETTING_TO_PLAY_TIME_MS) {
            counter_s.cpu_state_transition_ctr_ = 0;

            status_flag_s.enter_switch_psd_state_b_ = false;
            // Move to new game state
            cpu_state_e_ = CpuState::NEW_GAME_E;
            game_mode_s.game_setting_ = SETTING_OFF;
        }
    }
    else {
        status_flag_s.enter_switch_psd_state_b_ = false;
        counter_s.game_setting_transition_ctr_ = 0;
        counter_s.cpu_state_transition_ctr_ = 0;        
    }

    // Clear scores
    counter_s.p1_win_ctr_ = 0;
    counter_s.p2_win_ctr_ = 0;
    // Send the current configured game mode and move time mode settings
    p1.set_display(game_mode_s.mode_info_);
    p2.set_display(game_mode_s.mode_info_);
    p1_2.set_display(game_mode_s.move_time_info_); 
}

/**
    Game setting configuration (game mode and move time mode)
    Transition from setting ON_E to OFF_E

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{38} <br/>
    @req_SYS_TTT{40} <br/>
    @req_SYS_TTT{41} <br/>
    @req_SYS_TTT{43} <br/>
    @req_SYS_TTT{44} <br/>
    @req_SYS_TTT{47} <br/>
    @req_SYS_TTT{49} <br/>
    @req_SYS_TTT{50} <br/>
    @req_SYS_TTT{52} <br/>
    @req_SYS_TTT{53} <br/>
    @req_SYS_TTT{54} <br/>
    @req_SYS_TTT{55} <br/>
    @req_SYS_TTT{56} <br/>

    @param  none
    @return none
*/
void Game::setting_on() {

    // Number of game mode and move time elements
    const uint8_t ELEM_SIZE = 5;

    // Available game modes
    const uint8_t mode[] = {
        MODE1,
        MODE2,
        MODE3,
        MODE4,
        MODE5
    };
    
    // Available move time modes
    const uint8_t move_time[] = {
        MOVE_TIME1,
        MOVE_TIME2,
        MOVE_TIME3,
        MOVE_TIME4,
        MOVE_TIME5
    };

    if((SwitchState::PSD_NPSD_E == p1.get_switch_state()) || (SwitchState::PSD_NPSD_E == p2.get_switch_state())) {
        // Reset psd npsd state to be ready for next button pressed/released action.
        p1.reset_psd_npsd_state();
        p2.reset_psd_npsd_state();
        // Increment counter if either one of the select/move button is pressed and then released.
        counter_s.modes_ctr_++;
        if(counter_s.modes_ctr_ >= ELEM_SIZE) {
            counter_s.modes_ctr_ = 0;
        }
    }

    switch(game_modes_e_) {
        case GameModes::SETTING_UP_E: {

            if(p1_2.get_switch_state() != SwitchState::NPSD_E) { 
                // Reset psd npsd state to be ready for next button pressed/released action.
                p1_2.reset_psd_npsd_state();
            }
            else if(SwitchState::NPSD_E == p1_2.get_switch_state()) {
                // Start the game setting if enter button is not pressed.

                counter_s.game_setting_transition_ctr_++;
                if(counter_s.game_setting_transition_ctr_ >= ONE_SEC_TIME_MS) {
                    counter_s.game_setting_transition_ctr_ = 0;
                    p1_2.set_buzzer_state(BUZZER_OFF);  

                    if(GameModes::SETTING_UP_E == game_modes_buf_e_) {
                        game_modes_e_ = GameModes::MODE_E;
                    }
                    else {
                        game_modes_e_ = GameModes::MOVE_TIME_E;
                    }     
                }
            }
            else {
                // no action
            }
            break;
        }
        case GameModes::MODE_E: {

            if(SwitchState::PSD_NPSD_E == p1_2.get_switch_state()) {
                // Reset psd npsd state to be ready for next button pressed/released action.
                p1_2.reset_psd_npsd_state();
                // Registered/enabled the selected game mode
                game_mode_s.mode_info_ = mode[counter_s.modes_ctr_];
                // Send the current registered/enabled game mode
                p1.set_display(game_mode_s.mode_info_);
                p2.set_display(game_mode_s.mode_info_);
                // Set buzzer to short-sound to indicate mode has been succesfully registered.
                p1_2.set_buzzer_state(BUZZER_SHORT_SOUND);

                game_modes_e_     = GameModes::SETTING_UP_E;
                game_modes_buf_e_ = GameModes::MODE_E;
                counter_s.modes_ctr_ = 0;
            }
            else {
                // Send the current selected game mode
                p1.set_display(mode[counter_s.modes_ctr_]);
                p2.set_display(mode[counter_s.modes_ctr_]);
            }
            
            // Send invalid display as this one is for move time display
            p1_2.set_display(DISPLAY_SNA);
            break;
        }
        case GameModes::MOVE_TIME_E: {

            if(SwitchState::PSD_NPSD_E == p1_2.get_switch_state()) {
                // Reset psd npsd state to be ready for next button pressed/released action.
                p1_2.reset_psd_npsd_state();
                // Registered/enabled the selected move time mode and move to next state(Exit)
                game_mode_s.move_time_info_ = move_time[counter_s.modes_ctr_];
                // Send the current selected/registered/enabled move time mode
                p1_2.set_display(game_mode_s.move_time_info_);  
                // Set buzzer to short-sound to indicate move time mode has been succesfully registered.
                p1_2.set_buzzer_state(BUZZER_SHORT_SOUND);

                counter_s.modes_ctr_ = 0;
                // Move to exit state as game mode and move time mode are already configured.
                game_modes_e_ = GameModes::EXIT_E;
            }
            else {   
                // Send the current selected move time mode
                p1_2.set_display(move_time[counter_s.modes_ctr_]); 
            }
            
            // Send invalid display as this one is for mode display
            p1.set_display(DISPLAY_SNA);
            p2.set_display(DISPLAY_SNA);
            break;
        }
        case GameModes::EXIT_E: {

            // Wait for counter. Once counter has expired, it will move back automatically to game setting OFF_E.
            counter_s.game_setting_transition_ctr_++;
            if(counter_s.game_setting_transition_ctr_ >= SETTING_TIME_MS) {
                counter_s.game_setting_transition_ctr_ = 0;
                counter_s.modes_ctr_ = 0;
                // Set led indicators to Off
                p1.set_led_indicator_state(LED_OFF);
                p2.set_led_indicator_state(LED_OFF);

                game_mode_s.game_setting_ = SETTING_OFF;
                // Initilalized to default as cpu state went back to setting Off
                game_modes_e_     = GameModes::SETTING_UP_E;
                game_modes_buf_e_ = GameModes::SETTING_UP_E;
                // Go to setting off state
                game_setting_state_e_ = GameSettingState::OFF_E;
            } 
            break;
        }
        default: {
            // no action
            break;
        }
    }
}

/**
    Handles startup conditions. It will be called when connection between T3_CPU and T3_BCU is established.
    It will also be called when Players requested a game reset.
    Transition from cpu state STARTUP_E to SETTING_E

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{27} <br/>
    @req_SYS_TTT{28} <br/>

    @param  none
    @return none
*/
void Game::start_up() {

    // Startup time counter
    counter_s.cpu_state_transition_ctr_++;
    if(counter_s.cpu_state_transition_ctr_ >= STARTUP_TIME_MS) {
        counter_s.cpu_state_transition_ctr_ = 0;
        // Set buzzer to off
        p1_2.set_buzzer_state(BUZZER_OFF);
        turn_e_   = GameStatus::UNKNOWN_E;
        winner_e_ = GameStatus::UNKNOWN_E;
        // Reset all status flags
        status_flag_s.enter_switch_psd_state_b_ = false;
        status_flag_s.move_time_b_              = false;
        status_flag_s.game_interruption_b_      = false;

        // Move to setting state
        cpu_state_e_ = CpuState::SETTING_E;
    }
    else {
        // Set buzzer to beep-beep sound
        p1_2.set_buzzer_state(BUZZER_BEEP_BEEP_SOUND);
    }
}

/**
    Game setting checking and monitoring
    Transition from cpu state SETTING_E to NEW_GAME_E

    @param  none
    @return none
*/
void Game::game_setting() {
    
    switch(game_setting_state_e_) {
        case GameSettingState::OFF_E: {
            // Outside the game setting
            setting_off();
            break;
        }
        case GameSettingState::ON_E: {
            // Inside the game setting
            setting_on();
            break;
        }
        default: {
            // no action
            break;
        }
    }
}

/**
    Handles new game conditions. Everytime there is a new game, it will come here and do the new game sequence.
    Transition from cpu state NEW_GAME_E to PLAY_E

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{60} <br/>
    @req_SYS_TTT{61} <br/>

    @param  none
    @return none

*/
void Game::new_game() {

    counter_s.cpu_state_transition_ctr_++;
    if(counter_s.cpu_state_transition_ctr_ < (NEW_GAME_TIME_MS/2)) { // first 1sec. turn the led board to RED
        board_game.fill_board_game(BOARD_RED_ON);
    }
    else if(counter_s.cpu_state_transition_ctr_ < NEW_GAME_TIME_MS) { // the remaining 1sec. turn the led board to BLUE
        board_game.fill_board_game(BOARD_BLUE_ON);
    }
    else {
        // Set buzzer state to Off.
        p1_2.set_buzzer_state(BUZZER_OFF); 

        counter_s.cpu_state_transition_ctr_ = 0;

        // Clear board game
        board_game.fill_board_game(BOARD_OFF);

        // Assign the row0/col0 as possible next move. Here, led board row0/col0 will start to blink.
        board_game.next_move();

        // Check who will do the first move (Player1 or Player2)
        first_move();

        // Start the game
        cpu_state_e_ = CpuState::PLAY_E;
    }
}

/**
    Game monitoring (Players' movements, running game conditions, scores, indicators, etc.)
    Transition from cpu state PLAY_E to NEW_GAME_E/STOP_E/DRAW_E/GAME_OVER_E/STARTUP_E

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{69} <br/>
    @req_SYS_TTT{70} <br/>
    @req_SYS_TTT{75} <br/>
    @req_SYS_TTT{76} <br/>
    @req_SYS_TTT{80} <br/>
    @req_SYS_TTT{82} <br/>
    @req_SYS_TTT{87} <br/>
    @req_SYS_TTT{93} <br/>
    @req_SYS_TTT{94} <br/>

    @param  none
    @return none

*/
void Game::play() {

    if(GameStatus::PLAYER1_E == turn_e_) {
        // Player1's turn
            
        // Set player1 led indicator to Blinking and Player2 to OFF
        p1.set_led_indicator_state(LED_BLINKING);
        p2.set_led_indicator_state(LED_OFF);

        if(SwitchState::PSD_NPSD_E == p1.get_switch_state()) {
            // Reset psd npsd state to be ready for next button pressed/released action.
            p1.reset_psd_npsd_state();
            // Select move
            board_game.select_move();
        }
        
        // Check if enter button is pressed or move time counter has expired.
        if((SwitchState::PSD_NPSD_E == p1_2.get_switch_state()) || (status_flag_s.move_time_b_ != false)) {
            // Reset psd npsd state to be ready for next button pressed/released action.
            p1_2.reset_psd_npsd_state();

            // Reset the move time status flag
            status_flag_s.move_time_b_ = false;

            // Registered the selected move
            board_game.update_board_game(BOARD_RED_ON);
            
            if(true == (board_game.check_winner(P1_WIN_TAG))) { // Check for winner
                winner_e_ = GameStatus::PLAYER1_E;
                // Increment player1 win counter
                counter_s.p1_win_ctr_ +=1;

                // Set player1 led indicator to ON and Player2 to OFF
                p1.set_led_indicator_state(LED_ON);
                p2.set_led_indicator_state(LED_OFF);

                check_matches();
            }
            else if(true == (board_game.check_draw())) { // Check for draw
                // Set player1 and 2 led indicators to OFF
                p1.set_led_indicator_state(LED_OFF);
                p2.set_led_indicator_state(LED_OFF);
                // Game is draw.
                cpu_state_e_ = CpuState::DRAW_E;
            }
            else {
                // Assign possible next move. Here, the next vacant led board slot will start to blink.
                board_game.next_move();
                // Change turn
                turn_e_ = GameStatus::PLAYER2_E;
            }
        }
    }
    else {
        // Player2's turn

        // Set player2 led indicator to Blinking and Player1 to OFF
        p2.set_led_indicator_state(LED_BLINKING);
        p1.set_led_indicator_state(LED_OFF);

        if(SwitchState::PSD_NPSD_E == p2.get_switch_state()) {
            // reset psd npsd state to be ready for next button pressed/released action.
            p2.reset_psd_npsd_state();
            // Select move
            board_game.select_move();
        }
        
        // Check if enter button is pressed then released or move time counter has expired.
        if((SwitchState::PSD_NPSD_E == p1_2.get_switch_state()) || (status_flag_s.move_time_b_ != false)) {
            // reset psd npsd state to be ready for next button pressed/released action.
            p1_2.reset_psd_npsd_state();

            // Reset the move time status flag
            status_flag_s.move_time_b_ = false;

            // Registered the selected move
            board_game.update_board_game(BOARD_BLUE_ON);
            
            if(true == (board_game.check_winner(P2_WIN_TAG))) { // Check for winner
                winner_e_ = GameStatus::PLAYER2_E;
                // Increment player1 win counter
                counter_s.p2_win_ctr_ +=1;

                // Set player2 led indicator to ON and Player1 to OFF
                p1.set_led_indicator_state(LED_OFF);
                p2.set_led_indicator_state(LED_ON);

                check_matches();
            }
            else if(true == (board_game.check_draw())) { // Check for draw
                // Set player1 and 2 led indicators to OFF
                p1.set_led_indicator_state(LED_OFF);
                p2.set_led_indicator_state(LED_OFF);
                // Game is draw.
                cpu_state_e_ = CpuState::DRAW_E;
            }
            else {
                // Assign possible next move. Here, the next vacant led board slot will start to blink. 
                board_game.next_move();
                // Change turn
                turn_e_ = GameStatus::PLAYER1_E;
            }
        }
    }
    
    // Display the scores
    p1.set_display(counter_s.p1_win_ctr_);
    p2.set_display(counter_s.p2_win_ctr_);
}

/**
    Handles game draw conditions
    Transition from cpu state DRAW_E to NEW_GAME_E
    
    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{92} <br/>

    @param  none
    @return none
*/
void Game::draw() {
    
    counter_s.cpu_state_transition_ctr_++;
    if(counter_s.cpu_state_transition_ctr_ >= DRAW_TO_NEW_GAME_TIME_MS) {
        counter_s.cpu_state_transition_ctr_ = 0;

        // Timer has elapsed. Move to new game state
        cpu_state_e_ = CpuState::NEW_GAME_E;
    }
}

/**
    Checking and monitoring of any possible game interruption (e.g. Stop/Reset).
    It will be called inside PLAY_E state.
    Transition from cpu state PLAY_E to STOP_E/STARTUP_E

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{96} <br/>
    @req_SYS_TTT{98} <br/>
    @req_SYS_TTT{99} <br/>
    @req_SYS_TTT{106} <br/>

    @param  none
    @return none
*/
void Game::check_game_interruption() {

    if((SwitchState::PSD_E == p1.get_switch_state()) && (SwitchState::PSD_E == p2.get_switch_state())) {
        // Set game interruption flag
        status_flag_s.game_interruption_b_ = true;

        if(counter_s.cpu_state_transition_ctr_ >= GAME_INTERRUPT_TIME_MS) {
            // Set led indicators to Off
            p1.set_led_indicator_state(LED_OFF);
            p2.set_led_indicator_state(LED_OFF);
        }
        else {
            counter_s.cpu_state_transition_ctr_++;
        }
    }
    else if(    (    (SwitchState::PSD_E == p1.get_switch_state()) 
                  || (SwitchState::PSD_E == p2.get_switch_state())
                 )
             && (SwitchState::PSD_E == p1_2.get_switch_state())   ) 
    {
        // Set game interruption flag
        status_flag_s.game_interruption_b_ = true;
        
        if(counter_s.restart_ctr_ >= GAME_INTERRUPT_TIME_MS) {

            // Set led indicators to Off
            p1.set_led_indicator_state(LED_OFF);
            p2.set_led_indicator_state(LED_OFF);
            // Clear board game
            board_game.fill_board_game(BOARD_OFF);
        }
        else {
            counter_s.restart_ctr_++;
        }
    }
    else if(counter_s.cpu_state_transition_ctr_ >= GAME_INTERRUPT_TIME_MS) {
        counter_s.restart_ctr_ = 0;

        if((SwitchState::PSD_NPSD_E == p1.get_switch_state()) && (SwitchState::PSD_NPSD_E == p2.get_switch_state())) {
            // Reset psd npsd state to be ready for next button pressed/released action.
            p1.reset_psd_npsd_state();
            p2.reset_psd_npsd_state();

            counter_s.cpu_state_transition_ctr_ = 0;
            // Move to Stop state
            cpu_state_e_ = CpuState::STOP_E;
        }
    }
    else if(counter_s.restart_ctr_ >= GAME_INTERRUPT_TIME_MS) {
        counter_s.cpu_state_transition_ctr_ = 0;

        if(    (     (SwitchState::PSD_NPSD_E == p1.get_switch_state()) 
                  || (SwitchState::PSD_NPSD_E == p2.get_switch_state())
                )
             && (SwitchState::PSD_NPSD_E == p1_2.get_switch_state())   ) 
        {
            // Reset psd npsd state to be ready for next button pressed/released action.
            p1.reset_psd_npsd_state();
            p2.reset_psd_npsd_state();
            p1_2.reset_psd_npsd_state();

            counter_s.restart_ctr_ = 0;
            // Move to startup state
            cpu_state_e_ = CpuState::STARTUP_E;  
        }
    }
    else {
        counter_s.restart_ctr_ = 0;
        counter_s.cpu_state_transition_ctr_ = 0;
        // Clear the interruption flag
        status_flag_s.game_interruption_b_ = false;        
    }   
}

/**
    Handles game resume conditons
    Transition from cpu state STOP_E to PLAY_E

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{101} <br/>
    @req_SYS_TTT{103} <br/>
    @req_SYS_TTT{104} <br/>

    @param  none
    @return none

*/
void Game::resume() {

    if((SwitchState::PSD_E == p1.get_switch_state()) && (SwitchState::PSD_E == p2.get_switch_state())) {
        // Switches are pressed. Wait for game interrupt counter to expired.
        if(counter_s.cpu_state_transition_ctr_ <= GAME_INTERRUPT_TIME_MS) {
            counter_s.cpu_state_transition_ctr_++;
        }
    }
    else if(counter_s.cpu_state_transition_ctr_ >= GAME_INTERRUPT_TIME_MS) { // Game interrupt counter has expired.
        if((SwitchState::PSD_NPSD_E == p1.get_switch_state()) && (SwitchState::PSD_NPSD_E == p2.get_switch_state())) {
            // Reset psd npsd state to be ready for next button pressed/released action.
            p1.reset_psd_npsd_state();
            p2.reset_psd_npsd_state();

            counter_s.cpu_state_transition_ctr_ = 0;
            // Reset the interruption flag
            status_flag_s.game_interruption_b_ = false;
            // Buttons are released. Move to play state and continue the game
            cpu_state_e_ = CpuState::PLAY_E;
        }
    }
    else {
        counter_s.cpu_state_transition_ctr_ = 0;
    }
}

/**
    Handles game over conditons
    Transition from cpu state GAME_OVER_E to NEW_GAME_E/SETTING_E

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{85} <br/>
    @req_SYS_TTT{86} <br/>
    @req_SYS_TTT{111} <br/>
    @req_SYS_TTT{112} <br/>

    @param  none
    @return none
*/
void Game::game_over() {

    if(SwitchState::PSD_NPSD_E == p1_2.get_switch_state()) {
        // Reset psd npsd state to be ready for next button pressed/released action.
        p1_2.reset_psd_npsd_state();
        
        // Set led indicators to Off
        p1.set_led_indicator_state(LED_OFF);
        p2.set_led_indicator_state(LED_OFF);

        if(game_mode_s.mode_info_ != MODE1) {
            // Clear board game
            board_game.fill_board_game(BOARD_OFF);
            // Go back to setting state. Here, Players can able to change the game setting.
            cpu_state_e_ = CpuState::SETTING_E;            
        }
        else {
            // Move to new game state to start a new game
            cpu_state_e_ = CpuState::NEW_GAME_E;            
        }
    }
}

/**
    Module main state machine

    @param  none
    @return none
*/
void Game::game_task() {
 
    switch(cpu_state_e_) {
        case CpuState::STARTUP_E: {
            start_up();
            break;
        }
        case CpuState::SETTING_E: {
            game_setting();
            break;
        }
        case CpuState::NEW_GAME_E: {
            new_game();
            break;
        }
        case CpuState::PLAY_E: { // Playing the game
            // check any possible interruption
            check_game_interruption();

            if(status_flag_s.game_interruption_b_ != true) {
                // No interruption, continue the game
                move_time_monitoring();
                play();
            }
            break;
        }
        case CpuState::DRAW_E: {
            draw();
            break;
        }
        case CpuState::STOP_E: {
            resume();
            break;
        }
        case CpuState::GAME_OVER_E: {
            game_over();
            break;
        }
        default : {
            // no action
            break;
        }
    }
}

/**
    Publishing ROS messages

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{116} <br/>
    @req_SYS_TTT{117} <br/>

    @param  none
    @return none
*/
void T3CpuNode::publish_message() {
    const int TASK_100MS = 2;
    const int TASK_200MS = 4;
    const int TASK_500MS = 10;
    const int TASK_1S    = 20;
    static int scheduler = 0;
    uint8_t board[ROW_COL_SIZE];

    scheduler++;
    //_____________________________________
    // 100ms task
    if(0 == (scheduler % TASK_100MS)) {
        // Publish Led board game message
        tictactoe_msgs::T3_Led_Board_Game_Msg led_board_msgs;
        board_game.get_led_board(board);
        led_board_msgs.Row0_Col0_Bi_Color_Led = board[0];
        led_board_msgs.Row0_Col1_Bi_Color_Led = board[1];
        led_board_msgs.Row0_Col2_Bi_Color_Led = board[2];
        led_board_msgs.Row1_Col0_Bi_Color_Led = board[3];
        led_board_msgs.Row1_Col1_Bi_Color_Led = board[4];
        led_board_msgs.Row1_Col2_Bi_Color_Led = board[5];
        led_board_msgs.Row2_Col0_Bi_Color_Led = board[6];
        led_board_msgs.Row2_Col1_Bi_Color_Led = board[7];
        led_board_msgs.Row2_Col2_Bi_Color_Led = board[8];
        led_board_pub_.publish(led_board_msgs);
    }
    
    //_____________________________________
    // 200ms task
    if(0 == (scheduler % TASK_200MS)) {
        // Publish the LED and buzzer indicator message
        tictactoe_msgs::T3_Indicator_Msg ind_msgs;
        ind_msgs.Player1_Led_Indicator_Status = p1.get_led_indicator_state();
        ind_msgs.Player2_Led_Indicator_Status = p2.get_led_indicator_state();
        ind_msgs.Buzzer_Status                = p1_2.get_buzzer_state();
        indicator_pub_.publish(ind_msgs);
    }

    //_____________________________________
    // 500ms task
    if(0 == (scheduler % TASK_500MS)) {
        // Publish the 7-Segment display message
        tictactoe_msgs::T3_7Segment_Display_Msg display_msgs;
        display_msgs.Player1_Score_Board = p1.get_display();
        display_msgs.Player2_Score_Board = p2.get_display();
        display_msgs.Timer_Board         = p1_2.get_display();
        display_pub_.publish(display_msgs);
    }

    //_____________________________________
    // 1000ms task
    if(0 == (scheduler % TASK_1S)) {
        scheduler = 0; // reset it to start all over again

        // Publish the game setting message
        // tictactoe_msgs::T3_Game_Mode_Msg game_mode_msgs; 
        // game_mode_msgs.Game_Setting_Status      = 0;
        // game_mode_msgs.Mode_Setting_Status      = 0;
        // game_mode_msgs.Move_Time_Setting_Status = 0;
        // game_mode_msgs.Mode_Info                = 0;
        // game_mode_msgs.Move_Time_Info           = 0;
        // game_mode_pub_.publish(game_mode_msgs);
    }
}

/**
    Subscribing the push button switch ROS message
    This also serves as the base scheduling of the module.

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{116} <br/>
    @req_SYS_TTT{117} <br/>

    @param  none
    @return none
*/
void T3CpuNode::push_button_switch_subscribeCallBack(const tictactoe_msgs::T3_Push_Button_Switch_Msg& sws) {

    // Set to player1 the received push button select/move switch state from t3_bcu
    p1.set_switch_state(sws.Player1_SelectMove_Button_Status);
    // Set to player2 the received push button select/move switch state from t3_bcu
    p2.set_switch_state(sws.Player2_SelectMove_Button_Status);
    // Set the received push button enter switch state from t3_bcu
    p1_2.set_switch_state(sws.Enter_Button_Status);

    // Internal task execution within t3_cpu
    game.game_task();
    // Publish the necessary messages to t3_bcu
    publish_message();        
}
