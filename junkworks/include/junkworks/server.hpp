#ifndef SERVER_HEADER

#include "junkworks/udp_socket.hpp"

namespace junkworks
{
   class Server
   {
      Server(void) = delete;
      Server(const unsigned int port);
      
      private:
         UdpSocket socket_;
   };
}

#endif
