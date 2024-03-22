#include "junkworks/udp_socket.hpp"

#include "junkworks/basic_udp.h"
#include <iostream>

int main(int argc, char ** argv)
{
   if (argc < 2)
   {
      std::cout << "need port number as first argument\n";
      return 0;
   }

   initialize_sockets();

   junkworks::UdpSocket socket(atoi(argv[1]));

   if (!socket.bound())
   {
      std::cout << "Couldn't bind socket\n";
      return 1;
   }

   socket.try_send(
      junkworks::ipv4add(127, 0, 0, 1), 8123, "stuff", sizeof("stuff")
   );

   return 0;
}
