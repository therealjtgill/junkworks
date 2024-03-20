#ifndef SERVER_BEHAVIOR_HEADER
#define SERVER_BEHAVIOR_HEADER

#include "junkworks/udp_socket.hpp"

namespace junkworks
{
   class IServerBehavior
   {
      public:
         IServerBehavior(UdpSocket & socket);

      protected:
         void send_packet(void);

      private:
         UdpSocket * socket_;
   };
}

#endif
