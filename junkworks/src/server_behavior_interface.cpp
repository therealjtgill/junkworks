#include "junkworks/server_behavior_interface.hpp"

namespace junkworks
{
   IServerBehavior::IServerBehavior(UdpSocket & socket)
      : socket_(&socket)
   { }
}
