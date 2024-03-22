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

   junkworks::Client client(bind_port, server_ip, 8123);

   int i = 0;
   junkworks::byte_data_t<128> temp_bytes;
   temp_bytes[0] = 0;
   temp_bytes[1] = bind_port - 8000;
   temp_bytes.size = 128;

   while(i < 65536)
   {
      std::vector<junkworks::byte_data_t<128> > temp_vec;
      client.update();

      if (client.behavior_running())
      {
         counter_to_bytes(i, temp_bytes);
         temp_vec.push_back(temp_bytes);
         client.set_tx_bytes(temp_vec);

         ++i;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
   }

   return 0;
}
