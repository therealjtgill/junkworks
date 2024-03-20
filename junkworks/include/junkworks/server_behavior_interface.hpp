#ifndef SERVER_BEHAVIOR_HEADER
#define SERVER_BEHAVIOR_HEADER

#include "junkworks/types.hpp"
#include "junkworks/udp_socket_wrapper.hpp"

#include <vector>

namespace junkworks
{
   class IServerBehavior
   {
      public:
         IServerBehavior(UdpSocketWrapper & socket_wrapper);

         void update(const std::vector<raw_rx_payload_t<128> > & packets);

         virtual void process_packet(const raw_rx_payload_t<128> & packet) = 0;

         virtual std::vector<unsigned char> supported_packet_types(void) const = 0;

      protected:
         void send_packet(const raw_tx_payload_t<128> & tx_packet);

      private:
         UdpSocketWrapper socket_wrapper_;
   };
}

#endif
