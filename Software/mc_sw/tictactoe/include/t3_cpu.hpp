#ifndef T3_CPU_H_
#define T3_CPU_H_

#include <ros/ros.h>
#include <tictactoe_msgs/T3_Push_Button_Switch_Msg.h>

class T3CpuNode {
    public:
        // Constructor
        T3CpuNode();
        
        void publish_message();
        void push_button_switch_subscribeCallBack(const tictactoe_msgs::T3_Push_Button_Switch_Msg& sws);

    private:
        ros::NodeHandle nh_;
        ros::Publisher indicator_pub_;
        ros::Publisher display_pub_;
        ros::Publisher led_board_pub_;
        ros::Publisher game_mode_pub_;
        ros::Subscriber sws_button_sub_;      
};

#endif // T3_CPU_H_
