#ifndef UDP_SOCKET_WRAPPER_HEADER
#define UDP_SOCKET_WRAPPER_HEADER

#include "junkworks/types.hpp"
#include "junkworks/udp_socket.hpp"

namespace junkworks
{
   // Thin wrapper around a post-handshake UDP socket that knows packet
   // destinations.
   class UdpSocketWrapper
   {
      public:
         UdpSocketWrapper(void)
            : initialized_(false)
            , socket_(nullptr)
            , dest_ip_(0)
            , dest_port_(0)
         { }

         UdpSocketWrapper(
            UdpSocket & socket,
            const ipv4add dest_ip,
            const unsigned int dest_port
         );

         void set_connection(
            UdpSocket & socket,
            const ipv4add dest_ip,
            const unsigned int dest_port
         );

         void operator()(const raw_tx_payload_t<128> & tx_packet);

         void send_packet(const raw_tx_payload_t<128> & tx_packet);

      private:
         bool initialized_;

         UdpSocket * socket_;

         ipv4add dest_ip_;

         unsigned int dest_port_;
   };
}

#endif
