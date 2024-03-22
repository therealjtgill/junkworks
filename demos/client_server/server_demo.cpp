#include "junkworks/udp_socket.hpp"

#include <iostream>

int main(int argc, char ** argv)
{
   if (argc < 2)
   {
      std::cout << "need port number as first argument\n";
      return 0;
   }

   char packet_data[6];

   junkworks::UdpSocket socket(atoi(argv[1]));

   if (!socket.bound())
   {
      std::cout << "Couldn't bind socket\n";
      return 1;
   }

   while(true)
   {
      if (socket.try_receive(6, packet_data) > 0)
      {
         std::cout << "packet: " << packet_data << "\n";
      }
   }

   return 0;
}
