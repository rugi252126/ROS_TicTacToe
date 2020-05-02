/**
    t3_bcu.cpp
    Purpose: ROS Tic-Tac-Toe Base Control Unit. Communication to T3_CPU is done here and also the tasks scheduling.

    @author: Alfonso, Rudy Manalo
    @version 1.1 4/4/2020
*/
#include <Arduino.h>
#include "ros.h" // add ROS
#include "tictactoe_msgs/T3_Push_Button_Switch_Msg.h"
#include "tictactoe_msgs/T3_Indicator_Msg.h"
#include "tictactoe_msgs/T3_7Segment_Display_Msg.h"
#include "tictactoe_msgs/T3_Led_Board_Game_Msg.h"
#include "tictactoe_msgs/T3_Game_Mode_Msg.h"
#include "io_pins.h"
#include "com.h"
#include "project.h"
#include "switch_in.h"
#include "led_indicator.h"
#include "seven_segment.h"
#include "led_board.h"
#include "buzzer.h"

ros::NodeHandle nh; // create ROS handler "nh"

// Constant declaration
// Connection blinking frequency is 2Hz. Equivalent to 500ms.
const int CONNECTION_BLINKING_FREQ_MS   = (500/BASE_TASK_MS);

// Main states
enum class MainState{
    SW_INIT_E,
    CONNECTION_E,
    RUN_E,
};

// Used in scheduler
union uint8_t_bitfields {
    uint8_t data_byte;
    struct {
        uint8_t b0:1;
        uint8_t b1:1;
        uint8_t b2:1;
        uint8_t b3:1;
        uint8_t b4:1;
        uint8_t b5:1;
        uint8_t b6:1;
        uint8_t b7:1;
    } data_bit;
};

// Module states and data
struct {
    // Main state machine
    MainState main_state_e;
    union uint8_t_bitfields new_time_u;
    union uint8_t_bitfields time_flag_u;
    // incremented every 10ms
    uint8_t time_slice_cycle_ctr; 
    // Connection counter to blink the Led
    int connection_led_blinking_ctr;
    // TRUE connection is established between T3_CPU and T3_BCU
    bool connection_status;
} bcu_s;


// Object instances
// From switch_in module
SwitchIn p1_switch(DI__PLAYER1_SELECT_SWITCH, SWS_SNA);
SwitchIn p2_switch(DI__PLAYER2_SELECT_SWITCH, SWS_SNA);
SwitchIn enter_switch(DI__ENTER_SWITCH,SWS_SNA);
// From led_indicator module
LedIndicator led1_ind(DO__PLAYER1_LED_IND, LED_OFF);
LedIndicator led2_ind(DO__PLAYER2_LED_IND, LED_OFF);
// From seven_segment module
SevenSegment disp_7segment;
// From led_board module
LedBoard bcu_board;
// From buzzer module
Buzzer buzzer(DO__BUZZER, BUZZER_OFF);


// Local function prototypes
#if defined(SW_DEVELOPMENT_DEBUG_PORT_USED)
/**
    Debug port used for SW development to measure timing related information (e.g. scheduler, ROS Tx/Rx time)

    @param  none
    @return none
*/
void toggle_port();
#endif // SW_DEVELOPMENT_DEBUG_PORT_USED
/**
    Initialization of Timer1 interrupt used for scheduling

    @param none
    @return none
*/
void timer_interrupt_setup();

// ROS callback function prototype
/**
    ROS callback function for indicators (led, buzzer)

    @param  ind {see T3_Indicator_Msg message}
    @return none
*/
void indicator_callback(const tictactoe_msgs::T3_Indicator_Msg& ind);

/**
    ROS callback function for 7-segment display

    @param  disp {see T3_7Segment_Display_Msg message}
    @return none
*/
void display_callback(const tictactoe_msgs::T3_7Segment_Display_Msg& disp);

/**
    ROS callback function for led board

    @param  board {see T3_Led_Board_Game_Msg message}
    @return none
*/
void led_board_callback(const tictactoe_msgs::T3_Led_Board_Game_Msg& board);

/**
    ROS callback function for game setting

    @param  setting {see T3_Game_Mode_Msg message}
    @return none
*/
void game_mode_callback(const tictactoe_msgs::T3_Game_Mode_Msg& setting);

// ROS subcriber
ros::Subscriber<tictactoe_msgs::T3_Indicator_Msg>ind_sub("indicator", indicator_callback);
ros::Subscriber<tictactoe_msgs::T3_7Segment_Display_Msg>disp_sub("display", display_callback);
ros::Subscriber<tictactoe_msgs::T3_Led_Board_Game_Msg>board_sub("led_board", led_board_callback);
ros::Subscriber<tictactoe_msgs::T3_Game_Mode_Msg>setting_sub("game_mode", game_mode_callback);
// ROS publisher
tictactoe_msgs::T3_Push_Button_Switch_Msg raw_sws; 
ros::Publisher raw_sws_pub("button", &raw_sws);

// Initialize Timer1 interrupt
void timer_interrupt_setup() {
    cli(); // stop interrupts

    // Set timer1 interrupt to 100Hz
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1 = 0; // initialize counter value to 0
    // Set compare match register for 100Hz increments
    OCR1A = 155; // = (16MHz) / (100*1024) - 1 (must be < 65536)
    // Turn ON CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS12 and CS10 bits for 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);
    // Enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);

    sei(); // allow interrupts
}

// Setups operation of the module
void setup() {

    // Initialize ROS node
    nh.initNode(); 
    // Subscriber
    nh.subscribe(ind_sub);
    nh.subscribe(disp_sub);
    nh.subscribe(board_sub);
    nh.subscribe(setting_sub);
    // Publisher
    nh.advertise(raw_sws_pub);
    // Initialize led connection indicator as digital output
    pinMode(DO__CONNECTION_LED_IND, OUTPUT);

#if defined(SW_DEVELOPMENT_DEBUG_PORT_USED)
    pinMode(DO__TOGGLE_PORT1, OUTPUT);
#endif

    // Initialize the timer interrupt
    timer_interrupt_setup();

    // Move state to SW init
    bcu_s.main_state_e = MainState::SW_INIT_E;
}

#if defined(SW_DEVELOPMENT_DEBUG_PORT_USED)
// Debug port for SW development only
void toggle_port() {

    static bool toggle_b = true;

    if(toggle_b) {
        // Set port to ON
        digitalWrite(DO__TOGGLE_PORT1, HIGH);
        toggle_b = 0;
    }
    else {
        // Set port to OFF
        digitalWrite(DO__TOGGLE_PORT1, LOW);
        toggle_b = 1;
    }  
}
#endif // SW_DEVELOPMENT_DEBUG_PORT_USED

/**
    Initialization of software data within the module

    @param none
    @return none
*/
void t3_bcu_software_initialization() {
    bcu_s.new_time_u.data_byte = 0;
    bcu_s.time_flag_u.data_byte = 0;
    bcu_s.time_slice_cycle_ctr = 0;
    bcu_s.connection_led_blinking_ctr = 0;
    bcu_s.connection_status = false; // Connection is not yet established by default.

    // Move state to Connection and wait until connection is established.
    bcu_s.main_state_e = MainState::CONNECTION_E;
}

/**
    Handles connection led blinking indicator. 
    It will only be called if connection is not yet established or connection is lost.

    @param  none
    @return none
*/
void bcu_led_connection_indicator() {
    static bool connection_b = false;

    if(connection_b) {
        // Set port to ON
        digitalWrite(DO__CONNECTION_LED_IND, HIGH);
        connection_b = 0;
    }
    else {
        // Set port to OFF
        digitalWrite(DO__CONNECTION_LED_IND, LOW);
        connection_b = 1;
    }  
}

/*
    Monitoring the connection between T3_CPU and T3_BCU

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{21} <br/>
    @req_SYS_TTT{23} <br/>
    @req_SYS_TTT{24} <br/>
    @req_SYS_TTT{25} <br/>
    
    @param  none
    @return none
*/
void bcu_connection() {

    while(!nh.connected()) {
        // Call all the callbacks waiting to be called.
        nh.spinOnce();

        if(bcu_s.connection_led_blinking_ctr >= CONNECTION_BLINKING_FREQ_MS) {
            bcu_s.connection_led_blinking_ctr = 0;
            bcu_led_connection_indicator();
        }
        else {
            // no action
        }
    }

    // ROS Info
    nh.loginfo("T3_BCU is connected.");

    // Connection is established. 
    bcu_s.connection_status = true;
    bcu_s.connection_led_blinking_ctr = 0;
    // Switch-ON the LED indicator permanently.
    digitalWrite(DO__CONNECTION_LED_IND, HIGH);
    // Proceed to run state since connection is already established.
    bcu_s.main_state_e = MainState::RUN_E;
}

/*
    Tasks scheduling processing

    Satisfy Requirement(s): <br/>
    @req_SYS_TTT{25} <br/>
    
    @param  none
    @return none
*/
void bcu_run() {
    static int sws_ctr = 0;
    static int led_ind_ctr = 0;
    bool pub_stat_b = false;

    // TASK: 10ms
    if(bcu_s.time_slice_cycle_ctr > 0) {
        bcu_s.time_slice_cycle_ctr = 0;

        // Update the 7-segment displays
        disp_7segment.update_display();

        // Read the current state of the switches
        p1_switch.update_switch_state();
        p2_switch.update_switch_state();
        enter_switch.update_switch_state();
     
        // Create 50ms task
        sws_ctr++;
        if(sws_ctr >= (SWITCH_IN_TASK_MS/BASE_TASK_MS)) {
            sws_ctr = 0;

            // Update led board states
            bcu_board.update_led_board();
            
            // Update buzzer state
            buzzer.update_buzzer_state();

            // Publish the switch state information
            raw_sws.Player1_SelectMove_Button_Status = p1_switch.get_switch_state();
            raw_sws.Player2_SelectMove_Button_Status = p2_switch.get_switch_state();
            raw_sws.Enter_Button_Status = enter_switch.get_switch_state();
            raw_sws_pub.publish(&raw_sws);

            // Set to true as publish has been done.
            pub_stat_b = true;
        }
    }
    else {
        // no action
    }

    if(bcu_s.time_flag_u.data_bit.b0) {
        // TASK: 20ms
        bcu_s.time_flag_u.data_bit.b0 = 0;

        // Create 100ms task
        led_ind_ctr++;
        if(led_ind_ctr >= (LED_INDICATOR_TASK_MS/LED_INDICATOR_BASE_TASK_MS)) {
            led_ind_ctr = 0;

            // Update Led indicator states
            led1_ind.update_led_indicator_state();
            led2_ind.update_led_indicator_state();
        }
    }
    else if(bcu_s.time_flag_u.data_bit.b1) {
        // TASK: 40ms
        bcu_s.time_flag_u.data_bit.b1 = 0;
    }
    else if(bcu_s.time_flag_u.data_bit.b2) {
        // TASK: 80ms
        bcu_s.time_flag_u.data_bit.b2 = 0;
    }
    else if(bcu_s.time_flag_u.data_bit.b3) {
        // TASK: 160ms
        bcu_s.time_flag_u.data_bit.b3 = 0;
    }
    else if(bcu_s.time_flag_u.data_bit.b4) {
        // TASK: 320ms
        bcu_s.time_flag_u.data_bit.b4 = 0;
    }
    else if(bcu_s.time_flag_u.data_bit.b5) {
        // TASK: 640ms
        bcu_s.time_flag_u.data_bit.b5 = 0;
    }
    else if(bcu_s.time_flag_u.data_bit.b6) {
        // TASK: 1280ms
        bcu_s.time_flag_u.data_bit.b6 = 0;
        
        // Re-check connection. In case connection is lost.
        if(!nh.connected()) {
            // Clear all indicators and displays
            disp_7segment.clear_display();
            led1_ind.clear_led_indicator();
            led2_ind.clear_led_indicator();
            bcu_board.clear_led_board();
            buzzer.clear_buzzer();
            // Connection is lost.
            bcu_s.connection_status = false;
            // Go back to connection state and wait for the connection to be established again.
            bcu_s.main_state_e = MainState::CONNECTION_E;
        }
    }  
    else if(bcu_s.time_flag_u.data_bit.b7) {
        // TASK: 2560ms
        bcu_s.time_flag_u.data_bit.b7 = 0;
    }
    else {
        // no action
    }

    // Call only if there is data that need to be published.
    if(pub_stat_b != true) {
        pub_stat_b = false;
        // Call all the callbacks waiting to be called.
        nh.spinOnce();
    } 
}

/**
    Controls operation of the program.
    Module main state machine

    @param  none
    @return none
*/
void loop() {

    switch(bcu_s.main_state_e) {
        case MainState::SW_INIT_E: {
            t3_bcu_software_initialization();
            break;
        }
        case MainState::CONNECTION_E: {
            bcu_connection();
            break;
        }
        case MainState::RUN_E: {
            bcu_run();
            break;
        }
            default: {
            // no action
        }
    }

}

/**
    It will be called by timer interrupt and manage the time slice shceduling

    @param  none
    @return none
*/
void isr_processing_time_slice() {
    static uint8_t tmp;
    static uint8_t d;

    if(false == bcu_s.connection_status) {
        bcu_s.connection_led_blinking_ctr +=1;
    }
    else { // Connection is established. Process the time slices

        bcu_s.time_slice_cycle_ctr +=1;

        // This will go from 0 to 255 and then overflow which means go back to 0 again.
        bcu_s.new_time_u.data_byte +=1;
        tmp = bcu_s.new_time_u.data_byte;

        for(d=0; d < 8; d++) {
            if(tmp & 0x01) {
                break;
            }
            else {
                tmp >>=1;
            }
        }

        if(d < 8) {
            tmp = 1;
            tmp <<= d;

            bcu_s.time_flag_u.data_byte |= tmp;
        }
        else {
            // no action
        }
    }
}

// Timer interrupt callback function
ISR(TIMER1_COMPA_vect) {

    // Generate interrupt every 10ms
    isr_processing_time_slice();
}

//! ROS Callback
/**
    ROS callback function for led board
    It will be called every 100ms. See <Schedule Tables> requirement for more info.
*/
void led_board_callback(const tictactoe_msgs::T3_Led_Board_Game_Msg& board) {
    cli(); // stop interrupts

    uint8_t rx_board[ROW_COL_SIZE];

    // Copy the led board data
    rx_board[0] = board.Row0_Col0_Bi_Color_Led;
    rx_board[1] = board.Row0_Col1_Bi_Color_Led;
    rx_board[2] = board.Row0_Col2_Bi_Color_Led;
    rx_board[3] = board.Row1_Col0_Bi_Color_Led;
    rx_board[4] = board.Row1_Col1_Bi_Color_Led;
    rx_board[5] = board.Row1_Col2_Bi_Color_Led;
    rx_board[6] = board.Row2_Col0_Bi_Color_Led;
    rx_board[7] = board.Row2_Col1_Bi_Color_Led;
    rx_board[8] = board.Row2_Col2_Bi_Color_Led;
    // Set the led board data
    bcu_board.set_led_board(rx_board);

    sei(); // allow interrupts
}

/**
    ROS callback function for indicators (led, buzzer)
    It will be called every 200ms. See <Schedule Tables> requirement for more info.
*/
void indicator_callback(const tictactoe_msgs::T3_Indicator_Msg& ind) {
    cli(); // stop interrupts

    // Set led indicator state
    led1_ind.set_led_indicator_state(ind.Player1_Led_Indicator_Status);
    led2_ind.set_led_indicator_state(ind.Player2_Led_Indicator_Status);

    // Set buzzer state
    buzzer.set_buzzer_state(ind.Buzzer_Status);

    sei(); // allow interrupts
}

/**
    ROS callback function for 7-segment displays
    It will be called every 500ms. See <Schedule Tables> requirement for more info.
*/
void display_callback(const tictactoe_msgs::T3_7Segment_Display_Msg& disp) {
    cli(); // stop interrupts

    // Set 7-display infromation
    disp_7segment.set_display(disp.Player1_Score_Board, disp.Player2_Score_Board, disp.Timer_Board);

    sei(); // allow interrupts
}

/**
    ROS callback function for game settings
    It will be called every 1 second. See <Schedule Tables> requirement for more info.
*/
void game_mode_callback(const tictactoe_msgs::T3_Game_Mode_Msg& setting) {
    cli(); // stop interrupts

    sei(); // allow interrupts
}





