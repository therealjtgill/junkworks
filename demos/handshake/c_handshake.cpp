#include "junkworks/client.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

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

   junkworks::Client client(bind_port, server_ip, 8123);

   while(client.handshake_in_progress())
   {
      client.update();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
   }

   return 0;
}
