#include "junkworks/server.hpp"

#include <chrono>
#include <iostream>
#include <thread>

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
               std::cout << "\t" << rx_bytes[2] << " " << rx_bytes[3] << " " << rx_bytes[4] << " " << rx_bytes[5] << "\n";
            }
         }
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
   }

   return 0;
}
