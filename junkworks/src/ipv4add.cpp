#include "junkworks/ipv4add.hpp"

#include "junkworks/network_common.h"

namespace junkworks
{
      ipv4add::ipv4add(
         const unsigned char o1,
         const unsigned char o2,
         const unsigned char o3,
         const unsigned char o4
      )
         : ip{o1, o2, o3, o4}
      { }

      // Returns a number that can be inserted directly into
      // 'sockaddr_in::sin_addr::s_addr'
      unsigned int ipv4add::internet_address(void) const
      {
         unsigned int temp_ip = (
            (ip[0] << 24)
            | (ip[1] << 16)
            | (ip[2] << 8)
            | ip[3]
         );

         return htonl(temp_ip);
      }

      unsigned int & ipv4add::operator[](const unsigned int i)
      {
         return ip[i];
      }

      const unsigned int & ipv4add::operator[](const unsigned int i) const
      {
         return ip[i];
      }

}
