#include "junkworks/udp_socket.hpp"

#include <chrono>
#include <iostream>
#include <thread>

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
         std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
   }

   return 0;
}
