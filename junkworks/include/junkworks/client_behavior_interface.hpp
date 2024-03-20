#ifndef CLIENT_BEHAVIOR_HEADER
#define CLIENT_BEHAVIOR_HEADER

#include "junkworks/types.hpp"
#include "junkworks/udp_socket.hpp"

#include <vector>

namespace junkworks
{
   class IClientBehavior
   {
      public:
         IClientBehavior(UdpSocket & socket);

         void update(const std::vector<raw_payload_t<128> > & packets);

      protected:
         void send_packet(void);

      private:
         UdpSocket * socket_;
   };
}

#endif
