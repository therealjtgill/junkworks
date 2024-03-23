#include "junkworks/udp_socket.hpp"

#include "junkworks/basic_udp.h"
#include <iostream>

int main(int argc, char ** argv)
{
   if (argc < 6)
   {
      std::cout << "server_ip0 server_ip1 server_ip2 server_ip3 client_port\n";
      return 1;
   }

   junkworks::ipv4add server_ip(
      std::atoi(argv[1]),
      std::atoi(argv[2]),
      std::atoi(argv[3]),
      std::atoi(argv[4])
   );

   const unsigned int bind_port = std::atoi(argv[5]);

   initialize_sockets();

   junkworks::UdpSocket socket(bind_port);

   if (!socket.bound())
   {
      std::cout << "Couldn't bind socket\n";
      return 1;
   }

   const unsigned char greeting[] = "stuff";

   socket.try_send(
      server_ip, 8123, greeting, sizeof(greeting)
   );

   return 0;
}
