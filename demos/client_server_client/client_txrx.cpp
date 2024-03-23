#include "junkworks/client.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

int bytes_to_int(const junkworks::byte_data_t<128> & bytes, const int offset)
{
   int val = 0;
   for (int i = 0; i < 4; ++i)
   {
      int temp = bytes[offset + i];
      val += temp << (i * 8);
   }

   return val;
}

void int_to_bytes(
   const int i, junkworks::byte_data_t<128> & bytes, const int offset
)
{
   bytes[offset + 0] = i & (255);
   bytes[offset + 1] = (i & (255 << 8)) >> 8;
   bytes[offset + 2] = (i & (255 << 16)) >> 16;
   bytes[offset + 3] = (i & (255 << 24)) >> 24;
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
         int_to_bytes(i, temp_bytes, 2);
         temp_vec.push_back(temp_bytes);
         client.set_tx_bytes(temp_vec);

         auto vec_rx_data = client.get_rx_bytes();
         if (!vec_rx_data.empty())
         {
            std::cout << "Received " << vec_rx_data.size() << " packets\n";
         }

         for (const auto & rx_data : vec_rx_data)
         {
            int client_uid = bytes_to_int(rx_data, 2);
            int client_counter = bytes_to_int(rx_data, 6);
            std::cout << "\tfrom " << client_uid << ": " << client_counter << "\n";
         }

         ++i;
      }
      else
      {
         break;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(20));
   }

   return 0;
}
