#include "junkwork/basic_udp.hpp"

#include <iostream>

int main(int argc, char ** argv)
{
   if (argc < 2)
   {
      return 0;
   }

   // create the socket
   int handle = create_socket();

   // bind the socket
   unsigned int bind_port = atoi(argv[1]);

   std::cout << "port: " << bind_port << "\n";

   if (!bind_socket(handle, bind_port))
   {
      return 1;
   }

   sockaddr_in from_address;
   char packet_data[6];

   while(true)
   {
      if (receive_packet(handle, 6, &from_address, packet_data))
      {
         std::cout << "packet: " << packet_data << "\n";
      }
   }

   return 0;
}
