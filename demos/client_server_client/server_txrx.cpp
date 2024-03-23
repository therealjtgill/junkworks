#include "junkworks/server.hpp"

#include <chrono>
#include <iostream>
#include <map>
#include <thread>
#include <vector>

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
   junkworks::Server server(8123);

   std::map<unsigned int, std::vector<int> > client_rx_data;
   std::map<unsigned int, std::vector<junkworks::byte_data_t<128> > > client_tx_data;

   while(true)
   {
      server.update();

      const auto client_uids = server.get_uids();

      // if (!uids.empty())
      // {
      //    std::cout << "there are " << uids.size() << " simultaneous server connections\n";
      // }

      for (const auto client_uid : client_uids)
      {
         client_rx_data[client_uid];
         client_rx_data[client_uid].clear();
         const auto rx_data = server.get_client_rx_bytes(client_uid);
         if (!rx_data.empty())
         {
            // std::cout << "got " << rx_data.size() << " packets from uid " << uid << "\n";
            for (const auto & rx_bytes : rx_data)
            {
               // std::cout << "\t" << bytes_to_int(rx_bytes) << "\n";
               client_rx_data[client_uid].push_back(bytes_to_int(rx_bytes, 2));
            }
         }
      }

      for (const auto client_uid : client_uids)
      {
         client_tx_data[client_uid];
         client_tx_data[client_uid].clear();

         for (const auto & client_data : client_rx_data)
         {
            if (client_data.first == client_uid)
            {
               continue;
            }
            for (const auto single_int : client_data.second)
            {
               junkworks::byte_data_t<128> temp_bytes;
               temp_bytes.size = 128;
               int int_uid = static_cast<int>(client_data.first);
               int_to_bytes(int_uid, temp_bytes, 2);
               int_to_bytes(single_int, temp_bytes, 6);

               client_tx_data[client_uid].push_back(temp_bytes);
            }
         }
      }

      for (const auto & client_data : client_tx_data)
      {
         server.set_client_tx_bytes(client_data.first, client_data.second);
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
   }

   return 0;
}
