#ifndef SERVER_HEADER
#define SERVER_HEADER

#include "junkworks/udp_socket.hpp"

#include "junkworks/types.hpp"

#include <map>
#include <vector>

namespace junkworks
{
   class Server
   {
      public:
         Server(void) = delete;
         Server(const unsigned int port);

         void update(void);

         void handle_handshake(const raw_payload_t<128> & payload);

      private:
         UdpSocket socket_;

         std::vector<raw_payload_t<128> > packets_;

         std::map<client_connection_t, unsigned int> connections_;

         void send_negative_handshake(
            const unsigned int addr, const unsigned int port
         );

         void send_affirmative_handshake(
            const unsigned int addr,
            const unsigned int port,
            const unsigned int uid
         );
   };
}

#endif
