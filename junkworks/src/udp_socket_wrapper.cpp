#include "junkworks/udp_socket_wrapper.hpp"

namespace junkworks
{
   UdpSocketWrapper::UdpSocketWrapper(
      UdpSocket & socket,
      const ipv4add dest_ip,
      const unsigned int dest_port
   )
      : socket_(&socket)
      , dest_ip_(dest_ip)
      , dest_port_(dest_port)
   { }

   void UdpSocketWrapper::operator()(const raw_tx_payload_t<128> & tx_packet)
   {
      send_packet(tx_packet);
   }

   void UdpSocketWrapper::send_packet(const raw_tx_payload_t<128> & tx_packet)
   {
      socket_->try_send(dest_ip_, dest_port_, tx_packet.data, tx_packet.size);
   }
}
