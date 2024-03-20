#include "junkworks/client_behavior_interface.hpp"

namespace junkworks
{
   IClientBehavior::IClientBehavior(UdpSocket & socket)
      : socket_(&socket)
   { }

   void IClientBehavior::send_packet(void)
   {
      // socket_->try_send()
   }
}
