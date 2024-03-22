#include "junkworks/server.hpp"

#include <chrono>
#include <iostream>
#include <thread>

int bytes_to_int(const junkworks::byte_data_t<128> & bytes)
{
   int val = 0;
   for (int i = 0; i < 4; ++i)
   {
      int temp = bytes[2 + i];
      val += temp << (3 - i);
   }

   return val;
}

int main(void)
{
   junkworks::Server server(8123);

   while(true)
   {
      server.update();

      const auto uids = server.get_uids();

      if (!uids.empty())
      {
         std::cout << "there are " << uids.size() << " simultaneous server connections\n";
      }

      for (const auto uid : uids)
      {
         const auto rx_data = server.get_client_rx_bytes(uid);
         if (!rx_data.empty())
         {
            std::cout << "got " << rx_data.size() << " packets from uid " << uid << "\n";
            for (const auto & rx_bytes : rx_data)
            {
               std::cout << "\t" << bytes_to_int(rx_bytes) << "\n";
            }
         }
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
   }

   return 0;
}
