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

         bool handshake_in_progress(void) const
         {
            return handshake_in_progress_;
         }

         bool behavior_running(void) const
         {
            return behavior_running_;
         }

      private:
         UdpSocket socket_;

         ipv4add server_ip_;

         unsigned int server_port_;

         unsigned int max_handshake_attempts_;

         unsigned int no_ack_counter_;

         bool handshake_in_progress_;

         bool first_handshake_sent_;

         int uid_;

         bool behavior_running_;

         std::vector<raw_rx_payload_t<128> > rx_packets_;

         void send_handshake_packet(void);
   };
}

#endif
