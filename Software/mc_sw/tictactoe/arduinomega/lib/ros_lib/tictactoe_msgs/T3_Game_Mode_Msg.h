#ifndef _ROS_tictactoe_msgs_T3_Game_Mode_Msg_h
#define _ROS_tictactoe_msgs_T3_Game_Mode_Msg_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace tictactoe_msgs
{

  class T3_Game_Mode_Msg : public ros::Msg
  {
    public:
      typedef uint8_t _Game_Setting_Status_type;
      _Game_Setting_Status_type Game_Setting_Status;
      typedef uint8_t _Mode_Setting_Status_type;
      _Mode_Setting_Status_type Mode_Setting_Status;
      typedef uint8_t _Move_Time_Setting_Status_type;
      _Move_Time_Setting_Status_type Move_Time_Setting_Status;
      typedef uint8_t _Mode_Info_type;
      _Mode_Info_type Mode_Info;
      typedef uint8_t _Move_Time_Info_type;
      _Move_Time_Info_type Move_Time_Info;

    T3_Game_Mode_Msg():
      Game_Setting_Status(0),
      Mode_Setting_Status(0),
      Move_Time_Setting_Status(0),
      Mode_Info(0),
      Move_Time_Info(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->Game_Setting_Status >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Game_Setting_Status);
      *(outbuffer + offset + 0) = (this->Mode_Setting_Status >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Mode_Setting_Status);
      *(outbuffer + offset + 0) = (this->Move_Time_Setting_Status >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Move_Time_Setting_Status);
      *(outbuffer + offset + 0) = (this->Mode_Info >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Mode_Info);
      *(outbuffer + offset + 0) = (this->Move_Time_Info >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Move_Time_Info);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->Game_Setting_Status =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Game_Setting_Status);
      this->Mode_Setting_Status =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Mode_Setting_Status);
      this->Move_Time_Setting_Status =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Move_Time_Setting_Status);
      this->Mode_Info =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Mode_Info);
      this->Move_Time_Info =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Move_Time_Info);
     return offset;
    }

    const char * getType(){ return "tictactoe_msgs/T3_Game_Mode_Msg"; };
    const char * getMD5(){ return "e45357766f4ebf1a973c66b0fc0819cf"; };

  };

}
#endif