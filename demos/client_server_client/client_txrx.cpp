#include "junkworks/client.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

void counter_to_bytes(
   const int i, junkworks::byte_data_t<128> & bytes
)
{
   bytes[2] = i & (255);
   bytes[3] = (i & (255 << 8)) >> 8;
   bytes[4] = (i & (255 << 16)) >> 16;
   bytes[5] = (i & (255 << 24)) >> 24;
}

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

   return 0;
}
