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

   return 0;
}
