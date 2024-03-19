#ifndef CLIENT_HEADER
#define CLIENT_HEADER

#include "junkworks/udp_socket.hpp"

namespace junkworks
{
   class Client
   {
      public:
         Client(void) = delete;
         Client(
            const unsigned int bind_port,
            const ipv4add server_ip,
            const unsigned int server_port
         );

         void update(void);

         void handshake(void);

      private:
         UdpSocket socket_;

         ipv4add server_ip_;

         unsigned int server_port_;

         unsigned int max_handshake_attempts_;

         unsigned int no_ack_counter_;

         bool handshake_in_progress_;

         bool first_handshake_sent_;

         int uid_;

         std::vector<raw_payload_t<128> > packets_;

         void send_handshake_packet(void);
   };
}

#endif
