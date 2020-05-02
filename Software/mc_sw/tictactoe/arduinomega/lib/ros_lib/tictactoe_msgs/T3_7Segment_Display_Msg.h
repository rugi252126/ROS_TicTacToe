#ifndef _ROS_tictactoe_msgs_T3_7Segment_Display_Msg_h
#define _ROS_tictactoe_msgs_T3_7Segment_Display_Msg_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace tictactoe_msgs
{

  class T3_7Segment_Display_Msg : public ros::Msg
  {
    public:
      typedef uint8_t _Player1_Score_Board_type;
      _Player1_Score_Board_type Player1_Score_Board;
      typedef uint8_t _Player2_Score_Board_type;
      _Player2_Score_Board_type Player2_Score_Board;
      typedef uint8_t _Timer_Board_type;
      _Timer_Board_type Timer_Board;

    T3_7Segment_Display_Msg():
      Player1_Score_Board(0),
      Player2_Score_Board(0),
      Timer_Board(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->Player1_Score_Board >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Player1_Score_Board);
      *(outbuffer + offset + 0) = (this->Player2_Score_Board >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Player2_Score_Board);
      *(outbuffer + offset + 0) = (this->Timer_Board >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Timer_Board);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->Player1_Score_Board =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Player1_Score_Board);
      this->Player2_Score_Board =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Player2_Score_Board);
      this->Timer_Board =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Timer_Board);
     return offset;
    }

    const char * getType(){ return "tictactoe_msgs/T3_7Segment_Display_Msg"; };
    const char * getMD5(){ return "8d19ae50892b25921b561a2524a520a5"; };

  };

}
#endif