#ifndef _ROS_tictactoe_msgs_T3_Push_Button_Switch_Msg_h
#define _ROS_tictactoe_msgs_T3_Push_Button_Switch_Msg_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace tictactoe_msgs
{

  class T3_Push_Button_Switch_Msg : public ros::Msg
  {
    public:
      typedef uint8_t _Player1_SelectMove_Button_Status_type;
      _Player1_SelectMove_Button_Status_type Player1_SelectMove_Button_Status;
      typedef uint8_t _Player2_SelectMove_Button_Status_type;
      _Player2_SelectMove_Button_Status_type Player2_SelectMove_Button_Status;
      typedef uint8_t _Enter_Button_Status_type;
      _Enter_Button_Status_type Enter_Button_Status;

    T3_Push_Button_Switch_Msg():
      Player1_SelectMove_Button_Status(0),
      Player2_SelectMove_Button_Status(0),
      Enter_Button_Status(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->Player1_SelectMove_Button_Status >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Player1_SelectMove_Button_Status);
      *(outbuffer + offset + 0) = (this->Player2_SelectMove_Button_Status >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Player2_SelectMove_Button_Status);
      *(outbuffer + offset + 0) = (this->Enter_Button_Status >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Enter_Button_Status);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->Player1_SelectMove_Button_Status =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Player1_SelectMove_Button_Status);
      this->Player2_SelectMove_Button_Status =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Player2_SelectMove_Button_Status);
      this->Enter_Button_Status =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Enter_Button_Status);
     return offset;
    }

    const char * getType(){ return "tictactoe_msgs/T3_Push_Button_Switch_Msg"; };
    const char * getMD5(){ return "dbff70a21f4f49f1e4284e28251177df"; };

  };

}
#endif