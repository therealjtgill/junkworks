#include "junkworks/udp_socket.hpp"

#include <iostream>

int main(int argc, char ** argv)
{
   if (argc < 2)
   {
      return 0;
   }

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
