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

         void handle_handshake(const raw_rx_payload_t<128> & payload);

         std::vector<byte_data_t<128> > get_client_rx_bytes(
            const unsigned int uid
         );

         void set_client_tx_bytes(
            const unsigned int uid, const std::vector<byte_data_t<128> > & tx_bytes
         );

      private:
         UdpSocket socket_;

         std::vector<raw_rx_payload_t<128> > rx_packets_;

         std::vector<raw_rx_payload_t<128> > rx_handshake_packets_;

         std::vector<raw_rx_payload_t<128> > rx_behavior_packets_;

         std::map<client_connection_t, unsigned int> connections_;

         std::map<
            unsigned int,
            std::vector<byte_data_t<128> >
         > uid_to_rx_bytes_;

         std::map<
            unsigned int,
            std::vector<byte_data_t<128> >
         > uid_to_tx_bytes_;

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
