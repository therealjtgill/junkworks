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

   // // bind the socket
   // unsigned int port = 8123;

   unsigned int bind_port = atoi(argv[1]);

   std::cout << "port: " << bind_port << "\n";

   if (!bind_socket(handle, bind_port))
   {
      return 1;
   }

   unsigned int a = 127;
   unsigned int b = 0;
   unsigned int c = 0;
   unsigned int d = 1;

   unsigned int ip_add = (a << 24) | (b << 16) | ( c << 8) | d;

   sockaddr_in send_address;
   send_address.sin_family = AF_INET;
   send_address.sin_addr.s_addr = htonl(ip_add);
   send_address.sin_port = htons(8123);

   send_packet(handle, send_address, "stuff", sizeof("stuff"));

   if (bind_port != 8123)
   {
      return 0;
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

   // std::cout << "got packet\n";

   // for (int i = 0; i < 5; ++i)
   // {
   //    std::cout << static_cast<int>(packet_data[i]) << " ";
   // }
   // std::cout << "\n";

   return 0;
}
