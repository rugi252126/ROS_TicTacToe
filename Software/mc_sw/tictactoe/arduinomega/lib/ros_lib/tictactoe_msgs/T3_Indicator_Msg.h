#ifndef _ROS_tictactoe_msgs_T3_Indicator_Msg_h
#define _ROS_tictactoe_msgs_T3_Indicator_Msg_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace tictactoe_msgs
{

  class T3_Indicator_Msg : public ros::Msg
  {
    public:
      typedef uint8_t _Player1_Led_Indicator_Status_type;
      _Player1_Led_Indicator_Status_type Player1_Led_Indicator_Status;
      typedef uint8_t _Player2_Led_Indicator_Status_type;
      _Player2_Led_Indicator_Status_type Player2_Led_Indicator_Status;
      typedef uint8_t _Buzzer_Status_type;
      _Buzzer_Status_type Buzzer_Status;

    T3_Indicator_Msg():
      Player1_Led_Indicator_Status(0),
      Player2_Led_Indicator_Status(0),
      Buzzer_Status(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->Player1_Led_Indicator_Status >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Player1_Led_Indicator_Status);
      *(outbuffer + offset + 0) = (this->Player2_Led_Indicator_Status >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Player2_Led_Indicator_Status);
      *(outbuffer + offset + 0) = (this->Buzzer_Status >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Buzzer_Status);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->Player1_Led_Indicator_Status =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Player1_Led_Indicator_Status);
      this->Player2_Led_Indicator_Status =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Player2_Led_Indicator_Status);
      this->Buzzer_Status =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Buzzer_Status);
     return offset;
    }

    const char * getType(){ return "tictactoe_msgs/T3_Indicator_Msg"; };
    const char * getMD5(){ return "fb4fb69bbc4404bf0c137a2d2106eb56"; };

  };

}
#endif