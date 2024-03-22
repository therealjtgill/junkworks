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

      if (uids.size() > 0)
      {
         std::cout << "there are " << uids.size() << " simultaneous server connections\n";
      }

      for (const auto uid : uids)
      {
         const auto rx_data = server.get_client_rx_bytes(uid);
         std::cout << "got " << rx_data.size() << " packets from uid " << uid << "\n";
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
   }

   return 0;
}
