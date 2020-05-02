#ifndef _ROS_tictactoe_msgs_T3_Led_Board_Game_Msg_h
#define _ROS_tictactoe_msgs_T3_Led_Board_Game_Msg_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace tictactoe_msgs
{

  class T3_Led_Board_Game_Msg : public ros::Msg
  {
    public:
      typedef uint8_t _Row0_Col0_Bi_Color_Led_type;
      _Row0_Col0_Bi_Color_Led_type Row0_Col0_Bi_Color_Led;
      typedef uint8_t _Row0_Col1_Bi_Color_Led_type;
      _Row0_Col1_Bi_Color_Led_type Row0_Col1_Bi_Color_Led;
      typedef uint8_t _Row0_Col2_Bi_Color_Led_type;
      _Row0_Col2_Bi_Color_Led_type Row0_Col2_Bi_Color_Led;
      typedef uint8_t _Row1_Col0_Bi_Color_Led_type;
      _Row1_Col0_Bi_Color_Led_type Row1_Col0_Bi_Color_Led;
      typedef uint8_t _Row1_Col1_Bi_Color_Led_type;
      _Row1_Col1_Bi_Color_Led_type Row1_Col1_Bi_Color_Led;
      typedef uint8_t _Row1_Col2_Bi_Color_Led_type;
      _Row1_Col2_Bi_Color_Led_type Row1_Col2_Bi_Color_Led;
      typedef uint8_t _Row2_Col0_Bi_Color_Led_type;
      _Row2_Col0_Bi_Color_Led_type Row2_Col0_Bi_Color_Led;
      typedef uint8_t _Row2_Col1_Bi_Color_Led_type;
      _Row2_Col1_Bi_Color_Led_type Row2_Col1_Bi_Color_Led;
      typedef uint8_t _Row2_Col2_Bi_Color_Led_type;
      _Row2_Col2_Bi_Color_Led_type Row2_Col2_Bi_Color_Led;

    T3_Led_Board_Game_Msg():
      Row0_Col0_Bi_Color_Led(0),
      Row0_Col1_Bi_Color_Led(0),
      Row0_Col2_Bi_Color_Led(0),
      Row1_Col0_Bi_Color_Led(0),
      Row1_Col1_Bi_Color_Led(0),
      Row1_Col2_Bi_Color_Led(0),
      Row2_Col0_Bi_Color_Led(0),
      Row2_Col1_Bi_Color_Led(0),
      Row2_Col2_Bi_Color_Led(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->Row0_Col0_Bi_Color_Led >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Row0_Col0_Bi_Color_Led);
      *(outbuffer + offset + 0) = (this->Row0_Col1_Bi_Color_Led >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Row0_Col1_Bi_Color_Led);
      *(outbuffer + offset + 0) = (this->Row0_Col2_Bi_Color_Led >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Row0_Col2_Bi_Color_Led);
      *(outbuffer + offset + 0) = (this->Row1_Col0_Bi_Color_Led >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Row1_Col0_Bi_Color_Led);
      *(outbuffer + offset + 0) = (this->Row1_Col1_Bi_Color_Led >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Row1_Col1_Bi_Color_Led);
      *(outbuffer + offset + 0) = (this->Row1_Col2_Bi_Color_Led >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Row1_Col2_Bi_Color_Led);
      *(outbuffer + offset + 0) = (this->Row2_Col0_Bi_Color_Led >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Row2_Col0_Bi_Color_Led);
      *(outbuffer + offset + 0) = (this->Row2_Col1_Bi_Color_Led >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Row2_Col1_Bi_Color_Led);
      *(outbuffer + offset + 0) = (this->Row2_Col2_Bi_Color_Led >> (8 * 0)) & 0xFF;
      offset += sizeof(this->Row2_Col2_Bi_Color_Led);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->Row0_Col0_Bi_Color_Led =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Row0_Col0_Bi_Color_Led);
      this->Row0_Col1_Bi_Color_Led =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Row0_Col1_Bi_Color_Led);
      this->Row0_Col2_Bi_Color_Led =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Row0_Col2_Bi_Color_Led);
      this->Row1_Col0_Bi_Color_Led =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Row1_Col0_Bi_Color_Led);
      this->Row1_Col1_Bi_Color_Led =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Row1_Col1_Bi_Color_Led);
      this->Row1_Col2_Bi_Color_Led =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Row1_Col2_Bi_Color_Led);
      this->Row2_Col0_Bi_Color_Led =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Row2_Col0_Bi_Color_Led);
      this->Row2_Col1_Bi_Color_Led =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Row2_Col1_Bi_Color_Led);
      this->Row2_Col2_Bi_Color_Led =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->Row2_Col2_Bi_Color_Led);
     return offset;
    }

    const char * getType(){ return "tictactoe_msgs/T3_Led_Board_Game_Msg"; };
    const char * getMD5(){ return "87e56076851f097a3cdfaaccdae20e93"; };

  };

}
#endif