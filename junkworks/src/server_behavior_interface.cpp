#include "junkworks/server_behavior_interface.hpp"

namespace junkworks
{
   IServerBehavior::IServerBehavior(UdpSocketWrapper & socket_wrapper)
      : socket_wrapper_(socket_wrapper)
   { }

   void IServerBehavior::set_connection(
      UdpSocket & socket,
      const ipv4add dest_ip,
      const unsigned int dest_port
   )
   {
      socket_wrapper_.set_connection(socket, dest_ip, dest_port);
   }

   void IServerBehavior::update(
      const std::vector<raw_rx_payload_t<128> > & packets
   )
   {
      for (const auto & packet : packets)
      {
         process_packet(packet);
      }
   }

   void IServerBehavior::send_packet(const raw_tx_payload_t<128> & tx_packet)
   {
      socket_wrapper_(tx_packet);
   }
}
