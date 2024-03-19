#include "junkworks/udp_socket.hpp"

#include <iostream>

int main(int argc, char ** argv)
{
   if (argc < 2)
   {
      return 0;
   }

   // // create the socket
   // int handle = create_socket();

   // // bind the socket
   // unsigned int bind_port = atoi(argv[1]);

   // std::cout << "port: " << bind_port << "\n";

   // if (!bind_socket(handle, bind_port))
   // {
   //    return 1;
   // }

   // Ipv4Address ip(127, 0, 0, 1);

   // sockaddr_in send_address;
   // send_address.sin_family = AF_INET;
   // send_address.sin_addr.s_addr = ip.internet_address();
   // send_address.sin_port = htons(8123);

   // send_packet(handle, send_address, "stuff", sizeof("stuff"));

   junkworks::UdpSocket socket(atoi(argv[1]));

   if (!socket.bound())
   {
      std::cout << "Couldn't bind socket\n";
      return 1;
   }

   socket.try_send(
      junkworks::Ipv4Address(127, 0, 0, 1), 8123, "stuff", sizeof("stuff")
   );

   return 0;
}
